#include "lstimer_msp.h"
#include "lstimer.h"
#include "reg_rcc.h"
#include "field_manipulate.h"
#include "le501x.h"
#include "HAL_def.h"
#include "sys_stat.h"
#include "platform.h"

static TIM_HandleTypeDef *inst_env[5];

void BSTIM1_Handler(void)
{
    HAL_TIM_IRQHandler(inst_env[0]);
}

void GPTIMA1_Handler(void)
{
    HAL_TIM_IRQHandler(inst_env[1]);
}

void GPTIMB1_Handler(void)
{
    HAL_TIM_IRQHandler(inst_env[2]);
}

void GPTIMC1_Handler(void)
{
    HAL_TIM_IRQHandler(inst_env[3]);
}

void ADTIM1_Handler(void)
{
    HAL_TIM_IRQHandler(inst_env[4]);
}

void HAL_TIM_MSP_Init(TIM_HandleTypeDef *inst)
{
    switch((uint32_t)inst->Instance)
    {
    case (uint32_t)LSBSTIM:
    REG_FIELD_WR(RCC->APB1RST, RCC_BSTIM1, 1);
    REG_FIELD_WR(RCC->APB1RST, RCC_BSTIM1, 0);
    inst_env[0] = inst;
    arm_cm_set_int_isr(BSTIM1_IRQn,BSTIM1_Handler);
    __NVIC_ClearPendingIRQ(BSTIM1_IRQn);
    __NVIC_EnableIRQ(BSTIM1_IRQn);
    REG_FIELD_WR(RCC->APB1EN, RCC_BSTIM1, 1); 
    break;
    case (uint32_t)LSGPTIMA:
    REG_FIELD_WR(RCC->APB1RST, RCC_GPTIMA1, 1);
    REG_FIELD_WR(RCC->APB1RST, RCC_GPTIMA1, 0);
    inst_env[1] = inst;
    arm_cm_set_int_isr(GPTIMA1_IRQn,GPTIMA1_Handler);
    __NVIC_ClearPendingIRQ(GPTIMA1_IRQn);
    __NVIC_EnableIRQ(GPTIMA1_IRQn);
    REG_FIELD_WR(RCC->APB1EN, RCC_GPTIMA1, 1);
    break;
    case (uint32_t)LSGPTIMB:
    REG_FIELD_WR(RCC->APB1RST, RCC_GPTIMB1, 1);
    REG_FIELD_WR(RCC->APB1RST, RCC_GPTIMB1, 0);
    inst_env[2] = inst;
    arm_cm_set_int_isr(GPTIMB1_IRQn,GPTIMB1_Handler);
    __NVIC_ClearPendingIRQ(GPTIMB1_IRQn);
    __NVIC_EnableIRQ(GPTIMB1_IRQn);
    REG_FIELD_WR(RCC->APB1EN, RCC_GPTIMB1, 1);
    break;
    case (uint32_t)LSGPTIMC:
    REG_FIELD_WR(RCC->APB2RST, RCC_GPTIMC1, 1);
    REG_FIELD_WR(RCC->APB2RST, RCC_GPTIMC1, 0);
    inst_env[3] = inst;
    arm_cm_set_int_isr(GPTIMC1_IRQn,GPTIMC1_Handler);
    __NVIC_ClearPendingIRQ(GPTIMC1_IRQn);
    __NVIC_EnableIRQ(GPTIMC1_IRQn);
    REG_FIELD_WR(RCC->APB2EN, RCC_GPTIMC1, 1);
    break;
    case (uint32_t)LSADTIM:
    REG_FIELD_WR(RCC->APB2RST, RCC_ADTIM1, 1);
    REG_FIELD_WR(RCC->APB2RST, RCC_ADTIM1, 0);
    inst_env[4] = inst;
    arm_cm_set_int_isr(ADTIM1_IRQn,ADTIM1_Handler);
    __NVIC_ClearPendingIRQ(ADTIM1_IRQn);
    __NVIC_EnableIRQ(ADTIM1_IRQn);
    REG_FIELD_WR(RCC->APB2EN, RCC_ADTIM1, 1);
    break;
    }
}

void HAL_TIM_MSP_DeInit(TIM_HandleTypeDef *inst)
{
    switch((uint32_t)inst->Instance)
    {
    case (uint32_t)LSBSTIM:
    REG_FIELD_WR(RCC->APB1EN, RCC_BSTIM1, 0);
    __NVIC_DisableIRQ(BSTIM1_IRQn); 
    break;
    case (uint32_t)LSGPTIMA:
    REG_FIELD_WR(RCC->APB1EN, RCC_GPTIMA1, 0);
    __NVIC_DisableIRQ(GPTIMA1_IRQn);
    break;
    case (uint32_t)LSGPTIMB:
    REG_FIELD_WR(RCC->APB1EN, RCC_GPTIMB1, 0);
    __NVIC_DisableIRQ(GPTIMB1_IRQn);
    break;
    case (uint32_t)LSGPTIMC:
    REG_FIELD_WR(RCC->APB2EN, RCC_GPTIMC1, 0);
    __NVIC_DisableIRQ(GPTIMC1_IRQn);
    break;
    case (uint32_t)LSADTIM:
    REG_FIELD_WR(RCC->APB2EN, RCC_ADTIM1, 0);
    __NVIC_DisableIRQ(ADTIM1_IRQn);
    break;
    }
}

static void timer_status_set(TIM_HandleTypeDef *inst,uint8_t status)
{
    switch((uint32_t)inst->Instance)
    {
    case (uint32_t)LSBSTIM:
    bstimer1_status_set(status);
    break;
    case (uint32_t)LSGPTIMA:
    gptimera1_status_set(status);
    break;
    case (uint32_t)LSGPTIMB:
    gptimerb1_status_set(status);
    break;
    case (uint32_t)LSGPTIMC:
    gptimerc1_status_set(status);
    break;
    case (uint32_t)LSADTIM:
    adtimer1_status_set(status);
    break;
    }
}

void HAL_TIM_MSP_Busy_Set(TIM_HandleTypeDef *inst)
{
    timer_status_set(inst,1);
}

void HAL_TIM_MSP_Idle_Set(TIM_HandleTypeDef *inst)
{
    timer_status_set(inst,0);
}


