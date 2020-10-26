#include "uart_param.h"
#include "reg_rcc.h"
#include "field_manipulate.h"
#include "lsuart.h"
#include "le501x.h"
#include "HAL_def.h"
#include "sys_stat.h"
#include "platform.h"

static UART_HandleTypeDef *UART_inst_env[3];
void (*uart_isr)(UART_HandleTypeDef *);

void uart_sw_reset(UART_HandleTypeDef *inst)
{
    if (inst->UARTX == UART1)
    {
        REG_FIELD_WR(RCC->APB2RST, RCC_UART1, 1);
        REG_FIELD_WR(RCC->APB2RST, RCC_UART1, 0);
    }
    if (inst->UARTX == UART2)
    {
        REG_FIELD_WR(RCC->APB1RST, RCC_UART2, 1);
        REG_FIELD_WR(RCC->APB1RST, RCC_UART2, 0);
    }
    if (inst->UARTX == UART3)
    {
        REG_FIELD_WR(RCC->APB1RST, RCC_UART3, 1);
        REG_FIELD_WR(RCC->APB1RST, RCC_UART3, 0);
    }
}

void uart_clock_enable(UART_HandleTypeDef *inst,uint8_t status)
{
    if (inst->UARTX == UART1)
    {
        REG_FIELD_WR(RCC->APB2EN, RCC_UART1, status);
    }
    if (inst->UARTX == UART2)
    {
        REG_FIELD_WR(RCC->APB1EN, RCC_UART2, status);
    }
    if (inst->UARTX == UART3)
    {
        REG_FIELD_WR(RCC->APB1EN, RCC_UART3, status);
    }
}

void UART1_Handler(void)
{
    uart_isr( UART_inst_env[0]);
}

void UART2_Handler(void)
{
    uart_isr( UART_inst_env[1]);
}

void UART3_Handler(void)
{
    uart_isr( UART_inst_env[2]);
}

void uart_int_op(void (*isr)(UART_HandleTypeDef *),UART_HandleTypeDef *inst,uint8_t states)
{
    if (states)
    {
        uart_isr=isr; 
        if (inst->UARTX == UART1)
        {
            NVIC_ClearPendingIRQ(UART1_IRQn);
            UART_inst_env[0] = inst;
            NVIC_EnableIRQ(UART1_IRQn);
            arm_cm_set_int_isr(UART1_IRQn,UART1_Handler);
        }
        if (inst->UARTX == UART2)
        {
            NVIC_ClearPendingIRQ(UART2_IRQn);
            UART_inst_env[1] = inst;
            NVIC_EnableIRQ(UART2_IRQn);
            arm_cm_set_int_isr(UART2_IRQn,UART2_Handler);
        }
        if (inst->UARTX == UART3)
        {
            NVIC_ClearPendingIRQ(UART3_IRQn);
            UART_inst_env[2] = inst;
            NVIC_EnableIRQ(UART3_IRQn);
            arm_cm_set_int_isr(UART3_IRQn,UART3_Handler);
        }
    }
    else
    {
        if (inst->UARTX == UART1)
        {
            NVIC_DisableIRQ(UART1_IRQn);
        }
        if (inst->UARTX == UART2)
        {
            NVIC_DisableIRQ(UART2_IRQn);
        }
        if (inst->UARTX == UART3)
        {
            NVIC_DisableIRQ(UART3_IRQn);
        }
        uart_isr = NULL ;
    }
}

void uart_status_set(UART_HandleTypeDef *inst,uint8_t status)
{
    switch((uint32_t)inst->UARTX)
    {
    case (uint32_t)UART1:
        uart1_status_set(status);
    break;
    case (uint32_t)UART2:
        uart2_status_set(status);
    break;
    case (uint32_t)UART3:
        uart3_status_set(status);
    break;
    }
}



