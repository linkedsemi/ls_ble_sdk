#include "modem_rf_le501x.h"
#include "reg_rcc.h"
#include "field_manipulate.h"
#include "reg_rf.h"
#include "reg_mdm2.h"
#include "reg_syscfg.h"

#define RF_GAIN_TBL_SIZE           (8)
#define RF_PWR_TBL_SIZE            (8)

#define PLL_GAIN_CAL_FRQ_DIS0  20

uint8_t   pll_int_vtxd_ext;


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

// TX max powerf
#define RF_POWER_MAX                0xf
#define RF_POWER_MIN                0


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
    return RF_POWER_MAX;
}

uint8_t rx_txpwr_max_get()
{
    return RF_POWER_MAX;
}

uint8_t rx_txpwr_min_get()
{
    return RF_POWER_MIN;
}
static void rf_reg_init()
{
    RF->REG00 = FIELD_BUILD(RF_EN_DAC_DIG_PWR, 1)
               | FIELD_BUILD(RF_EN_AGC_PWR, 1)
               | FIELD_BUILD(RF_EN_PLL_SDM, 1)
               | FIELD_BUILD(RF_EN_ADC_DIG, 1)
               | FIELD_BUILD(RF_EN_LDO_PAHP, 1)
               | FIELD_BUILD(RF_EN_LDO_PLL, 1)
               | FIELD_BUILD(RF_EN_LDO_VCO, 1)
               | FIELD_BUILD(RF_EN_LDO_PA, 1)
               | FIELD_BUILD(RF_EN_LDO_IF, 1)
               | FIELD_BUILD(RF_EN_LDO_TX, 1)
               | FIELD_BUILD(RF_EN_LDO_RX, 1)
               | FIELD_BUILD(RF_EN_LDO_PAHP_BYPS, 0)
               | FIELD_BUILD(RF_EN_PAHP, 0)
               | FIELD_BUILD(RF_EN_DAC_ZB, 0)
               | FIELD_BUILD(RF_EN_DAC_BLE, 0)
               | FIELD_BUILD(RF_EN_PA_STG2, 0)
               | FIELD_BUILD(RF_EN_PA_STG1, 0)
               | FIELD_BUILD(RF_EN_PA, 0)
               | FIELD_BUILD(RF_EN_PLL, 1)
               | FIELD_BUILD(RF_EN_AGC, 0)
               | FIELD_BUILD(RF_EN_ADC, 1)
               | FIELD_BUILD(RF_EN_LMT_RSSI, 1)
               | FIELD_BUILD(RF_EN_BPF, 1)
               | FIELD_BUILD(RF_EN_MIXL, 1)
               | FIELD_BUILD(RF_EN_MIXH, 1)
               | FIELD_BUILD(RF_EN_LNA, 1);
    RF->REG04 = FIELD_BUILD(RF_LNA_VB_ADJ,7)
               | FIELD_BUILD(RF_LNA_TANK_TUNE,1)
               | FIELD_BUILD(RF_LNA_R_ADJ,0)
               | FIELD_BUILD(RF_MIXL_BIAS_CTL,3)
               | FIELD_BUILD(RF_MIXL_BIAS_SEL,0)
               | FIELD_BUILD(RF_MIXH_BIAS_CTL,3)
               | FIELD_BUILD(RF_MIXH_BIAS_SEL,0)
               | FIELD_BUILD(RF_BPF_CAL_CODE_EXT,0x1d) // 1a  -1d
               | FIELD_BUILD(RF_BPF_CAL_CODE_EXT_EN,1)
               | FIELD_BUILD(RF_BPF_CAL_EN,0)
               | FIELD_BUILD(RF_EN_LNA_BYPS,0)
               | FIELD_BUILD(RF_LNA_GAIN,0)
               | FIELD_BUILD(RF_MIXL_GAIN_CTL,0)
               | FIELD_BUILD(RF_MIXH_GAIN_CTL,0)
               | FIELD_BUILD(RF_BPF_GAIN_ADJ,3)
               | FIELD_BUILD(RF_MIX_ENB_CAP,0);
    RF->REG08 = FIELD_BUILD(RF_LDO_RX_TRIM,4)
               | FIELD_BUILD(RF_LDO_TX_TRIM,4)
               | FIELD_BUILD(RF_CF_BW12M_ADJ,0)
               | FIELD_BUILD(RF_TX_RATE,0)
               | FIELD_BUILD(RF_CF_BW08M_ADJ,0)
               | FIELD_BUILD(RF_TX_DATA_TST_EN,0)
               | FIELD_BUILD(RF_PA_VCAS_RES_ADJ,1)
               | FIELD_BUILD(RF_PA_GAIN,0xf)
               | FIELD_BUILD(RF_PA_TANK_Q_ADJ,0)
               | FIELD_BUILD(RF_EN_PA_IBX2,0);
    RF->REG0C = FIELD_BUILD(RF_PA_TANK_TUNE,1)
               | FIELD_BUILD(RF_EN_RSSI_Q,1)
               | FIELD_BUILD(RF_EN_RSSI_I,1)
               | FIELD_BUILD(RF_PA_VB1_ADJ,0)
               | FIELD_BUILD(RF_PA_VB2_ADJ,0)
               | FIELD_BUILD(RF_PA_PTAT_ADJ,1)
               | FIELD_BUILD(RF_EN_PA_IPTAT,1)
               | FIELD_BUILD(RF_PA_BG_ADJ,4)
               | FIELD_BUILD(RF_EN_PA_IBG,1)
               | FIELD_BUILD(RF_PLL_BAND_CAL_SEL,0)
               | FIELD_BUILD(RF_PLL_AFC_FRAC_EN,0)
               | FIELD_BUILD(RF_PLL_AFC_DC_EN,0)
               | FIELD_BUILD(RF_PLL_VCTRL_EXT_EN,0)
               | FIELD_BUILD(RF_PLL_DIV_ADJ,2)
               | FIELD_BUILD(RF_PLL_SEL_RTX_BW,0);
    RF->REG10 = FIELD_BUILD(RF_PLL_DI_S,5)
               | FIELD_BUILD(RF_PLL_RTX_SEL,0)
               | FIELD_BUILD(RF_PLL_OPEN_EN,0)
               | FIELD_BUILD(RF_PLL_CAL_EN,0)
               | FIELD_BUILD(RF_PLL_FREQ_ADJ_EXT,0)
               | FIELD_BUILD(RF_PLL_FREQ_EXT_EN,0)
               | FIELD_BUILD(RF_PLL_FAST_LOCK_EN,1)
               | FIELD_BUILD(RF_PLL_REF_SEL,0)
               | FIELD_BUILD(RF_PLL_VREF_ADJ,0)
               | FIELD_BUILD(RF_PLL_FBDIV_PD_BYPS,0)
               | FIELD_BUILD(RF_PLL_BW_ADJ,6)
               | FIELD_BUILD(RF_PLL_LOCK_BYPS,0)
               | FIELD_BUILD(RF_PLL_CP_OS_ADJ,0)
               | FIELD_BUILD(RF_PLL_CP_OS_EN,0)
               | FIELD_BUILD(RF_PLL_VCO_ADJ,2);//2020 5.22CAI 2-6
    RF->REG14 = FIELD_BUILD(RF_PLL_FRAC,0)
               | FIELD_BUILD(RF_DAC_CAL_DATA_EXT,0)
               | FIELD_BUILD(RF_DAC_CAL_EN_EXT,0)
               | FIELD_BUILD(RF_DAC_EXT_EN,0)
               | FIELD_BUILD(RF_DAC_BLE_DELAY_ADJ,0x10);
    RF->REG18 = FIELD_BUILD(RF_DAC_REFL_ADJ,4)
               | FIELD_BUILD(RF_ADC_MUX_SEL,0)
               | FIELD_BUILD(RF_ADC_VREF_ADJ,0)
               | FIELD_BUILD(RF_ADC_TEST_SEL,0)
               | FIELD_BUILD(RF_EN_ADC_CNT_MODE,0)
               | FIELD_BUILD(RF_ADC_START,0);
    RF->REG1C = FIELD_BUILD(RF_EN_LDO_PLL_BYPS,0)
               | FIELD_BUILD(RF_EN_LDO_RX_BYPS,0)
               | FIELD_BUILD(RF_EN_LDO_TX_BYPS,0)
               | FIELD_BUILD(RF_EN_LDO_IF_BYPS,0)
               | FIELD_BUILD(RF_EN_LDO_PA_BYPS,0)
               | FIELD_BUILD(RF_EN_LDO_VCO_BYPS,0)
               | FIELD_BUILD(RF_ADC_REFBUF_LP,0)
               | FIELD_BUILD(RF_DAC_REFH_ADJ,1)
               | FIELD_BUILD(RF_PLL_DI_P,0x1f)
               | FIELD_BUILD(RF_PLL_FBDIV_PD,0)
               | FIELD_BUILD(RF_PLL_SDM_TEST_EN,0)
               | FIELD_BUILD(RF_PLL_FRAC_INT_MODE,0);
    RF->REG20 = FIELD_BUILD(RF_BPF_IADJ,4)
               | FIELD_BUILD(RF_BPF_BW_ADJ,1)
               | FIELD_BUILD(RF_BPF_MODE_SEL,0)
               | FIELD_BUILD(RF_BPF_CENT_ADJ,2)
               | FIELD_BUILD(RF_AT0_SEL,0xe)
               | FIELD_BUILD(RF_AT1_SEL,0xe);
    RF->REG24 = FIELD_BUILD(RF_AGC_S00L,8)
               | FIELD_BUILD(RF_AGC_S11_LNA_BYPS_ADJ,1)
               | FIELD_BUILD(RF_AGC_S10_LNA_BYPS_ADJ,1)
               | FIELD_BUILD(RF_AGC_S00H,0x2a)
               | FIELD_BUILD(RF_AGC_S01_MIX_ADJ,0)
               | FIELD_BUILD(RF_AGC_S01H,0x30)
               | FIELD_BUILD(RF_AGC_S10_MIX_ADJ,0x1) 
               | FIELD_BUILD(RF_AGC_S01L,0x1a)
               | FIELD_BUILD(RF_AGC_POWER_DET_EN,1)
               | FIELD_BUILD(RF_AGC_TEST_EN,0);
    RF->REG28 = FIELD_BUILD(RF_AGC_S10L,0x1d)
               | FIELD_BUILD(RF_AGC_S11_LNA_EN_ADJ,0)
               | FIELD_BUILD(RF_AGC_S10_LNA_EN_ADJ,0)
               | FIELD_BUILD(RF_AGC_S10_BPF_ADJ,1)
               | FIELD_BUILD(RF_AGC_T_ADJ,0)
               | FIELD_BUILD(RF_AGC_VH_ADD_ADJ,0)
               | FIELD_BUILD(RF_AGC_S01_BPF_ADJ,0)
               | FIELD_BUILD(RF_AGC_S11_BPF_ADJ,1)
               | FIELD_BUILD(RF_AGC_S00_BPF_ADJ,1)
               | FIELD_BUILD(RF_AGC_S11_MIX_ADJ,1)
               | FIELD_BUILD(RF_AGC_S00_MIX_ADJ,0)
               | FIELD_BUILD(RF_AGC_S11_LNA_ADJ,3)
               | FIELD_BUILD(RF_AGC_S10_LNA_ADJ,3)
               | FIELD_BUILD(RF_AGC_S01_LNA_ADJ,3)
               | FIELD_BUILD(RF_AGC_S00_LNA_ADJ,0);
    RF->REG2C = FIELD_BUILD(RF_PLL_GAIN_CAL_SEL,0)
               | FIELD_BUILD(RF_PLL_FBDIV_RST_SEL,0)
               | FIELD_BUILD(RF_PLL_FBDIV_RST_EXT,0)
               | FIELD_BUILD(RF_PLL_PS_CNT_RST_SEL,0)
               | FIELD_BUILD(RF_AGC_TEST_S,0)
               | FIELD_BUILD(RF_PA_MN_TUNE,1)
               | FIELD_BUILD(RF_PLL_GAIN_CAL_TH,0x64)
               | FIELD_BUILD(RF_PLL_VTXD_EXT,pll_int_vtxd_ext)
               | FIELD_BUILD(RF_PLL_VTXD_EXT_EN,1)
               | FIELD_BUILD(RF_PLL_GAIN_CAL_EN,0)
               | FIELD_BUILD(RF_PLL_GAIN_CAL_DC,1);
    RF->REG30 = FIELD_BUILD(RF_RSV,0x44)
               | FIELD_BUILD(RF_LDO_PA_TRIM,6)
               | FIELD_BUILD(RF_EN_LMT_OUTI_EXT,1)
               | FIELD_BUILD(RF_EN_LMT_OUTQ_EXT,1)
               | FIELD_BUILD(RF_PAHP_SEL,0)
               | FIELD_BUILD(RF_LDO_PAHP_TRIM,0)
               | FIELD_BUILD(RF_EN_AT,0)
               | FIELD_BUILD(RF_PAHP_ADJ,0xf);
    RF->REG50 = FIELD_BUILD(RF_ANA_TEST_EN,0)
               | FIELD_BUILD(RF_PLL_AFC_BP,0)
               | FIELD_BUILD(RF_PLL_GAIN_BP,1)
               | FIELD_BUILD(RF_PPF_RC_BP,1)
               | FIELD_BUILD(RF_LDO_TEST_EN,0)
               | FIELD_BUILD(RF_RD_CLK_EN,1)
               | FIELD_BUILD(RF_PLL_TEST_EN,0)
               | FIELD_BUILD(RF_CH_SEL,1)
               | FIELD_BUILD(RF_PA_VB_SEL,0)
               | FIELD_BUILD(RF_PA_VB_TARGET,6)
               | FIELD_BUILD(RF_LDO_START_CNT,6)
               | FIELD_BUILD(RF_PA_STEP_SET,1);
    RF->REG58 = FIELD_BUILD(RF_EN_DAC_CNT,10)
               | FIELD_BUILD(RF_PLL_CAL_EN_CNT,0x08)
               | FIELD_BUILD(RF_PLL_GAIN_CAL_EN_CNT,4)
               | FIELD_BUILD(RF_EN_PA_CNT,0x48);
    RF->REG5C = FIELD_BUILD(RF_EN_PA_STG1_CNT,0x4a)
               | FIELD_BUILD(RF_EN_PA_STG2_CNT,0x4c)
               | FIELD_BUILD(RF_PLL_LOCK_CNT,0x4f)
               | FIELD_BUILD(RF_EN_RX_CNT,2);


    RF->REG70 = FIELD_BUILD(RF_RX2MBW_FORCE_EN,0)
               | FIELD_BUILD(RF_INT_VTXD_CHN_THR1,0x19)
               | FIELD_BUILD(RF_INT_VTXD_CHN_THR0,0xc)
               | FIELD_BUILD(RF_INT_VTXD_EXT2,pll_int_vtxd_ext)
               | FIELD_BUILD(RF_INT_VTXD_EXT1,pll_int_vtxd_ext);

}

