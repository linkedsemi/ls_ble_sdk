#include <stddef.h>
#include "sdk_config.h"
#include "spi_flash.h"
#include "lsqspi.h"
#include "flash_svcall.h"
#include "cpu.h"
#include "compile_flag.h"
#include "ls_dbg.h"
#include "platform.h"
#ifndef FLASH_PROG_ALGO
#define PUYA_SUSPEND_WORKAROUND 1
#else
#define PUYA_SUSPEND_WORKAROUND 0
#endif
#if PUYA_SUSPEND_WORKAROUND
#include "systick.h"
#endif
#define WRITE_STATUS_REGISTER_OPCODE 0x01
#define READ_STATUS_REGISTER_0_OPCODE 0x05
#define READ_STATUS_REGISTER_1_OPCODE 0x35
#define WRITE_ENABLE_OPCODE 0x06
#define PAGE_ERASE_OPCODE 0x81
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
#define PROG_ERASE_SUSPEND 0x75
#define PROG_ERASE_RESUME 0x7a

#define XIP_MODE_BITS 0x20

#define DUMMY_BYTE_VAL 0x3c

struct spi_flash_status
{
    bool writing;
    bool xip;
}flash_stat;

XIP_BANNED void spi_flash_xip_status_set(bool xip)
{
    flash_stat.xip = xip;
}

XIP_BANNED void spi_flash_writing_status_set(bool writing)
{
    flash_stat.writing = writing;
}

XIP_BANNED void spi_flash_drv_var_init(bool xip,bool writing)
{
    spi_flash_xip_status_set(xip);
    spi_flash_writing_status_set(writing);
}

XIP_BANNED void spi_flash_init()
{
    lsqspi_init();
}

XIP_BANNED static void quad_io_read_dummy(uint8_t opcode_en,uint8_t mode_bits)
{
    uint8_t dummy;
    struct lsqspi_stig_rd_wr_param param;
    param.start.data = &dummy;
    param.start.opcode = QUAD_IO_READ_OPCODE;
    param.start.addr = 0;
    param.start.dummy_bytes = 1;
    param.start.dummy_bytes_en = 1;
    param.start.addr_en = 1;
    param.start.quad_addr = 1;
    param.start.mode_bits_en = 1;
    param.start.opcode_en = opcode_en;
    param.size = sizeof(dummy);
    param.mode_bits = mode_bits;
    param.quad_data = true;
    lsqspi_stig_read_data(&param);
    #ifndef FLASH_PROG_ALGO
    LS_RAM_ASSERT(dummy == DUMMY_BYTE_VAL);
    #endif

}

XIP_BANNED void spi_flash_xip_start()
{
    if(flash_stat.xip)
    {
        return;
    }
    uint32_t cpu_stat = enter_critical();
    quad_io_read_dummy(1,XIP_MODE_BITS);
    struct lsqspi_direct_read_config_param direct_read_param; //do not initialize this variable with a const struct
    direct_read_param.opcode = QUAD_IO_READ_OPCODE;
    direct_read_param.dummy_bytes = 2;
    direct_read_param.quad_addr = 1;
    direct_read_param.quad_data = 1;
    direct_read_param.mode_bits_en = 1;
    direct_read_param.mode_bits = XIP_MODE_BITS;
    lsqspi_direct_read_config(&direct_read_param);
    flash_stat.xip = true;
    exit_critical(cpu_stat);
}

XIP_BANNED void spi_flash_xip_stop()
{
    if(flash_stat.xip == false)
    {
        return;
    }
    uint32_t cpu_stat = enter_critical();
    quad_io_read_dummy(0,0);
    flash_stat.xip = false;
    exit_critical(cpu_stat);
}

XIP_BANNED static void spi_flash_write_enable()
{
    lsqspi_stig_send_command(WRITE_ENABLE_OPCODE);
}

XIP_BANNED void spi_flash_read_status_register_0(uint8_t *status_reg_0)
{
    lsqspi_stig_read_register(READ_STATUS_REGISTER_0_OPCODE,status_reg_0,sizeof(uint8_t));
}

