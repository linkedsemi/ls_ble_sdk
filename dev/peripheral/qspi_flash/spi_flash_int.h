#ifndef SPI_FLASH_INT_H_
#define SPI_FLASH_INT_H_
#include <stdint.h>
#include <stdbool.h>

void do_spi_flash_fast_read(uint32_t offset,uint8_t *data,uint16_t length);

void do_spi_flash_quad_io_read(uint32_t offset,uint8_t *data,uint16_t length);

void do_spi_flash_sector_erase(uint32_t offset);

void do_spi_flash_program(uint32_t offset,uint8_t *data,uint16_t length,bool quad);

void do_spi_flash_chip_erase(void);

void do_spi_flash_write_status_reg(uint16_t status);

void do_spi_flash_erase_security_area(uint8_t idx);

void do_spi_flash_program_security_area(uint8_t idx, uint16_t addr, uint8_t * data, uint16_t length);

void do_spi_flash_read_security_area(uint8_t idx,uint16_t addr,uint8_t *data,uint16_t length);

#endif

