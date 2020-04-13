#include "le501x.h"
#include "spi_flash.h"
#include "section_def.h"
#include "lscache.h"
#include "platform.h"

#define ISR_VECTOR_ADDR ((uint32_t *)(0x0))
void cpu_sleep_asm(void);

void cpu_recover_asm(void);

void cpu_sleep_recover_init()
{
    ISR_VECTOR_ADDR[1] = (uint32_t)cpu_recover_asm;
}


XIP_BANNED static void cpu_flash_deep_sleep_and_recover()
{
    spi_flash_xip_stop();
    spi_flash_deep_power_down();
    cpu_sleep_asm();
    spi_flash_init();
    spi_flash_release_from_deep_power_down();
    spi_flash_xip_start();
    LSCACHE_CTRL_SET(1,1);
}

void deep_sleep()
{
    uint32_t ble_retetion[100];
    
    cpu_flash_deep_sleep_and_recover();
    irq_init();
    
}


