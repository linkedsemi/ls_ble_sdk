#ifndef FLASH_SVCALL_INT_H_
#define FLASH_SVCALL_INT_H_
#include <stdbool.h>
#include "le501x.h"
#include "flash_svcall.h"
#define GLOBAL_INT_MASK_STATUS() __get_PRIMASK()

#if defined(__CC_ARM)
#define SVCALL(number, return_type, signature) return_type __svc(number) signature
#else
#define SVCALL(number, return_type, signature)          \
  _Pragma("GCC diagnostic push")                        \
  _Pragma("GCC diagnostic ignored \"-Wreturn-type\"")   \
  __attribute__((naked))                                \
  __attribute__((unused))                               \
  static return_type signature                          \
  {                                                     \
    __asm(                                              \
        "svc %0\n"                                      \
        "bx r14" : : "I" (number) : "r0"   \
    );                                                  \
  }                                                     \
  _Pragma("GCC diagnostic pop")
#endif

SVCALL(SVCALL_FLASH_PROGRAM,void,do_spi_flash_program_svcall(uint32_t offset,uint8_t *data,uint16_t length,bool quad));
SVCALL(SVCALL_FLASH_SECTOR_ERASE,void, do_spi_flash_sector_erase_svcall(uint32_t addr));
SVCALL(SVCALL_FLASH_FAST_READ,void,do_spi_flash_fast_read_svcall(uint32_t offset, uint8_t * data, uint16_t length));
SVCALL(SVCALL_FLASH_QUAD_READ,void,do_spi_flash_quad_io_read_svcall(uint32_t offset, uint8_t * data, uint16_t length));
SVCALL(SVCALL_FLASH_CHIP_ERASE,void,do_spi_flash_chip_erase_svcall(void));
SVCALL(SVCALL_FLASH_ERASE_SECURITY,void,do_spi_flash_erase_security_area_svcall(uint8_t idx));
SVCALL(SVCALL_FLASH_PROGRAM_SECURITY,void,do_spi_flash_program_security_area_svcall(uint8_t idx, uint16_t addr, uint8_t * data, uint16_t length));
SVCALL(SVCALL_FLASH_READ_SECURITY,void,do_spi_flash_read_security_area_svcall(uint8_t idx, uint16_t addr, uint8_t * data, uint16_t length));

#endif
