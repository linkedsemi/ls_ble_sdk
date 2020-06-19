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
#include "reg_lsqspi.h"
#include "uart.h"
#include "lsgpio.h"
#include "cpu.h"

#define ISR_VECTOR_ADDR ((uint32_t *)(0x0))

bool waiting_ble_wkup_irq;
struct sleep_wakeup_type env_sleep_wkup;

void cpu_sleep_asm(void);

void cpu_recover_asm(void);

#if SDK_DCDC_BYPASS
void dcdc_off(){}
XIP_BANNED void dcdc_on(){}
#else
void dcdc_off()
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

XIP_BANNED static void ble_hclk_clr()
{
    REG_FIELD_WR(RCC->BLECFG, RCC_BLE_AHBEN, 0);
}

XIP_BANNED void before_wfi()
{
    while(REG_FIELD_RD(SYSCFG->PMU_PWR, SYSCFG_BLE_PWR3_ST));
    ble_hclk_clr();
    switch_to_xo16m();
    SYSCFG->ANACFG0 &= ~(SYSCFG_EN_DPLL_MASK | SYSCFG_EN_DPLL_16M_RF_MASK | SYSCFG_EN_DPLL_128M_RF_MASK | SYSCFG_EN_DPLL_128M_EXT_MASK | SYSCFG_EN_QCLK_MASK);
    MODIFY_REG(SYSCFG->ANACFG1,SYSCFG_XO16M_ADJ_MASK | SYSCFG_XO16M_LP_MASK,
        3<<SYSCFG_XO16M_ADJ_POS | 0<<SYSCFG_XO16M_LP_POS);
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

static void wkup_ble()
{
    RCC->BLECFG |= RCC_BLE_WKUP_RST_MASK;
}

XIP_BANNED void after_wfi()
{
    dcdc_on();
    MODIFY_REG(SYSCFG->ANACFG1,SYSCFG_XO16M_ADJ_MASK | SYSCFG_XO16M_LP_MASK,
        0<<SYSCFG_XO16M_ADJ_POS | 1<<SYSCFG_XO16M_LP_POS);
    SYSCFG->ANACFG0 |= (SYSCFG_EN_DPLL_MASK | SYSCFG_EN_DPLL_16M_RF_MASK | SYSCFG_EN_DPLL_128M_RF_MASK | SYSCFG_EN_DPLL_128M_EXT_MASK | SYSCFG_EN_QCLK_MASK);
    wait_dpll_lock();
    clk_switch();
    LS_RAM_ASSERT(__NVIC_GetPendingIRQ(BLE_WKUP_IRQn)==0);
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

void clr_ble_wkup_req()
{
    RCC->BLECFG &= ~RCC_BLE_WKUP_RST_MASK;
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
NOINLINE XIP_BANNED  void set_mode_sleep_lvl1(void)
{

}

NOINLINE XIP_BANNED  void set_mode_sleep_lvl2(void)
{

}

NOINLINE XIP_BANNED  void set_mode_deep_sleep_lvl3(struct sleep_wakeup_type *param)
{
    enter_critical();
    NVIC->ICER[0] = 0xffffffff;
    NVIC->ICPR[0] = 0xffffffff;
    
   spi_flash_xip_stop();
   spi_flash_deep_power_down();

   
   REG_FIELD_WR(RCC->AHBEN,RCC_GPIOA,1);
   REG_FIELD_WR(RCC->AHBEN,RCC_GPIOB,1);
   REG_FIELD_WR(RCC->AHBEN,RCC_GPIOC,1);

   WRITE_REG(LSGPIOA->MODE,0);
   WRITE_REG(LSGPIOB->MODE,0);
   WRITE_REG(LSGPIOC->MODE,0);

   WRITE_REG(LSGPIOA->OD,0);
   WRITE_REG(LSGPIOB->OD,0);
   WRITE_REG(LSGPIOC->OD,0);

   WRITE_REG(LSGPIOA->OE,0);
   WRITE_REG(LSGPIOB->OE,0);
   WRITE_REG(LSGPIOC->OE,0);

   WRITE_REG(LSGPIOA->IE,0);
   WRITE_REG(LSGPIOB->IE,0);
   WRITE_REG(LSGPIOC->IE,0);

   WRITE_REG(LSGPIOA->PUPD,0xAAAAAAAA);
   WRITE_REG(LSGPIOB->PUPD,0xAAAAAAA5);
   WRITE_REG(LSGPIOC->PUPD,0xAAAA555A);



   REG_FIELD_WR(SYSCFG->QSPICTL,SYSCFG_QSPI_DQ0_IE,0);
   REG_FIELD_WR(SYSCFG->QSPICTL,SYSCFG_QSPI_DQ1_IE,0);
   REG_FIELD_WR(SYSCFG->QSPICTL,SYSCFG_QSPI_DQ2_IE,0);
   REG_FIELD_WR(SYSCFG->QSPICTL,SYSCFG_QSPI_DQ3_IE,0);
   REG_FIELD_WR(SYSCFG->QSPICTL,SYSCFG_QSPI_NSS_IE,0);
   REG_FIELD_WR(SYSCFG->QSPICTL,SYSCFG_QSPI_SCK_IE,0);
   REG_FIELD_WR(SYSCFG->QSPICTL,SYSCFG_QSPI_CTL_EN,0x5B);

   SYSCFG->PMU_WKUP = FIELD_BUILD(SYSCFG_SLP_LVL, SLEEP_MOED3) 
                      | FIELD_BUILD(SYSCFG_WKUP_EDGE,param->trig_edge)
                      | FIELD_BUILD(SYSCFG_WKUP_EN, param->trig_src);  //PA00; 

   SYSCFG->PMU_PWR = 0;                      
   SCB->SCR |= (1<<2);
   __WFI();
   while(1);
}

void low_power_mode_init()
{
    SYSCFG->PMU_WKUP = FIELD_BUILD(SYSCFG_SLP_LVL,SLEEP_MOED0)
                        | FIELD_BUILD(SYSCFG_WKUP_EN,BLE_WKUP)
                        | FIELD_BUILD(SYSCFG_WKUP_EDGE,BLE_WKUP_EDGE_RISING);
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

static void ble_hclk_set()
{
    REG_FIELD_WR(RCC->BLECFG, RCC_BLE_AHBEN, 1);
}

static void ble_hclk_restore()
{
    LS_ASSERT(REG_FIELD_RD(SYSCFG->PMU_PWR,SYSCFG_BLE_PWR3_ST)==0);
    while(REG_FIELD_RD(SYSCFG->PMU_PWR,SYSCFG_BLE_PWR3_ST)==0);
    ble_hclk_set();
}




void enter_deep_sleep_lvl3_mode(struct sleep_wakeup_type *sleep_param)
{
   set_mode_deep_sleep_lvl3(sleep_param);
}

void check_wkup_state(void)
{
    if (REG_FIELD_RD(SYSCFG->PMU_WKUP,SYSCFG_WKUP_STAT))
	 {
         REG_FIELD_WR(SYSCFG->PMU_WKUP, SYSCFG_LP_WKUP_CLR,1);
     } 
}

static void cpu_deep_sleep_bit_set()
{
    SCB->SCR |= (1<<2);
}

void deep_sleep()
{
    power_down_hardware_modules();
    cpu_deep_sleep_bit_set();
    irq_disable_before_wfi();
    cpu_flash_deep_sleep_and_recover();
    wkup_ble();
    irq_reinit();
    ble_wkup_status_set(true);
    ble_hclk_restore();
}

bool ble_wkup_status_get(void)
{
    return waiting_ble_wkup_irq;
}

void LPWKUP_Handler(void)
{
     if (REG_FIELD_RD(SYSCFG->PMU_WKUP,SYSCFG_WKUP_STAT) & (env_sleep_wkup.trig_src<<WKUP_STATE_POS))
	 {
         MODIFY_REG(SYSCFG->PMU_WKUP,(env_sleep_wkup.trig_src<<WKUP_EN_POS),((~(env_sleep_wkup.trig_src))<<WKUP_EN_POS));
         MODIFY_REG(SYSCFG->PMU_WKUP,(env_sleep_wkup.trig_src<<WKUP_EN_POS),((env_sleep_wkup.trig_src)<<WKUP_EN_POS));
	 }
	 REG_FIELD_WR(SYSCFG->PMU_WKUP, SYSCFG_LP_WKUP_CLR,1);
}

