#ifndef _LS_SIG_MESH_PROVISIONER_H_
#define _LS_SIG_MESH_PROVISIONER_H_
#include <stdint.h>
#include "ls_ble.h"
#include "ls_sig_mesh.h"
#define __EMPTY

#define UUID_MESH_DEV_LEN (16)
#define MESH_AUTH_DATA_LEN (16)
#define MAX_MESH_MODEL_NB (14)
#define MAX_AUTO_PROV_MESH_MODEL_NB (10)
//#define MAX_MESH_MODEL_MSG_BUFFER      (380)
//#define MAX_MESH_MODEL_MSG_BUFFER (60)
#define CONNECT_IDX_INVALID 0xff
/*provisioner infomation*/
#define MESH_KEY_LENGTH (16)

//mesh server model index
#define GENERIC_ONOFF_SERVER             (0x1000)
#define GENERIC_ONOFF_CLIENT             (0x1001)
#define GENERIC_LVL_SERVER               (0x1002)
#define GENERIC_LVL_CLIENT               (0x1003)
#define LIGHTNESS_SERVER                 (0x1300)
#define LIGHTS_CTL_SERVER                (0x1303)
#define LIGHTS_HSL_SERVER                (0x1307)
#define VENDOR_TMALL_SERVER              (0x01A80000)
        
//Generic OnOff        
#define GENERIC_ONOFF_GET                (0x0182)
#define GENERIC_ONOFF_SET                (0x0282)
#define GENERIC_ONOFF_SET_UNAK           (0x0382)
#define GENERIC_ONOFF_STATUS             (0x0482)

//Generic OnOff        
#define GENERIC_LVL_GET                  (0x0582)
#define GENERIC_LVL_SET                  (0x0682)
#define GENERIC_LVL_SET_UNAK             (0x0782)
#define GENERIC_LVL_STATUS               (0x0882)

//Light Lightness        
#define LIGHT_LIGHTNESS_SET              (0x4c82)
#define LIGHT_LIGHTNESS_SET_UNAK         (0x4d82)
#define LIGHT_LIGHTNESS_STATUS           (0x4e82)

//Light HSL
#define LIGHT_HSL_SET                    (0x7682)
#define LIGHT_HSL_SET_UNACK              (0x7782)
#define LIGHT_HSL_STATUS                 (0x7882)

//Light CTL
#define LIGHT_CTL_SET                    (0x5E82)
#define LIGHT_CTL_SET_UNACK              (0x5F82)
#define LIGHT_CTL_STATUS                 (0x6082)



#define VENDOR_OPCODE_LEN (3)

enum config_client_get_type
{
    CONFIG_CLIENT_GET_TYPE_BEACON = 0,
    CONFIG_CLIENT_GET_TYPE_DFLT_TTL,
    CONFIG_CLIENT_GET_TYPE_GATT_PROXY,
    CONFIG_CLIENT_GET_TYPE_RELAY,
    CONFIG_CLIENT_GET_TYPE_FRIEND,
    CONFIG_CLIENT_GET_TYPE_HB_PUBLI,
    CONFIG_CLIENT_GET_TYPE_HB_SUBS,
    CONFIG_CLIENT_GET_TYPE_NET_TRANSMIT,
    CONFIG_CLIENT_GET_TYPE_NET_KEYS,
    CONFIG_CLIENT_GET_TYPE_COMPO_DATA,
    CONFIG_CLIENT_GET_TYPE_LPN_POLLTIEOUT,
    CONFIG_CLIENT_GET_TYPE_MAX,
};

enum mesh_provisioner_evt_type
{
    MESH_PROVER_ACTIVE_ENABLE = 0,
    MESH_PROVER_ACTIVE_DISABLE,
    MESH_PROVER_ACTIVE_NODE_FOUND,
    MESH_PROVER_ACTIVE_NODE_GATT,
    MESH_PROVER_ACTIVE_NODE_STOPPED,
    MESH_PROVER_ACTIVE_PROXY_SVC,
    MESH_PROVER_ACTIVE_STATE,
    MESH_PROVER_GET_PROV_AUTH_INFO,
    MESH_PROVER_KEY_DEV_ADD_RSP_INFO,
    MESH_PROVER_KEY_NET_ADD_IND,
    MESH_PROVER_KEY_APP_ADD_IND,
    MESH_PROVER_HEALTH_MODEL_RSP_INFO,
    MESH_PROVER_SET_DEV_RSP_INFO,
    MESH_PROVER_IDENTIFY_REQ_IND_INFO,

};

enum prover_provisioning_state
{
    PROVISIONING_STARTED,
    PROVISIONING_SUCCEED,
    PROVISIONING_FAILED,
};

struct prover_node_scan_info
{
   uint32_t uri_hash;
   uint16_t oob_info;
   int8_t rssi;
   uint8_t dev_uuid[UUID_MESH_DEV_LEN];  
}__attribute__ ((packed));

