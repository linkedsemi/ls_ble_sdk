/*
 * @Author: your name
 * @Date: 2020-05-03 16:00:06
 * @LastEditTime: 2020-05-05 21:19:03
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \ble_sdk_app\dev\examples\sbl\sbl.c
 */
#include "spi_flash.h"
#include "le501x.h"
#include "lscache.h"
#include "compile_flag.h"
#include "reg_rcc.h"
#include "field_manipulate.h"
#include "sdk_config.h"
#define APP_IMAGE_BASE (0x18008000)

#if (SDK_HCLK_MHZ==16)
XIP_BANNED void switch_to_xo16m()
{
    REG_FIELD_WR(RCC->CFG, RCC_SYSCLK_SW, 1);
    REG_FIELD_WR(RCC->CFG, RCC_CKCFG, 1);
}

XIP_BANNED static void clk_switch()
{
    switch_to_xo16m();
}

#else
XIP_BANNED static void switch_to_pll()
{
    REG_FIELD_WR(RCC->CFG, RCC_SYSCLK_SW, 4);
    REG_FIELD_WR(RCC->CFG, RCC_CKCFG, 1);
}
XIP_BANNED static void switch_to_rc32k()
{
    REG_FIELD_WR(RCC->CFG, RCC_SYSCLK_SW, 2);
    REG_FIELD_WR(RCC->CFG, RCC_CKCFG, 1);
}
#if (SDK_HCLK_MHZ==32)
XIP_BANNED static void clk_switch()
{
    switch_to_rc32k();
    REG_FIELD_WR(RCC->CFG, RCC_HCLK_SCAL,0x8);
    switch_to_pll();
}
#elif(SDK_HCLK_MHZ==64)
XIP_BANNED static void clk_switch()
{
    switch_to_rc32k();
    switch_to_pll();
}
#else
#error HCLK not supported
#endif

#endif

XIP_BANNED int main()
{
    clk_switch();
    __disable_irq();
    spi_flash_drv_var_init(false,false);
    spi_flash_init();
    spi_flash_qe_status_read_and_set();
    spi_flash_xip_start();
    lscache_cache_ctrl(0,1);
    uint32_t *msp = (void *)APP_IMAGE_BASE;
    void (**reset_handler)(void) = (void *)(APP_IMAGE_BASE + 4);
    __set_MSP(*msp);
    (*reset_handler)();
    return 0;
}
