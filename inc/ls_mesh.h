#ifndef _LS_MESH_H_
#define _LS_MESH_H_
#include <stdint.h>
#define __EMPTY 
typedef uint8_t SIGMESH_ModelHandle_TypeDef;
typedef uint8_t SIGMESH_NodeInfo_TypeDef;

#define UUID_MESH_DEV_LEN              (16)
#define MESH_AUTH_DATA_LEN             (16)
#define MAX_MESH_MODEL_NB              (14)
#define MAX_MESH_MODEL_MSG_BUFFER      (380)

enum mesh_evt_type
{
    MESH_ACTIVE_ENABLE                        = 0,
    MESH_ACTIVE_DISABLE                       = 1,
    MESH_ACTIVE_REGISTER_MODEL                = 2,
    MESH_ACTIVE_MODEL_PUBLISH                 = 3,
    MESH_ACTIVE_MODEL_SUBSCRIBE                = 4,        
    MESH_ACTIVE_MODEL_RSP_SEND                = 5,
    MESH_ACTIVE_LPN_START                     = 6,
    MESH_ACTIVE_LPN_STOP                      = 7,
    MESH_ACTIVE_LPN_SELECT_FRIEND             = 8,
    MESH_ACTIVE_PROXY_CTL                     = 9,
    MESH_ACTIVE_STORAGE_LOAD                  = 10,
    MESH_ACTIVE_STORAGE_SAVE                  = 11,
    MESH_ACTIVE_STORAGE_CONFIG                = 12,
    MESH_GET_PROV_INFO                        = 13,
    MESH_GET_PROV_AUTH_INFO                   = 14,   
    MESH_RESET_NODE                           = 15,
    MESH_REPORT_ATTENTION_STATE               = 16,
    MESH_REPOPT_PROV_RESULT                   = 17,
    MESH_ACCEPT_MODEL_INFO                    = 18,
    MESH_RSP_MODEL_INFO                       = 19,  
    MESH_REPORT_TIMER_STATE                   = 20,                   
};

/// Mesh Supported Features
enum mesh_feature
{
    EN_RELAY_NODE       = 0x00000001,
    EN_PROXY_NODE       = 0x00000002,
    EN_FRIEND_NODE      = 0x00000004,
    EN_LOW_POWER_NODE   = 0x00000008,
    EN_MSG_API          = 0x00010000,
    EN_PB_GATT          = 0x00020000,
    EN_DYN_BCN_INTV     = 0x00040000,
};

enum mesh_provisioned_state
{
    PROVISIONED_OK = 0x00,
    UNPROVISIONED_KO = 0x01,
};

enum mesh_provision_result
{
    PROV_FAIL = 0x00,
    PROV_SUCCESS = 0x01,
};

enum mesh_timer_state
{
     MESH_TIMER_DOING  = 0x00,
     MESH_TIMER_DONE   = 0x01,
};

struct reqister_model_info
{
    uint32_t model_id;
    uint8_t elmt_idx;
    uint8_t config;
};

 struct report_model_loc_id_info
 {
   uint8_t nb_model;
   uint16_t model_lid[MAX_MESH_MODEL_NB]; 
};

struct report_dev_provisioned_state_info
{
    uint8_t proved_state;  
};

struct mesh_prov_info
{
    uint8_t  DevUuid[UUID_MESH_DEV_LEN];
    uint32_t  UriHash;
    uint16_t  OobInfo;
    uint8_t  PubKeyOob;
    uint8_t  StaticOob;
    uint8_t  OutOobSize;
    uint8_t  InOobSize;
    uint16_t OutOobAction;
    uint16_t InOobAction;
    uint8_t  Info;
};

struct mesh_prov_auth_info
{
    uint8_t  Adopt;
    uint8_t  AuthSize;
    uint8_t  AuthBuffer[MESH_AUTH_DATA_LEN];
};

struct report_mesh_attention_info
{
    uint8_t  state;
};

struct rsp_model_info
{
    uint8_t  ModelHandle;
    uint32_t opcode;
    uint16_t len;
    uint8_t  info[MAX_MESH_MODEL_MSG_BUFFER];
};

struct model_rx_info
{
    uint8_t  ModelHandle;
    uint8_t  app_key_lid;
    int8_t   rssi;
    uint8_t  not_relayed;
    uint32_t opcode;
    uint16_t rx_info_len;
    uint8_t  info[__EMPTY];
};


struct report_mesh_prov_result_info
{
   uint8_t state;
   uint16_t status;
};

struct report_mesh_timer_state_info
{
   uint8_t timer_id;
   uint8_t status;
};

union mesh_evt_u
{
    struct reqister_model_info register_model_param;
    struct report_model_loc_id_info loc_id_param;
    struct report_dev_provisioned_state_info st_proved;
    struct report_mesh_attention_info update_attention;
    struct report_mesh_prov_result_info prov_rslt_sate;
    struct model_rx_info rx_msg;
    struct report_mesh_timer_state_info mesh_timer_state;
};


struct ls_mesh_cfg
{
    uint32_t MeshFeatures;
    uint16_t MeshCompanyID;
    uint16_t MeshProID;
    uint16_t MeshProVerID;
    uint16_t MeshLocDesc;
    uint8_t  NbCompDataPage;
    uint8_t FrdRxWindowMS;
    uint8_t FrdQueueSize;
    uint8_t FrdSubsListSize;
};


struct model_id_info
{
   uint8_t nb_model;
   uint16_t model_id[MAX_MESH_MODEL_NB]; 
};

void prf_mesh_callback_init(void (*evt_cb)(enum mesh_evt_type,union mesh_evt_u *));
void dev_manager_prf_mesh_add(uint8_t sec_lvl, struct ls_mesh_cfg *cfg);
void ls_mesh_init(struct model_id_info *param);
void ls_mesh_platform_reset(void);
void set_prov_param( struct mesh_prov_info *param);
void set_prov_auth_info(struct mesh_prov_auth_info *param);
void model_subscribe(uint8_t const ModelHandle, uint16_t const Addr);
void rsp_model_info_ind(struct rsp_model_info *param);
void TIMER_Set(uint8_t TimerID, uint32_t DelayMS);
void TIMER_Cancel(uint8_t TimerID);
void SIGMESH_UnbindAll(void);
#endif //(_LS_MESH_H_
