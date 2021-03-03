#ifndef FLASH_SVCALL_INT_H_
#define FLASH_SVCALL_INT_H_
#include "ARMCM3.h"
#include "svcall.h"
#ifdef FLASH_PROG_ALGO
#define GLOBAL_INT_MASK_STATUS() __get_PRIMASK()
#else
#define GLOBAL_INT_MASK_STATUS() __get_BASEPRI()
#endif

#endif
