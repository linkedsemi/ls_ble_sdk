#include <stdint.h>
#include "sdk_config.h"
#include "platform.h"

extern void *_gapc_env_ptr;
extern void *_gapc_state_ptr;
extern void *_gattc_env_ptr;
extern void *_gapm_env_actvs_ptr;
extern void *_prf_env_tag_ptr;
extern void *_gattc_state_ptr;
extern void *_l2cc_env_ptr;
extern void *_l2cc_state_ptr;
extern void *_ble_util_buf_env_llcp_tx_pool_ptr;
extern void *_ble_util_buf_env_rx_pool_ptr;
extern void *_ble_util_buf_env_acl_tx_pool_ptr;
extern void *_ble_util_buf_env_adv_tx_pool_ptr;

extern void *_llc_env_ptr;
extern void *_llc_state_ptr;
extern void *_lld_env_adv_sids_ptr;
extern void *_lld_adv_env_ptr;
extern void *_lld_con_env_ptr;
extern void *_lld_per_adv_env_ptr;
extern void *_lld_sync_env_ptr;
extern void *_llm_env_ral_ptr;
extern void *_llm_env_act_info_ptr;
extern void *_llm_env_dev_list_ptr;
extern void *_hci_env_ble_con_state_ptr;
extern void *_hci_tl_env_per_adv_rep_chain_stat_ptr;
extern void *_sch_slice_env_ble_ptr;
extern void *_ke_task_env_task_list_ptr;
extern void *_rwip_heap_env_ptr;
extern void *_rwip_heap_db_ptr;
extern void *_rwip_heap_msg_ptr;
extern void *_rwip_heap_non_ret_ptr;


extern void (*rwip_assert_asm_fn)(uint32_t,uint32_t,uint32_t);
extern void (*app_init_fn)(void); 
extern void (*platform_reset_fn)(uint32_t);
extern struct rwip_eif_api* (*rwip_eif_get_fn)(uint8_t idx);
extern void (*rand_init_fn) (unsigned int seed);
extern int (*rand_fn) (void);
extern uint64_t (*idiv_acc_fn)(uint32_t,uint32_t,bool);
extern void (*ecc_calc_fn)(const uint8_t*,const uint8_t*,const uint8_t*,uint8_t*,uint8_t*,void *);

extern uint8_t main_task;
extern uint8_t max_activity_num;
extern uint8_t max_profile_num;
extern uint8_t max_ral_num;
extern uint8_t max_user_task_num;

extern uint32_t rwip_heap_env_size; 

extern uint32_t rwip_heap_db_size;

extern uint32_t rwip_heap_msg_size;

extern uint32_t rwip_heap_non_ret_size;


uint32_t ke_task_env_task_list_buf[9 + SDK_MAX_PROFILE_NUM + SDK_MAX_USER_TASK_NUM];

struct {
	uint32_t env[5];
}prf_env_tag_buf[SDK_MAX_PROFILE_NUM];

void* gapc_env_buf[SDK_MAX_CONN_NUM];

void* gattc_env_buf[SDK_MAX_CONN_NUM];

void *l2cc_env_buf[SDK_MAX_CONN_NUM];

void *gapm_env_actvs_buf[SDK_MAX_ACT_NUM];

void* llc_env_buf[SDK_MAX_ACT_NUM];

void* lld_adv_env_buf[SDK_MAX_ACT_NUM];

void* lld_con_env_buf[SDK_MAX_ACT_NUM];

void* lld_per_adv_env_buf[SDK_MAX_ACT_NUM];

void* lld_sync_env_buf[SDK_MAX_ACT_NUM];

struct 
{
	uint32_t env[18];
}llm_env_act_info_buf[SDK_MAX_ACT_NUM];

struct{
	uint32_t env[2];
}sch_slice_env_ble_buf[SDK_MAX_ACT_NUM];

struct
{
	uint32_t env[2];
}ble_util_buf_env_llcp_tx_pool_buf[(2*SDK_MAX_ACT_NUM)];

struct 
{
	uint32_t env[2];
}ble_util_buf_env_rx_pool_buf[9];

struct{
	uint32_t env[2];
}ble_util_buf_env_acl_tx_pool_buf[(SDK_MAX_ACT_NUM + 2)];

struct{
	uint32_t env[2];
}ble_util_buf_env_adv_tx_pool_buf[SDK_MAX_ACT_NUM];

struct 
{
	uint16_t env[5];
}llm_env_dev_list_buf[(SDK_MAX_ACT_NUM + 2)];

uint16_t lld_env_adv_sids_buf[(SDK_MAX_ACT_NUM + 2)];

bool hci_env_ble_con_state_buf[SDK_MAX_ACT_NUM];

uint8_t hci_tl_env_per_adv_rep_chain_stat_buf[SDK_MAX_ACT_NUM];

uint8_t llc_state_buf[SDK_MAX_ACT_NUM];

