#include "spi_flash.h"
#include "le501x.h"
#include "lscache.h"
#define APP_IMAGE_BASE (0x18036000)

int main()
{
    __disable_irq();
    spi_flash_init();
    spi_flash_qe_status_read_and_set();
    spi_flash_xip_start();
    LSCACHE_CTRL_SET(1,1);
    uint32_t *msp = (void *)APP_IMAGE_BASE;
    void (**reset_handler)(void) = (void *)(APP_IMAGE_BASE + 4);
    __set_MSP(*msp);
    (*reset_handler)();
    return 0;
}
