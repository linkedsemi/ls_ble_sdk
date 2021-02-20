#include "i2c_msp.h"
#include "reg_rcc.h"
#include "field_manipulate.h"
#include "lsi2c.h"
#include "le501x.h"
#include "HAL_def.h"
#include "sys_stat.h"
#include "platform.h"
#include "dmac_config.h"
#include "dmac_msp.h"
#include <stddef.h>

static I2C_HandleTypeDef *i2c_inst_env[2];

void I2C1_Handler(void)
{
    HAL_I2C_IRQHandler( i2c_inst_env[0]);
}

void I2C2_Handler(void)
{
    HAL_I2C_IRQHandler( i2c_inst_env[1]);
}

void HAL_I2C_MSP_Init(I2C_HandleTypeDef *inst)
{
    switch((uint32_t)inst->Instance)
    {
    case (uint32_t)I2C1:
        REG_FIELD_WR(RCC->APB1RST, RCC_I2C1, 1);
        REG_FIELD_WR(RCC->APB1RST, RCC_I2C1, 0);
        arm_cm_set_int_isr(I2C1_IRQn,I2C1_Handler);
        i2c_inst_env[0] = inst;
        __NVIC_ClearPendingIRQ(I2C1_IRQn);
        __NVIC_EnableIRQ(I2C1_IRQn);
        REG_FIELD_WR(RCC->APB1EN, RCC_I2C1, 1);
    break;
    case (uint32_t)I2C2:
        REG_FIELD_WR(RCC->APB1RST, RCC_I2C2, 1);
        REG_FIELD_WR(RCC->APB1RST, RCC_I2C2, 0);
        arm_cm_set_int_isr(I2C2_IRQn,I2C2_Handler);
        i2c_inst_env[1] = inst;
        __NVIC_ClearPendingIRQ(I2C2_IRQn);
        __NVIC_EnableIRQ(I2C2_IRQn);
        REG_FIELD_WR(RCC->APB1EN, RCC_I2C2, 1);
    break;
    }
}

void HAL_I2C_MSP_DeInit(I2C_HandleTypeDef *inst)
{
    switch((uint32_t)inst->Instance)
    {
    case (uint32_t)I2C1:
        REG_FIELD_WR(RCC->APB1EN, RCC_I2C1, 0);
        __NVIC_DisableIRQ(I2C1_IRQn);
    break;
    case (uint32_t)I2C2:
        REG_FIELD_WR(RCC->APB1EN, RCC_I2C2, 0);
        __NVIC_DisableIRQ(I2C2_IRQn);
    break;
    }
}

static void i2c_status_set(I2C_HandleTypeDef *inst,bool status)
{
    switch((uint32_t)inst->Instance)
    {
    case (uint32_t)I2C1:
        i2c1_status_set(status);
    break;
    case (uint32_t)I2C2:
        i2c2_status_set(status);
    break;
    }
}

void HAL_I2C_MSP_Busy_Set(I2C_HandleTypeDef *inst)
{
    i2c_status_set(inst,true);
}

void HAL_I2C_MSP_Idle_Set(I2C_HandleTypeDef *inst)
{
    i2c_status_set(inst,false);
}

uint8_t HAL_I2C_TX_DMA_Handshake_Get(I2C_HandleTypeDef *inst)
{
    uint8_t handshake = CH_NONE;
    switch((uint32_t)inst->Instance)
    {
    case (uint32_t)I2C1:
        handshake = CH_I2C1_TX;
    break;
    case (uint32_t)I2C2:
        handshake = CH_I2C2_TX;
    break;
    }
    return handshake;
}

uint8_t HAL_I2C_RX_DMA_Handshake_Get(I2C_HandleTypeDef *inst)
{
    uint8_t handshake = CH_NONE;
    switch((uint32_t)inst->Instance)
    {
    case (uint32_t)I2C1:
        handshake = CH_I2C1_RX;
    break;
    case (uint32_t)I2C2:
        handshake = CH_I2C2_RX;
    break;
    }
    return handshake;
}
