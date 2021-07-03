#define LOG_TAG "24g_main"
#include <string.h>
#include "cpu.h"
#include "ls_ble.h"
#include "platform.h"
#include "ls_24g.h"
#include "log.h"
#include "ls_dbg.h"

void lib_fn_init(void)
{
    extern uint64_t (*idiv_acc_fn)(uint32_t,uint32_t,bool);
    extern void (*exit_critical_fn)(uint32_t);
    extern uint32_t (*enter_critical_fn)(void);

    idiv_acc_fn = idiv_acc;
    enter_critical_fn = enter_critical;
    exit_critical_fn = exit_critical;
}
