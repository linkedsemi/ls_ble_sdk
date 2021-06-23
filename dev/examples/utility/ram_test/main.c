#include "spi_flash.h"
#include "spi_flash_int.h"
#include <string.h>
#include <stdlib.h>
#include "field_manipulate.h"
#include "cpu.h"

int main_flash()
{
    uint8_t status[2];
    disable_global_irq();
    spi_flash_drv_var_init(false,false);
    spi_flash_init();
    spi_flash_software_reset();
 //   spi_flash_write_status_register(0);
    spi_flash_qe_status_read_and_set();
    spi_flash_read_status_register_0(&status[0]);
    spi_flash_read_status_register_1(&status[1]);
    //spi_flash_write_status_register(status[1]<<8|status[0]&~4<<2);
//    spi_flash_sector_erase(0x2000);
//    spi_flash_chip_erase();
//    for(uint32_t i=0;i<1024;i+=4)
//    {
//        *(uint32_t *)&test_buf[i] = rand();
//    }
//    spi_flash_quad_page_program(0,test_buf,256);
//    spi_flash_quad_io_read(0,dst,sizeof(dst));
//    spi_flash_xip_start();

//    memcpy(test_buf,(void *)0x18002000,sizeof(test_buf));
    while(1);
}


int main()
{

    //main_flash();
    while(1);
}
