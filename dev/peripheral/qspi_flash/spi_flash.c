#include <stddef.h>
#include "spi_flash.h"
#include "lsqspi.h"
#define WRITE_STATUS_REGISTER_OPCODE 0x01
#define READ_STATUS_REGISTER_0_OPCODE 0x05
#define READ_STATUS_REGISTER_1_OPCODE 0x35
#define WRITE_ENABLE_OPCODE 0x06
#define SECTOR_ERASE_OPCODE 0x20
#define CHIP_ERASE_OPCODE 0x60
#define PAGE_PROGRAM_OPCODE 0x02
#define QUAD_PAGE_PROGRAM_OPCODE 0x32
#define QUAD_IO_READ_OPCODE 0xeb
#define FAST_READ_OPCODE 0x0b
#define DEEP_POWER_DOWN_OPCODE 0xb9
#define RELEASE_FROM_DEEP_POWER_DOWN_OPCODE 0xab
#define READ_IDENTIFICATION_OPCODE 0x9f
#define READ_UNIQUE_ID_OPCODE 0x4b
#define ERASE_SECURITY_AREA_OPCODE 0x44
#define PROGRAM_SECURITY_AREA_OPCODE 0x42
#define READ_SECURITY_AREA_OPCODE 0x48
#define RESET_EN_OPCODE 0x66
#define RESET_OPCODE 0x99

#define XIP_MODE_BITS 0x20
static struct lsqspi_instance lsqspi_inst;

void spi_flash_init()
{
	lsqspi_inst.reg = LSQSPI;
	lsqspi_init(&lsqspi_inst);
}

void spi_flash_xip_start()
{
	lsqspi_mode_bits_set(&lsqspi_inst,XIP_MODE_BITS);
	uint8_t dummy;
	struct lsqspi_stig_read_data_param stig_read_param = {
		.data = &dummy,
		.addr = 0,
		.opcode = QUAD_IO_READ_OPCODE,
		.size = sizeof(dummy),
		.dummy_bytes = 2,
		.quad_addr = 1,
		.quad_data = 1,
		.mode_bits_en = 1,
	};
	lsqspi_stig_read_data(&lsqspi_inst,&stig_read_param);
	struct lsqspi_direct_read_config_param direct_read_param = 
	{
		.opcode = QUAD_IO_READ_OPCODE,
		.dummy_bytes = 2,
		.quad_addr = 1,
		.quad_data = 1,
		.mode_bits_en = 1,
	};
	lsqspi_direct_read_config(&lsqspi_inst,&direct_read_param);
}

static void spi_flash_write_enable()
{
	lsqspi_stig_write_register(&lsqspi_inst,WRITE_ENABLE_OPCODE,NULL,0);
}

void spi_flash_read_status_register_0(uint8_t *status_reg_0)
{
	lsqspi_stig_read_register(&lsqspi_inst,READ_STATUS_REGISTER_0_OPCODE,status_reg_0,sizeof(uint8_t));
}

void spi_flash_read_status_register_1(uint8_t *status_reg_1)
{
	lsqspi_stig_read_register(&lsqspi_inst,READ_STATUS_REGISTER_1_OPCODE,status_reg_1,sizeof(uint8_t));
}

static bool spi_flash_write_in_process()
{
	uint8_t status_reg_0;
	spi_flash_read_status_register_0(&status_reg_0);
	return status_reg_0 & 0x1 ? true : false;
}

static void spi_flash_write_status_check()
{
	while(spi_flash_write_in_process());
}

void spi_flash_write_status_register(uint16_t status)
{
	spi_flash_write_enable();
	lsqspi_stig_write_register(&lsqspi_inst,WRITE_STATUS_REGISTER_OPCODE,(uint8_t *)&status, sizeof(uint16_t));
	spi_flash_write_status_check();
}

static void spi_flash_program(uint32_t offset,uint8_t *data,uint16_t length,bool quad)
{
	spi_flash_write_enable();
	struct lsqspi_direct_write_data_param param = {
		.data = data,
		.addr = offset,
		.opcode = quad? QUAD_PAGE_PROGRAM_OPCODE : PAGE_PROGRAM_OPCODE,
		.size = length,
		.quad_data = quad,
	};
	lsqspi_direct_write_data(&lsqspi_inst, &param);
	spi_flash_write_status_check();
}

void spi_flash_quad_page_program(uint32_t offset,uint8_t *data,uint16_t length)
{
	spi_flash_program(offset,data,length,true);
}

void spi_flash_page_program(uint32_t offset,uint8_t *data,uint16_t length)
{
	spi_flash_program(offset,data,length,false);
}

