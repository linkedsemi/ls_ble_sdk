#include "i2c_msp.h"
#include "field_manipulate.h"
#include "gemini.h"
#include "HAL_def.h"
#include "platform.h"
#include "reg_sysc_per_type.h"
#include <stddef.h>

static I2C_HandleTypeDef *i2c_inst_env[3];

void I2C1_Handler(void)
{
    HAL_I2C_IRQHandler( i2c_inst_env[0]);
}

void I2C2_Handler(void)
{
    HAL_I2C_IRQHandler( i2c_inst_env[1]);
}

void I2C3_Handler(void)
{
    HAL_I2C_IRQHandler( i2c_inst_env[2]);
}

void HAL_I2C_MSP_Init(I2C_HandleTypeDef *inst)
{
    switch((uint32_t)inst->Instance)
    {
    case (uint32_t)I2C1:
        REG_FIELD_WR(SYSC_PER->PD_PER_CLKG0, SYSC_PER_CLKG_SET_I2C1, 1);
        arm_cm_set_int_isr(I2C1_IRQn,I2C1_Handler);
        i2c_inst_env[0] = inst;
        __NVIC_ClearPendingIRQ(I2C1_IRQn);
        __NVIC_EnableIRQ(I2C1_IRQn);
    break;
    case (uint32_t)I2C2:
        REG_FIELD_WR(SYSC_PER->PD_PER_CLKG0, SYSC_PER_CLKG_SET_I2C2, 1);
        arm_cm_set_int_isr(I2C2_IRQn,I2C2_Handler);
        i2c_inst_env[1] = inst;
        __NVIC_ClearPendingIRQ(I2C2_IRQn);
        __NVIC_EnableIRQ(I2C2_IRQn);

    break;
    case (uint32_t)I2C3:
        REG_FIELD_WR(SYSC_PER->PD_PER_CLKG0, SYSC_PER_CLKG_SET_I2C3, 1);
        arm_cm_set_int_isr(I2C3_IRQn,I2C3_Handler);
        i2c_inst_env[2] = inst;
        __NVIC_ClearPendingIRQ(I2C3_IRQn);
        __NVIC_EnableIRQ(I2C3_IRQn);

    break;
    }
}

void HAL_I2C_MSP_DeInit(I2C_HandleTypeDef *inst)
{
    switch((uint32_t)inst->Instance)
    {
    case (uint32_t)I2C1:
        REG_FIELD_WR(SYSC_PER->PD_PER_CLKG0, SYSC_PER_CLKG_CLR_I2C1, 1);
        __NVIC_DisableIRQ(I2C1_IRQn);
    break;
    case (uint32_t)I2C2:
        REG_FIELD_WR(SYSC_PER->PD_PER_CLKG0, SYSC_PER_CLKG_CLR_I2C2, 1);
        __NVIC_DisableIRQ(I2C2_IRQn);
    break;
    case (uint32_t)I2C3:
        REG_FIELD_WR(SYSC_PER->PD_PER_CLKG0, SYSC_PER_CLKG_CLR_I2C3, 1);
        __NVIC_DisableIRQ(I2C3_IRQn);
    break;
    }
}

static void i2c_status_set(I2C_HandleTypeDef *inst,bool status)
{

}

void HAL_I2C_MSP_Busy_Set(I2C_HandleTypeDef *inst)
{
    i2c_status_set(inst,true);
}

void HAL_I2C_MSP_Idle_Set(I2C_HandleTypeDef *inst)
{
    i2c_status_set(inst,false);
}
