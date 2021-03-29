#include <string.h>
#include "platform.h"
#include "ARMCM3.h"
#include "reg_sysc_awo.h"
#include "field_manipulate.h"
#include "io_config.h"
#include "swint_call_asm.h"
#include "reg_sysc_ble.h"
#include "tinyfs.h"
#include "spi_flash.h"
#include "compile_flag.h"
#include "reg_v33_rg.h"


void modem_rf_init(void);
#define ISR_VECTOR_ADDR ((uint32_t *)(0x400000))

void ble_pkt_isr(void);
void ble_util_isr(void);
void SWINT_Handler_ASM(void);
void swint2_process(void);
void main_task_app_init(void);
void main_task_itf_init(void);
void stack_var_ptr_init(void);

__attribute__((weak)) void SystemInit(){
    SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
    SCB->VTOR = (uint32_t)ISR_VECTOR_ADDR;
}

static void irq_priority()
{
    __NVIC_SetPriority(SVCall_IRQn,1);
    __NVIC_SetPriority(PendSV_IRQn,7);

    __NVIC_SetPriority(RTC1_IRQn,7);
    __NVIC_SetPriority(IWDT_IRQn,7);
    __NVIC_SetPriority(RTC2_IRQn,7);
    __NVIC_SetPriority(EXT_IRQn,7);
    __NVIC_SetPriority(LVD33_IRQn,7);
    __NVIC_SetPriority(MAC_LP_IRQn,0);
    __NVIC_SetPriority(MAC1_IRQn,0);
    __NVIC_SetPriority(DMA_IRQn,7);
    __NVIC_SetPriority(QSPI_IRQn,7);
    __NVIC_SetPriority(SWINT1_IRQn,1);
    __NVIC_SetPriority(CACHE_IRQn,7);
    __NVIC_SetPriority(I8080_IRQn,7);
    __NVIC_SetPriority(TRNG_IRQn,7);
    __NVIC_SetPriority(ECC_IRQn,7);
    __NVIC_SetPriority(CRYPT_IRQn,7);
    __NVIC_SetPriority(BSTIM1_IRQn,7);
    __NVIC_SetPriority(GPTIMA1_IRQn,7);
    __NVIC_SetPriority(GPTIMB1_IRQn,7);
    __NVIC_SetPriority(GPTIMC1_IRQn,7);
    __NVIC_SetPriority(ADTIM1_IRQn,7);
    __NVIC_SetPriority(I2C1_IRQn,7);
    __NVIC_SetPriority(I2C2_IRQn,7);
    __NVIC_SetPriority(I2C3_IRQn,7);
    __NVIC_SetPriority(UART1_IRQn,7);
    __NVIC_SetPriority(UART2_IRQn,7);
    __NVIC_SetPriority(UART3_IRQn,7);
    __NVIC_SetPriority(SPI1_IRQn,7);
    __NVIC_SetPriority(SPI2_IRQn,7);
    __NVIC_SetPriority(PDM_IRQn,7);
    __NVIC_SetPriority(GPIO_IRQn,7);
    __NVIC_SetPriority(WWDT_IRQn,7);
    __NVIC_SetPriority(SWINT2_IRQn,1);
    __NVIC_SetPriority(LCD_IRQn,7);
    __NVIC_SetPriority(ADC_IRQn,7);
    __NVIC_SetPriority(DAC_IRQn,7);
    __NVIC_SetPriority(MAC2_IRQn,2);
    __NVIC_SetPriority(ADC24_IRQn,7);

}

void arm_cm_set_int_isr(uint8_t type,void (*isr)())
{
    ISR_VECTOR_ADDR[type + 16] = (uint32_t)isr;
}

static bool lock_check()
{
    if(REG_FIELD_RD(SYSC_AWO->ANA_STAT,SYSC_AWO_DPLL_LOCK)==0)
    {
        return false;
    }
    DELAY_US(1000);
    return REG_FIELD_RD(SYSC_AWO->ANA_STAT,SYSC_AWO_DPLL_LOCK);
}

