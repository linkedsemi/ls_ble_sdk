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
#include "spi_flash.h"
#include "compile_flag.h"
#include "lscache.h"
#include "reg_rcc.h"
#include "modem_rf_le501x.h"
#include "calc_acc.h"
#include "builtin_timer.h"
#define IRQ_NVIC_PRIO(IRQn,priority) (((priority << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL) << _BIT_SHIFT(IRQn))

RESET_RETAIN uint32_t reset_reason;

void stack_var_ptr_init(void);

void main_task_app_init(void);

void main_task_itf_init(void);

static void irq_priority()
{
    __NVIC_SetPriority(SVCall_IRQn,1);
    NVIC->IP[0] = IRQ_NVIC_PRIO(EXTI_IRQn,2) | IRQ_NVIC_PRIO(WWDT_IRQn,2) | IRQ_NVIC_PRIO(LPWKUP_IRQn,2) | IRQ_NVIC_PRIO(BLE_IRQn,0);
    NVIC->IP[1] = IRQ_NVIC_PRIO(RTC_IRQn,2) | IRQ_NVIC_PRIO(DMA_IRQn,2) | IRQ_NVIC_PRIO(QSPI_IRQn,2) | IRQ_NVIC_PRIO(ECC_IRQn,2);
    NVIC->IP[2] = IRQ_NVIC_PRIO(CACHE_IRQn,2) | IRQ_NVIC_PRIO(TRNG_IRQn,2) | IRQ_NVIC_PRIO(IWDT_IRQn,2) | IRQ_NVIC_PRIO(CRYPT_IRQn,2);
    NVIC->IP[3] = IRQ_NVIC_PRIO(PDM_IRQn,2) | IRQ_NVIC_PRIO(BLE_WKUP_IRQn,0) | IRQ_NVIC_PRIO(ADC_IRQn,2) | IRQ_NVIC_PRIO(ADTIM1_IRQn,2);
    NVIC->IP[4] = IRQ_NVIC_PRIO(BSTIM1_IRQn,2) | IRQ_NVIC_PRIO(GPTIMA1_IRQn,2) | IRQ_NVIC_PRIO(GPTIMB1_IRQn,2) | IRQ_NVIC_PRIO(BLE_ERR_IRQn,0);
    NVIC->IP[5] = IRQ_NVIC_PRIO(LVD33_IRQn,2) | IRQ_NVIC_PRIO(GPTIMC1_IRQn,2) | IRQ_NVIC_PRIO(LPTIM_IRQn,2) | IRQ_NVIC_PRIO(I2C1_IRQn,2);
    NVIC->IP[6] = IRQ_NVIC_PRIO(I2C2_IRQn,2) | IRQ_NVIC_PRIO(SPI1_IRQn,2) | IRQ_NVIC_PRIO(SPI2_IRQn,2) | IRQ_NVIC_PRIO(UART1_IRQn,2);
    NVIC->IP[7] = IRQ_NVIC_PRIO(UART2_IRQn,2) | IRQ_NVIC_PRIO(UART3_IRQn,2) | IRQ_NVIC_PRIO(BLE_FIFO_IRQn,0) | IRQ_NVIC_PRIO(BLE_CRYPT_IRQn,0);
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
    //clear ble irq
    NVIC->ICPR[0] = 1<<BLE_IRQn | 1<<BLE_ERR_IRQn | 1<<BLE_WKUP_IRQn | 1<<BLE_FIFO_IRQn | 1<<BLE_CRYPT_IRQn;
    //enable ble irq
    NVIC->ISER[0] = 1<<BLE_IRQn | 1<<BLE_ERR_IRQn | 1<<BLE_WKUP_IRQn | 1<<BLE_FIFO_IRQn | 1<<BLE_CRYPT_IRQn;
}

static uint32_t flash_data_storage_base_offset()
{
    extern uint32_t __stack_lma__;
    return ((uint32_t)&__stack_lma__ & ~0xfff) 
        - TINYFS_SECTION_NUM*TINYFS_SECTION_SIZE - LSQSPI_MEM_MAP_BASE_ADDR;
}

void irq_init()
{
    irq_priority();
    ble_irq_enable();
}

static void mac_init()
{
    RCC->BLECFG = 1<<RCC_BLE_MRST_POS | 1<<RCC_BLE_CRYPT_RST_POS | 1<<RCC_BLE_LCK_RST_POS | 1<<RCC_BLE_AHB_RST_POS | 1<<RCC_BLE_WKUP_RST_POS
        | 1<<RCC_BLE_LPWR_CKEN_POS | 1<<RCC_BLE_AHBEN_POS | 1<<RCC_BLE_MDM_REFCLK_CKEN_POS;
    RCC->BLECFG &= ~(1<<RCC_BLE_MRST_POS | 1<<RCC_BLE_CRYPT_RST_POS | 1<<RCC_BLE_LCK_RST_POS | 1<<RCC_BLE_AHB_RST_POS | 1<<RCC_BLE_WKUP_RST_POS);
}

static void module_init()
{
    //TODO
    LOG_INIT();
    LOG_I("sys init");
    irq_init();
    calc_acc_init();
    cpu_sleep_recover_init();
    uint32_t base_offset = flash_data_storage_base_offset();
    tinyfs_init(base_offset);
    mac_init();
    modem_rf_init();
}

static void var_init()
{
    stack_data_bss_init();
    stack_var_ptr_init();
    spi_flash_drv_var_init();
}

void sys_init_itf()
{
    var_init();
    main_task_itf_init();
    module_init();
}

void sys_init_app()
{
    var_init();
    main_task_app_init();
    module_init();
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
    reset_reason = error;
    while(1);
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
    dcdc_on();
}

uint32_t plf_get_reset_error()
{
    return reset_reason;
}
