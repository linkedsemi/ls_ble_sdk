#include "le501x.h"
#include "spi_flash.h"
#include "compile_flag.h"
#include "lscache.h"
#include "platform.h"

#define BLE_MAC_REG_BASE 0x50000000
#define BLE_MAC_REG_OFFSET_WORD_MAX (0x18C/4)
#define ISR_VECTOR_ADDR ((uint32_t *)(0x0))
void cpu_sleep_asm(void);

void cpu_recover_asm(void);

XIP_BANNED void dcdc_off()
{

}

XIP_BANNED void dcdc_on()
{

}

void cpu_sleep_recover_init()
{
    ISR_VECTOR_ADDR[1] = (uint32_t)cpu_recover_asm;
}


NOINLINE XIP_BANNED static void cpu_flash_deep_sleep_and_recover()
{
    spi_flash_xip_stop();
    spi_flash_deep_power_down();
    cpu_sleep_asm();
    spi_flash_init();
    spi_flash_release_from_deep_power_down();
    spi_flash_xip_start();
    LSCACHE_CTRL_SET(1,1);
}

void memcpy32(uint32_t *dest, const uint32_t *src, uint32_t size_word)
{
    while(size_word--)
    {
        *dest++ = *src++;
    }
}

void deep_sleep()
{
    uint32_t ble_retetion[BLE_MAC_REG_OFFSET_WORD_MAX];
    SCB->SCR |= (1<<2);
    memcpy32(&ble_retetion[0], (uint32_t*)BLE_MAC_REG_BASE, BLE_MAC_REG_OFFSET_WORD_MAX);
    cpu_flash_deep_sleep_and_recover();
    memcpy32((uint32_t*)BLE_MAC_REG_BASE, &ble_retetion[0], BLE_MAC_REG_OFFSET_WORD_MAX);
    __disable_irq();
    irq_init();
    
}


