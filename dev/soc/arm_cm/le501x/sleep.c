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

#define ISR_VECTOR_ADDR ((uint32_t *)(0x0))
void cpu_sleep_asm(void);

void cpu_recover_asm(void);

#if SDK_DCDC_BYPASS
XIP_BANNED void dcdc_off(){}
XIP_BANNED void dcdc_on(){}
#else
XIP_BANNED void dcdc_off()
{
//    REG_FIELD_WR(SYSCFG->DCDC, SYSCFG_EN, 0);
}

XIP_BANNED void dcdc_on()
{
//    REG_FIELD_WR(SYSCFG->DCDC, SYSCFG_EN, 1);
}

#endif

XIP_BANNED void before_wfi()
{
    while(REG_FIELD_RD(SYSCFG->PMU_PWR, SYSCFG_BLE_PWR3_ST));
    dcdc_off();
}

XIP_BANNED void after_wfi()
{
    dcdc_on();
}



void cpu_sleep_recover_init()
{
    ISR_VECTOR_ADDR[1] = (uint32_t)cpu_recover_asm;
}

XIP_BANNED static void power_up_hardware_modules()
{
    SYSCFG->PMU_PWR = FIELD_BUILD(SYSCFG_PERI_PWR2_PD, 0) 
                    | FIELD_BUILD(SYSCFG_SEC_PWR4_PD,0)
                    | FIELD_BUILD(SYSCFG_PERI_ISO2_EN,1)
                    | FIELD_BUILD(SYSCFG_SEC_ISO4_EN, 1);
    while((SYSCFG->PMU_PWR & (SYSCFG_PERI_PWR2_ST_MASK|SYSCFG_SEC_PWR4_ST_MASK)));
    SYSCFG->PMU_PWR = FIELD_BUILD(SYSCFG_PERI_PWR2_PD, 0) 
                    | FIELD_BUILD(SYSCFG_SEC_PWR4_PD,0)
                    | FIELD_BUILD(SYSCFG_PERI_ISO2_EN,0)
                    | FIELD_BUILD(SYSCFG_SEC_ISO4_EN, 0);
}

NOINLINE XIP_BANNED static void cpu_flash_deep_sleep_and_recover()
{
    spi_flash_xip_stop();
    spi_flash_deep_power_down();
    cpu_sleep_asm();
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
    REG_FIELD_WR(SYSCFG->PMU_TRIM, SYSCFG_XTAL_STBTIME, XTAL_STARTUP_CYCLES);
}

static void power_down_hardware_modules()
{
    SYSCFG->PMU_PWR = FIELD_BUILD(SYSCFG_PERI_PWR2_PD, 1) 
                    | FIELD_BUILD(SYSCFG_SEC_PWR4_PD,1)
                    | FIELD_BUILD(SYSCFG_PERI_ISO2_EN,1)
                    | FIELD_BUILD(SYSCFG_SEC_ISO4_EN, 1);

}

bool is_ble_power_on()
{
    return REG_FIELD_RD(SYSCFG->PMU_PWR, SYSCFG_BLE_PWR3_ST);
}

void ble_wakeup_request()
{
    if(is_ble_power_on()==false)
    {
        RCC->BLECFG |= RCC_BLE_WKUP_RST_MASK;
        while(is_ble_power_on()==false);
    }
}

void deep_sleep()
{
    power_down_hardware_modules();
    SCB->SCR |= (1<<2);
    cpu_flash_deep_sleep_and_recover();
    irq_reinit();
}

void LPWKUP_Handler()
{
    REG_FIELD_WR(SYSCFG->PMU_WKUP, SYSCFG_LP_WKUP_CLR,1);
}

