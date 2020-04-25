#include "spi_flash.h"
#include "spi_flash_int.h"
#include "le501x.h"
#include <string.h>
#include <stdlib.h>
uint8_t test_buf[1024];
uint8_t dst[1024];
int main()
{
    __disable_irq();
    spi_flash_drv_var_init();
    spi_flash_init();
    spi_flash_software_reset();
 //   spi_flash_write_status_register(0);
    spi_flash_qe_status_read_and_set();
    spi_flash_sector_erase(0x2000);
    spi_flash_chip_erase();
    for(uint32_t i=0;i<1024;i+=4)
    {
        *(uint32_t *)&test_buf[i] = rand();
    }
    spi_flash_quad_page_program(0,test_buf,256);
    spi_flash_quad_io_read(0,dst,sizeof(dst));
//    spi_flash_xip_start();

//    memcpy(test_buf,(void *)0x18002000,sizeof(test_buf));
    while(1);
    return 0;
}