void pll_enable()
{
    while(lock_check()==false)
    {
        REG_FIELD_WR(SYSC_AWO->PD_AWO_ANA0,SYSC_AWO_AWO_EN_DPLL,0);
        SYSC_AWO->PD_AWO_ANA0 |= SYSC_AWO_AWO_DPLL_SEL_REF_24M_MASK | SYSC_AWO_AWO_EN_QCLK_MASK | SYSC_AWO_AWO_EN_DPLL_16M_RF_MASK | SYSC_AWO_AWO_EN_DPLL_128M_RF_MASK | SYSC_AWO_AWO_EN_DPLL_128M_EXT_MASK;
        REG_FIELD_WR(SYSC_AWO->PD_AWO_ANA0,SYSC_AWO_AWO_EN_DPLL,1);
        DELAY_US(100);
        REG_FIELD_WR(SYSC_AWO->PD_AWO_ANA0,SYSC_AWO_AWO_EN_DPLL,0);
        REG_FIELD_WR(SYSC_AWO->PD_AWO_ANA0,SYSC_AWO_AWO_DPLL_SEL_REF_24M,0);
        REG_FIELD_WR(SYSC_AWO->PD_AWO_ANA0,SYSC_AWO_AWO_EN_DPLL,1);
        DELAY_US(100);
    }
}
XIP_BANNED bool clk_check();
#if (SDK_HCLK_MHZ==16)
XIP_BANNED void switch_to_xo16m()
{
    REG_FIELD_WR(SYSC_AWO->PD_AWO_CLK_CTRL,SYSC_AWO_CLK_SEL_HBUS_L0,2);//16M
}
XIP_BANNED bool clk_check()
{
    return REG_FIELD_RD(SYSC_AWO->PD_AWO_CLK_CTRL,SYSC_AWO_CLK_SEL_HBUS_L0) == 2 ;
}

XIP_BANNED void clk_switch()
{
    pll_enable();
    switch_to_xo16m();
}

#else

XIP_BANNED  void switch_to_pll()
{
    REG_FIELD_WR(SYSC_AWO->PD_AWO_CLK_CTRL,SYSC_AWO_CLK_SEL_HBUS_L0,4);//dpll enable
}
#if (SDK_HCLK_MHZ==128)
XIP_BANNED bool clk_check()
{
    return REG_FIELD_RD(SYSC_AWO->PD_AWO_CLK_CTRL, SYSC_AWO_CLK_SEL_HBUS_L0) == 4 && REG_FIELD_RD(SYSC_AWO->PD_AWO_CLK_CTRL,SYSC_AWO_CLK_SEL_HBUS_L1) == 0x1;
}
XIP_BANNED void clk_switch()
{
    pll_enable();
    REG_FIELD_WR(SYSC_AWO->PD_AWO_CLK_CTRL,SYSC_AWO_CLK_SEL_HBUS_L1,1);
    switch_to_pll();
}
#else
XIP_BANNED void dpll_div(uint8_t hclk_scal)
{
    REG_FIELD_WR(SYSC_AWO->PD_AWO_CLKG_SRST,SYSC_AWO_CLKG_CLR_DIV_HBUS,1);
    REG_FIELD_WR(SYSC_AWO->PD_AWO_CLK_CTRL,SYSC_AWO_CLK_DIV_PARA_HBUS_M1,hclk_scal-1);
    REG_FIELD_WR(SYSC_AWO->PD_AWO_CLKG_SRST,SYSC_AWO_CLKG_SET_DIV_HBUS,1);
    REG_FIELD_WR(SYSC_AWO->PD_AWO_CLK_CTRL,SYSC_AWO_CLK_SEL_HBUS_L1,2);
}
#if(SDK_HCLK_MHZ==32)
XIP_BANNED bool clk_check()
{
    return REG_FIELD_RD(SYSC_AWO->PD_AWO_CLK_CTRL, SYSC_AWO_CLK_SEL_HBUS_L0) == 4 && REG_FIELD_RD(SYSC_AWO->PD_AWO_CLK_CTRL,SYSC_AWO_CLK_DIV_PARA_HBUS_M1) == 0x2;
}
XIP_BANNED void clk_switch()
{
    pll_enable();
    dpll_div(4);
    switch_to_pll();
}
#elif(SDK_HCLK_MHZ==64)
XIP_BANNED bool clk_check()
{
    return REG_FIELD_RD(SYSC_AWO->PD_AWO_CLK_CTRL, SYSC_AWO_CLK_SEL_HBUS_L0) == 4 && REG_FIELD_RD(SYSC_AWO->PD_AWO_CLK_CTRL,SYSC_AWO_CLK_DIV_PARA_HBUS_M1) == 0x2;
}
XIP_BANNED void clk_switch()
{
    pll_enable();
    dpll_div(2);
    switch_to_pll();
}
#endif 
#endif
#endif