XIP_BANNED void spi_flash_read_status_register_1(uint8_t *status_reg_1)
{
    lsqspi_stig_read_register(READ_STATUS_REGISTER_1_OPCODE,status_reg_1,sizeof(uint8_t));
}

XIP_BANNED static bool spi_flash_write_in_process()
{
    uint8_t status_reg_0;
    spi_flash_read_status_register_0(&status_reg_0);
    return status_reg_0 & 0x1 ? true : false;
}

XIP_BANNED static void spi_flash_write_status_check()
{
    while(spi_flash_write_in_process());
}

#if ROM_CODE == 1
static void flash_reading_critical(void (*func)(void *),void *param)
{
    func(param);
}

static void flash_writing_critical(void (*func)(void *),void *param)
{
    uint32_t cpu_stat = enter_critical();
    spi_flash_write_enable();
    func(param);
    flash_stat.writing = true;
    exit_critical(cpu_stat);
    spi_flash_write_status_check();
    flash_stat.writing = false;
}
#else
NOINLINE XIP_BANNED static void flash_reading_critical(void (*func)(void *),void *param)
{
    spi_flash_xip_stop();
    func(param);
    spi_flash_xip_start();
}

#if PUYA_SUSPEND_WORKAROUND
static void do_spi_flash_prog_func(void *param);
XIP_BANNED static void flash_writing_critical(void (*func)(void *),void *param)
{
    uint32_t cpu_stat = enter_critical();
    spi_flash_xip_stop();
    spi_flash_write_enable();
    func(param);
    uint32_t writing_end_time = systick_get_value();
    DELAY_US(500);
    flash_stat.writing = true;
    exit_critical(cpu_stat);
    systick_poll_timeout(writing_end_time,func == do_spi_flash_prog_func ? 1000*SDK_HCLK_MHZ : 6900*SDK_HCLK_MHZ,NULL);
    cpu_stat = enter_critical();
    spi_flash_write_status_check();
    exit_critical(cpu_stat);
    flash_stat.writing = false;
    spi_flash_xip_start();
}


#else

XIP_BANNED static void flash_writing_critical(void (*func)(void *),void *param)
{
    uint32_t cpu_stat = enter_critical();
    spi_flash_xip_stop();
    spi_flash_write_enable();
    func(param);
    flash_stat.writing = true;
    exit_critical(cpu_stat);
    spi_flash_write_status_check();
    flash_stat.writing = false;
    spi_flash_xip_start();
}
#endif

#endif



XIP_BANNED static void do_spi_flash_write_status_reg_func(void * param)
{
    lsqspi_stig_write_register(WRITE_STATUS_REGISTER_OPCODE,param, sizeof(uint16_t));
}

XIP_BANNED void spi_flash_write_status_register(uint16_t status)
{
    flash_writing_critical(do_spi_flash_write_status_reg_func,&status);
}

XIP_BANNED static void do_spi_flash_prog_func(void *param)
{
    lsqspi_stig_write_data( param);
}

void do_spi_flash_program(uint32_t offset,uint8_t *data,uint16_t length,bool quad)
{
    struct lsqspi_stig_rd_wr_param param;
    param.start.data = data;
    param.start.opcode = quad? QUAD_PAGE_PROGRAM_OPCODE : PAGE_PROGRAM_OPCODE;
    param.start.addr = offset;
    param.start.dummy_bytes_en = 0;
    param.start.addr_en = 1;
    param.start.quad_addr = 0;
    param.start.mode_bits_en = 0;
    param.start.opcode_en = 1;
    param.size = length;
    param.quad_data = quad;
    flash_writing_critical(do_spi_flash_prog_func,&param);
}

void spi_flash_quad_page_program(uint32_t offset,uint8_t *data,uint16_t length)
{
    spi_flash_program_operation(offset,data,length,true);
}

void spi_flash_page_program(uint32_t offset,uint8_t *data,uint16_t length)
{
    spi_flash_program_operation(offset,data,length,false);
}

struct flash_erase_param
{
    uint8_t addr[3];
    uint8_t opcode;
};

