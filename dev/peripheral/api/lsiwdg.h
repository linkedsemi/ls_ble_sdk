#ifndef LSIWDG_H_
#define LSIWDG_H_
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include "HAL_def.h"

HAL_StatusTypeDef HAL_IWDG_Init(uint32_t LoadValue);

HAL_StatusTypeDef HAL_IWDG_Refresh(void);

#endif 

