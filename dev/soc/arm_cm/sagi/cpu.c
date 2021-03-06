#include "ARMCM3.h"
#include "cpu.h"
#include "compile_flag.h"
#if defined(FLASH_PROG_ALGO) || BOOT_RAM==1
XIP_BANNED uint32_t enter_critical()
{
    uint32_t stat = __get_PRIMASK();
    __disable_irq();
    return stat;
}

XIP_BANNED void exit_critical(uint32_t prev_stat)
{
    if(prev_stat==0)
    {
        __enable_irq();
    }
}
#else
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
#endif

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
