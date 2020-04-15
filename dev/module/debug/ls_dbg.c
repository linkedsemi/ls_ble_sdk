#include "le501x.h"
#include <stdarg.h>
#include "log.h"
#include "compile_flag.h"

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


struct
{
    uint32_t msp;
    uint32_t psp;
    uint32_t lr;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
}hardfault_env;
void hardfault_print(void)
{
    enum{
    R0_INSTACK,
    R1_INSTACK,
    R2_INSTACK,
    R3_INSTACK,
    R12_INSTACK,
    LR_INSTACK,
    PC_INSTACK,
    xPSR_INSTACK,
    };

    LOG_RAW( "!!!!!!!!!!HardFault Handler is triggered!!!!!!!!!!\r\n");
    LOG_RAW( "Prolog:\r\n");
    LOG_RAW( "R4   = 0x%08x\r\n", hardfault_env.r4);
    LOG_RAW( "R5   = 0x%08x\r\n", hardfault_env.r5);
    LOG_RAW( "R6   = 0x%08x\r\n", hardfault_env.r6);
    LOG_RAW( "R7   = 0x%08x\r\n", hardfault_env.r7);
    LOG_RAW( "lr   = 0x%08x\r\n", hardfault_env.lr);
    LOG_RAW( "msp  = 0x%08x\r\n", hardfault_env.msp);
    LOG_RAW( "psp  = 0x%08x\r\n", hardfault_env.psp);
    uint32_t *sp;
    if(hardfault_env.lr==0xfffffffd)
    {
        sp = (uint32_t*)hardfault_env.psp;
        LOG_RAW("PSP Stack Info:\r\n");
    }
    else{
        sp = (uint32_t*)hardfault_env.msp;
        LOG_RAW("MSP Stack Info:\r\n");
    }

    // Try to dump
    LOG_RAW( "R0   = 0x%08x\r\n", sp[R0_INSTACK]);
    LOG_RAW( "R1   = 0x%08x\r\n", sp[R1_INSTACK]);
    LOG_RAW( "R2   = 0x%08x\r\n", sp[R2_INSTACK]);
    LOG_RAW( "R3   = 0x%08x\r\n", sp[R3_INSTACK]);
    LOG_RAW( "R12  = 0x%08x\r\n", sp[R12_INSTACK]);
    LOG_RAW( "LR   = 0x%08x\r\n", sp[LR_INSTACK]);
    LOG_RAW( "PC   = 0x%08x\r\n", sp[PC_INSTACK]);
    LOG_RAW( "xPSR = 0x%08x\r\n", sp[xPSR_INSTACK]);
}
void hardfault_env_save(void*,uint32_t,uint32_t);

void HardFault_Handler(void)
{
    uint32_t canonical_frame_addr = __get_MSP() + 8;
    uint32_t *lr_addr = (uint32_t *)(canonical_frame_addr - 4);
    hardfault_env_save(&hardfault_env,canonical_frame_addr,*lr_addr);
    hardfault_print();
    while(*(volatile uint8_t *)1);
}



