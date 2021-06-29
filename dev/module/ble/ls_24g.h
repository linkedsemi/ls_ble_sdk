#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifndef _LS_24G_H_
#define _LS_24G_H_

enum prop_24g_state
{
    PROP_24G_STATE_IDLE = 0,
    PROP_24G_STATE_SCAN,
    PROP_24G_STATE_ADV,
};
enum prop_24g_tx_power_config
{
    TX_POWER_N_20dbm = 0,              // -20dbm
    TX_POWER_N_8dbm = 2,               // -8dbm
    TX_POWER_0dbm = 4,                 // 0dbm
    TX_POWER_4dbm = 8,                 // 4dbm
    TX_POWER_10dbm = 7,                // 10dbm
    TX_POWER_12dbm = 0xB,              // 12dbm

    TX_POWER_MAX = TX_POWER_12dbm + 1,
};

struct prop_24g_tx_rx_cntl_env
{
    uint8_t *dataptr;
    void (*cb)(void*);
    void *param;
    uint8_t *length;
    bool finished;
};

void RF_24g_Init(void);
// void RF_24g_Start(void);
// void RF_24g_Stop(void);
void RF_24g_SetChannel(uint16_t channel);
uint16_t RF_24g_GetChannel(void);
void RF_24g_SetPower(enum prop_24g_tx_power_config tx_pwr_config);
void RF_24g_Tx(uint8_t *txBuf, uint8_t txLen, void (*Tx_cmpt_cb)(void *), void *param);
void RF_24g_Rx(uint8_t *rxBuf, uint8_t *rxLen, void (*Rx_cmpt_cb)(void *), void *param);
#endif
