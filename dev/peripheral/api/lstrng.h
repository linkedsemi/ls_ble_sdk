#ifndef LSTRNG_H_
#define LSTRNG_H_
#include <stdint.h>
#include "HAL_def.h"
#include "reg_base_addr.h"
#include "reg_lstrng_type.h"
#ifdef LSTRNG_BASE_ADDR
#define LSTRNG ((reg_trng_t *)LSTRNG_BASE_ADDR)
#endif

HAL_StatusTypeDef HAL_TRNG_Init(void);
HAL_StatusTypeDef HAL_TRNG_DeInit(void);

HAL_StatusTypeDef HAL_TRNG_GenerateRandomNumber(uint32_t *random32bit);
HAL_StatusTypeDef HAL_TRNG_GenerateRandomNumber_IT(void);

void HAL_TRNG_IRQHandler(void);
void HAL_TRNG_ReadyDataCallback(uint32_t random32bit);

#endif