static void ble_irq_config()
{
    arm_cm_set_int_isr(MAC1_IRQn,ble_pkt_isr);
    arm_cm_set_int_isr(MAC2_IRQn,ble_util_isr);
    arm_cm_set_int_isr(SWINT1_IRQn,SWINT_Handler_ASM);
    arm_cm_set_int_isr(SWINT2_IRQn,swint2_process);
    __NVIC_EnableIRQ(MAC1_IRQn);
    __NVIC_EnableIRQ(MAC2_IRQn);
    __NVIC_EnableIRQ(SWINT1_IRQn);
    __NVIC_EnableIRQ(SWINT2_IRQn);
}

static void module_init()
{
    SYSC_AWO->PIN_SEL3 = FIELD_BUILD(SYSC_AWO_MAC_DBG_EN, 0xFFFF);
    //SYSC_AWO->PIN_SEL3 = FIELD_BUILD(SYSC_AWO_MDM_DBG_EN, 0xFFFF);
    SYSC_BLE->PD_BLE_CLKG = SYSC_BLE_CLKG_SET_MAC_MASK | SYSC_BLE_CLKG_SET_MDM_MASK | SYSC_BLE_CLKG_SET_RF_MASK;
    io_cfg_output(PB02);
    io_cfg_output(PB03);
    io_cfg_output(PB04);
    irq_priority();
    ble_irq_config();
    tinyfs_init(0x802000);
    modem_rf_init();
}

static void analog_init()
{
    dcdc_init();
    if(clk_check()==false)
    {
        clk_switch();
    }
    REG_FIELD_WR(SYSC_AWO->PD_AWO_ANA0,SYSC_AWO_AWO_BG_RES_TRIM,0x33);
    REG_FIELD_WR(SYSC_AWO->PD_AWO_ANA2,SYSC_AWO_AWO_BG_RBIAS_TRIM,0xf);
    MODIFY_REG(SYSC_AWO->PD_AWO_ANA1,SYSC_AWO_AWO_XO16M_LP_MASK|SYSC_AWO_AWO_XO16M_CAP_TRIM_MASK,1<<SYSC_AWO_AWO_XO16M_LP_POS | 7<<SYSC_AWO_AWO_XO16M_CAP_TRIM_POS);

}

static void host_bss_init()
{
    extern uint32_t __stack_bss_start__;
    extern uint32_t __stack_bss_end__;
    memset(&__stack_bss_start__,0,(uint32_t)&__stack_bss_end__-(uint32_t)&__stack_bss_start__);
}

