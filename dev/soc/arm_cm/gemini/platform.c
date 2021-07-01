#include <string.h>
#include "platform.h"
#include "gemini.h"
#include "field_manipulate.h"
#include "io_config.h"
#include "spi_flash.h"
#include "compile_flag.h"
#include "reg_sysc_awo_type.h"

#define ISR_VECTOR_ADDR ((uint32_t *)(0x20000000))

__attribute__((weak)) void SystemInit(){
    SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
    SCB->VTOR = (uint32_t)ISR_VECTOR_ADDR;
}

void clk_switch()
{

}

void irq_priority()
{

}

void arm_cm_set_int_isr(uint8_t type,void (*isr)())
{
    ISR_VECTOR_ADDR[type + 16] = (uint32_t)isr;
}

void sys_init_none()
{
    spi_flash_drv_var_init(true,false);
    io_init();
}

void platform_reset(uint32_t error)
{

}

#define APP_IMAGE_BASE_OFFSET (0x1C)
#define FOTA_IMAGE_BASE_OFFSET (0x28)
#define DATA_STORAGE_BASE_OFFSET (0x2c)

uint32_t config_word_get(uint32_t offset)
{
    uint32_t data;
    spi_flash_quad_io_read(offset,(uint8_t *)&data,sizeof(data));
    return data;
}

uint32_t get_app_image_base()
{
    return config_word_get(APP_IMAGE_BASE_OFFSET);
}

