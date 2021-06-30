#define LOG_TAG "24g_main"
#include <string.h>
#include "cpu.h"
#include "ls_ble.h"
#include "platform.h"
#include "ls_24g.h"
#include "log.h"
#include "ls_dbg.h"

#define VALID_TX_LEN_MAX 253
// The macro means the function will be called in interrupts.
#define __ISR

static struct prop_24g_tx_rx_cntl_env prop_24g_tx_env = {
    .dataptr = NULL,
};

static struct prop_24g_tx_rx_cntl_env prop_24g_rx_env = {
    .dataptr = NULL,
};

__attribute__((weak)) void app_user_24g_init(void)
{}
extern uint64_t (*idiv_acc_fn)(uint32_t,uint32_t,bool);
extern void (*exit_critical_fn)(uint32_t);
extern uint32_t (*enter_critical_fn)(void);
extern void prop_24g_init(void);
extern void prop_24g_tx_start(const uint8_t *tx_data, uint8_t length);
extern void prop_24g_rx_start(void);
extern uint8_t get_cur_prop_24g_state(void);
static void lib_fn_init(void)
{
    idiv_acc_fn = idiv_acc;
    enter_critical_fn = enter_critical;
    exit_critical_fn = exit_critical;
}
static void app_prop_24g_init(void)
{
    memset(&prop_24g_tx_env, 0, sizeof(prop_24g_tx_env));
    memset(&prop_24g_rx_env, 0, sizeof(prop_24g_rx_env));
    prop_24g_init();
}
NOINLINE XIP_BANNED static void memcpy_ram(void *dst, const void *src, uint32_t length)
{
    uint8_t const *src_addr = src;
    uint8_t *dst_addr = dst;
    while(length--)
    {
        *dst_addr++=*src_addr++;
    }
}
__ISR XIP_BANNED void app_prop_24g_tx_cb(void)
{
    // Don't modify any code in the function!!!
    prop_24g_tx_env.finished = true;
}
__ISR XIP_BANNED void app_prop_24g_rx_cb(const uint8_t *rx_buf, uint8_t *length)
{
    // Don't modify any code in the function!!!
    prop_24g_rx_env.finished = true;
    *prop_24g_rx_env.length = *length;
    memcpy_ram(prop_24g_rx_env.dataptr, rx_buf, *length);
}
void RF_24g_Tx(uint8_t *txBuf, uint8_t txLen, void (*Tx_cmpt_cb)(void *), void *param)
{
    LOG_I("RF_24g_Tx");
    LS_ASSERT(txLen <= VALID_TX_LEN_MAX && txBuf != NULL);
    prop_24g_tx_env.dataptr = txBuf;
    *prop_24g_tx_env.length = txLen;
    prop_24g_tx_env.cb = Tx_cmpt_cb;
    prop_24g_tx_env.param = param; 
}
void RF_24g_Rx(uint8_t *rxBuf, uint8_t *rxLen, void (*Rx_cmpt_cb)(void *), void *param)
{
    LS_ASSERT(rxLen != NULL && rxBuf != NULL);
    prop_24g_rx_env.dataptr = rxBuf;
    prop_24g_rx_env.length = rxLen;
    prop_24g_rx_env.cb = Rx_cmpt_cb;
    prop_24g_rx_env.param = param;
}
static void app_prop_24g_tx_cb_handle(void)
{
    if (prop_24g_tx_env.finished)
    {
        prop_24g_tx_env.finished = false;
        if (prop_24g_tx_env.cb != NULL)
        {
            prop_24g_tx_env.cb(prop_24g_tx_env.param);
        }
    }
}
static NOINLINE void app_prop_24g_tx_loop(void)
{
    app_prop_24g_tx_cb_handle();
    if (prop_24g_tx_env.dataptr != NULL)
    {
        prop_24g_tx_start(prop_24g_tx_env.dataptr, *prop_24g_tx_env.length);
        prop_24g_tx_env.dataptr = NULL;
    }
}
static NOINLINE void app_prop_24g_rx_loop(void)
{
    bool rx_finished = false;
    uint32_t cpu_stat = enter_critical();
    if (prop_24g_rx_env.finished)
    {
        prop_24g_rx_env.finished = false;
        rx_finished = true;
    }
    exit_critical(cpu_stat);
    if (rx_finished)
    {
        if (prop_24g_rx_env.dataptr != NULL && prop_24g_rx_env.cb != NULL)
        {
            prop_24g_rx_env.cb(prop_24g_rx_env.param); // no clear action for prop_24g_rx_env.dataptr
        }
    }
    if (get_cur_prop_24g_state() == PROP_24G_STATE_IDLE && prop_24g_rx_env.dataptr != NULL)
    {
         prop_24g_rx_start();
    }
}
int main()
{
    lib_fn_init();
    sys_init_24g();
    app_prop_24g_init();
    app_user_24g_init();
    while (1)
    {
        app_prop_24g_tx_loop();
        app_prop_24g_rx_loop();
    }
}