struct prover_active_state_info
{
   uint8_t state;
   uint16_t unicast_addr;
}__attribute__ ((packed));

struct prover_add_dev_key_rsp_info
{
    uint8_t  dev_key_lid;
}__attribute__ ((packed));

struct prover_add_net_key_ind_info
{
    uint8_t  net_key_lid;
}__attribute__ ((packed));

struct prover_add_app_key_ind_info
{
    uint8_t  app_key_lid;
}__attribute__ ((packed));


struct prover_health_client_model_rsp_info
{
    uint8_t  mdl_lid;
}__attribute__ ((packed));

struct prover_identify_req_ind_info
{
    uint8_t  dev_nb_elt;
    uint16_t dev_algorithms;
    uint8_t  dev_pub_key_type;
    uint8_t  dev_static_oob_type;
    uint8_t  dev_out_oob_size;
    uint16_t dev_out_oob_action;
    uint8_t  dev_in_oob_size;
    uint16_t dev_in_oob_action;
}__attribute__ ((packed));


union ls_sig_mesh_provisioner_evt_u {
  struct prover_node_scan_info prover_node_scan_info;
  struct prover_active_state_info  prover_node_state_info;
  struct prover_add_dev_key_rsp_info  prover_node_add_dev_key_rsp_info;
  struct prover_health_client_model_rsp_info prover_node_health_model_rsp_info;
  struct prover_add_net_key_ind_info prover_node_add_net_key_ind_info;
  struct prover_add_app_key_ind_info prover_node_add_app_key_ind_info;
  struct prover_identify_req_ind_info prover_identify_req_ind_info;
};
/**
 * @brief 
 * 
 * @param evt_cb 
 */
void prf_ls_sig_mesh_provisioner_callback_init(void (*evt_cb)(enum mesh_provisioner_evt_type, union ls_sig_mesh_provisioner_evt_u *));
/**
 * @brief 
 * 
 */
void ls_sig_mesh_provisioner_init(void);
/**
 * @brief 
 * 
 * @param address 
 * @param dev_key 
 */
void ls_sig_mesh_provisioner_add_dev_key(const uint16_t address, const uint8_t* dev_key);
/**
 * @brief 
 * 
 * @param netkey_id 
 * @param net_key 
 */
void ls_sig_mesh_provisioner_add_net_key(const uint16_t netkey_id, const uint8_t* net_key);
/**
 * @brief 
 * 
 * @param netkey_id 
 */
void ls_sig_mesh_provisioner_use_new_net_key(const uint8_t netkey_id);
/**
 * @brief 
 * 
 * @param netkey_id 
 * @param net_key 
 */
void ls_sig_mesh_provisioner_update_net_key(const uint8_t netkey_id, const uint8_t* net_key);
/**
 * @brief 
 * 
 * @param netkey_id 
 */
void ls_sig_mesh_provisioner_revoke_old_net_key(const uint8_t netkey_id);
/**
 * @brief 
 * 
 * @param netkey_id 
 */
void ls_sig_mesh_provisioner_delete_net_key(const uint8_t netkey_id);
/**
 * @brief 
 * 
 * @param netkey_id 
 * @param appkey_id 
 * @param app_key 
 */
void ls_sig_mesh_provisioner_add_app_key(const uint8_t netkey_id, const uint8_t appkey_id, const uint8_t* app_key);
/**
 * @brief 
 * 
 * @param netkey_id 
 * @param appkey_id 
 * @param app_key 
 */
void ls_sig_mesh_provisioner_update_app_key(const uint8_t netkey_id, const uint8_t appkey_id, const uint8_t* app_key);
/**
 * @brief 
 * 
 * @param netkey_id 
 * @param appkey_id 
 */
void ls_sig_mesh_provisioner_delete_app_key(const uint8_t netkey_id, const uint8_t appkey_id);
/**
 * @brief 
 * 
 * @param timeout 
 */
void ls_sig_mesh_provisioner_scan(uint16_t timeout);
/**
 * @brief 
 * 
 * @param conidx 
 * @param dev_uuid 
 * @param attention_dur_s 
 */
void ls_sig_mesh_provisioner_invite(const uint8_t conidx, const uint8_t *dev_uuid,const uint8_t attention_dur_s);
/**
 * @brief 
 * 
 */
void ls_sig_mesh_provisioner_stop(void);
/**
 * @brief 
 * 
 * @param param 
 */
void app_mesh_prover_set_prov_auth_info(struct mesh_prov_auth_info *param);
/**
 * @brief 
 * 
 * @param accept 
 * @param netkey_lid 
 * @param unicast_addr 
 * @param algo 
 * @param pub_key 
 * @param auth_method 
 * @param auth_action 
 * @param auth_size 
 */
void ls_sig_mesh_identify_cfm(bool accept, uint8_t netkey_lid, uint16_t unicast_addr, uint8_t algo, uint8_t pub_key, uint8_t auth_method, uint8_t auth_action, uint8_t auth_size);
/**
 * @brief 
 * 
 */
