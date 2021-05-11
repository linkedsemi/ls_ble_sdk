#include "spi_msp.h"
#include "reg_rcc.h"
#include "field_manipulate.h"
#include "lsspi.h"
#include "le501x.h"
#include "HAL_def.h"
#include "sys_stat.h"
#include "platform.h"
#include "dmac_config.h"
#include "dmac_msp.h"
#include <stddef.h>

static SPI_HandleTypeDef *spi_inst_env;


void SPI2_Handler(void)
{
    /* code */
    HAL_SPI_IRQHandler(spi_inst_env);   
}

void HAL_SPI_MSP_Init(SPI_HandleTypeDef *inst)
{
    if (inst->Instance == SPI2)
    {
        REG_FIELD_WR(RCC->APB1RST, RCC_SPI2, 1);
        REG_FIELD_WR(RCC->APB1RST, RCC_SPI2, 0);
        arm_cm_set_int_isr(SPI2_IRQn,SPI2_Handler);
        spi_inst_env = inst;
        NVIC_ClearPendingIRQ(SPI2_IRQn);
        NVIC_EnableIRQ(SPI2_IRQn);
        REG_FIELD_WR(RCC->APB1EN, RCC_SPI2, 1);
    }
}

void HAL_SPI_MSP_DeInit(SPI_HandleTypeDef *inst)
{
    if (inst->Instance == SPI2)
    {
        REG_FIELD_WR(RCC->APB1EN, RCC_SPI2, 0);
        NVIC_DisableIRQ(SPI2_IRQn);
    }
}

static void spi_status_set(SPI_HandleTypeDef *inst,bool status)
{
    switch((uint32_t)inst->Instance)
    {
    case (uint32_t)SPI2:
        spi2_status_set(status);
    break;
    }
}

void HAL_SPI_MSP_Busy_Set(SPI_HandleTypeDef *inst)
{
    spi_status_set(inst,1);
}

void HAL_SPI_MSP_Idle_Set(SPI_HandleTypeDef *inst)
{
    spi_status_set(inst,0);
}


