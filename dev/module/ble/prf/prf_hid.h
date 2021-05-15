#ifndef PRF_HID_H_
#define PRF_HID_H_
#include <stdint.h>

#define HID_NB_ADD_MAX    (2)
#define HID_NB_REPORT_MAX (5)

enum hid_svc_feature
{
    HID_KEYBOARD        = 0X01,
    HID_MOUSE           = 0X02,
    HID_PROTO_MODE      = 0X04,
    HID_EXT_REF         = 0X08,
    HID_BOOT_KB_WR      = 0X10,
    HID_BOOT_MOUSE_WR   = 0X20,
    HID_MASK            = 0X3F,
    HID_REPORT_NTF_EN   = 0X40,
};

enum hid_report_cfg
{
    HID_REPORT_IN   = 0X01,
    HID_REPORT_OUT  = 0X02,
    HID_REPORT_FEAT = 0X03,
    HID_REPORT_WR   = 0X10,
};

enum hid_info_flag
{
    HID_WKUP_FOR_REMOTE = 0X01,
    HID_NORM_CONN,
};

struct hid_info
{
    uint16_t bcdHID;
    uint8_t bCountryCode;
    uint8_t flags;
};

struct hids_cfg
{
    uint8_t svc_features;
    uint8_t report_nb;
    uint8_t report_cfg[HID_NB_REPORT_MAX];
    uint8_t report_id[HID_NB_REPORT_MAX];
    struct hid_info info; 
};

struct hid_db_cfg
{
    uint8_t hids_nb;
    struct hids_cfg cfg[HID_NB_ADD_MAX];
};

enum hid_evt_type
{
    HID_REPORT_READ,
    HID_NTF_CFG,
    HID_NTF_DONE,
    HID_REPORT_WRITE,
};

enum app_hogpd_report_type
{
    /// The Report characteristic is used to exchange data between a HID Device and a HID Host.
    APP_HOGPD_REPORT,
    /// The Report Map characteristic
    APP_HOGPD_REPORT_MAP,
    /// Boot Keyboard Input Report
    APP_HOGPD_BOOT_KEYBOARD_INPUT_REPORT,
    /// Boot Keyboard Output Report
    APP_HOGPD_BOOT_KEYBOARD_OUTPUT_REPORT,
    /// Boot Mouse Input Report
    APP_HOGPD_BOOT_MOUSE_INPUT_REPORT,
};

struct hid_read_report_req_evt
{
    uint16_t length;
    uint8_t* value;
    /// HIDS Instance
    uint8_t  hid_idx;
    /// type of report (@see enum app_hogpd_report_type)
    uint8_t  type;
    /// Report Instance - 0 for boot reports and report map
    uint8_t  idx;
};

struct hid_write_report_req_evt
{
    uint16_t length;
    uint8_t* value;
};

struct hid_ntf_cfg_evt
{
    uint16_t value;
};
union hid_evt_u
{
    struct hid_read_report_req_evt read_report_req;
    struct hid_ntf_cfg_evt ntf_cfg;
    struct hid_write_report_req_evt write_report_req;
};


void prf_hid_server_callback_init(void (*evt_cb)(enum hid_evt_type, union hid_evt_u *,uint8_t));
void dev_manager_prf_hid_server_add(uint8_t sec_lvl, struct hid_db_cfg* cfg,uint16_t len);
void app_hid_send_keyboard_report(uint8_t report_idx, uint8_t *report_data,uint8_t len,uint8_t conidx);
void hid_ntf_cfg_init(uint16_t ntf_cfg,uint8_t con_idx,uint8_t peer_id);
#endif
