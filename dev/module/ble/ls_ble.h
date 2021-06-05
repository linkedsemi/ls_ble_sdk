#ifndef LS_BLE_H_
#define LS_BLE_H_
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/** \addtogroup BLE
 *  @{
 */

#define INVALID_CON_IDX (0xff)
#define INVALID_PEER_ID (0xff)

#define BLE_ADDR_LEN 6
#define BLE_KEY_LEN 16                  /*!< The length of the paired temporary key*/

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

enum gap_adv_type
{
    /// Flag
    GAP_ADV_TYPE_FLAGS                      = 0x01,//!< GAP_ADV_TYPE_FLAGS
    /// Use of more than 16 bits UUID
    GAP_ADV_TYPE_MORE_16_BIT_UUID           = 0x02,//!< GAP_ADV_TYPE_MORE_16_BIT_UUID
    /// Complete list of 16 bit UUID
    GAP_ADV_TYPE_COMPLETE_LIST_16_BIT_UUID  = 0x03,//!< GAP_ADV_TYPE_COMPLETE_LIST_16_BIT_UUID
    /// Use of more than 32 bit UUD
    GAP_ADV_TYPE_MORE_32_BIT_UUID           = 0x04,//!< GAP_ADV_TYPE_MORE_32_BIT_UUID
    /// Complete list of 32 bit UUID
    GAP_ADV_TYPE_COMPLETE_LIST_32_BIT_UUID  = 0x05,//!< GAP_ADV_TYPE_COMPLETE_LIST_32_BIT_UUID
    /// Use of more than 128 bit UUID
    GAP_ADV_TYPE_MORE_128_BIT_UUID          = 0x06,//!< GAP_ADV_TYPE_MORE_128_BIT_UUID
    /// Complete list of 128 bit UUID
    GAP_ADV_TYPE_COMPLETE_LIST_128_BIT_UUID = 0x07,//!< GAP_ADV_TYPE_COMPLETE_LIST_128_BIT_UUID
    /// Shortened device name
    GAP_ADV_TYPE_SHORTENED_NAME             = 0x08,//!< GAP_ADV_TYPE_SHORTENED_NAME
    /// Complete device name
    GAP_ADV_TYPE_COMPLETE_NAME              = 0x09,//!< GAP_ADV_TYPE_COMPLETE_NAME
    /// Transmit power
    GAP_ADV_TYPE_TRANSMIT_POWER             = 0x0A,//!< GAP_ADV_TYPE_TRANSMIT_POWER
    /// Class of device
    GAP_ADV_TYPE_CLASS_OF_DEVICE            = 0x0D,//!< GAP_ADV_TYPE_CLASS_OF_DEVICE
    /// Simple Pairing Hash C
    GAP_ADV_TYPE_SP_HASH_C                  = 0x0E,//!< GAP_ADV_TYPE_SP_HASH_C
    /// Simple Pairing Randomizer
    GAP_ADV_TYPE_SP_RANDOMIZER_R            = 0x0F,//!< GAP_ADV_TYPE_SP_RANDOMIZER_R
    /// Temporary key value
    GAP_ADV_TYPE_TK_VALUE                   = 0x10,//!< GAP_ADV_TYPE_TK_VALUE
    /// Out of Band Flag
    GAP_ADV_TYPE_OOB_FLAGS                  = 0x11,//!< GAP_ADV_TYPE_OOB_FLAGS
    /// Slave connection interval range
    GAP_ADV_TYPE_SLAVE_CONN_INT_RANGE       = 0x12,//!< GAP_ADV_TYPE_SLAVE_CONN_INT_RANGE
    /// Require 16 bit service UUID
    GAP_ADV_TYPE_RQRD_16_BIT_SVC_UUID       = 0x14,//!< GAP_ADV_TYPE_RQRD_16_BIT_SVC_UUID
    /// Require 32 bit service UUID
    GAP_ADV_TYPE_RQRD_32_BIT_SVC_UUID       = 0x1F,//!< GAP_ADV_TYPE_RQRD_32_BIT_SVC_UUID
    /// Require 128 bit service UUID
    GAP_ADV_TYPE_RQRD_128_BIT_SVC_UUID      = 0x15,//!< GAP_ADV_TYPE_RQRD_128_BIT_SVC_UUID
    /// Service data 16-bit UUID
    GAP_ADV_TYPE_SERVICE_16_BIT_DATA        = 0x16,//!< GAP_ADV_TYPE_SERVICE_16_BIT_DATA
    /// Service data 32-bit UUID
    GAP_ADV_TYPE_SERVICE_32_BIT_DATA        = 0x20,//!< GAP_ADV_TYPE_SERVICE_32_BIT_DATA
    /// Service data 128-bit UUID
    GAP_ADV_TYPE_SERVICE_128_BIT_DATA       = 0x21,//!< GAP_ADV_TYPE_SERVICE_128_BIT_DATA
    /// Public Target Address
    GAP_ADV_TYPE_PUB_TGT_ADDR               = 0x17,//!< GAP_ADV_TYPE_PUB_TGT_ADDR
    /// Random Target Address
    GAP_ADV_TYPE_RAND_TGT_ADDR              = 0x18,//!< GAP_ADV_TYPE_RAND_TGT_ADDR
    /// Appearance
    GAP_ADV_TYPE_APPEARANCE                 = 0x19,//!< GAP_ADV_TYPE_APPEARANCE
    /// Advertising Interval
    GAP_ADV_TYPE_ADV_INTV                   = 0x1A,//!< GAP_ADV_TYPE_ADV_INTV
    /// LE Bluetooth Device Address
    GAP_ADV_TYPE_LE_BT_ADDR                 = 0x1B,//!< GAP_ADV_TYPE_LE_BT_ADDR
    /// LE Role
    GAP_ADV_TYPE_LE_ROLE                    = 0x1C,//!< GAP_ADV_TYPE_LE_ROLE
    /// Simple Pairing Hash C-256
    GAP_ADV_TYPE_SPAIR_HASH                 = 0x1D,//!< GAP_ADV_TYPE_SPAIR_HASH
    /// Simple Pairing Randomizer R-256
    GAP_ADV_TYPE_SPAIR_RAND                 = 0x1E,//!< GAP_ADV_TYPE_SPAIR_RAND
    /// 3D Information Data
    GAP_ADV_TYPE_3D_INFO                    = 0x3D,//!< GAP_ADV_TYPE_3D_INFO

