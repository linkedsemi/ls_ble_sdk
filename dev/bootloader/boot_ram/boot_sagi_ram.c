#include "io_config.h"
#include "spi_flash.h"
#include "lscache.h"
#include "platform.h"
#include "prf_fotas.h"
#include <string.h>
#include "common.h"
#include "ARMCM3.h"

void cpu_sleep_asm(void);
void cpu_recover_asm(void);

static void boot_app(uint32_t base)
{
    uint32_t *msp = (void *)base;
    void (**reset_handler)(void) = (void *)(base + 4);
    __set_MSP(*msp);
    __enable_irq();
    (*reset_handler)();
}

void boot_ram_start(uint32_t exec_addr)
{
    if(clk_check()==false)
    {
        clk_switch();
    }
    spi_flash_drv_var_init(false,false);
    spi_flash_init();
    spi_flash_qe_status_read_and_set();
    spi_flash_xip_start();
    lscache_cache_enable(0);
    uint32_t image_base;
    image_base = get_app_image_base();
    boot_app(image_base);
    while(1);
}
