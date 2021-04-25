#include "core_rv32.h"
#include "cpu.h"
#include "compile_flag.h"

XIP_BANNED uint32_t enter_critical()
{
    uint32_t stat = CLIC->MINTTHRESH;
    CLIC->MINTTHRESH = 254<<24;
    return stat;
}

XIP_BANNED void exit_critical(uint32_t prev_stat)
{
    if(prev_stat==0)
    {
        CLIC->MINTTHRESH = 0;
    }
}

bool in_interrupt()
{
    return false;
}

XIP_BANNED void disable_global_irq()
{
    __disable_irq();
}

void enable_global_irq()
{
    __enable_irq();
}
