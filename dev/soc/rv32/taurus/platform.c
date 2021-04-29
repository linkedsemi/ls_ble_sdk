#include "taurus.h"
#include "core_rv32.h"
#include "swint_call_asm.h"

__attribute__((weak)) void SystemInit(){}

void sys_init_itf()
{
    
    
}

void flash_swint_set()
{
    csi_vic_set_pending_irq(SWINT3_IRQn);
}

void clk_switch(void)
{
    
}