    /// Manufacturer specific data
    GAP_ADV_TYPE_MANU_SPECIFIC_DATA         = 0xFF,//!< GAP_ADV_TYPE_MANU_SPECIFIC_DATA
};

struct dev_addr
{
    uint8_t addr[BLE_ADDR_LEN];
};

struct ble_addr
{
    struct dev_addr addr;
    uint8_t type;
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

struct ext_adv_obj_param
{
    struct legacy_adv_obj_param legacy_adv_obj;
    uint8_t max_skip;
    uint8_t phy;
    uint8_t adv_sid;
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
    OBJ_DELETED,
    ADV_REPORT,
};

enum prf_id
{
    PRF_DIS_SERVER,
    PRF_MESH,
    PRF_LS_MESH,
    PRF_FOTA_SERVER,
    PRF_HID,
    PRF_BASS,
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

struct obj_deleted_evt
{
    uint8_t handle;
    uint8_t status;
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
    struct obj_deleted_evt deleted;
    struct adv_report_evt adv_report;
};

struct ble_stack_cfg
{
    bool private_addr;
    bool controller_privacy;
};
/**
  * @brief Length of UUID
  */
enum uuid_length
{
    UUID_LEN_16BIT = 0,                  /*!< 16bits UUID*/
    UUID_LEN_32BIT,                      /*!< 32bits UUID*/
    UUID_LEN_128BIT,                     /*!< 128bits UUID*/
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
    const uint8_t *uuid;
    struct{
        uint16_t max_len:12,
                eks:1,
                uuid_len:2,
                read_indication:1;        
    }s;
    struct char_permissions char_perm;
    struct char_properties char_prop; 
};

struct svc_decl
{
    const uint8_t *uuid;
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
    CONN_PARAM_REQ,
    CONN_PARAM_UPDATED,
    MASTER_PAIR_REQ,
    SLAVE_SECURITY_REQ,
    PAIR_DONE,
    ENCRYPT_FAIL,
    ENCRYPT_DONE,
    DISPLAY_PASSKEY,
    REQUEST_PASSKEY,
    NUMERIC_COMPARE,
    REQUEST_LEGACY_OOB,
    REQUEST_SC_OOB,
    GET_DEV_INFO_DEV_NAME,
    GET_DEV_INFO_APPEARANCE,
    GET_DEV_INFO_SLV_PRE_PARAM,
    GET_DEV_INFO_PEER_RSSI,
};
enum LS_BLE_ROLE
{
    LS_BLE_ROLE_MASTER,
    LS_BLE_ROLE_SLAVE,
};
struct gap_conn_param
{
    /// Connection interval minimum
    uint16_t intv_min;
    /// Connection interval maximum
    uint16_t intv_max;
    /// Latency
    uint16_t latency;
    /// Supervision timeout
    uint16_t time_out;
};

struct gap_conn_param_req
{
    struct gap_conn_param const *conn_param;
    bool *accept;
};

struct gap_conn_param_updated
{
    ///Connection interval value
    uint16_t            con_interval;
    ///Connection latency value
    uint16_t            con_latency;
    ///Supervision timeout
    uint16_t            sup_to;
};
/**
  * @brief SEC OOB value.
  */
struct gap_sc_oob
{
    uint8_t conf[BLE_KEY_LEN];              /*!< Confirm Value*/
    uint8_t rand[BLE_KEY_LEN];              /*!< Random Number*/
};
/**
  * @brief Slave Authentication mask.
  */
enum gap_slave_auth_mask
{
    GAP_SLAVE_AUTH_NONE           = 0,                   /*!< No Flag set*/
    GAP_SLAVE_AUTH_BOND           = (1 << 0),            /*!< Bond authentication*/
    GAP_SLAVE_AUTH_MITM           = (1 << 2),            /*!< Man In the middle protection*/
    GAP_SLAVE_AUTH_SEC_CON        = (1 << 3),            /*!< Secure Connection*/
    GAP_SLAVE_AUTH_KEY_NOTIF      = (1 << 4)             /*!< Key Notification*/
};
/**
  * @brief Slave Authentication Requirements.
  */
enum gap_slave_auth
{
    GAP_SLAVE_AUTH_REQ_NO_MITM_NO_BOND  = (GAP_SLAVE_AUTH_NONE),                                                    /*!< No MITM No Bonding*/
    GAP_SLAVE_AUTH_REQ_NO_MITM_BOND     = (GAP_SLAVE_AUTH_BOND),                                                    /*!< No MITM Bonding*/
    GAP_SLAVE_AUTH_REQ_MITM_NO_BOND     = (GAP_SLAVE_AUTH_MITM),                                                    /*!< MITM No Bonding*/
    GAP_SLAVE_AUTH_REQ_MITM_BOND        = (GAP_SLAVE_AUTH_MITM | GAP_SLAVE_AUTH_BOND),                              /*!< MITM and Bonding*/
    GAP_SLAVE_AUTH_REQ_SEC_CON_NO_BOND  = (GAP_SLAVE_AUTH_SEC_CON | GAP_SLAVE_AUTH_MITM),                           /*!< SEC_CON and No Bonding*/
    GAP_SLAVE_AUTH_REQ_SEC_CON_BOND     = (GAP_SLAVE_AUTH_SEC_CON | GAP_SLAVE_AUTH_MITM | GAP_SLAVE_AUTH_BOND),     /*!< SEC_CON and Bonding*/
};
/**
  * @brief Defgroup BLE_GAP_IO_CAPS GAP IO Capabilities.
  */
enum gap_io_caps
{
    BLE_GAP_IO_CAPS_DISPLAY_ONLY,           /*!< Display Only*/
    BLE_GAP_IO_CAPS_DISPLAY_YESNO,          /*!< Display and Yes/No entry*/
    BLE_GAP_IO_CAPS_KEYBOARD_ONLY,          /*!< Keyboard Only*/
    BLE_GAP_IO_CAPS_NONE,                   /*!< No I/O capabilities*/
    BLE_GAP_IO_CAPS_KEYBOARD_DISPLAY,       /*!< Keyboard and Display*/
};
/**
  * @brief Defgroup SEC_OOB_FLAG  OOB Data Flag.
  */
enum gap_pair_oob
{
    BLE_GAP_OOB_DISABLE,                    /*!< OOB Authentication data not present*/
    BLE_GAP_OOB_ENABLE,                     /*!< OOB Authentication data from remote device present*/
};
/**
  * @brief Defgroup SEC_AUTH_FLAG  SEC Auth Flag.
  */
enum gap_pair_auth
{
    AUTH_NONE              =  0,           /*!< No auth requirement*/
    AUTH_BOND              = (1 << 0),     /*!< Bond flag*/
    AUTH_MITM              = (1 << 2),     /*!< MITM flag*/
    AUTH_SEC_CON           = (1 << 3),     /*!< Security connection flag*/
    AUTH_KEY_PRESS_NOTIFY  = (1 << 4),     /*!< Key press notify flag*/
};
/**
  * @brief Defgroup SEC_KEY_DIST_FLAG  SEC Key Distribution Flag.
  */
enum gap_key_dist
{
    KDIST_NONE             =  0,           /*!< No auth requirement*/
    KDIST_ENCKEY           = (1 << 0),     /*!< Bond flag*/
    KDIST_IDKEY            = (1 << 2),     /*!< MITM flag*/
    KDIST_SIGNKEY          = (1 << 3),     /*!< Security connection flag*/
};
/**
  * @brief Set security parameter.
  */
struct pair_feature
{
    uint8_t iocap;                          /*!< Set the IO capability, This parameter can be a value of @ref gap_io_caps*/
    uint8_t oob;                            /*!< Indicate whether OOB is supported, This parameter can be a value of @ref gap_pair_oob*/
    uint8_t auth;                           /*!< Set the auth, This parameter can be a value of @ref gap_pair_auth*/
    uint8_t key_size;                       /*!< Indicate the supported maximum LTK size (range: 7-16), This parameter can be a value of @ref gap_io_caps*/
    uint8_t ikey_dist;                      /*!< Set the initial key distribution, This parameter can be a value of @ref gap_key_dist*/
    uint8_t rkey_dist;                      /*!< Set the response key distribution, This parameter can be a value of @ref gap_key_dist*/
};

struct gap_connected
{
    uint16_t con_interval;
    uint16_t con_latency;
    uint16_t sup_to;
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

struct gap_encrypt_fail
{
    uint8_t reason;
};

struct gap_encrypt_done
{
    uint8_t auth;
};

struct gap_pin_str
{
    char pin[6];
    char str_pad;
};

struct gap_display_passkey
{
    struct gap_pin_str passkey;
};

struct gap_numeric_compare
{
    struct gap_pin_str number;
};

struct gap_dev_info_dev_name
{
    uint16_t length;
    uint8_t* value;
};

struct gap_dev_info_appearance
{
    uint16_t appearance;
};

struct gap_dev_info_slave_pref_param
{
    uint16_t con_intv_min;
    uint16_t con_intv_max;
    uint16_t slave_latency;
    uint16_t conn_timeout;
};

struct gap_dev_info_peer_rssi
{
    int8_t rssi;
};

union gap_evt_u
{
    struct gap_connected connected;
    struct gap_disconnected disconnected;
    struct gap_conn_param_req conn_param_req;
    struct gap_conn_param_updated conn_param_updated;
    struct gap_master_pair_req master_pair_req;
    struct gap_slave_security_req slave_security_req;
    struct gap_pair_done pair_done;
    struct gap_encrypt_fail encrypt_fail;
    struct gap_encrypt_done encrypt_done;
    struct gap_display_passkey display_passkey;
    struct gap_numeric_compare numeric_compare;
    struct gap_dev_info_dev_name get_dev_name;
    struct gap_dev_info_appearance get_appearance;
    struct gap_dev_info_slave_pref_param slv_pref_param;
    struct gap_dev_info_peer_rssi peer_rssi;
};

struct gap_update_conn_param
{
    uint16_t intv_min;
    uint16_t intv_max;
    uint16_t latency;
    uint16_t sup_timeout;
    uint16_t ce_len_min;
    uint16_t ce_len_max;
};

struct gap_set_pkt_size
{
    uint16_t pkt_size;
};

/**
  * @brief GATT event types.
  * @note  Event types include request/done/indication or other messages sent to applications 
  * by stack.
  */
enum gatt_evt_type
{
    SERVER_READ_REQ = 0,               /*!< Read request for server*/
    SERVER_WRITE_REQ,                  /*!< write request for server*/
    SERVER_NOTIFICATION_DONE,          /*!< Send notification done for server*/
    SERVER_INDICATION_DONE,            /*!< Send indication done for server*/

