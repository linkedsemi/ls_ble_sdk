#define LOG_TAG "MAIN"
#include "ls_ble.h"
#include "platform.h"
#include "prf_hid.h"
#include "log.h"
#include "ls_dbg.h"
#include "cpu.h"
#include "lsuart.h"
#include "builtin_timer.h"
#include <string.h>
#include "co_math.h"
#include "io_config.h"
#include "SEGGER_RTT.h"

#define APP_HID_DEV_NAME ("LS_HID_Demo")
#define APP_HID_DEV_NAME_LEN (sizeof(APP_HID_DEV_NAME))

#define UART_SERVER_MAX_MTU 247
#define UART_SERVER_MTU_DFT 23
#define UART_SERVER_MAX_DATA_LEN (uart_server_mtu - 3)
#define UART_SVC_RX_MAX_LEN (UART_SERVER_MAX_MTU - 3)
#define UART_SVC_TX_MAX_LEN (UART_SERVER_MAX_MTU - 3)
#define UART_SVC_BUFFER_SIZE (1024)

#define UART_SERVER_TIMEOUT 50 // timer units: ms

uint8_t ble_device_name[DEV_NAME_MAX_LEN] = "LS_HID_Demo";
static const uint8_t ls_uart_svc_uuid_128[] = {0x9e, 0xca, 0xdc, 0x24, 0x0e, 0xe5, 0xa9, 0xe0, 0x93, 0xf3, 0xa3, 0xb5, 0x01, 0x00, 0x40, 0x6e};
static const uint8_t ls_uart_rx_char_uuid_128[] = {0x9e, 0xca, 0xdc, 0x24, 0x0e, 0xe5, 0xa9, 0xe0, 0x93, 0xf3, 0xa3, 0xb5, 0x02, 0x00, 0x40, 0x6e};
static const uint8_t ls_uart_tx_char_uuid_128[] = {0x9e, 0xca, 0xdc, 0x24, 0x0e, 0xe5, 0xa9, 0xe0, 0x93, 0xf3, 0xa3, 0xb5, 0x03, 0x00, 0x40, 0x6e};
static const uint8_t att_decl_char_array[] = {0x03, 0x28};
static const uint8_t att_desc_client_char_cfg_array[] = {0x02, 0x29};
enum uart_svc_att_db_handles
{
    UART_SVC_IDX_RX_CHAR,
    UART_SVC_IDX_RX_VAL,
    UART_SVC_IDX_TX_CHAR,
    UART_SVC_IDX_TX_VAL,
    UART_SVC_IDX_TX_NTF_CFG,
    UART_SVC_ATT_NUM
};

enum adv_status
{
    ADV_IDLE,
    ADV_BUSY,
};

