#include "ls_ble.h"

static void gap_manager_callback(enum gap_evt_type type,union gap_evt_u *evt,uint8_t con_idx)
{
	
	
}

static void gatt_manager_callback(enum gatt_evt_type type,union gatt_evt_u *evt,uint8_t con_idx)
{
	
}

void dev_event_callback(enum dev_evt_type type,union dev_evt_u *evt)
{
	switch(type)
	{
	case STACK_INIT:
		dev_init();
		gap_manager_init(gap_manager_callback);
		gatt_manager_init(gatt_manager_callback);
	break;
	case STACK_READY:
		//add_profile();
		//add_service();
		//scan_start();
	break;
	case PROFILE_ADDED:
		//adv_start();
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


void main()
{
	sys_init();
	ble_init();
	while(1)
	{
		ble_loop();
	}
}