#include "ls_ble.h"
#include "platform.h"

void main()
{
	sys_init(false);
	ble_init();
	while(1)
	{
		ble_loop();
	}
}

