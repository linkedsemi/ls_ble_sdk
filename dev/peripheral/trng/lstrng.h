#ifndef __LSTRNG_H__
#define __LSTRNG_H__
#include "stdint.h"
#include "reg_lstrng.h"
#include "reg_lstrng_type.h"

void lstrng_init(void);
void lstrng_deinit(void);
void lstrng_start(void);
void lstrng_stop(void);
uint32_t lstrng_getdata(void);

#endif //(__LSTRNG_H__)
