#include "spi_flash_int.h"

#if defined(__CC_ARM)
#define SVCALL(number, return_type, signature) return_type __svc(number) signature
#else
#define SVCALL(number, return_type, signature)          \
  _Pragma("GCC diagnostic push")                        \
  _Pragma("GCC diagnostic ignored \"-Wreturn-type\"")   \
  __attribute__((naked))                                \
  __attribute__((unused))                               \
  return_type signature                          \
  {                                                     \
    __asm(                                              \
        "svc %0\n"                                      \
        "bx r14" : : "I" (number) : "r0"   \
    );                                                  \
  }                                                     \
  _Pragma("GCC diagnostic pop")
#endif

enum svcall_num_enum
{
    SVCALL_FLASH_PROGRAM,
    SVCALL_FLASH_SECTOR_ERASE,
    SVCALL_FLASH_FAST_READ,
    SVCALL_FLASH_QUAD_READ,
    SVCALL_FLASH_CHIP_ERASE,
    SVCALL_FLASH_ERASE_SECURITY,
    SVCALL_FLASH_PROGRAM_SECURITY,
    SVCALL_FLASH_READ_SECURITY,
    SVCALL_NUM_MAX,
};

typedef uint32_t (*svcall_fn_t)(uint32_t,uint32_t,uint32_t,uint32_t);

const svcall_fn_t svcall_table[SVCALL_NUM_MAX] = {
    [SVCALL_FLASH_PROGRAM] = (svcall_fn_t)do_spi_flash_program,
    [SVCALL_FLASH_SECTOR_ERASE] = (svcall_fn_t)do_spi_flash_sector_erase,
    [SVCALL_FLASH_FAST_READ] = (svcall_fn_t)do_spi_flash_fast_read,
    [SVCALL_FLASH_QUAD_READ] = (svcall_fn_t)do_spi_flash_quad_io_read,
    [SVCALL_FLASH_CHIP_ERASE] = (svcall_fn_t)do_spi_flash_chip_erase,
    [SVCALL_FLASH_ERASE_SECURITY] = (svcall_fn_t)do_spi_flash_erase_security_area,
    [SVCALL_FLASH_PROGRAM_SECURITY] = (svcall_fn_t)do_spi_flash_program_security_area,
    [SVCALL_FLASH_READ_SECURITY] = (svcall_fn_t)do_spi_flash_read_security_area,
};

void SVC_Handler_C(uint32_t *svc_args)
{
    uint8_t svc_num = ((uint8_t *)svc_args[6])[-2];
    if(svc_num<SVCALL_NUM_MAX)
    {
        svc_args[0] = svcall_table[svc_num](svc_args[0],svc_args[1],svc_args[2],svc_args[3]);
    }
}

SVCALL(SVCALL_FLASH_PROGRAM,void,do_spi_flash_program_svcall(uint32_t offset,uint8_t *data,uint16_t length,bool quad));
SVCALL(SVCALL_FLASH_SECTOR_ERASE,void, do_spi_flash_sector_erase_svcall(uint32_t addr));
SVCALL(SVCALL_FLASH_FAST_READ,void,do_spi_flash_fast_read_svcall(uint32_t offset, uint8_t * data, uint16_t length));
SVCALL(SVCALL_FLASH_QUAD_READ,void,do_spi_flash_quad_io_read_svcall(uint32_t offset, uint8_t * data, uint16_t length));
SVCALL(SVCALL_FLASH_CHIP_ERASE,void,do_spi_flash_chip_erase_svcall(void));
SVCALL(SVCALL_FLASH_ERASE_SECURITY,void,do_spi_flash_erase_security_area_svcall(uint8_t idx));
SVCALL(SVCALL_FLASH_PROGRAM_SECURITY,void,do_spi_flash_program_security_area_svcall(uint8_t idx, uint16_t addr, uint8_t * data, uint16_t length));
SVCALL(SVCALL_FLASH_READ_SECURITY,void,do_spi_flash_read_security_area_svcall(uint8_t idx, uint16_t addr, uint8_t * data, uint16_t length));