void spi_flash_sector_erase(uint32_t offset)
{
	spi_flash_write_enable();
	uint8_t addr[3];
	addr[0] = offset>>16&0xff;
	addr[1] = offset>>8&0xff;
	addr[2] = offset&0xff;
	lsqspi_stig_write_register(&lsqspi_inst,SECTOR_ERASE_OPCODE, addr, 3);
	spi_flash_write_status_check();
}

void spi_flash_chip_erase()
{
	spi_flash_write_enable();
	lsqspi_stig_write_register(&lsqspi_inst,CHIP_ERASE_OPCODE,NULL,0);
	spi_flash_write_status_check();
}

void spi_flash_quad_io_read(uint32_t offset,uint8_t *data,uint16_t length)
{
	lsqspi_mode_bits_set(&lsqspi_inst,0);
	struct lsqspi_stig_read_data_param param = {
		.data = data,
		.addr = offset,
		.opcode = QUAD_IO_READ_OPCODE,
		.size = length,
		.dummy_bytes = 2,
		.quad_addr = 1,
		.quad_data = 1,
		.mode_bits_en = 1,
	};
	lsqspi_stig_read_data(&lsqspi_inst,&param);
}

void spi_flash_std_fast_read(uint32_t offset,uint8_t *data,uint16_t length)
{
	struct lsqspi_stig_read_data_param param = {
		.data = data,
		.addr = offset,
		.opcode = FAST_READ_OPCODE,
		.size = length,
		.dummy_bytes = 1,
		.quad_addr = 0,
		.quad_data = 0,
		.mode_bits_en = 0,
	};
	lsqspi_stig_read_data(&lsqspi_inst,&param);
}

void spi_flash_deep_power_down()
{
	lsqspi_stig_write_register(&lsqspi_inst, DEEP_POWER_DOWN_OPCODE,NULL, 0);
}

void spi_flash_release_from_deep_power_down()
{
	lsqspi_stig_write_register(&lsqspi_inst,RELEASE_FROM_DEEP_POWER_DOWN_OPCODE,NULL,0);
}

void spi_flash_read_id(uint8_t *manufacturer_id,uint8_t *mem_type_id,uint8_t *capacity_id)
{
	uint8_t buf[3];
	lsqspi_stig_read_register(&lsqspi_inst, READ_IDENTIFICATION_OPCODE, buf, 3);
	*manufacturer_id = buf[0];
	*mem_type_id = buf[1];
	*capacity_id = buf[2];
}

void spi_flash_read_unique_id(uint8_t unique_serial_id[16])
{
	struct lsqspi_stig_read_data_param param = {
		.data = unique_serial_id,
		.addr = 0,
		.opcode = READ_UNIQUE_ID_OPCODE,
		.size = 16,
		.dummy_bytes = 1,
		.quad_addr = 0,
		.quad_data = 0,
		.mode_bits_en = 0,
	};
	lsqspi_stig_read_data(&lsqspi_inst, &param);
}

void spi_flash_erase_security_area(uint8_t idx,uint16_t addr)
{
	spi_flash_write_enable();
	uint8_t buf[3];
	buf[0] = 0;
	buf[1] = idx<<4 | (addr>>8 & 0x1);
	buf[2] = addr & 0xff;
	lsqspi_stig_write_register(&lsqspi_inst,ERASE_SECURITY_AREA_OPCODE,buf,3);
	spi_flash_write_status_check();
}

void spi_flash_program_security_area(uint8_t idx,uint16_t addr,uint8_t *data,uint16_t length)
{
	spi_flash_write_enable();
	struct lsqspi_direct_write_data_param param = {
		.data = data,
		.addr = idx<<12 | (addr&0x1ff),
		.opcode = PROGRAM_SECURITY_AREA_OPCODE,
		.size = length,
		.quad_data = false,
	};
	lsqspi_direct_write_data(&lsqspi_inst, &param);
	spi_flash_write_status_check();
}

void spi_flash_read_security_area(uint8_t idx,uint16_t addr,uint8_t *data,uint16_t length)
{
	struct lsqspi_stig_read_data_param param = {
		.data = data,
		.addr = idx<<12 | (addr&0x1ff),
		.opcode = READ_SECURITY_AREA_OPCODE,
		.size = length,
		.dummy_bytes = 1,
		.quad_addr = 0,
		.quad_data = 0,
		.mode_bits_en = 0,
	};
	lsqspi_stig_read_data(&lsqspi_inst, &param);
}

void spi_flash_software_reset()
{
	lsqspi_stig_write_register(&lsqspi_inst,RESET_EN_OPCODE, NULL, 0);
	lsqspi_stig_write_register(&lsqspi_inst,RESET_OPCODE, NULL, 0);
}