static const struct att_decl ls_uart_server_att_decl[UART_SVC_ATT_NUM] =
{
    [UART_SVC_IDX_RX_CHAR] = {
        .uuid = att_decl_char_array,
        .s.max_len = 0,
        .s.uuid_len = UUID_LEN_16BIT,
        .s.read_indication = 1,
        .char_prop.rd_en = 1,
    },
    [UART_SVC_IDX_RX_VAL] = {
        .uuid = ls_uart_rx_char_uuid_128,
        .s.max_len = UART_SVC_RX_MAX_LEN,
        .s.uuid_len = UUID_LEN_128BIT,
        .s.read_indication = 1,
        .char_prop.wr_cmd = 1,
        .char_prop.wr_req = 1,
    },
    [UART_SVC_IDX_TX_CHAR] = {
        .uuid = att_decl_char_array,
        .s.max_len = 0,
        .s.uuid_len = UUID_LEN_16BIT,
        .s.read_indication = 1,
        .char_prop.rd_en = 1,
    },
    [UART_SVC_IDX_TX_VAL] = {
        .uuid = ls_uart_tx_char_uuid_128,
        .s.max_len = UART_SVC_TX_MAX_LEN,
        .s.uuid_len = UUID_LEN_128BIT,
        .s.read_indication = 1,
        .char_prop.ntf_en = 1,
    },
    [UART_SVC_IDX_TX_NTF_CFG] = {
        .uuid = att_desc_client_char_cfg_array,
        .s.max_len = 0,
        .s.uuid_len = UUID_LEN_16BIT,
        .s.read_indication = 1,
        .char_prop.rd_en = 1,
        .char_prop.wr_req = 1,
    },
};
static const struct svc_decl ls_uart_server_svc =
{
    .uuid = ls_uart_svc_uuid_128,
    .att = (struct att_decl *)ls_uart_server_att_decl,
    .nb_att = UART_SVC_ATT_NUM,
    .uuid_len = UUID_LEN_128BIT,
};
static struct gatt_svc_env ls_uart_server_svc_env;
static uint8_t connect_id = 0xff;
static uint8_t uart_server_rx_byte;
static uint8_t uart_server_buf[UART_SVC_BUFFER_SIZE];
static uint8_t uart_server_tx_buf[UART_SVC_BUFFER_SIZE];
static uint16_t uart_server_rx_index = 0;
static UART_HandleTypeDef UART_Server_Config;
static bool uart_server_tx_busy;
static bool uart_server_ntf_done = true;
static uint16_t uart_server_mtu = UART_SERVER_MTU_DFT;
static struct builtin_timer *uart_server_timer_inst = NULL;
static bool update_adv_intv_flag = false;
static uint16_t cccd_config = 0;

static uint8_t adv_obj_hdl;
static uint8_t advertising_data[28];
static uint8_t scan_response_data[31];
static uint8_t adv_status = ADV_IDLE;

/**defgroup BLE_GAP_IO_CAPS GAP IO Capabilities**/
#define BLE_GAP_IO_CAPS_DISPLAY_ONLY 0x0     /**< Display Only. */
#define BLE_GAP_IO_CAPS_DISPLAY_YESNO 0x1    /**< Display and Yes/No entry. */
#define BLE_GAP_IO_CAPS_KEYBOARD_ONLY 0x2    /**< Keyboard Only. */
#define BLE_GAP_IO_CAPS_NONE 0x3             /**< No I/O capabilities. */
#define BLE_GAP_IO_CAPS_KEYBOARD_DISPLAY 0x4 /**< Keyboard and Display. */

#define OOB_DATA_FLAG 0x0
#define AUTHREQ 0x1 //0x15
#define KEY_SIZE 0x10
#define INIT_KEY_DIST 0x3
#define RESP_KEY_DIST 0x3
#define passkey_number               \
    {                                \
        '1', '2', '3', '4', '5', '6' \
    }

struct gap_slave_security_req test_auth;
struct pair_feature test_feat = {BLE_GAP_IO_CAPS_NONE,
                                 OOB_DATA_FLAG,
                                 AUTHREQ,
                                 KEY_SIZE,
                                 INIT_KEY_DIST,
                                 RESP_KEY_DIST};

struct gap_pin_str test_passkey = {passkey_number, 0};

//struct gap_pin_str test_passkey;

static void ls_uart_server_init(void);
static void ls_uart_server_timer_cb(void *param);
static void ls_uart_init(void);
static void ls_uart_server_read_req_ind(uint8_t att_idx, uint8_t con_idx);
static void ls_uart_server_write_req_ind(uint8_t att_idx, uint8_t con_idx, uint16_t length, uint8_t const *value);
static void ls_uart_server_send_notification(void);
static void at_start_adv(void);
static void hid_server_get_dev_name(struct gap_dev_info_dev_name *dev_name_ptr, uint8_t con_idx);