uint8_t gapc_state_buf[SDK_MAX_CONN_NUM];

uint8_t gattc_state_buf[SDK_MAX_CONN_NUM];

uint8_t l2cc_state_buf[SDK_MAX_CONN_NUM];

struct
{
	uint8_t env[39];
}llm_env_ral_buf[SDK_MAX_RAL_NUM];

#define ENV_BUF_SIZE 2048
#define DB_BUF_SIZE 4096
#define MSG_BUF_SIZE 4096
#define NON_RET_BUF_SIZE 0

uint32_t rwip_heap_env_buf[ENV_BUF_SIZE/sizeof(uint32_t)];

uint32_t rwip_heap_db_buf[DB_BUF_SIZE/sizeof(uint32_t)];

uint32_t rwip_heap_msg_buf[MSG_BUF_SIZE/sizeof(uint32_t)];

uint32_t rwip_heap_non_ret_buf[NON_RET_BUF_SIZE/sizeof(uint32_t)];

void stack_var_ptr_init()
{
    _gapc_env_ptr = &gapc_env_buf;
    _gapc_state_ptr = &gapc_state_buf;
    _gattc_env_ptr = &gattc_env_buf;
    _gapm_env_actvs_ptr = &gapm_env_actvs_buf;
    _prf_env_tag_ptr = &prf_env_tag_buf;
    _gattc_state_ptr = &gattc_state_buf;
    _l2cc_env_ptr = &l2cc_env_buf;
    _l2cc_state_ptr = &l2cc_state_buf;
    _ble_util_buf_env_llcp_tx_pool_ptr = &ble_util_buf_env_llcp_tx_pool_buf;
    _ble_util_buf_env_rx_pool_ptr = &ble_util_buf_env_rx_pool_buf;
    _ble_util_buf_env_acl_tx_pool_ptr = &ble_util_buf_env_acl_tx_pool_buf;
    _ble_util_buf_env_adv_tx_pool_ptr = &ble_util_buf_env_adv_tx_pool_buf;
	
    _llc_env_ptr = &llc_env_buf;
    _llc_state_ptr = &llc_state_buf;
    _lld_env_adv_sids_ptr = &lld_env_adv_sids_buf;
    _lld_adv_env_ptr = &lld_adv_env_buf;
    _lld_con_env_ptr = &lld_con_env_buf;
    _lld_per_adv_env_ptr = &lld_per_adv_env_buf;
    _lld_sync_env_ptr = &lld_sync_env_buf;
    _llm_env_ral_ptr = &llm_env_ral_buf;
    _llm_env_act_info_ptr = &llm_env_act_info_buf;
    _llm_env_dev_list_ptr = &llm_env_dev_list_buf;
    _hci_env_ble_con_state_ptr = &hci_env_ble_con_state_buf;
    _hci_tl_env_per_adv_rep_chain_stat_ptr = &hci_tl_env_per_adv_rep_chain_stat_buf;
    _sch_slice_env_ble_ptr = &sch_slice_env_ble_buf;
    _ke_task_env_task_list_ptr = &ke_task_env_task_list_buf;
    _rwip_heap_env_ptr = &rwip_heap_env_buf;
    _rwip_heap_db_ptr = &rwip_heap_db_buf;
    _rwip_heap_msg_ptr = &rwip_heap_msg_buf;
    _rwip_heap_non_ret_ptr = &rwip_heap_non_ret_buf;


    rwip_assert_asm_fn = stack_assert_asm;
    platform_reset_fn = platform_reset;
    ecc_calc_fn = ecc_calc_start;
	rand_init_fn = true_rand_init;
	rand_fn = true_rand_gen;
    idiv_acc_fn = idiv_acc;

    max_activity_num = SDK_MAX_ACT_NUM;
    max_profile_num = SDK_MAX_PROFILE_NUM;
    max_ral_num = SDK_MAX_RAL_NUM;
    max_user_task_num = SDK_MAX_USER_TASK_NUM;

	rwip_heap_env_size = ENV_BUF_SIZE;
	rwip_heap_db_size = DB_BUF_SIZE;
	rwip_heap_msg_size = MSG_BUF_SIZE;
	rwip_heap_non_ret_size = NON_RET_BUF_SIZE;
}

static bool dummy()
{
	return true;
}

extern void (*eif_read) (uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy);
extern void (*eif_write)(uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy);
extern void (*eif_flow_on)(void);
extern bool (*eif_flow_off)(void);

extern void app_init(void);

void main_task_app_init()
{
    main_task = 3;
    app_init_fn = app_init;
	eif_read = (void *)dummy;
	eif_write = (void *)dummy;
	eif_flow_on = (void *)dummy;
	eif_flow_off = (void *)dummy;
}


void main_task_itf_init()
{
    main_task = 9;
	app_init_fn = (void *)dummy;
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
