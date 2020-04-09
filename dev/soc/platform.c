#define LOG_TAG "PLF"
#include <string.h>
#include "platform.h"
#include "calc_div.h"
#include "cpu.h"
#include "tinyfs_config.h"
#include "tinyfs.h"
#include "sleep.h"
#include "le501x.h"
#include "log.h"
#include "lsqspi_param.h"

void stack_var_ptr_init(void);

void main_task_app_init(void);

void main_task_itf_init(void);

static void irq_priority()
{
    __NVIC_SetPriority(SVCall_IRQn,1);
    __NVIC_SetPriority(EXTI_IRQn,2);
    __NVIC_SetPriority(WWDT_IRQn,2);
    __NVIC_SetPriority(LPWKUP_IRQn,2);
    __NVIC_SetPriority(BLE_IRQn,0);
    __NVIC_SetPriority(RTC_IRQn,2);
    __NVIC_SetPriority(DMA_IRQn,2);
    __NVIC_SetPriority(QSPI_IRQn,2);
    __NVIC_SetPriority(ECC_IRQn,2);
    __NVIC_SetPriority(CACHE_IRQn,2);
    __NVIC_SetPriority(TRNG_IRQn,2);
    __NVIC_SetPriority(IWDT_IRQn,2);
    __NVIC_SetPriority(CRYPT_IRQn,2);
    __NVIC_SetPriority(PDM_IRQn,2);
    __NVIC_SetPriority(BLE_WKUP_IRQn,0);
    __NVIC_SetPriority(ADC_IRQn,2);
    __NVIC_SetPriority(ADTIM1_IRQn,2);
    __NVIC_SetPriority(BSTIM1_IRQn,2);
    __NVIC_SetPriority(GPTIMA1_IRQn,2);
    __NVIC_SetPriority(GPTIMB1_IRQn,2);
    __NVIC_SetPriority(BLE_ERR_IRQn,0);
    __NVIC_SetPriority(LVD33_IRQn,2);
    __NVIC_SetPriority(GPTIMC1_IRQn,2);
    __NVIC_SetPriority(LPTIM_IRQn,2);
    __NVIC_SetPriority(I2C1_IRQn,2);
    __NVIC_SetPriority(I2C2_IRQn,2);
    __NVIC_SetPriority(SPI1_IRQn,2);
    __NVIC_SetPriority(SPI2_IRQn,2);
    __NVIC_SetPriority(BLE_FIFO_IRQn,0);
    __NVIC_SetPriority(BLE_CRYPT_IRQn,0);
}

static void stack_data_bss_init()
{
    extern uint32_t __stack_bss_start__;
    extern uint32_t __stack_bss_size__;
    memset(&__stack_bss_start__,0,(uint32_t)&__stack_bss_size__);
    extern uint32_t __stack_data_lma__;
    extern uint32_t __stack_data_start__;
    extern uint32_t __stack_data_size__;
    memcpy(&__stack_data_start__,&__stack_data_lma__,(uint32_t)&__stack_data_size__);
}

static void ble_irq_enable()
{
    __NVIC_ClearPendingIRQ(BLE_IRQn);
    __NVIC_EnableIRQ(BLE_IRQn);
    __NVIC_ClearPendingIRQ(BLE_ERR_IRQn);
    __NVIC_EnableIRQ(BLE_ERR_IRQn);
    __NVIC_ClearPendingIRQ(BLE_WKUP_IRQn);
    __NVIC_EnableIRQ(BLE_WKUP_IRQn);
    __NVIC_ClearPendingIRQ(BLE_FIFO_IRQn);
    __NVIC_EnableIRQ(BLE_FIFO_IRQn);
    __NVIC_ClearPendingIRQ(BLE_CRYPT_IRQn);
    __NVIC_EnableIRQ(BLE_CRYPT_IRQn);

}

static uint32_t flash_data_storage_base_offset()
{
    extern uint32_t __stack_lma__;
    return ((uint32_t)&__stack_lma__ & ~0xfff) 
        - TINYFS_SECTION_NUM*TINYFS_SECTION_SIZE - LSQSPI_MEM_MAP_BASE_ADDR;
}

void sys_init(bool app)
{
    stack_data_bss_init();
    stack_var_ptr_init();
    if(app)
    {
        main_task_app_init();
    }else
    {
        main_task_itf_init();
    }
    //TODO
    LOG_INIT();
    LOG_I("sys init");
    irq_priority();
    ble_irq_enable();
    cpu_sleep_recover_init();
    uint32_t base_offset = flash_data_storage_base_offset();
    tinyfs_init(base_offset);
    
}

void uart_eif_read(uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy)
{


}

void uart_eif_write(uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy)
{


}

void uart_eif_flow_on(void)
{

}

bool uart_eif_flow_off(void)
{
    return true;
}

void platform_reset(uint32_t error)
{

}

void ecc_calc_start(const uint8_t* secret_key,const uint8_t* pub_x,const uint8_t* pub_y,uint8_t* result_x,uint8_t* result_y,void (*cb)(void *),void *param)
{

}


void true_rand_init(unsigned int seed)
{


}

int true_rand_gen(void)
{

    return 0;
}

uint64_t idiv_acc(uint32_t dividend,uint32_t divisor,bool signed_int)
{
    uint64_t retval;
    struct {
        uint32_t r0;
        uint32_t r1;
    }*ret_ptr = (void *)&retval;
    enter_critical();
    calc_div(dividend, divisor, signed_int,&ret_ptr->r0,&ret_ptr->r1);
    exit_critical();
    return retval;
}

void SystemInit()
{

}

uint32_t plf_get_reset_error()
{

    return 0;
}
