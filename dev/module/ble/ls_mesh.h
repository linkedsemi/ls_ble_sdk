#ifndef _LS_MESH_H_
#define _LS_MESH_H_
#include <stdint.h>
#define __EMPTY
enum ls_mesh_evt_type
{
    LS_MESH_RX_MSG_EVT,
    LS_MESH_FINISH_EVT,
};

struct ls_mesh_rx_msg_evt
{
    uint8_t  msg_len;
    uint8_t  adv_type;
    uint16_t uuid;
    uint16_t handle;
    uint32_t version;
    uint8_t value[__EMPTY];
} __attribute__ ((packed));

union ls_mesh_evt_u {
    struct ls_mesh_rx_msg_evt ls_mesh_send_msg;
};

void prf_ls_mesh_callback_init(void (*evt_cb)(enum ls_mesh_evt_type, union ls_mesh_evt_u *));
void ls_mesh_init(void);
void dev_manager_prf_ls_mesh_add(uint8_t sec_lvl, void *param);
void ls_mesh_start(void);
void ls_mesh_set_beacon_value_ind(const uint8_t *value, uint8_t len);
#endif //_LS_MESH_H_