const uint8_t hid_report_map[] =
{
    0x05, 0x0D,       // Usage Page (Digitizer)
    0x09, 0x04,       // Usage (Touch Screen)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0x03,       // Report ID (3)
    0x09, 0x22,       // Usage (Finger)
    0xA1, 0x02,       // Collection (Logical)
    0x09, 0x42,       // Usage (Tip Switch)
    0x15, 0x00,       // Logical Minimum (0)
    0x25, 0x01,       // Logical Maximum (1)
    0x75, 0x01,       // Report Size (1)
    0x95, 0x01,       // Report Count (1)
    0x81, 0x02,       // Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x32,       // Usage (In Range)
    0x81, 0x02,       // Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x06,       // Report Count (6)
    0x81, 0x03,       // Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x75, 0x08,       // Report Size (8)
    0x09, 0x51,       // Usage (0x51)
    0x95, 0x01,       // Report Count (1)
    0x81, 0x02,       // Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x01,       // Usage Page (Generic Desktop Ctrls)
    0x26, 0xFF, 0x0F, // Logical Maximum (4095)
    0x75, 0x10,       // Report Size (16)
    0x55, 0x0E,       // Unit Exponent (-2)
    0x65, 0x33,       // Unit (System: English Linear, Length: Inch)
    0x09, 0x30,       // Usage (X)
    0x35, 0x00,       // Physical Minimum (0)
    0x46, 0xB5, 0x04, // Physical Maximum (1205)
    0x81, 0x02,       // Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x46, 0x8A, 0x03, // Physical Maximum (906)
    0x09, 0x31,       // Usage (Y)
    0x81, 0x02,       // Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,             // End Collection

    0x05, 0x0D, // Usage Page (Digitizer)
    0x09, 0x54, // Usage (0x54)
    0x95, 0x01, // Report Count (1)
    0x75, 0x08, // Report Size (8)
    0x81, 0x02, // Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x85, 0x01, // Report ID (8)
    0x09, 0x55, // Usage (0x55)
    0x25, 0x02, // Logical Maximum (2)
    0xB1, 0x02, // Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,       // End Collection

    0x05, 0x0C, // Usage Page (Consumer)
    0x09, 0x01, // Usage (Consumer Control)
    0xA1, 0x01, // Collection (Application)
    0x85, 0x01, // Report ID (2)
    0x09, 0xE9, // Usage (Volume Increment)
    0x09, 0xEA, // Usage (Volume Decrement)
    0x09, 0xE2, // Usage (Mute)
    0x09, 0x30, // Usage (Power)
    0x15, 0x01, // Logical Minimum (1)
    0x25, 0x0C, // Logical Maximum (12)
    0x75, 0x10, // Report Size (16)
    0x95, 0x01, // Report Count (1)
    0x81, 0x00, // Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,       // End Collection
};
#define HID_REPORT_MAP_LEN (sizeof(hid_report_map))

static void ls_uart_server_init(void)
{
    uart_server_timer_inst = builtin_timer_create(ls_uart_server_timer_cb);
    builtin_timer_start(uart_server_timer_inst, UART_SERVER_TIMEOUT, NULL);
}

static void ls_uart_server_update_adv_interval(uint8_t input_intv)
{
    LOG_I("input_char: %d", input_intv);
    uint32_t new_intv = (input_intv - '0') * 160;
    dev_manager_update_adv_interval(adv_obj_hdl, new_intv, new_intv);
    dev_manager_stop_adv(adv_obj_hdl);
    update_adv_intv_flag = true;
}

