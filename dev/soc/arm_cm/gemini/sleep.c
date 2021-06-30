#include "reg_v33_rg_type.h"
#include "gemini.h"
#include "spi_flash.h"
#include "compile_flag.h"
#include "platform.h"
#include "reg_sysc_awo_type.h"
#include "reg_v33_rg_type.h"
#include "field_manipulate.h"
#include "sleep.h"
#include "io_config.h"
static uint32_t sleep_msp;
void cpu_sleep_asm(void);
void cpu_recover_asm(void);
void xo16m(void);
void cpu_sleep_recover_init()
{
    V33_RG->SFT_CTRL03 = (uint32_t)cpu_recover_asm >> 1;
}

XIP_BANNED void xo16m()
{
    
}

XIP_BANNED void store_msp()   
{
    sleep_msp = __get_MSP();
}

XIP_BANNED void restore_msp()
{
    __set_MSP(sleep_msp);
}

XIP_BANNED void pwr_delay()
{
    REG_FIELD_WR(V33_RG->WKUP_TIM,V33_RG_STB_CLK_M1,0xff);
    REG_FIELD_WR(V33_RG->WKUP_TIM1,V33_RG_STB_LDO_M1,7);
}

XIP_BANNED void before_wfi()
{

}

XIP_BANNED void after_wfi()
{
}

XIP_BANNED void power_down_config()
{
    V33_RG->SFT_CTRL03 = 0x0; 
    V33_RG->PWR_CTRL = FIELD_BUILD(V33_RG_LPLDO_PD_EN,1)
                          |FIELD_BUILD(V33_RG_HPLDO_PD_EN,1)
                          |FIELD_BUILD(V33_RG_MSI_PD_EN,1)
                          |FIELD_BUILD(V33_RG_BG_PD_EN,0)
                          |FIELD_BUILD(V33_RG_BGIB_PD_EN,1)
                          |FIELD_BUILD(V33_RG_LSI_PD_EN,1)
                          |FIELD_BUILD(V33_RG_HSE_PD_EN,1)
                          |FIELD_BUILD(V33_RG_SRAM_DS_PD_EN,1)
                          |FIELD_BUILD(V33_RG_LSE_PD_EN,1)
                          |FIELD_BUILD(V33_RG_HSI_PD_EN,1)
                          |FIELD_BUILD(V33_RG_HSE_PU_EN,1)
                          |FIELD_BUILD(V33_RG_LSE_PU_EN,1)
                          |FIELD_BUILD(V33_RG_SRAM1_PD_EN,1)
                          |FIELD_BUILD(V33_RG_SRAM2_PD_EN,1)
                          |FIELD_BUILD(V33_RG_SRAM3_PD_EN,1)
                          |FIELD_BUILD(V33_RG_SRAM4_PD_EN,1)
                          |FIELD_BUILD(V33_RG_SRAM_DS_PU_EN,1)
                          |FIELD_BUILD(V33_RG_RCO_BIAS_FC,0)
                          |FIELD_BUILD(V33_RG_HPSW_PU_LATE,1)
                          |FIELD_BUILD(V33_RG_PD_GPIO_SEL,0);
    V33_RG->MISC_CTRL1 = FIELD_BUILD(V33_RG_LATCH_GPIO,1)
                          |FIELD_BUILD(V33_RG_PD_GPIO,0)
                          |FIELD_BUILD(V33_RG_PD_ADC12,1)
                          |FIELD_BUILD(V33_RG_PD_AMIC,1)
                          |FIELD_BUILD(V33_RG_PD_TK,1)
                          |FIELD_BUILD(V33_RG_PD_DAC12,1)
                          |FIELD_BUILD(V33_RG_BAT_DTCT_EN,1);
    V33_RG->MISC_CTRL1 = FIELD_BUILD(V33_RG_LATCH_GPIO,1)
                          |FIELD_BUILD(V33_RG_PD_GPIO,1)
                          |FIELD_BUILD(V33_RG_PD_ADC12,1)
                          |FIELD_BUILD(V33_RG_PD_AMIC,1)
                          |FIELD_BUILD(V33_RG_PD_TK,1)
                          |FIELD_BUILD(V33_RG_PD_DAC12,1)
                          |FIELD_BUILD(V33_RG_BAT_DTCT_EN,1);

    SCB->SCR = 0x01;
}

NOINLINE XIP_BANNED static void cpu_flash_deep_sleep_and_recover()
{
    spi_flash_xip_stop();
    spi_flash_deep_power_down();
    cpu_sleep_asm();
    spi_flash_init();
    spi_flash_release_from_deep_power_down();
    DELAY_US(8);
    spi_flash_xip_start();

}

XIP_BANNED void deep_sleep()
{
    power_down_config();
    cpu_flash_deep_sleep_and_recover();
}

XIP_BANNED void lvl2_lvl3_mode_prepare()
{
    power_down_config();
    REG_FIELD_WR(V33_RG->WKUP_CTRL,V33_RG_WKUP_MSK,0x08);
}

XIP_BANNED void enter_deep_sleep_mode_lvl2_lvl3()
{
    spi_flash_xip_stop();
    spi_flash_deep_power_down();
    lvl2_lvl3_mode_prepare();
    SCB->SCR |= (1<<2);
    __WFI();
    while(1);
}
