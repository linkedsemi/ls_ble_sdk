#include "ARMCM3.h"
#include "cpu.h"
#include "compile_flag.h"

XIP_BANNED uint32_t enter_critical()
{
    uint32_t basepri = __get_BASEPRI();
    __set_BASEPRI((1 << (8U - __NVIC_PRIO_BITS)));
    return basepri;
}

XIP_BANNED void exit_critical(uint32_t cpu_stat)
{
    __set_BASEPRI(cpu_stat);
}

bool in_interrupt()
{
    return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0;
}

XIP_BANNED void disable_global_irq()
{
    __disable_irq();
}

void enable_global_irq()
{
    __enable_irq();
}
