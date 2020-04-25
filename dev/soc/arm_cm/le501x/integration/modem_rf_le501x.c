#include "modem_rf_le501x.h"
#include "reg_rcc.h"
#include "field_manipulate.h"
#include "reg_rf.h"
#include "reg_mdm2.h"

#define RF_GAIN_TBL_SIZE           (8)
#define RF_PWR_TBL_SIZE            (8)

// Gain table
static const uint8_t RF_RX_GAIN_TBL[RF_GAIN_TBL_SIZE] =
{
    [0] = 43,
    [1] = 37,
    [2] = 31,
    [3] = 25,
    [4] = 19,
    [5] = 13,
    [6] = 7,
    [7] = 1
};

// Power table
static const int8_t RF_TX_PW_CONV_TBL[RF_PWR_TBL_SIZE] =
{
    [0] = -23,
    [1] = -20,
    [2] = -17,
    [3] = -14,
    [4] = -11,
    [5] = -8,
    [6] = -5,
    [7] = -2
};

// TX max power
#define RF_POWER_MAX                6
#define RF_POWER_MIN                1


uint8_t rf_txpwr_dbm_get(uint8_t txpwr_idx, uint8_t modulation)
{
    // power table is the same for BR and EDR
    return (RF_TX_PW_CONV_TBL[txpwr_idx]);
}

void rf_reset(void)
{
    return;
}

void rf_force_agc_enable(bool en)
{
    return;
}

int8_t rf_rssi_convert(uint8_t rssi_reg)
{
    int8_t rssi_dbm;
    uint16_t power_modem;

    /* Get the RSSI value from the look up table and get its signed value
     * Get the 2-complements signed value on 8 bits */
    power_modem = ((rssi_reg & 0xF8) >> 3) * 2;
    rssi_dbm = power_modem - RF_RX_GAIN_TBL[rssi_reg & 0x07] - 64;

    return (rssi_dbm);
}

uint8_t rf_txpwr_cs_get(int8_t txpwr_dbm, bool high)
{
    uint8_t i;

    for (i = RF_POWER_MIN; i <= RF_POWER_MAX; i++)
    {
        // Loop until we find a power just higher or equal to the requested one
        if (RF_TX_PW_CONV_TBL[i] >= txpwr_dbm)
        {
            break;
        }
    }

    // If equal to value requested, do nothing
    // Else if 'high' is false and index higher than the minimum one, decrement by one
    if ((RF_TX_PW_CONV_TBL[i] != txpwr_dbm) && (!high) && (i > RF_POWER_MIN))
    {
        i--;
    }

    return (i);
}

static void rf_reg_init()
{

}

static void modem_reg_init()
{
    REG_FIELD_WR(MDM2->REG08, MDM2_IF_SHIFT, 0x400);
    REG_FIELD_WR(MDM2->REG20, MDM2_LR_IF_SHIFT, 0x400);
    REG_FIELD_WR(MDM2->REG20, MDM2_LR_RX_INVERT, 1);
}

void modem_rf_init()
{
    RCC->APB1EN |= 1<<RCC_RF_POS | 1<<RCC_MDM2_POS;
    rf_reg_init();
    modem_reg_init();
}

