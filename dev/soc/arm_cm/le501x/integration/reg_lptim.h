#ifndef REG_LPTIM_H_
#define REG_LPTIM_H_
#include <stdint.h>
#include "reg_base_addr.h"
typedef struct
{
    volatile uint32_t CON0;
    volatile uint32_t CON1;
    volatile uint32_t ARR;
    volatile uint32_t CNT;
    volatile uint32_t CMP;
    volatile uint32_t IER;
    volatile uint32_t ISR;
    volatile uint32_t IFC;
    volatile uint32_t UPDATE;
    volatile uint32_t SYNCSTAT;
}reg_lptim_t;

#define LPTIM ((reg_lptim_t *)0x40010000)
#endif
