#include "sha_msp.h"
#include "reg_sysc_cpu_type.h"
#include "gemini.h"
#include "platform.h"
#include "lssha.h"

void HAL_LSSHA_MSP_Init(void)
{
    SYSC_CPU->PD_CPU_CLKG = SYSC_CPU_CLKG_SET_CALC_SHA_MASK;
    arm_cm_set_int_isr(SHA_IRQn,LSSHA_IRQHandler);
    __NVIC_ClearPendingIRQ(SHA_IRQn);
    __NVIC_EnableIRQ(SHA_IRQn);
}

void HAL_LSSHA_MSP_DeInit(void)
{
    SYSC_CPU->PD_CPU_CLKG = SYSC_CPU_CLKG_CLR_CALC_SHA_MASK;
    __NVIC_DisableIRQ(SHA_IRQn);
}

void HAL_LSSHA_MSP_Busy_Set(void)
{

}

void HAL_LSSHA_MSP_Idle_Set(void)
{
    
}