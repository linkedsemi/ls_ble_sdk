#ifndef PLATFORM_H_
#define PLATFORM_H_
#include <stdint.h>
#include <stdbool.h>

void sys_init_app(void);

void sys_init_itf(void);

void irq_init(void);

void flash_init_xip_start(void);

void uart_eif_read(uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy);

void uart_eif_write(uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy);

void uart_eif_flow_on(void);

bool uart_eif_flow_off(void);

void stack_assert_asm(uint32_t,uint32_t,uint32_t);

void platform_reset(uint32_t error);

void ecc_calc_start(const uint8_t* secret_key,const uint8_t* pub_x,const uint8_t* pub_y,uint8_t* result_x,uint8_t* result_y,void (*cb)(void *),void *param);

uint64_t idiv_acc(uint32_t,uint32_t,bool);
#endif
