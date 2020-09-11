#include "uart_param.h"
#include "field_manipulate.h"
#include "lsuart.h"
#include "ARMCM3.h"
#include "HAL_def.h"
#include "reg_sysc_per.h"
#include "platform.h"

static UART_HandleTypeDef *UART_inst_env[3];
void (*uart_isr)(UART_HandleTypeDef *);

void uart_sw_reset(UART_HandleTypeDef *inst)
{
    uart_clock_enable(inst,0);
    switch((uint32_t)inst->UARTX)
    {
    case (uint32_t)UART1:
        SYSC_PER->PD_PER_SRST0 = SYSC_PER_SRST_CLR_UART1_N_MASK;
        SYSC_PER->PD_PER_SRST0 = SYSC_PER_SRST_SET_UART1_N_MASK;
    break;
    case (uint32_t)UART2:
        SYSC_PER->PD_PER_SRST0 = SYSC_PER_SRST_CLR_UART2_N_MASK;
        SYSC_PER->PD_PER_SRST0 = SYSC_PER_SRST_SET_UART2_N_MASK;
    break;
    case (uint32_t)UART3:
        SYSC_PER->PD_PER_SRST0 = SYSC_PER_SRST_CLR_UART3_N_MASK;
        SYSC_PER->PD_PER_SRST0 = SYSC_PER_SRST_SET_UART3_N_MASK;
    break;
    }

}
void uart_clock_enable(UART_HandleTypeDef *inst,uint8_t status)
{
    switch((uint32_t)inst->UARTX)
    {
    case (uint32_t)UART1:
        SYSC_PER->PD_PER_CLKG0 = status ? SYSC_PER_CLKG_SET_UART1_MASK : SYSC_PER_CLKG_CLR_UART1_MASK;
    break;
    case (uint32_t)UART2:
        SYSC_PER->PD_PER_CLKG0 = status ? SYSC_PER_CLKG_SET_UART2_MASK : SYSC_PER_CLKG_CLR_UART2_MASK;
    break;
    case (uint32_t)UART3:
        SYSC_PER->PD_PER_CLKG0 = status ? SYSC_PER_CLKG_SET_UART3_MASK : SYSC_PER_CLKG_CLR_UART3_MASK;
    break;
    }
    DELAY_US(1);
}

void uart_status_set(UART_HandleTypeDef *inst, uint8_t status)
{

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


