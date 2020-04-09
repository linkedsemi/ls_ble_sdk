#ifndef LS_BLE_H_
#define LS_BLE_H_
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#define INVALID_CON_IDX (0xff)
#define INVALID_PEER_ID (0xff)

#define BLE_ADDR_LEN 6
#define BLE_KEY_LEN 16

enum gap_own_addr_type
{
    PUBLIC_OR_RANDOM_STATIC_ADDR,
    RESOLVABLE_PRIVATE_ADDR,
    NON_RESOLVABLE_PRIVATE_ADDR,
};

enum gap_peer_addr_type
{
    PUBLIC_ADDR,
    RANDOM_ADDR,
};

struct dev_addr
{
    uint8_t addr[BLE_ADDR_LEN];
};

enum sec_lvl_type
{
    NO_SEC,
    UNAUTH_SEC,
    AUTH_SEC,
    SEC_CON_SEC,
};

struct legacy_adv_prop
{
    uint8_t connectable:1,
            scannable:1,
            directed:1,
            high_duty_cycle:1;
};

enum adv_disc_mode
{
    /// Mode in non-discoverable
    ADV_MODE_NON_DISC = 0,
    /// Mode in general discoverable
    ADV_MODE_GEN_DISC,
    /// Mode in limited discoverable
    ADV_MODE_LIM_DISC,
    /// Broadcast mode without presence of AD_TYPE_FLAG in advertising data
    ADV_MODE_BEACON,
    ADV_MODE_MAX,
};

struct legacy_adv_obj_param
{
    struct dev_addr *peer_addr;
    uint16_t adv_intv_min;
    uint16_t adv_intv_max;
    enum gap_own_addr_type own_addr_type;
    enum gap_peer_addr_type peer_addr_type;
    uint8_t filter_policy;
    uint8_t ch_map;
    enum adv_disc_mode disc_mode;
    struct legacy_adv_prop prop;
};

enum scan_type
{
    GENERAL_DISCOVERABLE,
    LIMITED_DISCOVERABLE,
    OBSERVER,
    OBSERVER_WHITELIST,
    CONNECTABLE,
    CONNECTABLE_WHITELIST,
};

struct start_scan_param
{
     uint16_t scan_intv;
    uint16_t scan_window;
    uint16_t duration;
    uint16_t period;
    enum scan_type type;
    uint8_t active:1,
            filter_duplicates:2;
};

enum init_type
{
    DIRECT_CONNECTION,
    AUTO_CONNECTION_WHITELIST,
};

struct start_init_param
{
    struct dev_addr *peer_addr;
    uint16_t scan_intv;
    uint16_t scan_window;
    /// Timeout for automatic connection establishment (in unit of 10ms). Cancel the procedure if not all
    /// indicated devices have been connected when the timeout occurs. 0 means there is no timeout
    uint16_t conn_to;
    /// Minimum value for the connection interval (in unit of 1.25ms). Shall be less than or equal to
    /// conn_intv_max value. Allowed range is 7.5ms to 4s.
    uint16_t conn_intv_min;
    /// Maximum value for the connection interval (in unit of 1.25ms). Shall be greater than or equal to
    /// conn_intv_min value. Allowed range is 7.5ms to 4s.
    uint16_t conn_intv_max;
    /// Slave latency. Number of events that can be missed by a connected slave device
    uint16_t conn_latency;
    /// Link supervision timeout (in unit of 10ms). Allowed range is 100ms to 32s
    uint16_t supervision_to;
    uint8_t peer_addr_type;
    enum init_type type;
};


enum dev_evt_type
{
    STACK_INIT,
    STACK_READY,
    PROFILE_ADDED,
    SERVICE_ADDED,
    ADV_OBJ_CREATED,
    SCAN_OBJ_CREATED,
    INIT_OBJ_CREATED,
    ADV_STOPPED,
    SCAN_STOPPED,
    INIT_STOPPED,
    ADV_REPORT,
};

enum prf_id
{
    PRF_DIS_SERVER,
};

struct profile_added_evt
{
    uint16_t start_hdl;
    enum prf_id id;
    
};

struct service_added_evt
{
    uint16_t start_hdl;
    uint8_t status;
};

struct obj_created_evt
{
    uint8_t handle;
    uint8_t status;
};

struct stopped_evt
{
    uint8_t handle;
    uint8_t reason;
};

struct adv_report_info
{
    uint8_t evt_type:3,
            complete:1,
            connectable:1,
            scannable:1,
            directed:1;    
};

struct adv_report_evt
{
    uint8_t *data;
    struct dev_addr *adv_addr;
    uint16_t length;
    uint8_t adv_addr_type;
    int8_t rssi;
    struct adv_report_info info;
};

union dev_evt_u
{
    struct profile_added_evt profile_added;
    struct service_added_evt service_added;
    struct obj_created_evt obj_created;
    struct stopped_evt stopped;
    struct adv_report_evt adv_report;
};

struct ble_stack_cfg
{
    bool private_addr;
    bool controller_privacy;
};

enum uuid_length
{
    UUID_LEN_16BIT = 0,
    UUID_LEN_32BIT,
    UUID_LEN_128BIT,
};