static void var_init()
{
    host_bss_init();
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

void dcdc_init()
{
    V33_RG->DCDC0 = FIELD_BUILD(V33_RG_DCDC_TEST,0)
                    |FIELD_BUILD(V33_RG_DCDC_DELAY,1)
                    |FIELD_BUILD(V33_RG_DCDC_EN_OVC,1)
                    |FIELD_BUILD( V33_RG_DCDC_EN_ZOC,0)
                    |FIELD_BUILD(V33_RG_DCDC_RAMP_I_HALF,0)
                    |FIELD_BUILD(V33_RG_DCDC_OUT_ADJ,4);  //0-7  mean 1.1 - 1.8
    V33_RG->DCDC1 = FIELD_BUILD(V33_RG_DCDC_PSW_ADJ,3)
                    |FIELD_BUILD(V33_RG_DCDC_NSW_ADJ,3)
                    |FIELD_BUILD(V33_RG_DCDC_VBG_VCTL,8);
    V33_RG->DCDC2 = FIELD_BUILD(V33_RG_DCDC_VCTL,13)
                    |FIELD_BUILD(V33_RG_DCDC_OVC_CTL,3);
    V33_RG->DCDC3 = FIELD_BUILD(V33_RG_DCDC_PWM_FI,4)
                    |FIELD_BUILD(V33_RG_DCDC_PWM_FC,4);
    V33_RG->DCDC4 = FIELD_BUILD(V33_RG_DCDC_PWM_HYS_CTL,4)
                    |FIELD_BUILD(V33_RG_DCDC_PFM_HYS_CTL,4);
}

void dcdc_switch_to_pwm()
{
    V33_RG->PMU_SET_VAL = FIELD_BUILD(V33_RG_CLK_SET_LSE,1)
                          |FIELD_BUILD(V33_RG_CLK_SET_HSI,1)
                          |FIELD_BUILD(V33_RG_CLK_SET_HSE,1)
                          |FIELD_BUILD(V33_RG_CLK_SET_HSE_BUF,1)
                          |FIELD_BUILD(V33_RG_PMU_SET_DCDC_BYP,1)
                          |FIELD_BUILD(V33_RG_PMU_SET_DCDC_PWM,1)
                          |FIELD_BUILD(V33_RG_PMU_SET_DCDC_PFM,0)
                          |FIELD_BUILD(V33_RG_PMU_SET_DCDC_FLT,0);
    V33_RG->PMU_SET_TGGL = FIELD_BUILD(V33_RG_PMU_SET_TGGL,!REG_FIELD_RD(V33_RG->PMU_SET_TGGL,V33_RG_PMU_SET_TGGL));
}

void dcdc_switch_to_pfm()
{
    V33_RG->DCDC0 = FIELD_BUILD(V33_RG_DCDC_TEST,0)
                    |FIELD_BUILD(V33_RG_DCDC_DELAY,1)
                    |FIELD_BUILD(V33_RG_DCDC_EN_OVC,1)
                    |FIELD_BUILD( V33_RG_DCDC_EN_ZOC,1)
                    |FIELD_BUILD(V33_RG_DCDC_RAMP_I_HALF,0)
                    |FIELD_BUILD(V33_RG_DCDC_OUT_ADJ,4);
    V33_RG->PMU_SET_VAL = FIELD_BUILD(V33_RG_CLK_SET_LSE,1)
                          |FIELD_BUILD(V33_RG_CLK_SET_HSI,1)
                          |FIELD_BUILD(V33_RG_CLK_SET_HSE,1)
                          |FIELD_BUILD(V33_RG_CLK_SET_HSE_BUF,1)
                          |FIELD_BUILD(V33_RG_PMU_SET_DCDC_BYP,1)
                          |FIELD_BUILD(V33_RG_PMU_SET_DCDC_PWM,0)
                          |FIELD_BUILD(V33_RG_PMU_SET_DCDC_PFM,1)
                          |FIELD_BUILD(V33_RG_PMU_SET_DCDC_FLT,0);
    V33_RG->PMU_SET_TGGL = FIELD_BUILD(V33_RG_PMU_SET_TGGL,!REG_FIELD_RD(V33_RG->PMU_SET_TGGL,V33_RG_PMU_SET_TGGL));
}

void dcdc_switch_to_bypass()
{
    V33_RG->PMU_SET_VAL = FIELD_BUILD(V33_RG_CLK_SET_LSE,1)
                          |FIELD_BUILD(V33_RG_CLK_SET_HSI,1)
                          |FIELD_BUILD(V33_RG_CLK_SET_HSE,1)
                          |FIELD_BUILD(V33_RG_CLK_SET_HSE_BUF,1)
                          |FIELD_BUILD(V33_RG_PMU_SET_DCDC_BYP,0)
                          |FIELD_BUILD(V33_RG_PMU_SET_DCDC_PWM,0)
                          |FIELD_BUILD(V33_RG_PMU_SET_DCDC_PFM,0)
                          |FIELD_BUILD(V33_RG_PMU_SET_DCDC_FLT,0);
    V33_RG->PMU_SET_TGGL = FIELD_BUILD(V33_RG_PMU_SET_TGGL,!REG_FIELD_RD(V33_RG->PMU_SET_TGGL,V33_RG_PMU_SET_TGGL));
}

void sys_init_none()
{
    analog_init();
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

