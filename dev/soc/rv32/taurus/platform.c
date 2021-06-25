#include "taurus.h"
#include "core_rv32.h"
#include "swint_call_asm.h"
#include "field_manipulate.h"
#include "systick.h"
#include "cpu.h"
#include "log.h"
#include "compile_flag.h"
#include "reg_sysc_awo.h"
#include "platform.h"

__attribute__((weak)) void SystemInit(){}

void sys_init_itf()
{
    
    
}

XIP_BANNED void flash_swint_set()
{
    SWINT_SET_INLINE_ASM(SWINT3_IRQn);
}

void clk_switch(void)
{
    
}

void irq_priority()
{
    MODIFY_REG(CLIC->CLICCFG,CLIC_CLICCFG_NLBIT_Msk,0xf<<CLIC_CLICCFG_NLBIT_Pos);
    csi_vic_set_prio(RTC1_IRQn,7);
    csi_vic_set_prio(IWDT_IRQn,7);
    csi_vic_set_prio(RTC2_IRQn,7);
    csi_vic_set_prio(EXT_IRQn,7);
    csi_vic_set_prio(LVD33_IRQn,7);
    csi_vic_set_prio(MAC_LP_IRQn,7);
    csi_vic_set_prio(MAC1_IRQn,7);
    csi_vic_set_prio(MAC2_IRQn,7);
    csi_vic_set_prio(SWINT1_IRQn,7);
    csi_vic_set_prio(USB_IRQn,7);
    csi_vic_set_prio(QSPI_IRQn,7);
    csi_vic_set_prio(CACHE_IRQn,7);
    csi_vic_set_prio(GPTIMA1_IRQn,7);
    csi_vic_set_prio(GPTIMB1_IRQn,7);
    csi_vic_set_prio(GPTIMC1_IRQn,7);
    csi_vic_set_prio(ADTIM1_IRQn,7);
    csi_vic_set_prio(I2C1_IRQn,7);
    csi_vic_set_prio(I2C2_IRQn,7);
    csi_vic_set_prio(I2C3_IRQn,7);
    csi_vic_set_prio(UART1_IRQn,7);
    csi_vic_set_prio(UART2_IRQn,7);
    csi_vic_set_prio(UART3_IRQn,7);
    csi_vic_set_prio(SPI2_IRQn,7);
    csi_vic_set_prio(GPIO_IRQn,7);
    csi_vic_set_prio(WWDT_IRQn,7);
    csi_vic_set_prio(ADC_IRQn,7);
    csi_vic_set_prio(TK_IRQn,7);
    csi_vic_set_prio(SWINT2_IRQn,7);
    csi_vic_set_prio(SWINT3_IRQn,7);

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
        REG_FIELD_WR(SYSC_AWO->PD_AWO_ANA0,SYSC_AWO_AWO_EN_DPLL,0);
        REG_FIELD_WR(SYSC_AWO->PD_AWO_ANA0,SYSC_AWO_AWO_DPLL_SEL_REF_24M,0);
        REG_FIELD_WR(SYSC_AWO->PD_AWO_ANA0,SYSC_AWO_AWO_EN_DPLL,1);
        DELAY_US(10000);
    }
    MODIFY_REG(SYSC_AWO->PD_AWO_CLK_CTRL,SYSC_AWO_CLK_SEL_QSPI_MASK,4<<SYSC_AWO_CLK_SEL_QSPI_POS);
}

void sys_init_none()
{
    enable_global_irq();
    LOG_INIT();
    systick_start();
    pll_enable();
}

int _close (int fildes){  return -1;}

int _fstat (int fildes,void *st){  return -1;}

int _isatty (int file){  return 0;}

int _read (int file,char *ptr,int len){  return -1;}

int _lseek (int   file,int   ptr,int   dir){  return -1;}
