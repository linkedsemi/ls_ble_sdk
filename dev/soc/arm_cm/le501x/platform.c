#define LOG_TAG "PLF"
#include <string.h>
#include <stdlib.h>
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
#include "reg_syscfg.h"
#include "lsecc.h"
#include "lstrng.h"
#include "field_manipulate.h"
#define BASEBAND_MEMORY_ADDR   (0x50004000)
#define IRQ_NVIC_PRIO(IRQn,priority) (((priority << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL) << _BIT_SHIFT(IRQn))

RESET_RETAIN uint32_t reset_reason;

DEF_BUILTIN_TIMER_ENV(SDK_SW_TIMER_MAX);

void stack_var_ptr_init(void);

void main_task_app_init(void);

void main_task_itf_init(void);


static void bb_mem_clr(void)
{
    memset((void *)BASEBAND_MEMORY_ADDR,0,16384);
}

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

void ble_irq_enable()
{
    //clear ble irq
    NVIC->ICPR[0] = 1<<BLE_IRQn | 1<<BLE_ERR_IRQn | 1<<BLE_FIFO_IRQn | (unsigned int)1<<BLE_CRYPT_IRQn;
    //enable ble irq
    NVIC->ISER[0] |= 1<<BLE_IRQn | 1<<BLE_ERR_IRQn | 1<<BLE_FIFO_IRQn | (unsigned int)1<<BLE_CRYPT_IRQn;
}

static uint32_t flash_data_storage_base_offset()
{
    extern uint32_t __stack_lma__;
    return ((uint32_t)&__stack_lma__ & ~0xfff) 
        - TINYFS_SECTION_NUM*TINYFS_SECTION_SIZE - LSQSPI_MEM_MAP_BASE_ADDR;
}

static void wkup_irq_enable()
{
    NVIC->ICPR[0] = 1<<BLE_WKUP_IRQn | 1<<LPWKUP_IRQn;
    NVIC->ISER[0] = 1<<BLE_WKUP_IRQn | 1<<LPWKUP_IRQn;
}

void irq_reinit()
{
    irq_priority();
    wkup_irq_enable();
}

static void irq_init()
{
    irq_reinit();
    ble_irq_enable();
}

static void mac_init()
{
    uint8_t mac_clk;
    uint32_t clk_cfg = RCC->CFG;
    if(REG_FIELD_RD(clk_cfg, RCC_SYSCLK_SW) == 0x4)
    {
        if(REG_FIELD_RD(clk_cfg, RCC_HCLK_SCAL) == 0x8 )
        {
            mac_clk = 1; //AHB 32M
        }else
        {
            mac_clk = 2; //AHB 64M
        }
    }else
    {
        mac_clk = 0; //AHB 16M
    }
    RCC->BLECFG = mac_clk<<RCC_BLE_CK_SEL_POS| 1<<RCC_BLE_MRST_POS | 1<<RCC_BLE_CRYPT_RST_POS | 1<<RCC_BLE_LCK_RST_POS | 1<<RCC_BLE_AHB_RST_POS | 1<<RCC_BLE_WKUP_RST_POS
        | 1<<RCC_BLE_LPWR_CKEN_POS | 1<<RCC_BLE_AHBEN_POS | 1<<RCC_BLE_MDM_REFCLK_CKEN_POS;
    RCC->BLECFG &= ~(1<<RCC_BLE_MRST_POS | 1<<RCC_BLE_CRYPT_RST_POS | 1<<RCC_BLE_LCK_RST_POS | 1<<RCC_BLE_AHB_RST_POS | 1<<RCC_BLE_WKUP_RST_POS);
}


uint32_t get_trng_random(void)
{
    uint32_t result_random=0;
    uint8_t cnt_trng=2;
    do
    {
        lstrng_init();
        lstrng_start();
        result_random = lstrng_getdata();
        lstrng_stop();
        lstrng_deinit();
    } while(--cnt_trng);

    return result_random;
 }

static void module_init()
{
    //TODO
    LOG_INIT();
    LOG_I("sys init");
    irq_init();
    srand(0);
    INIT_BUILTIN_TIMER_ENV();
    lsecc_init();
    lstrng_init();
    uint32_t random_val = get_trng_random();
    srand(random_val);
    calc_acc_init();
    cpu_sleep_recover_init();
    uint32_t base_offset = flash_data_storage_base_offset();
    tinyfs_init(base_offset);
    tinyfs_print_dir_tree();
    mac_init();
    modem_rf_init();
    low_power_mode_set(0);
}

static void analog_init()
{
    dcdc_on();
//    SYSCFG->ANACFG0 = 0x30100a78;
//    SYSCFG->ANACFG1 = 0xb0a30000;
}

static void var_init()
{
    stack_data_bss_init();
    bb_mem_clr();
    stack_var_ptr_init();
    spi_flash_drv_var_init(true,false);
}

void sys_init_itf()
{
    analog_init();
    var_init();
    main_task_itf_init();
    module_init();
}

void sys_init_app()
{
    analog_init();
    var_init();
    main_task_app_init();
    module_init();
}

void platform_reset(uint32_t error)
{
    reset_reason = error;
    while(1);
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

__attribute__((weak)) void SystemInit(){}

uint32_t plf_get_reset_error()
{
    return reset_reason;
}