static void ls_uart_server_timer_cb(void *param)
{
    if (connect_id != 0xff)
    {
        enter_critical();
        // LOG_I("uart timer out, length=%d", uart_server_rx_index);
        ls_uart_server_send_notification();
        exit_critical();
    }
    uint8_t input_char = (uint8_t)SEGGER_RTT_GetKey();
    if (input_char != 0xff && input_char > '0' && input_char <= '9')
    {
        ls_uart_server_update_adv_interval(input_char);
    }
    if (uart_server_timer_inst)
    {
        builtin_timer_start(uart_server_timer_inst, UART_SERVER_TIMEOUT, NULL);
    }
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart, void *tx_arg)
{
    uart_server_tx_busy = false;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart, void *rx_arg)
{
    if (uart_server_rx_index < UART_SVC_BUFFER_SIZE)
    {
        uart_server_buf[uart_server_rx_index++] = uart_server_rx_byte;
    }
    else
    {
        LOG_I("uart server rx buffer overflow!");
    }
    HAL_UART_Receive_IT(&UART_Server_Config, &uart_server_rx_byte, 1, NULL);
}
static void ls_uart_init(void)
{
    uart1_io_init(PB00, PB01);
    UART_Server_Config.UARTX = UART1;
    UART_Server_Config.Init.BaudRate = UART_BAUDRATE_115200;
    UART_Server_Config.Init.MSBEN = 0;
    UART_Server_Config.Init.Parity = UART_NOPARITY;
    UART_Server_Config.Init.StopBits = UART_STOPBITS1;
    UART_Server_Config.Init.WordLength = UART_BYTESIZE8;
    HAL_UART_Init(&UART_Server_Config);  
}
static void ls_uart_server_read_req_ind(uint8_t att_idx, uint8_t con_idx)
{
    uint16_t handle = 0;
    if (att_idx == UART_SVC_IDX_TX_NTF_CFG)
    {
        handle = gatt_manager_get_svc_att_handle(&ls_uart_server_svc_env, att_idx);
        gatt_manager_server_read_req_reply(con_idx, handle, 0, (void *)&cccd_config, 2);
    }
}
static void ls_uart_server_write_req_ind(uint8_t att_idx, uint8_t con_idx, uint16_t length, uint8_t const *value)
{
    if (att_idx == UART_SVC_IDX_RX_VAL)
    {
        if (uart_server_tx_busy)
        {
            LOG_I("Uart tx busy, data discard!");
        }
        else
        {
            uart_server_tx_busy = true;
            LS_ASSERT(length <= UART_SVC_BUFFER_SIZE);
            memcpy(uart_server_tx_buf, (uint8_t *)value, length);
            HAL_UART_Transmit_IT(&UART_Server_Config, (uint8_t *)uart_server_tx_buf, length, NULL);
        }
    }
    else if (att_idx == UART_SVC_IDX_TX_NTF_CFG)
    {
        LS_ASSERT(length == 2);
        memcpy(&cccd_config, value, length);
    }
}
static void ls_uart_server_send_notification(void)
{
    if (uart_server_rx_index > 0 && uart_server_ntf_done)
    {
        uart_server_ntf_done = false;
        uint16_t handle = gatt_manager_get_svc_att_handle(&ls_uart_server_svc_env, UART_SVC_IDX_TX_VAL);
        uint16_t tx_len = uart_server_rx_index > co_min(UART_SERVER_MAX_DATA_LEN, UART_SVC_TX_MAX_LEN) ? co_min(UART_SERVER_MAX_DATA_LEN, UART_SVC_TX_MAX_LEN) : uart_server_rx_index;
        uart_server_rx_index -= tx_len;
        gatt_manager_server_send_notification(connect_id, handle, uart_server_buf, tx_len, NULL);
        memcpy((void *)&uart_server_buf[0], (void *)&uart_server_buf[tx_len], uart_server_rx_index);
    }
}

