#include <stdint.h>
#include <stdlib.h>
#include "sdk_config.h"
#include "platform.h"
#include "cpu.h"
#include "log.h"

#define ENV_BUF_SIZE 2048
#define DB_BUF_SIZE 4096
#if 0
#define MSG_BUF_SIZE 4096
#define NON_RET_BUF_SIZE 12
#else
#define MSG_BUF_SIZE 8192
#define NON_RET_BUF_SIZE (380*2)
#endif


extern void (*stack_assert_asm_fn)(uint32_t,uint32_t,uint32_t);
extern void (*app_init_fn)(void); 
extern void (*platform_reset_fn)(uint32_t);
extern int (*rand_fn) (void);
extern uint64_t (*idiv_acc_fn)(uint32_t,uint32_t,bool);
extern void (*ecc_calc_fn)(const uint8_t*,const uint8_t*,const uint8_t*,uint8_t*,uint8_t*,void (*)(void *),void *);
extern void (*exit_critical_fn)(void);
extern void (*enter_critical_fn)(void);
extern void (*log_output_fn)(bool linefeed,const char *format,...);
extern void (*log_hex_output_fn)(const void * data_pointer , uint16_t data_length);

extern uint8_t main_task;
extern uint8_t max_activity_num;
extern uint8_t max_profile_num;
extern uint8_t max_ral_num;
extern uint8_t max_user_task_num;

uint32_t task_list_buf[9 + SDK_MAX_PROFILE_NUM + SDK_MAX_USER_TASK_NUM];

struct {
    uint32_t env[5];
}prf_buf[SDK_MAX_PROFILE_NUM];

void* gapc_env_buf[SDK_MAX_CONN_NUM];

void* gattc_env_buf[SDK_MAX_CONN_NUM];

void *l2cc_env_buf[SDK_MAX_CONN_NUM];

void *gapm_env_actvs_buf[SDK_MAX_ACT_NUM];

void* llc_env_buf[SDK_MAX_ACT_NUM];

void* adv_env_buf[SDK_MAX_ACT_NUM];

void* con_env_buf[SDK_MAX_ACT_NUM];

void* per_adv_env_buf[SDK_MAX_ACT_NUM];

void* sync_env_buf[SDK_MAX_ACT_NUM];

struct 
{
    uint32_t env[18];
}act_info_buf[SDK_MAX_ACT_NUM];

struct{
    uint32_t env[2];
}slice_buf[SDK_MAX_ACT_NUM];

struct
{
    uint32_t env[2];
}llcp_tx_buf[(2*SDK_MAX_ACT_NUM)];

struct 
{
    uint32_t env[2];
}rx_buf[9];

struct{
    uint32_t env[2];
}acl_tx_buf[(SDK_MAX_ACT_NUM + 2)];

struct{
    uint32_t env[2];
}adv_tx_buf[SDK_MAX_ACT_NUM];

struct 
{
    uint16_t env[5];
}dev_list_buf[(SDK_MAX_ACT_NUM + 2)];

uint16_t adv_sids_buf[(SDK_MAX_ACT_NUM + 2)];

bool ble_con_state_buf[SDK_MAX_ACT_NUM];

uint8_t per_adv_rep_chain_stat_buf[SDK_MAX_ACT_NUM];

uint8_t llc_state_buf[SDK_MAX_ACT_NUM];

uint8_t gapc_state_buf[SDK_MAX_CONN_NUM];

uint8_t gattc_state_buf[SDK_MAX_CONN_NUM];

uint8_t l2cc_state_buf[SDK_MAX_CONN_NUM];

struct
{
    uint8_t env[39];
}ral_buf[SDK_MAX_RAL_NUM];

uint32_t heap_buf[ENV_BUF_SIZE/sizeof(uint32_t)+DB_BUF_SIZE/sizeof(uint32_t)+MSG_BUF_SIZE/sizeof(uint32_t)+NON_RET_BUF_SIZE/sizeof(uint32_t)];

void statck_buffer_init(uint32_t,uint32_t,uint32_t,uint32_t);

void prf_fn_init(void);

void stack_var_ptr_init()
{
    stack_assert_asm_fn = stack_assert_asm;
    platform_reset_fn = platform_reset;
    ecc_calc_fn = ecc_calc_start;
    rand_fn = rand;
    idiv_acc_fn = idiv_acc;
    enter_critical_fn = enter_critical;
    exit_critical_fn = exit_critical;
    log_output_fn = log_output;
    log_hex_output_fn = log_hex_output;
    
    max_activity_num = SDK_MAX_ACT_NUM;
    max_profile_num = SDK_MAX_PROFILE_NUM;
    max_ral_num = SDK_MAX_RAL_NUM;
    max_user_task_num = SDK_MAX_USER_TASK_NUM;

    statck_buffer_init(ENV_BUF_SIZE,DB_BUF_SIZE,MSG_BUF_SIZE,NON_RET_BUF_SIZE);
    prf_fn_init();
}

static bool dummy()
{
    return true;
}

extern void (*eif_read) (uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy);
extern void (*eif_write)(uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy);
extern void (*eif_flow_on)(void);
extern bool (*eif_flow_off)(void);

__attribute__((weak)) void uart_eif_read(uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy){}
__attribute__((weak)) void uart_eif_write(uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy){}
__attribute__((weak)) void uart_eif_flow_on(void){}
__attribute__((weak)) bool uart_eif_flow_off(void){return false;}

void app_init(void);

void main_task_app_init()
{
    main_task = 3;
    app_init_fn = app_init;
    eif_read = (void (*)(uint8_t *, uint32_t, void (*)(void *, uint8_t), void *))dummy;
    eif_write = (void (*)(uint8_t *, uint32_t, void (*)(void *, uint8_t), void *))dummy;
    eif_flow_on = (void (*)(void))dummy;
    eif_flow_off = (bool (*)(void))dummy;
}


void main_task_itf_init()
{
    main_task = 9;
    app_init_fn = (void (*)(void))dummy;
    eif_read = uart_eif_read;
    eif_write = uart_eif_write;
    eif_flow_on = uart_eif_flow_on;
    eif_flow_off = uart_eif_flow_off;
}

uint8_t peer_id_buf[SDK_MAX_CONN_NUM];

uint8_t *get_peer_id_buf()
{
    return peer_id_buf;
}

#define EM_MACRO_0 (SDK_MAX_ACT_NUM + 2)
const uint32_t em_end = ((((((((((((((((((((((((((112 * (SDK_MAX_ACT_NUM + 4) + 339) & 0xFFFFFFFC) + 12 * EM_MACRO_0 + 3) & 0xFFFFFFFC)
                               + 56 * SDK_MAX_RAL_NUM
                               + 3) & 0xFFFFFFFC)
                             + 171) & 0xFFFFFFFC)
                           + 112 * (SDK_MAX_ACT_NUM + 4)
                           - 445) & 0xFFFFFFFC)
                         + 70 * SDK_MAX_ACT_NUM
                         + 3) & 0xFFFFFFFC)
                       + 63 * SDK_MAX_ACT_NUM
                       + 3) & 0xFFFFFFFC)
                     + 1270 * SDK_MAX_ACT_NUM
                     + 3) & 0xFFFFFFFC)
                   + 105) & 0xFFFFFFFC)
                 + 2307) & 0xFFFFFFFC)
               + (EM_MACRO_0 << 8)
               + 3) & 0xFFFFFFFC)
             + 1011) & 0xFFFFFFFC)
           + 15) & 0xFFFFFFFC)
         + 12;
