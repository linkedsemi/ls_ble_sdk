/**
 * Driver for flash program.
 */
#include "spi_flash.h"
#include "reg_base_addr.h"
#include "platform.h"
#include "io_config.h"
#include "cpu.h"
/**
 * ERROR TYPE. MUST NOT BE MODIFIED
 */
#define ERROR_INIT      -200
#define ERROR_READID    -201
#define ERROR_PROGRAM   -202
#define ERROR_READ      -203
#define ERROR_ERASE     -204
#define ERROR_CHIPERASE -205
#define ERROR_UNINIT	-206
#define ERROR_CHECKSUM  -207

/**
 * Customize this method to perform any initialization
 * needed to access your flash device.
 *
 * @return: if this method returns an error,MUST RUTURN ERROR_INIT,
 * Otherwise return 0.
 */
int  flashInit(){
    disable_global_irq();
    clk_switch();
    qspi_flash_io_init();
    spi_flash_drv_var_init(false,false);
    spi_flash_init();
    spi_flash_software_reset();
    DELAY_US(500);
    spi_flash_release_from_deep_power_down();
    DELAY_US(100);
    spi_flash_qe_status_read_and_set();
    return 0;
}

/**
 * Customize this method to perform any un-initialization
 * needed to access your flash device.
 *
 * @return: if this method returns an error,MUST RUTURN ERROR_UNINIT,
 * Otherwise return 0.
 */
int  flashUnInit(){
    return 0;
}

/**
 * Customize this method to read flash ID
 *
 * @param flashID: returns for flash ID
 *
 * @return: if this method returns an error,MUST RUTURN ERROR_READID,
 * Otherwise return 0.
 */
int  flashID(unsigned int* flashID){
    spi_flash_read_id((uint8_t *)flashID);
    return 0;
}

/**
 * This method takes the data pointed to by the src parameter
 * and writes it to the flash blocks indicated by the
 * dst parameter.
 *
 * @param dst : destination address where flash program
 * @param src : address of data
 * @param length : data length
 *
 * @return : if this method returns an error,MUST RUTURN ERROR_PROGRAM,
 * Otherwise return 0.
 */
int flashProgram(char* dst, char *src, int size){
    uint32_t current = (uint32_t)dst;
    uint8_t *data = (uint8_t *)src;
    uint16_t length;
    if(current % 256)
    {
        length = size > 256 - current % 256 ? 256 - current % 256 : size;
    }else
    {
        length = 0;
    }
    if(length)
    {
        spi_flash_quad_page_program(current - FLASH_BASE_ADDR,(void *)data,length);
        size -= length;
        current += length;
        data = (uint8_t *)data + length; 
    }
    while(size)
    {
        length = size > 256 ? 256 : size;
        spi_flash_quad_page_program(current - FLASH_BASE_ADDR,(void *)data,length);
        size -= length;
        current += length;
        data = (uint8_t *)data + length; 
    }
    return 0;
}

/**
 * Customize this method to read data from a group of flash blocks into a buffer
 *
 * @param dst : reads the contents of those flash blocks into the address pointed to by
 * the dst parameter.
 * @param src : a pointer to a single flash.
 * @param length : data length
 *
 *  @return: if this method returns an error,MUST RUTURN ERROR_READ,
 * Otherwise return 0.
 */
int flashRead(char* dst, char *src, int length){
    spi_flash_quad_io_read((uint32_t)src - FLASH_BASE_ADDR,(uint8_t *)dst,length);
    return 0;
}

/**
 * Customize this method to erase a group of flash blocks.
 *
 * @param dst : a pointer to the base of the flash device.
 * NOTE: dst will always be sector aligned, the sector size is stored in FlashDev.c#FlashDevices#Devs#PageSize
 * @param length : erase length
 * NOTE: length will always be sector aligned, the sector size is stored in FlashDev.c#FlashDevices#Devs#PageSize
 *
 * @return : if this method returns an error,MUST RUTURN ERROR_ERASE,
 * Otherwise return 0
 */
int flashErase(char *dst, int size){
    uint32_t offset = (uint32_t)dst;
    while(size)
    {
        spi_flash_sector_erase(offset);
        if(size > FLASH_SECTOR_SIZE)
        {
            size -= FLASH_SECTOR_SIZE;
            offset += FLASH_SECTOR_SIZE;
        }else
        {
            break;
        }
    }
    return 0;
}

/**
 * Customize this method to erase the whole flash.
 *
 * @return : if this method returns an error,MUST RUTURN ERROR_CHIPERASE,
 * Otherwise return 0.
 */
int flashChipErase( ){
    spi_flash_chip_erase();
    return 0;
}

/**
 * Customize this method to make the veryfiy process more quickly.
 * 
 * @param dst : a pointer to the base of the flash device.
 * NOTE: dst will always be 4 aligned.
 * @param length : the lenght of the data which will be used for checksum
 * NOTE: the length will always be 4 aligned.
 * @param checksum : the expected checksum value in the programmed file(ihex,bin,elf format)
 * 
 * @return : if the specified data's checksum from dst and lenght is checksum, return 0, else return ERROR_CHECKSUM
 * @example if the flash can be read directly, you can copy following code to replace current "return 0;"
 *
  int i, sum = 0;
  for (i = 0; i < length; i++) {
   sum += dst[i];
  }
  return sum == checksum ? 0 : ERROR_CHECKSUM;
 * 
 */
int flashChecksum(char*dst, int length, int checksum) {
    int i, sum = 0;
    spi_flash_xip_start();
    for (i = 0; i < length; i++) {
        sum += dst[i];
    }
    spi_flash_xip_stop();
    return sum == checksum ? 0 : ERROR_CHECKSUM;
}

// NOTING: when debug the driver, this macro defined as 1, and then
// it must be set as 0, for release to flash programmer library
#define DEBUG_DRIVER	0

/**
 * Debug entry for driver.
 *
 * @return : if this method returns an error,MUST RUTURN ERROR_CHIPERASE,
 * Otherwise return 0.
 */
int flashTest(){
#if DEBUG_DRIVER

	unsigned int ID;
	// read flash id
	flashID(&ID);

    flashProgram(FLASH_BASE_ADDR,0x780000,0x1000);
    // other drivers test
    return ID;
#else
	return 0;
#endif
}

void __bkpt_label()
{
    __asm("ebreak"::);
}

void __continue_label(){}