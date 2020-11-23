#ifndef UART_MSP_H_
#define UART_MSP_H_
#include "lsuart.h"

void HAL_UART_MSP_Init(UART_HandleTypeDef *inst);
void HAL_UART_MSP_DeInit(UART_HandleTypeDef *inst);
void HAL_UART_MSP_Busy_Set(UART_HandleTypeDef *inst);
void HAL_UART_MSP_Idle_Set(UART_HandleTypeDef *inst);

#endif
