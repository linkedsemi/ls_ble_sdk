#include "le501x.h"
#include "cpu.h"
#include "compile_flag.h"

static uint32_t critical_nested_cnt;
XIP_BANNED void enter_critical()
{
    __disable_irq();
    ++critical_nested_cnt;
}

XIP_BANNED void exit_critical()
{
    if(--critical_nested_cnt==0)
    {
        __enable_irq();
    }
}

bool in_interrupt()
{
    return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0;
}