static void gap_manager_callback(enum gap_evt_type type, union gap_evt_u *evt, uint8_t con_idx)
{
    switch (type)
    {
    case CONNECTED:
        connect_id = con_idx;
        LOG_I("connected!");
        test_auth.auth = 0x5;
        if (evt->connected.peer_id == 0xFF)
        {
            gap_manager_slave_security_req(connect_id, test_auth.auth);
        }
        break;
    case DISCONNECTED:
        LOG_I("disconnected!");
        connect_id = 0xff;
        uart_server_mtu = UART_SERVER_MTU_DFT;
        at_start_adv();
        break;
    case CONN_PARAM_REQ:

        break;
    case CONN_PARAM_UPDATED:

        break;
    case MASTER_PAIR_REQ: //4
        connect_id = con_idx;
        gap_manager_slave_pair_response_send(connect_id, true, &test_feat);
        //gap_manager_passkey_input(connect_id,&test_passkey);
        break;

    case SLAVE_SECURITY_REQ: //5
        LOG_I("SLAVE_SECURITY_REQ");
        connect_id = con_idx;

        break;

    case PAIR_DONE: //6
        connect_id = con_idx;
        LOG_I("PAIR_DONE");
        break;

    case ENCRYPT_DONE: //7
        connect_id = con_idx;

        break;

    case DISPLAY_PASSKEY: //8
        connect_id = con_idx;
        LOG_I("DISPLAY_PASSKEY");
        // LOG_I("passkey=%d",evt->display_passkey.passkey.pin);
        // LOG_I("DISPLAY_PASSKEY");
        //gap_manager_passkey_input(connect_id,&test_passkey);
        break;

    case REQUEST_PASSKEY: //9
        LOG_I("REQUEST_PASSKEY");
        connect_id = con_idx;
        //gap_manager_passkey_input(connect_id,&test_passkey);
        break;

    case NUMERIC_COMPARE: //10
        LOG_I("NUMERIC_COMPARE");
        connect_id = con_idx;
        //gap_manager_numeric_compare_set(connect_id,true);
        break;
    case GET_DEV_INFO_DEV_NAME:
        hid_server_get_dev_name((struct gap_dev_info_dev_name *)evt, con_idx);
        break;
    default:

        break;
    }
}

static void gatt_manager_callback(enum gatt_evt_type type, union gatt_evt_u *evt, uint8_t con_idx)
{
    switch (type)
    {
    case SERVER_READ_REQ:
        LOG_I("read req");
        ls_uart_server_read_req_ind(evt->server_read_req.att_idx, con_idx);
        break;
    case SERVER_WRITE_REQ:
        LOG_I("write req");
        ls_uart_server_write_req_ind(evt->server_write_req.att_idx, con_idx, evt->server_write_req.length, evt->server_write_req.value);
        break;
    case SERVER_NOTIFICATION_DONE:
        uart_server_ntf_done = true;
        LOG_I("ntf done");
        break;
    case MTU_CHANGED_INDICATION:
        uart_server_mtu = evt->mtu_changed_ind.mtu;
        LOG_I("mtu: %d", uart_server_mtu);
        break;
    default:
        LOG_I("Event not handled!");
        break;
    }
}

static void hid_server_get_dev_name(struct gap_dev_info_dev_name *dev_name_ptr, uint8_t con_idx)
{
    LS_ASSERT(dev_name_ptr);
    dev_name_ptr->value = (uint8_t *)APP_HID_DEV_NAME;
    dev_name_ptr->length = APP_HID_DEV_NAME_LEN;
}

static void prf_hid_server_callback(enum hid_evt_type type, union hid_evt_u *evt, uint8_t con_idx)
{
    switch (type)
    {
    case HID_REPORT_READ:
        evt->read_report_req.value = (uint8_t *)hid_report_map;
        evt->read_report_req.length = HID_REPORT_MAP_LEN;
        break;
    default:
        break;
    }
}