static uint16_t rf_pll_gain_cal(uint8_t tx_channel_dis)
{
    uint8_t pll_gain_cal_val            =0;
	int32_t cnt_timeout                 =0;

    REG_FIELD_WR(RF->REG10, RF_PLL_RTX_SEL, 1);  
    REG_FIELD_WR(RF->REG1C, RF_PLL_FRAC_INT_MODE, 1);  //Integer mode

	REG_FIELD_WR(RF->REG10, RF_PLL_DI_S, tx_channel_dis);
    //AFC 
    REG_FIELD_WR(RF->REG10,RF_PLL_VREF_ADJ,0);//vref_calib 
    REG_FIELD_WR(RF->REG10,RF_PLL_FREQ_EXT_EN,0);         
    REG_FIELD_WR(RF->REG10, RF_PLL_CAL_EN, 0);// AFC CAL disable
	REG_FIELD_WR(RF->REG10, RF_PLL_CAL_EN, 1);// AFC CAL enable

    cnt_timeout=50000;
    while((!(REG_FIELD_RD(RF->REG38,RF_PLL_BAND_CAL_DONE))) 
        || (cnt_timeout>0))
        {cnt_timeout--;}

    // gain cal 
    REG_FIELD_WR(RF->REG00,RF_EN_DAC_BLE,1); 
    REG_FIELD_WR(RF->REG2C,RF_PLL_VTXD_EXT_EN,0); 
    REG_FIELD_WR(RF->REG2C,RF_PLL_GAIN_CAL_EN,0); 
    REG_FIELD_WR(RF->REG2C,RF_PLL_GAIN_CAL_EN,1);  

    cnt_timeout=50000;
    while((!(REG_FIELD_RD(RF->REG38,RF_PLL_GAIN_CAL_DONE)))
        || (cnt_timeout>0))
        {cnt_timeout--;}

    pll_gain_cal_val =REG_FIELD_RD(RF->REG38, RF_PLL_DAC_ADJ_TEST);
    REG_FIELD_WR(RF->REG2C,RF_PLL_VTXD_EXT_EN,1); 
    //REG_FIELD_WR(RF->REG10,RF_PLL_FREQ_EXT_EN,1);   
    REG_FIELD_WR(RF->REG10, RF_PLL_CAL_EN, 0);// PLL CAL disable
    REG_FIELD_WR(RF->REG2C,RF_PLL_GAIN_CAL_EN,0);  //gain cal disable
    REG_FIELD_WR(RF->REG1C, RF_PLL_FRAC_INT_MODE, 0);
    return pll_gain_cal_val;
}

