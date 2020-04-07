#include "ls_ble.h"
#include "platform.h"

static void gap_manager_callback(enum gap_evt_type type,union gap_evt_u *evt,uint8_t con_idx)
{
    
    
}

static void gatt_manager_callback(enum gatt_evt_type type,union gatt_evt_u *evt,uint8_t con_idx)
{
    
}

static void dev_manager_callback(enum dev_evt_type type,union dev_evt_u *evt)
{
    switch(type)
    {

    }
}

void dev_event_callback(enum dev_evt_type type,union dev_evt_u *evt)
{
    switch(type)
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
        //add_profile();
        //add_service();
        //scan_start();
    break;
    case SERVICE_ADDED:
        //adv_start();
    break;
    case ADV_STOPPED:
    
    break;
    case SCAN_STOPPED:
    
    break;
        
        
    }
    
}


int main()
{
    sys_init(true);
    ble_init();
    dev_manager_init(dev_event_callback);
    gap_manager_init(gap_manager_callback);
    gatt_manager_init(gatt_manager_callback);
    while(1)
    {
        ble_loop();
    }
    return 0;
}
