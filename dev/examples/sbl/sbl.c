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
#include "sleep.h"
#include "reg_lsgpio.h"
#define APP_IMAGE_BASE (0x18008000)

XIP_BANNED void swd_pull_down()
{
    MODIFY_REG(LSGPIOB->PUPD,GPIO_PUPD5_MASK|GPIO_PUPD6_MASK,2<<GPIO_PUPD5_POS | 2<<GPIO_PUPD6_POS);
}

XIP_BANNED int main()
{
    clk_switch();
    __disable_irq();
    swd_pull_down();
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
