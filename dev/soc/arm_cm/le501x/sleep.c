#include "sdk_config.h"
#include "le501x.h"
#include "spi_flash.h"
#include "compile_flag.h"
#include "lscache.h"
#include "platform.h"
#include "modem_rf_le501x.h"
#include "reg_syscfg.h"
#include "field_manipulate.h"
#include "sleep.h"
#include "reg_rcc.h"
#include "reg_lsgpio.h"
#include "ls_dbg.h"
#define ISR_VECTOR_ADDR ((uint32_t *)(0x0))

bool waiting_ble_wkup_irq;

void cpu_sleep_asm(void);

void cpu_recover_asm(void);

#if SDK_DCDC_BYPASS
XIP_BANNED void dcdc_off(){}
XIP_BANNED void dcdc_on(){}
#else
XIP_BANNED void dcdc_off()
{
    REG_FIELD_WR(SYSCFG->DCDC, SYSCFG_EN, 0);
}

XIP_BANNED void dcdc_on()
{
    REG_FIELD_WR(SYSCFG->DCDC, SYSCFG_EN, 1);
}

#endif

uint8_t get_deep_sleep_enable(void)
{
    return SDK_DEEP_SLEEP_ENABLE;
}

XIP_BANNED void before_wfi()
{
    while(REG_FIELD_RD(SYSCFG->PMU_PWR, SYSCFG_BLE_PWR3_ST));
    switch_to_xo16m();
    SYSCFG->ANACFG0 &= ~(SYSCFG_EN_DPLL_MASK | SYSCFG_EN_DPLL_16M_RF_MASK | SYSCFG_EN_DPLL_128M_RF_MASK | SYSCFG_EN_DPLL_128M_EXT_MASK | SYSCFG_EN_QCLK_MASK);
    MODIFY_REG(SYSCFG->ANACFG1,SYSCFG_XO16M_ADJ_MASK | SYSCFG_XO16M_LP_MASK,
        3<<SYSCFG_XO16M_ADJ_POS | 0<<SYSCFG_XO16M_LP_POS);
    dcdc_off();
}

XIP_BANNED static void wait_dpll_lock()
{
    uint32_t i = 0;
    while(1)
    {
        if(REG_FIELD_RD(SYSCFG->ANACFG0, SYSCFG_DPLL_LOCK))
        {

            break;
        }
        if(i>10)
        {
            break;
        }
        i += 1;
    }
}

XIP_BANNED void after_wfi()
{
    dcdc_on();
    MODIFY_REG(SYSCFG->ANACFG1,SYSCFG_XO16M_ADJ_MASK | SYSCFG_XO16M_LP_MASK,
        0<<SYSCFG_XO16M_ADJ_POS | 1<<SYSCFG_XO16M_LP_POS);
    SYSCFG->ANACFG0 |= (SYSCFG_EN_DPLL_MASK | SYSCFG_EN_DPLL_16M_RF_MASK | SYSCFG_EN_DPLL_128M_RF_MASK | SYSCFG_EN_DPLL_128M_EXT_MASK | SYSCFG_EN_QCLK_MASK);
    wait_dpll_lock();
    clk_switch();
    LS_RAM_ASSERT(REG_FIELD_RD(SYSCFG->PMU_PWR, SYSCFG_BLE_PWR3_ST)==0);
}



void cpu_sleep_recover_init()
{
    ISR_VECTOR_ADDR[1] = (uint32_t)cpu_recover_asm;
}

XIP_BANNED static void power_up_hardware_modules()
{
    SYSCFG->PMU_PWR = FIELD_BUILD(SYSCFG_PERI_PWR2_PD, 0) 
                    | FIELD_BUILD(SYSCFG_PERI_ISO2_EN,1);
    while((SYSCFG->PMU_PWR & (SYSCFG_PERI_PWR2_ST_MASK)));
    SYSCFG->PMU_PWR = FIELD_BUILD(SYSCFG_PERI_PWR2_PD, 0) 
                    | FIELD_BUILD(SYSCFG_PERI_ISO2_EN,0);
}

XIP_BANNED static void wkup_ble()
{
    RCC->BLECFG |= RCC_BLE_WKUP_RST_MASK;
}

void clr_ble_wkup_req()
{
    RCC->BLECFG &= ~RCC_BLE_WKUP_RST_MASK;
}

NOINLINE XIP_BANNED static void cpu_flash_deep_sleep_and_recover()
{
    spi_flash_xip_stop();
    spi_flash_deep_power_down();
    cpu_sleep_asm();
    wkup_ble();
    spi_flash_init();
    spi_flash_release_from_deep_power_down();
    power_up_hardware_modules();
    __disable_irq();
    spi_flash_xip_start();
}

void low_power_mode_set(uint8_t mode)
{
    SYSCFG->PMU_WKUP = FIELD_BUILD(SYSCFG_SLP_LVL, mode) 
                    | FIELD_BUILD(SYSCFG_WKUP_EDGE,0x8)
                    | FIELD_BUILD(SYSCFG_WKUP_EN, 0x8);
    REG_FIELD_WR(SYSCFG->PMU_TRIM, SYSCFG_XTAL_STBTIME, XTAL_STB_VAL);
    REG_FIELD_WR(SYSCFG->PMU_TRIM,SYSCFG_LDO_LP_TRIM,5);
}

static void power_down_hardware_modules()
{
    SYSCFG->PMU_PWR = FIELD_BUILD(SYSCFG_PERI_PWR2_PD, 1) 
                    | FIELD_BUILD(SYSCFG_PERI_ISO2_EN,1);
}

void ble_wkup_status_set(bool status)
{
    waiting_ble_wkup_irq = status;
}

void deep_sleep()
{
    power_down_hardware_modules();
//    SCB->SCR |= (1<<2);
    irq_disable_before_wfi();
    cpu_flash_deep_sleep_and_recover();
    irq_reinit();
    ble_wkup_status_set(true);
}

bool ble_wkup_status_get(void)
{
    return waiting_ble_wkup_irq;
}

void LPWKUP_Handler()
{
    REG_FIELD_WR(SYSCFG->PMU_WKUP, SYSCFG_LP_WKUP_CLR,1);
}