static void at_start_adv(void)
{
    uint8_t *pos;
    uint8_t adv_data_len;
    uint8_t scan_rsp_data_len;

    if (adv_status != ADV_IDLE)
    {
        return;
    }
    adv_status = ADV_BUSY;

    pos = &scan_response_data[0];
    *pos++ = strlen((const char *)ble_device_name) + 1;
    *pos++ = '\x08';
    memcpy(pos, ble_device_name, strlen((const char *)ble_device_name));
    pos += strlen((const char *)ble_device_name);
    scan_rsp_data_len = ((uint32_t)pos - (uint32_t)(&scan_response_data[0]));

    pos = &advertising_data[0];

    uint8_t manufacturer_value[] = {0xB8, 0x08};
    *pos++ = sizeof(manufacturer_value) + 1;
    *pos++ = '\xff';
    memcpy(pos, manufacturer_value, sizeof(manufacturer_value));
    pos += sizeof(manufacturer_value);

    uint16_t uuid_value = 0x1812;
    *pos++ = sizeof(uuid_value) + 1;
    *pos++ = '\x03';
    memcpy(pos, (uint8_t *)&uuid_value, sizeof(uuid_value));
    pos += sizeof(uuid_value);

    adv_data_len = ((uint32_t)pos - (uint32_t)(&advertising_data[0]));

    LOG_I("start adv");
    dev_manager_start_adv(adv_obj_hdl, advertising_data, adv_data_len, scan_response_data, scan_rsp_data_len);
}
static void create_adv_obj()
{
    struct legacy_adv_obj_param adv_param = {
        .adv_intv_min = 0x20,
        .adv_intv_max = 0x20,
        .own_addr_type = PUBLIC_OR_RANDOM_STATIC_ADDR,
        .filter_policy = 0,
        .ch_map = 0x7,
        .disc_mode = ADV_MODE_GEN_DISC,
        .prop = {
            .connectable = 1,
            .scannable = 1,
            .directed = 0,
            .high_duty_cycle = 0,
        },
    };
    dev_manager_create_legacy_adv_object(&adv_param);
}

static void prf_added_handler(struct profile_added_evt *evt)
{
    LOG_I("profile:%d, start handle:0x%x\n", evt->id, evt->start_hdl);
    switch (evt->id)
    {
    case PRF_HID:
        prf_hid_server_callback_init(prf_hid_server_callback);
        create_adv_obj();
        break;
    default:
        break;
    }
}

static void dev_manager_callback(enum dev_evt_type type, union dev_evt_u *evt)
{
    switch (type)
    {
    case STACK_INIT:
    {
        struct ble_stack_cfg cfg = {
            .private_addr = false,
            .controller_privacy = false,
        };
        dev_manager_stack_init(&cfg);
    }
    break;
    case STACK_READY:
    {
        struct hid_db_cfg db_cfg;
        uint8_t addr[6];
        bool type;
        dev_manager_get_identity_bdaddr(addr, &type);
        LOG_I("type:%d,addr:", type);
        LOG_HEX(addr, sizeof(addr));

        db_cfg.hids_nb = 1;
        db_cfg.cfg[0].svc_features = HID_KEYBOARD;
        db_cfg.cfg[0].report_nb = 1;
        db_cfg.cfg[0].report_id[0] = 1;
        db_cfg.cfg[0].report_cfg[0] = HID_REPORT_IN;
        db_cfg.cfg[0].info.bcdHID = 0X0111;
        db_cfg.cfg[0].info.bCountryCode = 0;
        db_cfg.cfg[0].info.flags = HID_WKUP_FOR_REMOTE | HID_NORM_CONN;
        dev_manager_prf_hid_server_add(NO_SEC, &db_cfg, sizeof(db_cfg));
        ls_uart_init();
        HAL_UART_Receive_IT(&UART_Server_Config, &uart_server_rx_byte, 1, NULL);
        ls_uart_server_init();
    }
    break;
    case SERVICE_ADDED:
        gatt_manager_svc_register(evt->service_added.start_hdl, UART_SVC_ATT_NUM, &ls_uart_server_svc_env);
        // create_adv_obj();
        break;
    case PROFILE_ADDED:
        dev_manager_add_service((struct svc_decl *)&ls_uart_server_svc);
        prf_added_handler(&evt->profile_added);
        break;
    case ADV_OBJ_CREATED:
        LS_ASSERT(evt->obj_created.status == 0);
        adv_obj_hdl = evt->obj_created.handle;
        at_start_adv();
        break;
    case ADV_STOPPED:
        adv_status = ADV_IDLE;
        if (update_adv_intv_flag)
        {
            update_adv_intv_flag = false;
            at_start_adv();
        }
        break;
    case SCAN_STOPPED:

        break;
    default:

        break;
    }
}

int main()
{
    sys_init_app();
    ble_init();
    dev_manager_init(dev_manager_callback);
    gap_manager_init(gap_manager_callback);
    gatt_manager_init(gatt_manager_callback);
    ble_loop();
}
