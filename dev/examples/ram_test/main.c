#include "spi_flash.h"
#include "spi_flash_int.h"
#include "le501x.h"
#include <string.h>

uint8_t test_buf[1024];
int main()
{
    spi_flash_init();
    spi_flash_qe_status_read_and_set();
    spi_flash_xip_start();
    do_spi_flash_sector_erase(0);
    memcpy(test_buf,(void *)0x18002000,sizeof(test_buf));
    while(1);
    return 0;
}
