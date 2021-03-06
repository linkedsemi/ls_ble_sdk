#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "sdk_config.h"
#include "platform.h"
#include "cpu.h"
#include "log.h"

uint8_t peer_id_buf[SDK_MAX_CONN_NUM];

uint8_t *get_peer_id_buf()
{
    return peer_id_buf;
}

void app_init(void);
void host_read(uint8_t *, uint32_t, void (*)(void *, uint8_t), void *);
void host_write(uint8_t *, uint32_t ,void (*)(void *, uint8_t), void*);
void host_flow_on();
bool host_flow_off();
void controller_read(uint8_t *buf,uint16_t length,void (*cb)());
void controller_write(uint8_t *buf,uint16_t length,void (*cb)());


extern uint8_t main_task;
extern uint8_t max_activity_num;
extern uint8_t max_profile_num;
extern uint8_t max_ral_num;
extern uint8_t max_user_task_num;
extern void (*stack_assert_asm_fn)(uint32_t,uint32_t,uint32_t);
extern void (*platform_reset_fn)(uint32_t);
extern int (*rand_fn) (void);
extern void (*exit_critical_fn)(uint32_t);
extern uint32_t (*enter_critical_fn)(void);
extern void (*log_output_fn)(bool linefeed,const char *format,...);
extern void (*log_hex_output_fn)(const void * data_pointer , uint16_t data_length);
extern void (*app_init_fn)(void); 
extern void (*eif_read) (uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy);
extern void (*eif_write)(uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy);
extern void (*eif_flow_on)(void);
extern bool (*eif_flow_off)(void);
extern void (*hci_read)(uint8_t *buf,uint16_t length,void (*cb)());
extern void (*hci_write)(uint8_t *buf,uint16_t length,void (*cb)());

void main_task_app_init()
{
    main_task = 0;
    app_init_fn = app_init;
    eif_read = host_read;
    eif_write = host_write;
    eif_flow_on = host_flow_on;
    eif_flow_off = host_flow_off;
    hci_write = controller_write;
    hci_read = controller_read;
}

__attribute__((weak)) void uart_eif_read(uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy){}
__attribute__((weak)) void uart_eif_write(uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy){}


void main_task_itf_init()
{
    hci_write = (void *)uart_eif_write;
    hci_read = (void *)uart_eif_read;
}

void host_buffer_init();
void prf_fn_init(void);
void ble_storage_max_num_init(uint8_t num);
void stack_var_ptr_init()
{
    stack_assert_asm_fn = stack_assert_asm;
    platform_reset_fn = platform_reset;
    rand_fn = rand;
    enter_critical_fn = enter_critical;
    exit_critical_fn = exit_critical;
    log_output_fn = log_output;
    log_hex_output_fn = log_hex_output;

    host_buffer_init();
    prf_fn_init();
    ble_storage_max_num_init(SDK_BLE_STORAGE_PEER_MAX);
}
