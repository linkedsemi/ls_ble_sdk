#include "taurus.h"
#include "core_rv32.h"
#include "swint_call_asm.h"
#include "field_manipulate.h"

__attribute__((weak)) void SystemInit(){}

void sys_init_itf()
{
    
    
}

void flash_swint_set()
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