    CLIENT_RECV_NOTIFICATION,          /*!< Receive notification for client*/
    CLIENT_RECV_INDICATION,            /*!< Receive indication for client*/
    CLIENT_PRIMARY_SVC_DIS_IND,        /*!< Primary service discovery indication for client*/
    CLIENT_INCL_SVC_DIS_IND,           /*!< Included service discovery indication for client*/
    CLIENT_CHAR_DIS_BY_UUID_IND,       /*!< Characteristic discovery by UUID indication for client*/ 
    CLIENT_CHAR_DESC_DIS_BY_UUID_IND,  /*!< Characteristic descriptor discovery by UUID indication for client*/
    CLIENT_RD_CHAR_VAL_BY_UUID_IND,    /*!< Read characteristic value by UUID indication for client*/
    CLIENT_WRITE_WITH_RSP_DONE,        /*!<  Write response indication for client*/
    CLIENT_WRITE_NO_RSP_DONE,          /*!<  Write with no response indication for client*/

    MTU_CHANGED_INDICATION,            /*!<  MTU exchange indication for client & server*/
    GATT_EVT_MAX
};
/**
  * @brief Service set values status.
  */
enum svc_set_value_status
{
    SVC_SET_VAL_NO_ERROR,              /*!< No error*/
    SVC_SET_VAL_NOT_SUPPORTED,         /*!< Action not supported*/
    SVC_SET_VAL_INVALID_HANDLE,        /*!< Invalid handle*/
    SVC_SET_VAL_INVALID_OFFSET,        /*!< Invalid offset*/
    SVC_SET_VAL_INVALID_LENGTH,        /*!< Invalid length*/
};
/**
  * @brief Service get values status.
  */
enum svc_get_value_status   
{
    SVC_GET_VAL_NO_ERROR,              /*!< No error*/
    SVC_GET_VAL_NOT_SUPPORTED,         /*!< Action not supported*/
    SVC_GET_VAL_INVALID_HANDLE,        /*!< Invalid handle*/
    SVC_GET_VAL_APP_ERROR,             /*!< Application or profile error*/
};
/**
  * @brief GATT service environment.
  */
struct gatt_svc_env
{
    void *hdr;                         /*!< Pointer to next gatt_svc_env*/
    uint16_t start_hdl;                /*!< Start handle of the service*/
    uint8_t att_num;                   /*!< Attributes number in the service*/
};
/**
  * @brief GATT read request.
  */
struct gatt_server_read_req
{
    struct gatt_svc_env const *svc;    /*!< Pointer to serice containing the attribute to read*/
    uint8_t att_idx;                   /*!< Attribute index*/
};

/**
  * @brief GATT write request.
  */
struct gatt_server_write_req
{
    struct gatt_svc_env const *svc;    /*!< Pointer to serice containing the attribute to read*/
    uint8_t const *value;              /*!< Pointer to value to write*/
    uint8_t *return_status;            /*!< Return status*/
    uint16_t offset;                   /*!< Offset at which the data has to be written*/
    uint16_t length;                   /*!< Length of data to write*/
    uint8_t att_idx;                   /*!< Attribute index*/
};
/**
  * @brief Send notify/indicate done on GATT server.
  */
struct gatt_server_notify_indicate_done
{
    uint16_t transaction_id;           /*!< Index of transaction*/
    uint8_t status;                    /*!< Status of notifiy/indicate done*/
};
/**
  * @brief Received notify/indicate on GATT client.
  */
struct gatt_client_recv_notify_indicate
{
    uint16_t handle;                   /*!< Handle of notification/indication*/
    uint16_t length;                   /*!< Length of notification/indication*/
    uint8_t const *value;              /*!< Pointer to value in notification/indication*/
};
/**
  * @brief MTU exchange indicate.
  */
struct gatt_mtu_changed_indicate
{
    uint16_t mtu;                      /*!< MTU received*/
};
/**
  * @brief Range of GATT handles.
  */
struct gatt_handle_range
{
    uint16_t begin_handle;             /*!< Start handle*/
    uint16_t end_handle;               /*!< End handle*/
};
/**
  * @brief Service discovery indicate for GATT client.
  */
struct gatt_client_svc_disc_indicate
{
    const uint8_t *uuid;                         /*!< UUID of the service*/
    struct gatt_handle_range handle_range;       /*!< Handle range of the service*/
    enum uuid_length uuid_len;                   /*!< Length of the service UUID*/
};
/**
  * @brief Included service discovery indicate for GATT client.
  */
struct gatt_client_svc_disc_include_indicate
{
    const uint8_t *uuid;                          /*!< UUID of the service*/
    struct gatt_handle_range handle_range;        /*!< Handle range of the service*/
    uint16_t attr_handle;                         /*!< Attribute handle of included service*/
    enum uuid_length uuid_len;                    /*!< Length of the service UUID*/
};
/**
  * @brief Characteristic discovery indicate for GATT client.
  */
struct gatt_client_disc_char_indicate
{
    const uint8_t *uuid;                          /*!< UUID of the characteristic*/
    uint16_t attr_handle;                         /*!< Attribute handle of characteristic*/
    uint16_t pointer_handle;                      /*!< Pointer attribute handle to UUID*/
    uint8_t properties;                           /*!< Properities of the characteristic*/
    enum uuid_length uuid_len;                    /*!< Length of the service UUID*/
};
/**
  * @brief Characteristic descriptro discovery indicate for GATT client.
  */
struct gatt_client_disc_char_desc_indicate
{
    const uint8_t *uuid;                          /*!< UUID of the descriptor*/
    uint16_t attr_handle;                         /*!< Attribute handle of descriptor*/
    enum uuid_length uuid_len;                    /*!< Length of the service UUID*/
};
/**
  * @brief Read indication.
  */
struct gatt_read_indicate
{
    uint8_t const *value;                         /*!< Pointer to the value read*/
    uint16_t handle;                              /*!< Attribute handle*/
    uint16_t offset;                              /*!< Offset at which the data has to be written*/
    uint16_t length;                              /*!< Length of value*/
};
/**
  * @brief Response for write request.
  */
struct gatt_write_rsp
{
    uint16_t transaction_id;                      /*!< Index of transaction*/
    uint8_t status;                               /*!< Status of write*/
};
/**
  * @brief Response for write command.
  */
struct gatt_write_no_rsp
{
    uint16_t transaction_id;                      /*!< Index of transaction*/
    uint8_t status;                               /*!< Status of write*/
};
/**
  * @brief Union definition for GATT events.
  */
union gatt_evt_u
{
    struct gatt_server_read_req server_read_req;                                         /*!< GATT server read request.*/
    struct gatt_server_write_req server_write_req;                                       /*!< GATT server write request.*/
    struct gatt_server_notify_indicate_done server_notify_indicate_done;                 /*!< GATT server send notify/indicate done.*/           
    struct gatt_client_recv_notify_indicate client_recv_notify_indicate;                 /*!< GATT client receive notify/indicate.*/
    struct gatt_mtu_changed_indicate mtu_changed_ind;                                    /*!< MTU exchange indication.*/
    struct gatt_client_svc_disc_indicate client_svc_disc_indicate;                       /*!< GATT client service discovery indicate.*/
    struct gatt_client_svc_disc_include_indicate client_svc_disc_include_indicate;       /*!< GATT client included service discovery indicate.*/
    struct gatt_client_disc_char_indicate client_disc_char_indicate;                     /*!< GATT client characteristic discovery indicate.*/
    struct gatt_client_disc_char_desc_indicate client_disc_char_desc_indicate;           /*!< GATT client characteristic descriptor discovery indicate.*/
    struct gatt_read_indicate client_read_indicate;                                      /*!< GATT client read indicate.*/
    struct gatt_write_rsp client_write_rsp;                                              /*!< GATT client write request response.*/
    struct gatt_write_no_rsp client_write_no_rsp;                                        /*!< GATT client write command response.*/
};

uint8_t *adv_data_pack(uint8_t *buf,uint8_t field_nums,...);
#define ADV_DATA_PACK(buf,field_nums,...) (adv_data_pack((buf),(field_nums),__VA_ARGS__) - (buf))

void ble_init(void);

void ble_loop(void);

void dev_manager_init(void (*cb)(enum dev_evt_type,union dev_evt_u *));

void dev_manager_stack_init(struct ble_stack_cfg *cfg);

void dev_manager_get_identity_bdaddr(uint8_t *addr,bool *random);

void dev_manager_add_service(struct svc_decl *svc);

uint8_t dev_manager_svc_set_value(uint16_t handle, uint16_t length, uint8_t *value);

uint8_t dev_manager_svc_get_value(uint16_t handle, uint8_t *value, uint16_t *length);

void dev_manager_create_legacy_adv_object(struct legacy_adv_obj_param *p_param);

void dev_manager_create_ext_adv_object(struct ext_adv_obj_param *p_param);

void dev_manager_create_scan_object(enum gap_own_addr_type own_addr_type);

void dev_manager_create_init_object(enum gap_own_addr_type own_addr_type);

void dev_manager_start_adv(uint8_t adv_handle,uint8_t *adv_data,uint8_t adv_data_length,uint8_t *scan_rsp_data,uint8_t scan_rsp_data_length);

void dev_manager_set_adv_duration(uint16_t duration);

void dev_manager_update_adv_data(uint8_t adv_handle,uint8_t *adv_data,uint8_t adv_data_length,uint8_t *scan_rsp_data,uint8_t scan_rsp_data_length);

void dev_manager_stop_adv(uint8_t adv_handle);

void dev_manager_start_scan(uint8_t scan_handle, struct start_scan_param * param);

void dev_manager_stop_scan(uint8_t scan_handle);

void dev_manager_start_init(uint8_t init_handle, struct start_init_param * param);

void dev_manager_stop_init(uint8_t init_handle);

void dev_manager_delete_activity(uint8_t obj_hdl);

void dev_manager_set_mac_addr(uint8_t *addr);

uint8_t dev_manager_update_adv_interval(uint8_t adv_handle, uint32_t new_intv_min, uint32_t new_intv_max);

void gap_manager_init(void (*evt_cb)(enum gap_evt_type,union gap_evt_u *,uint8_t));

void gap_manager_disconnect(uint8_t con_idx,uint8_t reason);
/**
 ****************************************************************************************
 * \brief The master starts the bonding process
 * 
 * \param[in]  con_idx           Connection ID number.
 * \param[in]  pair_feat         Pairing parameter setting, This parameter can be a value of @ref pair_feature.
 ****************************************************************************************
 */
void gap_manager_master_bond(uint8_t con_idx, struct pair_feature * pair_feat);
/**
 ****************************************************************************************
 * \brief The master starts the secure connection process
 * 
 * \param[in]  con_idx           Connection ID number.
 ****************************************************************************************
 */
void gap_manager_master_encrypt(uint8_t con_idx);
/**
 ****************************************************************************************
 * \brief Initiate an encryption request from the slave. 
 * 
 * \param[in]  con_idx           Connection ID number.
 * \param[in]  auth              SEC Auth param, This parameter can be a value of @ref gap_slave_auth
 ****************************************************************************************
 */
void gap_manager_slave_security_req(uint8_t con_idx, uint8_t auth);
/**
 ****************************************************************************************
 * \brief The slave exchange pairs information.
 * 
 * \param[in]  con_idx           Connection ID number.
 * \param[in]  accept            Whether to save master pairing information,value(0 or 1).
 * \param[in]  feat              Pairing parameter setting, This parameter can be a value of @ref pair_feature.
 ****************************************************************************************
 */
void gap_manager_slave_pair_response_send(uint8_t con_idx,uint8_t accept,struct pair_feature *feat);
/**
 ****************************************************************************************
 * \brief Master and slave pair key input. 
 * 
 * \param[in]  con_idx           Connection ID number.
 * \param[in]  passkey           Connect the key, This parameter can be a value of @ref gap_pin_str.
 ****************************************************************************************
 */
void gap_manager_passkey_input(uint8_t con_idx,struct gap_pin_str *passkey);
/**
 ****************************************************************************************
 * \brief Verify that the key is the same in numerical comparison mode(Only for LE Secure Connections). 
 * 
 * \param[in]  con_idx           Connection ID number.
 * \param[in]  equal             Numeric comparison results.
 ****************************************************************************************
 */
void gap_manager_numeric_compare_set(uint8_t con_idx,bool equal);
/**
 ****************************************************************************************
 * \brief Set the security oob of the specified connection. 
 * 
 * \param[in]  con_idx           Connection ID number.
 * \param[in]  sc_oob            SEC OOB value, This parameter can be a value of @ref gap_sc_oob.
 ****************************************************************************************
 */
void gap_manager_sc_oob_set(uint8_t con_idx,struct gap_sc_oob *sc_oob);
/**
 ****************************************************************************************
 * \brief Set the security oob of the specified connection. 
 * 
 * \param[in]  con_idx           Connection ID number.
 * \param[in]  key               Pairing Temporary Key value.
 * \param[in]  BLE_KEY_LEN       The length of the paired temporary key, This parameter can be a value of @ref BLE_KEY_LEN.
 ****************************************************************************************
 */
void gap_manager_tk_set(uint8_t con_idx,uint8_t key[BLE_KEY_LEN]);

uint8_t gap_manager_get_role(uint8_t con_idx);
/**
 ****************************************************************************************
 * \brief Gets the security level of the specified connection. 
 * 
 * \param[in]  con_idx           Connection ID number.
 * \return The security level of the specified connection.
 ****************************************************************************************
 */
uint8_t gap_manager_get_sec_lvl(uint8_t con_idx);

void gap_manager_get_peer_addr(uint8_t conidx,struct ble_addr *addr);

void gap_manager_get_identity_addr(uint8_t peer_id,struct ble_addr *addr);

void gap_manager_update_conn_param(uint8_t con_idx,struct gap_update_conn_param *p_param);

void gap_manager_set_pkt_size(uint8_t con_idx, struct gap_set_pkt_size *p_param);
/**
 ****************************************************************************************
 * \brief Deletes the bound device information. 
 * 
 * \param[in]  peer_id           Pairing ID number.
 ****************************************************************************************
 */
void gap_manager_delete_bonding(uint8_t peer_id);
/**
 ****************************************************************************************
 * \brief Gets the pairing ID of the bound device. 
 * 
 * \param[in]  link_id           Connection ID number.
 * \return The pairing ID of the bound device.
 ****************************************************************************************
 */
uint8_t gap_manager_get_bonding_peer_id(uint8_t link_id);
/**
 ****************************************************************************************
 * \brief Gets the number of bound devices. 
 * 
 * \return The number of bound devices.
 ****************************************************************************************
 */
uint8_t gap_manager_get_bonded_dev_num(void);
/**
 ****************************************************************************************
 * \brief Gets the RSSI value of the specified connected device. 
 * 
 * \param[in]  link_id           Connection ID number.
 ****************************************************************************************
 */
void gap_manager_get_peer_rssi(uint8_t link_id);
/**
 ****************************************************************************************
 * \brief Initialize GATT manager.
 *
 * \param[in]  evt_cb               Callback function for gatt service.
 *
 ****************************************************************************************
 */
void gatt_manager_init(void (*evt_cb)(enum gatt_evt_type,union gatt_evt_u *,uint8_t));
/**
 ****************************************************************************************
 * \brief Register service in GATT manager.
 *
 * \param[in]  start_hdl               Start handle of the service to be registered.
 * \param[in]  att_num                 Number of attributes contained in the service.
 * \param[in]  svc                     Pointer of service to be registered.
 *
 ****************************************************************************************
 */
void gatt_manager_svc_register(uint16_t start_hdl,uint8_t att_num,struct gatt_svc_env *svc);
/**
 ****************************************************************************************
 * \brief Send reply to read request from GATT client.
 *
 * \param[in]  con_idx                 Connection index.
 * \param[in]  handle                  Handle of attribute to read.
 * \param[in]  status                  Status of read command execution in application.
 * \param[in]  data                    Pointer of data reply.
 * \param[in]  length                  Length of data to send in the units of bytes.
 *
 ****************************************************************************************
 */
void gatt_manager_server_read_req_reply(uint8_t con_idx,uint16_t handle,uint8_t status,uint8_t *data,uint16_t length);
/**
 ****************************************************************************************
 * \brief Send indication to client.
 *
 * \param[in]  con_idx                 Connection index.
 * \param[in]  handle                  Handle of attribute to send indication.
 * \param[in]  data                    Pointer of data reply.
 * \param[in]  length                  Length of data to send in the units of bytes.
 * \param[in]  transaction_id          Id of transaction between stack with application.
 *
 ****************************************************************************************
 */
void gatt_manager_server_send_indication(uint8_t con_idx,uint16_t handle,uint8_t *data,uint16_t length,uint16_t *transaction_id);
/**
 ****************************************************************************************
 * \brief Send notification to client.
 *
 * \param[in]  con_idx                 Connection index.
 * \param[in]  handle                  Handle of attribute to send notification.
 * \param[in]  data                    Pointer of data reply.
 * \param[in]  length                  Length of data to send in the units of bytes.
 * \param[in]  transaction_id          Id of transaction between stack with application.
 *
 ****************************************************************************************
 */
void gatt_manager_server_send_notification(uint8_t con_idx,uint16_t handle,uint8_t *data,uint16_t length,uint16_t *transaction_id);
/**
 ****************************************************************************************
 * \brief Send indication confirm to server.
 *
 * \param[in]  con_idx                 Connection index.
 * \param[in]  handle                  Handle of attribute to send confirm.
 *
 ****************************************************************************************
 */
void gatt_manager_client_indication_confirm(uint8_t con_idx,uint16_t handle);
/**
 ****************************************************************************************
 * \brief Get the handle of attribute in the specified service.
 *
 * \param[in]  svc                     Pointer of the service containing the attribute.
 * \param[in]  att_idx                 Attribute index.
 *
 ****************************************************************************************
 */
uint16_t gatt_manager_get_svc_att_handle(struct gatt_svc_env *svc,uint8_t att_idx);
/**
 ****************************************************************************************
 * \brief Send data to GATT server by writing command(write without response).
 *
 * \param[in]  con_idx                 Connection index.
 * \param[in]  handle                  Handle of attribute to write.
 * \param[in]  data                    Pointer of data to send.
 * \param[in]  length                  Length of data to send in the units of bytes.
 *
 ****************************************************************************************
 */
void gatt_manager_client_write_no_rsp(uint8_t con_idx,uint16_t handle,uint8_t *data,uint16_t length);
/**
 ****************************************************************************************
 * \brief Send data to GATT server by writing request(write with response).
 *
 * \param[in]  con_idx                 Connection index.
 * \param[in]  handle                  Handle of attribute to write.
 * \param[in]  data                    Pointer of data to send.
 * \param[in]  length                  Length of data to send in the units of bytes.
 *
 ****************************************************************************************
 */
void gatt_manager_client_write_with_rsp(uint8_t con_idx,uint16_t handle,uint8_t *data,uint16_t length);
/**
 ****************************************************************************************
 * \brief Enable cccd(client characteristic configuration descriptor) on GATT server service.
 *
 * \param[in]  con_idx                 Connection index.
 * \param[in]  handle                  Handle of attribute of cccd.
 * \param[in]  notification_en         1 = enable notification, 0 = disable notification.
 * \param[in]  indication_en           1 = enable indication, 0 = disable indication.
 *
 ****************************************************************************************
 */
void gatt_manager_client_cccd_enable(uint8_t con_idx,uint16_t handle,bool notification_en, bool indication_en);
/**
 ****************************************************************************************
 * \brief Discovery service by specified uuid.
 *
 * \param[in]  con_idx                 Connection index.
 * \param[in]  uuid                    Pointer of uuid to be discovered.
 * \param[in]  uuid_len                Length of uuid in units of ::uuid_length.
 * \param[in]  start_hdl               Start handle of the range to search.
 * \param[in]  end_hdl                 End handle of the range to search.
 *
 ****************************************************************************************
 */
void gatt_manager_client_svc_discover_by_uuid(uint8_t con_idx,uint8_t *uuid,enum uuid_length uuid_len,uint16_t start_hdl,uint16_t end_hdl);
/**
 ****************************************************************************************
 * \brief Discovery characteristic by specified uuid.
 *
 * \param[in]  con_idx                 Connection index.
 * \param[in]  uuid                    Pointer of uuid to be discovered.
 * \param[in]  uuid_len                Length of uuid in units of ::uuid_length.
 * \param[in]  start_hdl               Start handle of the range to search.
 * \param[in]  end_hdl                 End handle of the range to search.
 *
 ****************************************************************************************
 */
void gatt_manager_client_char_discover_by_uuid(uint8_t con_idx,uint8_t *uuid,enum uuid_length uuid_len,uint16_t start_hdl,uint16_t end_hdl);
/**
 ****************************************************************************************
 * \brief Discovery cccd.
 *
 * \param[in]  con_idx                 Connection index.
 * \param[in]  start_hdl               Start handle of the range to search.
 * \param[in]  end_hdl                 End handle of the range to search.
 *
 ****************************************************************************************
 */
void gatt_manager_client_desc_char_discover(uint8_t con_idx,uint16_t start_hdl,uint16_t end_hdl);
/**
 ****************************************************************************************
 * \brief Send MTU exchange to GATT server.
 *
 * \param[in]  con_idx                 Connection index.
 *
 ****************************************************************************************
 */
void gatt_manager_client_mtu_exch_send(uint8_t con_idx);
/**
 ****************************************************************************************
 * \brief Read attribute value by handle.
 *
 * \param[in]  con_idx                 Connection index.
 * \param[in]  handle                  Handle of attribute to read.
 *
 ****************************************************************************************
 */
void gatt_manager_client_read(uint8_t con_idx,uint16_t handle);

/** @}*/

#endif

