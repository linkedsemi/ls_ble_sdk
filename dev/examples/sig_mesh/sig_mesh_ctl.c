#include <string.h>
#include <stdio.h>
#include "sig_mesh_ctl.h"
#include "log.h"
#include "le501x.h"
#include "lsgpio.h"
#include "sig_light_cfg.h"

extern struct mesh_model_info model_env;
//static uint8_t advertising_data[28];

void dev_mesh_status_rsp(struct model_rx_info const *ind,uint32_t opcode,uint8_t *status,uint16_t cmd_len)
{
    struct rsp_model_info rsp;
    rsp.opcode = opcode;
    rsp.ModelHandle = ind->ModelHandle;
    rsp.app_key_lid = ind->app_key_lid;
    rsp.dest_addr = ind->source_addr;
    rsp.len = ind->rx_info_len;
    memcpy(rsp.info, status, cmd_len);
    rsp_model_info_ind(&rsp);
}

void sig_mesh_mdl_state_upd_hdl(struct model_state_upd* msg)
{
    LOG_I("evt id:%d",msg->state_id);
    switch(msg->state_id)
    {
        case MESH_STATE_GEN_ONOFF:
            sigmesh_set_onoff(msg->state);
        break;
        case MESH_STATE_GEN_LVL:
            sigmesh_set_lightness(msg->state - GENERIC_LEVEL_MIN);
        break;
        default:
        break;
    }
}
