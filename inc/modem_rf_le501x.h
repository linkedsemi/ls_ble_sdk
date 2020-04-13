#ifndef MODEM_RF_LE501X_H_
#define MODEM_RF_LE501X_H_
#include <stdint.h>
#include <stdbool.h>

uint8_t rf_txpwr_dbm_get(uint8_t txpwr_idx, uint8_t modulation);

void rf_reset(void);

void rf_force_agc_enable(bool en);

int8_t rf_rssi_convert(uint8_t rssi_reg);

uint8_t rf_txpwr_cs_get(int8_t txpwr_dbm, bool high);

void modem_rf_init(void);

#endif