struct char_properties
{
    uint8_t broadcast:1,
            rd_en:1,
            wr_cmd:1,
            wr_req:1,
            ntf_en:1,
            ind_en:1,            
            wr_signed:1,
            ext_prop:1;
};

struct char_permissions
{
    uint8_t rd_perm:2,
            wr_perm:2,
            ind_perm:2,
            ntf_perm:2;
};

struct att_decl
{
    uint8_t *uuid;
    struct char_permissions char_perm;
    struct char_properties char_prop; 
    enum uuid_length uuid_len;
};

struct svc_decl
{
    uint8_t *uuid;
    struct att_decl *att;
    uint8_t nb_att;
    uint8_t    sec_lvl:2,
            uuid_len:2,
            secondary:1;
};

enum gap_evt_type
{
    CONNECTED,
    DISCONNECTED,
    CONN_PARAM_UPDATED,
    MASTER_PAIR_REQ,
    SLAVE_SECURITY_REQ,
    PAIR_DONE,
    ENCRYPT_DONE,
    GET_LEGACY_OOB,
    DISPLAY_PASSKEY,
    REQUEST_PASSKEY,
};

struct pair_feature
{
    uint8_t iocap;
    uint8_t oob;
    uint8_t auth;
    uint8_t key_size;
    uint8_t ikey_dist;
    uint8_t rkey_dist;
};

struct gap_connected
{
    uint8_t peer_id;
};

struct gap_disconnected
{
    uint8_t reason;
};

struct gap_master_pair_req
{
    uint8_t auth;
};

struct gap_slave_security_req
{
    uint8_t auth;
};

struct gap_pair_done
{
    bool succeed;
    union{
        uint8_t auth;
        uint8_t fail_reason;
    }u;
};

struct gap_encrypt_done
{
    uint8_t auth;
};

struct gap_passkey
{
    char pin[6];
    char str_pad;
};

struct gap_display_passkey
{
    struct gap_passkey passkey;
};

union gap_evt_u
{
    struct gap_connected connected;
    struct gap_disconnected disconnected;
    struct gap_master_pair_req master_pair_req;
    struct gap_slave_security_req slave_security_req;
    struct gap_pair_done pair_done;
    struct gap_encrypt_done encrypt_done;
    struct gap_display_passkey display_passkey;
};

enum gatt_evt_type
{
    GATT_0,

};


union gatt_evt_u
{
    uint8_t evt0;
};

struct gatt_svc_env
{
    void *hdr;
    void (*read)(uint8_t,uint8_t);
    uint8_t (*write)(uint8_t,uint8_t,uint16_t,uint16_t,uint8_t const*);
    uint16_t start_hdl;
    uint8_t att_num;
};

#define DEF_GATT_SVC_CALLBACK(svc,read_cb,write_cb) \
    static struct gatt_svc_env svc##_cb_env = { \
        .read = (read_cb), \
        .write = (write_cb), \
    };

void ble_init(void);

void ble_loop(void);

void dev_manager_init(void (*cb)(enum dev_evt_type,union dev_evt_u *));

void dev_manager_stack_init(struct ble_stack_cfg *cfg);

void dev_manager_add_service(struct svc_decl *svc);

void dev_manager_create_legacy_adv_object(struct legacy_adv_obj_param *p_param);

void dev_manager_create_scan_object(enum gap_own_addr_type own_addr_type);

void dev_manager_start_adv(uint8_t adv_handle,uint8_t *adv_data,uint8_t adv_data_length,uint8_t *scan_rsp_data,uint8_t scan_rsp_data_length);

void dev_manager_update_adv_data(uint8_t adv_handle,uint8_t *adv_data,uint8_t adv_data_length,uint8_t *scan_rsp_data,uint8_t scan_rsp_data_length);

void dev_manager_stop_adv(uint8_t adv_handle);

void dev_manager_start_scan(uint8_t scan_handle, struct start_scan_param * param);

void dev_manager_stop_scan(uint8_t scan_handle);

void dev_manager_start_init(uint8_t init_handle, struct start_init_param * param);

void dev_manager_stop_init(uint8_t init_handle);

void gap_manager_init(void (*evt_cb)(enum gap_evt_type,union gap_evt_u *,uint8_t));

void gap_manager_disconnect(uint8_t con_idx,uint8_t reason);

void gap_manager_master_bond(uint8_t con_idx, struct pair_feature * pair_feat);

void gap_manager_master_encrypt(uint8_t con_idx);

void gap_manager_slave_security_req(uint8_t con_idx, uint8_t auth);

void gap_manager_slave_pair_response_send(uint8_t con_idx,uint8_t accept,struct pair_feature *feat);

void gap_manager_passkey_input(uint8_t con_idx,struct gap_passkey *passkey);

void gap_manager_tk_set(uint8_t con_idx,uint8_t key[BLE_KEY_LEN]);

uint8_t gap_manager_get_role(uint8_t con_idx);

uint8_t gap_manager_get_sec_lvl(uint8_t con_idx);

void gatt_manager_init(void (*evt_cb)(enum gatt_evt_type,union gatt_evt_u *,uint8_t));

void gatt_manager_svc_callback_register(uint16_t start_hdl,uint8_t att_num,struct gatt_svc_env *svc_cb);

#endif

