#include "le501x.h"
#include "spi_flash.h"
#include "section_def.h"
#include "lscache.h"
#define ISR_VECTOR_ADDR ((uint32_t *)(0x0))
void cpu_sleep_asm(void);

void cpu_recover_asm(void);

void cpu_sleep_recover_init()
{
    ISR_VECTOR_ADDR[1] = (uint32_t)cpu_recover_asm;
}

XIP_BANNED void cpu_deep_sleep_and_recover()
{
    cpu_sleep_asm();
    spi_flash_init();
    spi_flash_xip_start();
    LSCACHE_CTRL_SET(1,1);

}