XIP_BANNED static void do_spi_flash_erase_func(void *param)
{
    struct flash_erase_param *erase_param = param;
    lsqspi_stig_write_register(erase_param->opcode, erase_param->addr, 3);
}

void do_spi_flash_erase(uint32_t offset,uint8_t opcode)
{
    struct flash_erase_param param;
    param.addr[0] = offset>>16&0xff;
    param.addr[1] = offset>>8&0xff;
    param.addr[2] = offset&0xff;
    param.opcode = opcode;
    flash_writing_critical(do_spi_flash_erase_func,&param);
}

void spi_flash_page_erase(uint32_t offset)
{
    spi_flash_erase_operation(offset,PAGE_ERASE_OPCODE);
}

void spi_flash_sector_erase(uint32_t offset)
{
    spi_flash_erase_operation(offset,SECTOR_ERASE_OPCODE);
}

XIP_BANNED static void do_spi_flash_chip_erase_func(void *param)
{
    lsqspi_stig_send_command(CHIP_ERASE_OPCODE);
}

void do_spi_flash_chip_erase()
{
    flash_writing_critical(do_spi_flash_chip_erase_func,NULL);
}

void spi_flash_chip_erase()
{
    spi_flash_chip_erase_operation();
}

XIP_BANNED static void do_spi_flash_read_func(void *param)
{
    lsqspi_stig_read_data(param);
}

void do_spi_flash_read(void *param)
{
    flash_reading_critical(do_spi_flash_read_func,param);
}

void spi_flash_quad_io_read(uint32_t offset, uint8_t * data, uint16_t length)
{
    struct lsqspi_stig_rd_wr_param param;
    param.start.data = data;
    param.start.opcode = QUAD_IO_READ_OPCODE;
    param.start.addr = offset;
    param.start.dummy_bytes = 1;
    param.start.dummy_bytes_en = 1;
    param.start.addr_en = 1;
    param.start.quad_addr = 1;
    param.start.mode_bits_en = 1;
    param.start.opcode_en = 1;
    param.size = length;
    param.mode_bits = 0;
    param.quad_data = 1;
    spi_flash_read_operation(&param);
}

void spi_flash_fast_read(uint32_t offset, uint8_t * data, uint16_t length)
{
    struct lsqspi_stig_rd_wr_param param;
    param.start.data = data;
    param.start.opcode = FAST_READ_OPCODE;
    param.start.addr = offset;
    param.start.dummy_bytes = 0;
    param.start.dummy_bytes_en = 1;
    param.start.addr_en = 1;
    param.start.quad_addr = 0;
    param.start.mode_bits_en = 0;
    param.start.opcode_en = 1;
    param.size = length;
    param.quad_data = 0;
    spi_flash_read_operation(&param);
}

XIP_BANNED void spi_flash_deep_power_down()
{
    lsqspi_stig_send_command( DEEP_POWER_DOWN_OPCODE);
}

XIP_BANNED void spi_flash_release_from_deep_power_down()
{
    lsqspi_stig_send_command(RELEASE_FROM_DEEP_POWER_DOWN_OPCODE);
}

struct flash_read_reg_param
{
    uint8_t *buf;
    uint8_t opcode;
    uint8_t length;    
};

XIP_BANNED static void do_spi_flash_read_reg_func(void *param)
{
    struct flash_read_reg_param *ptr = param;
    lsqspi_stig_read_register(ptr->opcode,ptr->buf,ptr->length);
}

void do_spi_flash_read_reg(void *param)
{
    flash_reading_critical(do_spi_flash_read_reg_func,param);
}

void spi_flash_read_id(uint8_t jedec_id[3])
{
    struct flash_read_reg_param param;
    param.buf = jedec_id;
    param.opcode = READ_IDENTIFICATION_OPCODE;
    param.length = 3;
    spi_flash_read_reg_operation(&param);
}

