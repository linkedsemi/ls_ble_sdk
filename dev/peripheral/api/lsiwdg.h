#ifndef LSIWDG_H_
#define LSIWDG_H_
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include "HAL_def.h"
#include "reg_lsiwdg_type.h"
#include "reg_base_addr.h"
#ifdef LSIWDG_BASE_ADDR
#define LSIWDG ((reg_iwdg_t *)LSIWDG_BASE_ADDR)
#endif

HAL_StatusTypeDef HAL_IWDG_Init(uint32_t LoadValue);

HAL_StatusTypeDef HAL_IWDG_Refresh(void);

#endif 