void ls_sig_mesh_prover_config_reg_model(void);
/**
 * @brief 
 * 
 * @param dev_key_lid 
 * @param net_key_lid 
 * @param primary_addr 
 */
void ls_sig_mesh_prover_config_set_dev(uint8_t dev_key_lid, uint8_t net_key_lid, uint16_t primary_addr);
/**
 * @brief 
 * 
 * @param act_netkey_type 
 * @param net_key_id 
 * @param data 
 * @param data_len 
 */
void ls_sig_mesh_prover_config_client_active_netkey(uint8_t act_netkey_type, uint16_t net_key_id, const uint8_t *data ,uint8_t data_len);
/**
 * @brief 
 * 
 * @param act_appkey_type 
 * @param net_key_id 
 * @param app_key_id 
 * @param appkey 
 */
void ls_sig_mesh_prover_config_client_active_appkey(uint8_t act_appkey_type, uint16_t net_key_id, uint16_t app_key_id, const uint8_t *appkey);
/**
 * @brief 
 * 
 * @param get_model_type 
 * @param element_address 
 * @param model_id 
 */
void ls_sig_mesh_prover_config_client_get_model(uint8_t get_model_type, uint16_t element_address, uint32_t model_id);
/**
 * @brief 
 * 
 * @param model_subscript_act_type 
 * @param element_address 
 * @param model_id 
 * @param address_type 
 * @param length 
 * @param address_uuid 
 */
void ls_sig_mesh_prover_config_client_act_model_subscript(uint8_t model_subscript_act_type, uint16_t element_address, uint32_t model_id, bool address_type, uint16_t length, const uint8_t *address_uuid);
/**
 * @brief 
 * 
 * @param addr_type 
 * @param element_addr 
 * @param appkey_id 
 * @param cred_flag 
 * @param pub_ttl 
 * @param pub_period 
 * @param retx_cnt 
 * @param retx_intv_slots 
 * @param mdl_id 
 * @param length 
 * @param val 
 */
void ls_sig_mesh_prover_config_client_set_model_publication(uint8_t addr_type, uint16_t element_addr, uint16_t appkey_id, bool cred_flag, uint8_t pub_ttl, uint8_t pub_period, uint8_t retx_cnt, uint8_t retx_intv_slots, uint32_t mdl_id, uint16_t length, const uint8_t* val);
/**
 * @brief 
 * 
 * @param mdl_app_act_type 
 * @param element_addr 
 * @param appkey_id 
 * @param mdl_id 
 */
void ls_sig_mesh_prover_config_client_act_model_app(uint8_t mdl_app_act_type, uint16_t element_addr, uint16_t appkey_id, uint32_t mdl_id);
/**
 * @brief 
 * 
 * @param set_type 
 * @param value 
 * @param tx_cnt 
 * @param intv_slots 
 */
void ls_sig_mesh_prover_config_client_set(uint8_t set_type, uint8_t value, uint8_t tx_cnt, uint8_t intv_slots);
/**
 * @brief 
 * 
 * @param get_type 
 * @param value 
 */
void ls_sig_mesh_prover_config_client_get(uint8_t get_type, uint16_t value);
/**
 * @brief 
 * 
 * @param dst 
 * @param cnt 
 * @param period_s 
 * @param ttl 
 * @param features 
 * @param nekkey_id 
 */
void ls_sig_mesh_prover_config_client_set_heartbeat_publication(uint16_t dst, uint16_t cnt, uint16_t period_s, uint8_t ttl, uint16_t features, uint16_t nekkey_id);
/**
 * @brief 
 * 
 * @param src 
 * @param dst 
 * @param period_s 
 */
void ls_sig_mesh_prover_config_client_set_heartbeat_subscription(uint16_t src, uint16_t dst, uint16_t period_s);
/**
 * @brief 
 * 
 * @param address 
 * @param appkey_lid 
 * @param get_type 
 * @param company_id 
 */
void ls_sig_mesh_prover_health_client_get(uint16_t address, uint8_t appkey_lid, uint8_t get_type, uint16_t company_id);
/**
 * @brief 
 * 
 * @param address 
 * @param appkey_lid 
 * @param set_type 
 * @param set_cfg 
 * @param val 
 */
void ls_sig_mesh_prover_health_client_set(uint16_t address, uint8_t appkey_lid, uint8_t set_type, uint8_t set_cfg, uint8_t val);
/**
 * @brief 
 * 
 * @param address 
 * @param appkey_lid 
 * @param act_type 
 * @param act_cfg 
 * @param test_id 
 * @param company_id 
 */
void ls_sig_mesh_prover_health_client_act_fault(uint16_t address, uint8_t appkey_lid, uint8_t act_type, uint8_t act_cfg, uint8_t test_id, uint16_t company_id);
/**
 * @brief 
 * 
 */
void ls_sig_mesh_prover_health_client_reg_model(void);
/**
 * @brief 
 * 
 */
#endif //(_LS_SIG_MESH_PROVISIONER_H_
