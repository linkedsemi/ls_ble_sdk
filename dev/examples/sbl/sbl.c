#include "spi_flash.h"
#include "le501x.h"
#include "lscache.h"
#include "compile_flag.h"
#include "reg_rcc.h"
#include "field_manipulate.h"
#define APP_IMAGE_BASE (0x18036000)

XIP_BANNED static void switch_to_rc32k()
{
    REG_FIELD_WR(RCC->CFG, RCC_SYSCLK_SW, 2);
    REG_FIELD_WR(RCC->CFG, RCC_CKCFG, 1);
}

XIP_BANNED static void switch_to_pll64m()
{
    REG_FIELD_WR(RCC->CFG, RCC_SYSCLK_SW, 4);
    REG_FIELD_WR(RCC->CFG, RCC_CKCFG, 1);
}

XIP_BANNED static void rc24m_switch_to_pll64m()
{
    switch_to_rc32k();
    switch_to_pll64m();
}

XIP_BANNED static void switch_to_xo16m()
{
    REG_FIELD_WR(RCC->CFG, RCC_SYSCLK_SW, 1);
    REG_FIELD_WR(RCC->CFG, RCC_CKCFG, 1);
}


void SystemInit(){
//    rc24m_switch_to_pll64m();
}

XIP_BANNED int main()
{
//    rc24m_switch_to_pll64m();
    switch_to_xo16m();
    __disable_irq();
    spi_flash_drv_var_init();
    spi_flash_init();
    spi_flash_qe_status_read_and_set();
    spi_flash_xip_start();
    lscache_cache_ctrl(1,1);
    uint32_t *msp = (void *)APP_IMAGE_BASE;
    void (**reset_handler)(void) = (void *)(APP_IMAGE_BASE + 4);
    __set_MSP(*msp);
    (*reset_handler)();
    return 0;
}
