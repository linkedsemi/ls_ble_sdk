#define LOG_TAG "MAIN"
#include "ls_ble.h"
#include "platform.h"
#include "prf_diss.h"
#include "log.h"
#include "ls_dbg.h"

static uint8_t adv_obj_hdl;
static uint8_t advertising_data[31];
static uint8_t scan_response_data[31];

static void gap_manager_callback(enum gap_evt_type type,union gap_evt_u *evt,uint8_t con_idx)
{
    switch(type)
    {
    case CONNECTED:

    break;
    case DISCONNECTED:

    break;
    case CONN_PARAM_UPDATED:

    break;
    default:

    break;
    }
}

static void gatt_manager_callback(enum gatt_evt_type type,union gatt_evt_u *evt,uint8_t con_idx)
{
    
}

static void dis_server_get_value(uint8_t value)
{

}

static void prf_dis_server_callback(enum diss_evt_type type,union diss_evt_u *evt,uint8_t con_idx)
{
    switch(type)
    {
    case DIS_SERVER_SET_VALUE_RSP:

    break;
    case DIS_SERVER_GET_VALUE_IND:
        dis_server_get_value(evt->get_value_ind.value);
    break;
    default:
        LS_ASSERT(0);
    break;
    }
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
    LOG_I("profile:%d, start handle:0x%x\n",evt->id,evt->start_hdl);
    switch(evt->id)
    {
    case PRF_DIS_SERVER:
        prf_dis_server_callback_init(prf_dis_server_callback);
        create_adv_obj();
    break;
    default:

    break;
    }
}

static void dev_manager_callback(enum dev_evt_type type,union dev_evt_u *evt)
{
    switch(type)
    {
    case STACK_INIT:
    {
        struct ble_stack_cfg cfg = {
            .private_addr = true,
            .controller_privacy = false,
        };
        dev_manager_stack_init(&cfg);
    }
    break;
    case STACK_READY:
        dev_manager_prf_dis_server_add(NO_SEC,0xffff);
    break;
    case SERVICE_ADDED:

    break;
    case PROFILE_ADDED:
        prf_added_handler(&evt->profile_added);
    break;
    case ADV_OBJ_CREATED:
        adv_obj_hdl = evt->obj_created.handle;
        dev_manager_start_adv(adv_obj_hdl,advertising_data,sizeof(advertising_data),scan_response_data,sizeof(scan_response_data));
    break;
    case ADV_STOPPED:
    
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
    while(1)
    {
        ble_loop();
    }
    return 0;
}