static void pll_gain()
{
    uint8_t pll_gain_cal_val=0;

    // AFE TEST mode enable
    REG_FIELD_WR(RF->REG50,RF_ANA_TEST_EN,1);
    REG_FIELD_WR(RF->REG50,RF_LDO_TEST_EN,1);
    REG_FIELD_WR(RF->REG50,RF_PLL_TEST_EN,1);
    //TX
    REG_FIELD_WR(RF->REG10,RF_PLL_RTX_SEL,1);
    REG_FIELD_WR(RF->REG00,RF_EN_PA,1);
    REG_FIELD_WR(RF->REG00,RF_EN_PA_STG1,1);
    REG_FIELD_WR(RF->REG00,RF_EN_PA_STG2,1);
    REG_FIELD_WR(RF->REG00,RF_EN_LDO_RX,0);

    pll_gain_cal_val = rf_pll_gain_cal(PLL_GAIN_CAL_FRQ_DIS0);
    pll_int_vtxd_ext = pll_gain_cal_val; 

    REG_FIELD_WR(RF->REG2C,RF_PLL_VTXD_EXT_EN,0);  
    REG_FIELD_WR(RF->REG2C,RF_PLL_VTXD_EXT,pll_int_vtxd_ext);
    REG_FIELD_WR(RF->REG70,RF_INT_VTXD_EXT1,pll_int_vtxd_ext);
    REG_FIELD_WR(RF->REG70,RF_INT_VTXD_EXT2,pll_int_vtxd_ext);
    REG_FIELD_WR(RF->REG2C,RF_PLL_VTXD_EXT_EN,1);

    REG_FIELD_WR(RF->REG10,RF_PLL_RTX_SEL,0);
    REG_FIELD_WR(RF->REG00,RF_EN_PA_STG2,0);
    REG_FIELD_WR(RF->REG00,RF_EN_PA_STG1,0);
    REG_FIELD_WR(RF->REG00,RF_EN_PA,0);
    REG_FIELD_WR(RF->REG00,RF_EN_LDO_RX,1);
    //AFE TEST mode disable
    REG_FIELD_WR(RF->REG50,RF_ANA_TEST_EN,0);
    REG_FIELD_WR(RF->REG50,RF_LDO_TEST_EN,0);
    REG_FIELD_WR(RF->REG50,RF_PLL_TEST_EN,0);
}

static void modem_reg_init()
{
    MDM2->REG08 = FIELD_BUILD(MDM2_MIN_MAG_CONF,0x474)
               | FIELD_BUILD(MDM2_DRIFT_COR_SET,1)
               | FIELD_BUILD(MDM2_IF_SHIFT,0x400);
    MDM2->REG20 = FIELD_BUILD(MDM2_LR_IF_SHIFT,0x400)
               | FIELD_BUILD(MDM2_LR_RX_INVERT,1)
               | FIELD_BUILD(MDM2_LR_IQ_INVERT,0)
               | FIELD_BUILD(MDM2_LR_ACC_INVERT,0);
}

void modem_rf_reinit()
{
    rf_reg_init();
    modem_reg_init();
}

void modem_rf_init()
{
    RCC->APB1EN |= 1<<RCC_RF_POS | 1<<RCC_MDM2_POS;
    modem_rf_reinit();
    pll_gain();
}

