#include "i2c_msp.h"
#include "field_manipulate.h"
#include "ARMCM3.h"
#include "HAL_def.h"
#include "platform.h"

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

        
    break;
    case (uint32_t)I2C2:


    break;
    case (uint32_t)I2C3:


    break;
    }
}

void HAL_I2C_MSP_DeInit(I2C_HandleTypeDef *inst)
{
    switch((uint32_t)inst->Instance)
    {
    case (uint32_t)I2C1:

    break;
    case (uint32_t)I2C2:

    break;
    case (uint32_t)I2C3:

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
