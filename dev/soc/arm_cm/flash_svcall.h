#ifndef FLASH_SVCALL_H_
#define FLASH_SVCALL_H_
#include <stdint.h>
#include <stdbool.h>

void spi_flash_program_operation(uint32_t offset,uint8_t *data,uint16_t length,bool quad);

void spi_flash_sector_erase_operation(uint32_t offset);

void spi_flash_fast_read_operation(uint32_t offset, uint8_t * data, uint16_t length);

void spi_flash_quad_io_read_operation(uint32_t offset, uint8_t * data, uint16_t length);

void spi_flash_chip_erase_operation(void);

void spi_flash_erase_security_area_operation(uint8_t idx);

void spi_flash_program_security_area_operation(uint8_t idx, uint16_t addr, uint8_t * data, uint16_t length);

void spi_flash_read_security_area_operation(uint8_t idx, uint16_t addr, uint8_t * data, uint16_t length);

#endif

