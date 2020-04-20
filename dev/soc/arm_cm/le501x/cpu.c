#include "le501x.h"
#include "cpu.h"
#include "compile_flag.h"

static uint32_t critical_nested_cnt;
XIP_BANNED void enter_critical()
{
    ++critical_nested_cnt;
    __disable_irq();
}

XIP_BANNED void exit_critical()
{
    if(--critical_nested_cnt)
    {
        __enable_irq();
    }
}

bool in_interrupt()
{
    return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0;
}

