#include "le501x.h"
#include <stdarg.h>
#include "log.h"
#include "compile_flag.h"
#if 1
void ls_assert(const char *expr,const char *file,int line)
{
    __disable_irq();
    LOG_RAW("Assertion Failed: file %s, line %d, %s\n",file,line,expr);
    while(1);
}

XIP_BANNED void ls_ram_assert()
{
    while(1);
}

void stack_assert_c(uint32_t lvl,uint32_t param0,uint32_t param1,uint32_t lr)
{
    LOG_RAW("lvl:%x,lr=0x%08x,param0=0x%x,param1=0x%x\n",lvl,lr,param0,param1);
    if(lvl==LVL_ERROR)
    {
        __disable_irq();
        while(1);
    }
}
#else
void ls_assert(const char *expr,const char *file,int line)
{
    __disable_irq();
    LOG_RAW("Assertion Failed: file %s, line %d, %s\n",file,line,expr);
    NVIC_SystemReset();
}

XIP_BANNED void ls_ram_assert()
{
      __disable_irq();
     NVIC_SystemReset();
}

void stack_assert_c(uint32_t lvl,uint32_t param0,uint32_t param1,uint32_t lr)
{
    LOG_RAW("lvl:%x,lr=0x%08x,param0=0x%x,param1=0x%x\n",lvl,lr,param0,param1);
    if(lvl==LVL_ERROR)
    {
        __disable_irq();
        NVIC_SystemReset();
    }
}
#endif
