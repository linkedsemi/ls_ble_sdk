#include "uart_param.h"
#include "field_manipulate.h"
#include "lsuart.h"
#include "core_rv32.h"
#include "HAL_def.h"
#include "reg_sysc_per.h"
#include "exception_isr.h"
#include "taurus.h"
#include "platform.h"
static UART_HandleTypeDef *UART_inst_env[3];

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

void UART1_Handler(void)
{
    HAL_UARTx_IRQHandler( UART_inst_env[0]);
}

void UART2_Handler(void)
{
    HAL_UARTx_IRQHandler( UART_inst_env[1]);
}

void UART3_Handler(void)
{
    HAL_UARTx_IRQHandler( UART_inst_env[2]);
}

void uart_int_op(void (*isr)(UART_HandleTypeDef *),UART_HandleTypeDef *inst,uint8_t states)
{
    if (states)
    {        
        if (inst->UARTX == UART1)
        {
            rv_set_int_isr(UART1_IRQn,UART1_Handler);
            csi_vic_clear_pending_irq(UART1_IRQn);
            csi_vic_enable_irq(UART1_IRQn);
        }
        if (inst->UARTX == UART2)
        {
            rv_set_int_isr(UART2_IRQn,UART2_Handler);
            csi_vic_clear_pending_irq(UART2_IRQn);
            csi_vic_enable_irq(UART2_IRQn);
        }
        if(inst->UARTX == UART3)
        {
            rv_set_int_isr(UART3_IRQn,UART3_Handler);
            csi_vic_clear_pending_irq(UART3_IRQn);
            csi_vic_enable_irq(UART3_IRQn);
        }
    }
    else
    {
        if (inst->UARTX == UART1)
        {
            csi_vic_disable_irq(UART1_IRQn);
        }
        if (inst->UARTX == UART2)
        {
            csi_vic_disable_irq(UART2_IRQn);
        }
        if (inst->UARTX == UART3)
        {
            csi_vic_disable_irq(UART3_IRQn);
        }
    }
    
}


