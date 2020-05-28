#include "uart_param.h"
#include "reg_rcc.h"
#include "field_manipulate.h"
#include "uart.h"
#include "le501x.h"
#include "HAL_def.h"

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
    if (inst->UARTX == UART2)
    {
        REG_FIELD_WR(RCC->APB1RST, RCC_UART3, 1);
        REG_FIELD_WR(RCC->APB1RST, RCC_UART3, 0);
    }
}
void uart_clock_enable(UART_HandleTypeDef *inst)
{
    if (inst->UARTX == UART1)
    {
        REG_FIELD_WR(RCC->APB2EN, RCC_UART1, 1);
    }
    if (inst->UARTX == UART2)
    {
        REG_FIELD_WR(RCC->APB1EN, RCC_UART2, 1);
    }
    if (inst->UARTX == UART3)
    {
        REG_FIELD_WR(RCC->APB1EN, RCC_UART3, 1);
    }
}

void uart_int_op(void (*isr)(UART_HandleTypeDef *),UART_HandleTypeDef *inst,uint8_t states)
{
    if (uart_isr == NULL)
    {
        uart_isr=isr; 
    }
    if (states)
    {
        if (inst->UARTX == UART1)
        {
            NVIC_ClearPendingIRQ(UART1_IRQn);
            UART_inst_env[0] = inst;
            NVIC_EnableIRQ(UART1_IRQn);
        }
        if (inst->UARTX == UART2)
        {
            NVIC_ClearPendingIRQ(UART2_IRQn);
            UART_inst_env[1] = inst;
            NVIC_EnableIRQ(UART2_IRQn);
        }
        if (inst->UARTX == UART3)
        {
            NVIC_ClearPendingIRQ(UART3_IRQn);
            UART_inst_env[2] = inst;
            NVIC_EnableIRQ(UART3_IRQn);
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