void spi_flash_read_unique_id(uint8_t unique_serial_id[16])
{
    struct lsqspi_stig_rd_wr_param param;
    param.start.data = unique_serial_id;
    param.start.opcode = READ_UNIQUE_ID_OPCODE;
    param.start.addr = 0;
    param.start.dummy_bytes = 0;
    param.start.dummy_bytes_en = 1;
    param.start.addr_en = 1;
    param.start.quad_addr = 0;
    param.start.mode_bits_en = 0;
    param.start.opcode_en = 1;
    param.size = 16;
    param.quad_data = false;
    spi_flash_read_operation(&param);
}

XIP_BANNED static void do_spi_flash_erase_security_area_func(void *param)
{
    lsqspi_stig_write_register(ERASE_SECURITY_AREA_OPCODE,param,3);
}

void do_spi_flash_erase_security_area(uint8_t idx)
{
    uint8_t buf[3];
    buf[0] = 0;
    buf[1] = idx<<4;
    buf[2] = 0;
    flash_writing_critical(do_spi_flash_erase_security_area_func,buf);
}

void spi_flash_erase_security_area(uint8_t idx)
{
    spi_flash_erase_security_area_operation(idx);
}

XIP_BANNED static void do_spi_flash_program_security_area_func(void *param)
{
    lsqspi_stig_write_data( param);
}

void do_spi_flash_program_security_area(uint8_t idx,uint16_t addr,uint8_t *data,uint16_t length)
{
    struct lsqspi_stig_rd_wr_param param;
    param.start.data = data;
    param.start.opcode = PROGRAM_SECURITY_AREA_OPCODE;
    param.start.addr = idx<<12 | (addr&0x1ff);
    param.start.dummy_bytes_en = 0;
    param.start.addr_en = 1;
    param.start.quad_addr = 0;
    param.start.mode_bits_en = 0;
    param.start.opcode_en = 1;
    param.size = length;
    param.quad_data = false;
    flash_writing_critical(do_spi_flash_program_security_area_func, &param);
}

void spi_flash_program_security_area(uint8_t idx,uint16_t addr,uint8_t *data,uint16_t length)
{
    spi_flash_program_security_area_operation(idx, addr,data,length);
}

XIP_BANNED static void do_spi_flash_read_security_area_func(void *param)
{
    lsqspi_stig_read_data( param);
}

void do_spi_flash_read_security_area(uint8_t idx,uint16_t addr,uint8_t *data,uint16_t length)
{
    struct lsqspi_stig_rd_wr_param param;
    param.start.data = data;
    param.start.opcode = READ_SECURITY_AREA_OPCODE;
    param.start.addr = idx<<12 | (addr&0x1ff);
    param.start.dummy_bytes = 0;
    param.start.dummy_bytes_en = 1;
    param.start.addr_en = 1;
    param.start.quad_addr = 0;
    param.start.mode_bits_en = 0;
    param.start.opcode_en = 1;
    param.size = length;
    param.quad_data = false;
    flash_reading_critical(do_spi_flash_read_security_area_func,&param);
}

void spi_flash_read_security_area(uint8_t idx,uint16_t addr,uint8_t *data,uint16_t length)
{
    spi_flash_read_security_area_operation(idx, addr,data,length);
}

XIP_BANNED void spi_flash_software_reset()
{
    lsqspi_stig_send_command(RESET_EN_OPCODE);
    lsqspi_stig_send_command(RESET_OPCODE);
}

XIP_BANNED void spi_flash_qe_status_read_and_set()
{
    uint8_t status_reg[2];
    spi_flash_read_status_register_1(&status_reg[1]);
    if((status_reg[1]&0x02) == 0)
    {
        spi_flash_read_status_register_0(&status_reg[0]);
        spi_flash_write_status_register(status_reg[1]<<8|status_reg[0]|0x200);
    }
}

XIP_BANNED void spi_flash_prog_erase_suspend()
{
    lsqspi_stig_send_command(PROG_ERASE_SUSPEND);
}

XIP_BANNED void spi_flash_prog_erase_resume()
{
    lsqspi_stig_send_command(PROG_ERASE_RESUME);
}

XIP_BANNED bool spi_flash_writing_busy()
{
    return flash_stat.writing;
}

XIP_BANNED bool spi_flash_xip_status_get()
{
    return flash_stat.xip;
}


