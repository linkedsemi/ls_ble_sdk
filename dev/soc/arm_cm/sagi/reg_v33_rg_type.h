#ifndef REG_V33_RG_TYPE_H_
#define REG_V33_RG_TYPE_H_
#include <stdint.h>

typedef struct
{
    volatile uint32_t SFT_CTRL00;  //0x00
    volatile uint32_t SFT_CTRL01;
    volatile uint32_t SFT_CTRL02;
    volatile uint32_t SFT_CTRL03;
    volatile uint32_t SFT_CTRL04;  //0x10
    volatile uint32_t SFT_CTRL05;
    volatile uint32_t SFT_CTRL06;
    volatile uint32_t SFT_CTRL07;
    volatile uint32_t SFT_CTRL08;   //0x20
    volatile uint32_t SFT_CTRL09;   
    volatile uint32_t SFT_CTRL0A;
    volatile uint32_t SFT_CTRL0B;
    volatile uint32_t SFT_CTRL0C;   //0x30
    volatile uint32_t SFT_CTRL0D;
    volatile uint32_t SFT_CTRL0E;
    volatile uint32_t SFT_CTRL0F;
    volatile uint32_t TIM_CTRL0;    //x040
    volatile uint32_t TIM_CTRL1;
    volatile uint32_t TIM_CTRL2;
    volatile uint32_t TIM_CTRL3;
    volatile uint32_t TIM_CTRL4;    //x050
    volatile uint32_t LVD33;
    volatile uint32_t TRIM0;
    volatile uint32_t TRIM1;
    volatile uint32_t TRIM2;       //0x60
    volatile uint32_t WKUP_CTRL0;
    volatile uint32_t TRIM3;
    volatile uint32_t DCDC0;
    volatile uint32_t DCDC1;       //0x70
    volatile uint32_t DCDC2;
    volatile uint32_t DCDC3;
    volatile uint32_t DCDC4;
    volatile uint32_t WDT0;       //0x80
    volatile uint32_t WDT1;
    volatile uint32_t WDT2;
    volatile uint32_t WDT3;
    volatile uint32_t WDT4;      //0x90
    volatile uint32_t RESERVED0[3];
    volatile uint32_t RTC_CTRL;      //0xa0
    volatile uint32_t RTC_1HZ_H;
    volatile uint32_t RTC_1HZ_L;
    volatile uint32_t RTC_CALIB;
    volatile uint32_t RTC_SEC;       //0XB0
    volatile uint32_t RTC_MIN;
    volatile uint32_t RTC_HOUR;
    volatile uint32_t RTC_DAY;
    volatile uint32_t RTC_WEEK;     //0XC0
    volatile uint32_t RTC_MON;
    volatile uint32_t RTC_YEAR;
    volatile uint32_t RESERVED1[5];
    volatile uint32_t RST_SFT;    //0XE0
    volatile uint32_t LKRST;
    volatile uint32_t RST_SRC_CLR;
    volatile uint32_t WKUP_TIM0;
    volatile uint32_t WKUP_TIM1;    //0XF0
    volatile uint32_t PMU_SET_VAL;
    volatile uint32_t PWR_CTRL0;
    volatile uint32_t PWR_CTRL1;
    volatile uint32_t GPIOA_INTR_EN_L;    //0X100
    volatile uint32_t GPIOA_INTR_EN_H;
    volatile uint32_t GPIOB_INTR_EN_L;
    volatile uint32_t GPIOB_INTR_EN_H;
    volatile uint32_t GPIOC_INTR_EN_L;    //0X110
    volatile uint32_t GPIOC_INTR_EN_H;
    volatile uint32_t GPIOA_INTR_POL_L;
    volatile uint32_t GPIOA_INTR_POL_H;
    volatile uint32_t GPIOB_INTR_POL_L;   //0X120
    volatile uint32_t GPIOB_INTR_POL_H;
    volatile uint32_t GPIOC_INTR_POL_L;
    volatile uint32_t GPIOC_INTR_POL_H;
    volatile uint32_t GPIOA_INTR_CLR_L;   //0X130
    volatile uint32_t GPIOA_INTR_CLR_H;
    volatile uint32_t GPIOB_INTR_CLR_L;
    volatile uint32_t GPIOB_INTR_CLR_H;
    volatile uint32_t GPIOC_INTR_CLR_L;   //0X140
    volatile uint32_t GPIOC_INTR_CLR_H;
    volatile uint32_t SWD_IO_WKUP_EN;
    volatile uint32_t PMU_SET_TGGL;  //0X14C
    volatile uint32_t RESERVED2[12];
    volatile uint32_t WDT_CNT0;  //0X180
    volatile uint32_t WDT_CNT1;
    volatile uint32_t WDT_CNT2;
    volatile uint32_t WDT_CNT3;
    volatile uint32_t WDT_INTR;   //0X190
    volatile uint32_t RESERVED3[3];
    volatile uint32_t RTC_SEC_VAL;     //0X1A0
    volatile uint32_t RTC_MIN_VAL;
    volatile uint32_t RTC_HOUR_VAL;
    volatile uint32_t RTC_DAY_VAL;
    volatile uint32_t RTC_WEEK_VAL;    //0X1B0
    volatile uint32_t RTC_MON_VAL;
    volatile uint32_t RTC_YEAR_VAL;
    volatile uint32_t RTC_INTR;
    volatile uint32_t RST_SRC;      //0X1C0
    volatile uint32_t TIM_CNT0;
    volatile uint32_t TIM_CNT1;
    volatile uint32_t TIM_CNT2;
    volatile uint32_t TIM_CNT3;     //0X1D0
    volatile uint32_t TIM_INTR;
    volatile uint32_t RESERVED4[6];
    volatile uint32_t GPIOA_INTR_L;  //0X1F0
    volatile uint32_t GPIOA_INTR_H;
    volatile uint32_t GPIOB_INTR_L;
    volatile uint32_t GPIOB_INTR_H;
    volatile uint32_t GPIOC_INTR_L;   //0X200
    volatile uint32_t GPIOC_INTR_H;
    volatile uint32_t EXT_INTR;
    volatile uint32_t GPIOA_INTR_RAW_L;
    volatile uint32_t GPIOA_INTR_RAW_H;  //0X210
    volatile uint32_t GPIOB_INTR_RAW_L;
    volatile uint32_t GPIOB_INTR_RAW_H;
    volatile uint32_t GPIOC_INTR_RAW_L;
    volatile uint32_t GPIOC_INTR_RAW_H;   //0X220
    volatile uint32_t PMU_CTRL;    //0X224
}reg_v33_rg_t;

enum V33_RG_REG_SFT_CTRL00_FIELD
{
    V33_RG_SFT_CTRL00_MASK = (int)0xff,
    V33_RG_SFT_CTRL00_POS = 0,
};

enum V33_RG_REG_SFT_CTRL01_FIELD
{
    V33_RG_SFT_CTRL01_MASK = (int)0xff,
    V33_RG_SFT_CTRL01_POS = 0,
};

enum V33_RG_REG_SFT_CTRL02_FIELD
{
    V33_RG_SFT_CTRL02_MASK = (int)0xff,
    V33_RG_SFT_CTRL02_POS = 0,
};

enum V33_RG_REG_SFT_CTRL03_FIELD
{
    V33_RG_SFT_CTRL03_MASK = (int)0xff,
    V33_RG_SFT_CTRL03_POS = 0,
};

enum V33_RG_REG_SFT_CTRL04_FIELD
{
    V33_RG_SFT_CTRL04_MASK = (int)0xff,
    V33_RG_SFT_CTRL04_POS = 0,
};

enum V33_RG_REG_SFT_CTRL05_FIELD
{
    V33_RG_SFT_CTRL05_MASK = (int)0xff,
    V33_RG_SFT_CTRL05_POS = 0,
};

enum V33_RG_REG_SFT_CTRL06_FIELD
{
    V33_RG_SFT_CTRL06_MASK = (int)0xff,
    V33_RG_SFT_CTRL06_POS = 0,
};

enum V33_RG_REG_SFT_CTRL07_FIELD
{
    V33_RG_SFT_CTRL07_MASK = (int)0xff,
    V33_RG_SFT_CTRL07_POS = 0,
};

enum V33_RG_REG_SFT_CTRL08_FIELD
{
    V33_RG_SFT_CTRL08_MASK = (int)0xff,
    V33_RG_SFT_CTRL08_POS = 0,
};

enum V33_RG_REG_SFT_CTRL09_FIELD
{
    V33_RG_SFT_CTRL09_MASK = (int)0xff,
    V33_RG_SFT_CTRL09_POS = 0,
};

enum V33_RG_REG_SFT_CTRL0A_FIELD
{
    V33_RG_SFT_CTRL0A_MASK = (int)0xff,
    V33_RG_SFT_CTRL0A_POS = 0,
};

enum V33_RG_REG_SFT_CTRL0B_FIELD
{
    V33_RG_SFT_CTRL0B_MASK = (int)0xff,
    V33_RG_SFT_CTRL0B_POS = 0,
};

enum V33_RG_REG_SFT_CTRL0C_FIELD
{
    V33_RG_SFT_CTRL0C_MASK = (int)0xff,
    V33_RG_SFT_CTRL0C_POS = 0,
};

enum V33_RG_REG_SFT_CTRL0D_FIELD
{
    V33_RG_SFT_CTRL0D_MASK = (int)0xff,
    V33_RG_SFT_CTRL0D_POS = 0,
};

enum V33_RG_REG_SFT_CTRL0E_FIELD
{
    V33_RG_SFT_CTRL0E_MASK = (int)0xff,
    V33_RG_SFT_CTRL0E_POS = 0,
};

enum V33_RG_REG_SFT_CTRL0F_FIELD
{
    V33_RG_SFT_CTRL0F_MASK = (int)0xff,
    V33_RG_SFT_CTRL0F_POS = 0,
};

enum V33_RG_REG_TIM_CTRL0_FIELD
{
    V33_RG_TIM_TGT0_MASK = (int)0xff,
    V33_RG_TIM_TGT0_POS = 0,
};

enum V33_RG_REG_TIM_CTRL1_FIELD
{
    V33_RG_TIM_TGT1_MASK = (int)0xff,
    V33_RG_TIM_TGT1_POS = 0,
};

enum V33_RG_REG_TIM_CTRL2_FIELD
{
    V33_RG_TIM_TGT2_MASK = (int)0xff,
    V33_RG_TIM_TGT2_POS = 0,
};

enum V33_RG_REG_TIM_CTRL3_FIELD
{
    V33_RG_TIM_TGT3_MASK = (int)0xff,
    V33_RG_TIM_TGT3_POS = 0,
};

enum V33_RG_REG_TIM_CTRL4_FIELD
{
    V33_RG_TIM_EN_MASK = (int)0x1,
    V33_RG_TIM_EN_POS = 0,
    V33_RG_TIM_INTR_EN_MASK = (int)0x2,
    V33_RG_TIM_INTR_EN_POS = 1,
    V33_RG_TIM_INTR_CLR_MASK = (int)0x4,
    V33_RG_TIM_INTR_CLR_POS = 2,
    V33_RG_TIM_CLK_SEL_MASK = (int)0x30,
    V33_RG_TIM_CLK_SEL_POS = 4,
};

enum V33_RG_REG_LVD33_FIELD
{
    V33_RG_LVD33_INTR_EN_MASK = (int)0x1,
    V33_RG_LVD33_INTR_EN_POS = 0,
    V33_RG_LVD33_POL_MASK = (int)0x2,
    V33_RG_LVD33_POL_POS = 1,
    V33_RG_LVD33_INTR_CLR_MASK = (int)0x4,
    V33_RG_LVD33_INTR_CLR_POS = 2,
};

enum V33_RG_REG_TRIM0_FIELD
{
    V33_RG_SPI_CODE_L_MASK = (int)0xff,
    V33_RG_SPI_CODE_L_POS = 0,
};

enum V33_RG_REG_TRIM1_FIELD
{
    V33_RG_SPI_CODE_H_MASK = (int)0xf,
    V33_RG_SPI_CODE_H_POS = 0,
    V33_RG_RCO_I_ADJ_MASK = (int)0x10,
    V33_RG_RCO_I_ADJ_POS = 4,
};

enum V33_RG_REG_TRIM2_FIELD
{
    V33_RG_PD_AMIC_MASK = (int)0x2,
    V33_RG_PD_AMIC_POS = 1,
    V33_RG_latch_gpio_MASK = (int)0x4,
    V33_RG_latch_gpio_POS = 2,
    V33_RG_PD_ADC12_MASK = (int)0x8,
    V33_RG_PD_ADC12_POS = 3,
    V33_RG_PD_GPIO_MASK = (int)0x10,
    V33_RG_PD_GPIO_POS = 4,
    V33_RG_BOR_EN_MASK = (int)0x20,
    V33_RG_BOR_EN_POS = 5,
    V33_RG_BAT_DTCT_EN_MASK = (int)0x40,
    V33_RG_BAT_DTCT_EN_POS = 6,
    V33_RG_PD_BIM_MASK = (int)0x80,
    V33_RG_PD_BIM_POS = 7,
};

enum V33_RG_REG_WKUP_CTRL0_FIELD
{
    V33_RG_WKUP_MSK_MASK = (int)0x1f,
    V33_RG_WKUP_MSK_POS = 0,
};

enum V33_RG_REG_TRIM3_FIELD
{
    V33_RG_PD_GPIO_SEL_MASK = (int)0x1,
    V33_RG_PD_GPIO_SEL_POS = 0,
    V33_RG_PD_ADC24_MASK = (int)0x2,
    V33_RG_PD_ADC24_POS = 1,
    V33_RG_LPLDO_TRIM_MASK = (int)0x3c,
    V33_RG_LPLDO_TRIM_POS = 2,
    V33_RG_PD_TK_MASK = (int)0x40,
    V33_RG_PD_TK_POS = 6,
};

enum V33_RG_REG_DCDC0_FIELD
{
    V33_RG_DCDC_TEST_MASK = (int)0x1,
    V33_RG_DCDC_TEST_POS = 0,
    V33_RG_DCDC_DELAY_MASK = (int)0x2,
    V33_RG_DCDC_DELAY_POS = 1,
    V33_RG_DCDC_EN_OVC_MASK = (int)0x4,
    V33_RG_DCDC_EN_OVC_POS = 2,
    V33_RG_DCDC_EN_ZOC_MASK = (int)0x8,
    V33_RG_DCDC_EN_ZOC_POS = 3,
    V33_RG_DCDC_RAMP_I_HALF_MASK = (int)0x10,
    V33_RG_DCDC_RAMP_I_HALF_POS = 4,
    V33_RG_DCDC_OUT_ADJ_MASK = (int)0xe0,
    V33_RG_DCDC_OUT_ADJ_POS = 5,
};

enum V33_RG_REG_DCDC1_FIELD
{
    V33_RG_DCDC_PSW_ADJ_MASK = (int)0x3,
    V33_RG_DCDC_PSW_ADJ_POS = 0,
    V33_RG_DCDC_NSW_ADJ_MASK = (int)0xc,
    V33_RG_DCDC_NSW_ADJ_POS = 2,
    V33_RG_DCDC_VBG_VCTL_MASK = (int)0xf0,
    V33_RG_DCDC_VBG_VCTL_POS = 4,
};

enum V33_RG_REG_DCDC2_FIELD
{
    V33_RG_DCDC_VCTL_MASK = (int)0xf,
    V33_RG_DCDC_VCTL_POS = 0,
    V33_RG_DCDC_OVC_CTL_MASK = (int)0x30,
    V33_RG_DCDC_OVC_CTL_POS = 4,
};

enum V33_RG_REG_DCDC3_FIELD
{
    V33_RG_DCDC_PWM_FI_MASK = (int)0x7,
    V33_RG_DCDC_PWM_FI_POS = 0,
    V33_RG_DCDC_PWM_FC_MASK = (int)0x38,
    V33_RG_DCDC_PWM_FC_POS = 3,
};

enum V33_RG_REG_DCDC4_FIELD
{
    V33_RG_DCDC_PWM_HYS_CTL_MASK = (int)0x7,
    V33_RG_DCDC_PWM_HYS_CTL_POS = 0,
    V33_RG_DCDC_PFM_HYS_CTL_MASK = (int)0x38,
    V33_RG_DCDC_PFM_HYS_CTL_POS = 3,
};

enum V33_RG_REG_WDT0_FIELD
{
    V33_RG_WDT_LOAD0_MASK = (int)0xff,
    V33_RG_WDT_LOAD0_POS = 0,
};

enum V33_RG_REG_WDT1_FIELD
{
    V33_RG_WDT_LOAD1_MASK = (int)0xff,
    V33_RG_WDT_LOAD1_POS = 0,
};

enum V33_RG_REG_WDT2_FIELD
{
    V33_RG_WDT_LOAD2_MASK = (int)0xff,
    V33_RG_WDT_LOAD2_POS = 0,
};

enum V33_RG_REG_WDT3_FIELD
{
    V33_RG_WDT_LOAD3_MASK = (int)0xff,
    V33_RG_WDT_LOAD3_POS = 0,
};

enum V33_RG_REG_WDT4_FIELD
{
    V33_RG_WDT_EN_MASK = (int)0x1,
    V33_RG_WDT_EN_POS = 0,
    V33_RG_WDT_RST_EN_MASK = (int)0x2,
    V33_RG_WDT_RST_EN_POS = 1,
    V33_RG_WDT_INTR_CLR_MASK = (int)0x4,
    V33_RG_WDT_INTR_CLR_POS = 2,
    V33_RG_WDT_CLK_SEL_MASK = (int)0x30,
    V33_RG_WDT_CLK_SEL_POS = 4,
};

enum V33_RG_REG_RTC0_FIELD
{
    V33_RG_RTC_EN_MASK = (int)0x1,
    V33_RG_RTC_EN_POS = 0,
    V33_RG_CALIB_EN_MASK = (int)0x2,
    V33_RG_CALIB_EN_POS = 1,
    V33_RG_RTC_ALARM_EN_MASK = (int)0x4,
    V33_RG_RTC_ALARM_EN_POS = 2,
    V33_RG_RTC_SET_TGGL_MASK = (int)0x8,
    V33_RG_RTC_SET_TGGL_POS = 3,
    V33_RG_RTC_INTR_CLR_MASK = (int)0x10,
    V33_RG_RTC_INTR_CLR_POS = 4,
    V33_RG_RTC_CLK_SEL_MASK = (int)0xc0,
    V33_RG_RTC_CLK_SEL_POS = 6,
};

enum V33_RG_REG_RTC1_FIELD
{
    V33_RG_CYC_1HZ_M1_H_MASK = (int)0xff,
    V33_RG_CYC_1HZ_M1_H_POS = 0,
};

enum V33_RG_REG_RTC2_FIELD
{
    V33_RG_CYC_1HZ_M1_L_MASK = (int)0xff,
    V33_RG_CYC_1HZ_M1_L_POS = 0,
};

enum V33_RG_REG_RTC3_FIELD
{
    V33_RG_CALIB_CYC_MASK = (int)0x3f,
    V33_RG_CALIB_CYC_POS = 0,
};

enum V33_RG_REG_RTC4_FIELD
{
    V33_RG_RTC_SET_SEC_MASK = (int)0x3f,
    V33_RG_RTC_SET_SEC_POS = 0,
};

enum V33_RG_REG_RTC5_FIELD
{
    V33_RG_RTC_SET_MIN_MASK = (int)0x3f,
    V33_RG_RTC_SET_MIN_POS = 0,
};

enum V33_RG_REG_RTC6_FIELD
{
    V33_RG_RTC_SET_HOUR_MASK = (int)0x1f,
    V33_RG_RTC_SET_HOUR_POS = 0,
};

enum V33_RG_REG_RTC7_FIELD
{
    V33_RG_RTC_SET_DAY_MASK = (int)0x1f,
    V33_RG_RTC_SET_DAY_POS = 0,
};

enum V33_RG_REG_RTC8_FIELD
{
    V33_RG_RTC_SET_WEEK_MASK = (int)0x7,
    V33_RG_RTC_SET_WEEK_POS = 0,
};

enum V33_RG_REG_RTC9_FIELD
{
    V33_RG_RTC_SET_MON_MASK = (int)0xf,
    V33_RG_RTC_SET_MON_POS = 0,
};

enum V33_RG_REG_RTC10_FIELD
{
    V33_RG_RTC_SET_YEAR_MASK = (int)0xff,
    V33_RG_RTC_SET_YEAR_POS = 0,
};

enum V33_RG_REG_RST_SFT_FIELD
{
    V33_RG_RST_FROM_SFT_MASK = (int)0x1,
    V33_RG_RST_FROM_SFT_POS = 0,
};

enum V33_RG_REG_LKRST_FIELD
{
    V33_RG_LKRST_TIM_MASK = (int)0x3f,
    V33_RG_LKRST_TIM_POS = 0,
    V33_RG_LKRST_EN_MASK = (int)0x40,
    V33_RG_LKRST_EN_POS = 6,
    V33_RG_LKRST_POL_MASK = (int)0x80,
    V33_RG_LKRST_POL_POS = 7,
};

enum V33_RG_REG_RST_SRC_CLR_FIELD
{
    V33_RG_RST_SRC_CLR_MASK = (int)0x1,
    V33_RG_RST_SRC_CLR_POS = 0,
};

enum V33_RG_REG_WKUP_TIM0_FIELD
{
    V33_RG_STB_CLK_M1_MASK = (int)0xff,
    V33_RG_STB_CLK_M1_POS = 0,
};

enum V33_RG_REG_WKUP_TIM1_FIELD
{
    V33_RG_STB_DCDC_M1_MASK = (int)0xf,
    V33_RG_STB_DCDC_M1_POS = 0,
    V33_RG_STB_LDO_M1_MASK = (int)0xf0,
    V33_RG_STB_LDO_M1_POS = 4,
};

enum V33_RG_REG_PMU_SET_VAL_FIELD
{
    V33_RG_CLK_SET_LSE_MASK = (int)0x1,
    V33_RG_CLK_SET_LSE_POS = 0,
    V33_RG_CLK_SET_HSI_MASK = (int)0x2,
    V33_RG_CLK_SET_HSI_POS = 1,
    V33_RG_CLK_SET_HSE_MASK = (int)0x4,
    V33_RG_CLK_SET_HSE_POS = 2,
    V33_RG_CLK_SET_HSE_BUF_MASK = (int)0x8,
    V33_RG_CLK_SET_HSE_BUF_POS = 3,
    V33_RG_PMU_SET_DCDC_BYP_MASK = (int)0x10,
    V33_RG_PMU_SET_DCDC_BYP_POS = 4,
    V33_RG_PMU_SET_DCDC_PWM_MASK = (int)0x20,
    V33_RG_PMU_SET_DCDC_PWM_POS = 5,
    V33_RG_PMU_SET_DCDC_PFM_MASK = (int)0x40,
    V33_RG_PMU_SET_DCDC_PFM_POS = 6,
    V33_RG_PMU_SET_DCDC_FLT_MASK = (int)0x80,
    V33_RG_PMU_SET_DCDC_FLT_POS = 7,
};

enum V33_RG_REG_PWR_CTRL0_FIELD
{
    V33_RG_LPLDO_PD_EN_MASK = (int)0x1,
    V33_RG_LPLDO_PD_EN_POS = 0,
    V33_RG_HPLDO_PD_EN_MASK = (int)0x2,
    V33_RG_HPLDO_PD_EN_POS = 1,
    V33_RG_HSE_BUF_PD_EN_MASK = (int)0x4,
    V33_RG_HSE_BUF_PD_EN_POS = 2,
    V33_RG_HSE_BUF_PU_EN_MASK = (int)0x8,
    V33_RG_HSE_BUF_PU_EN_POS = 3,
    V33_RG_HS_GATE_PD_EN_MASK = (int)0x10,
    V33_RG_HS_GATE_PD_EN_POS = 4,
    V33_RG_HS_GATE_PU_EN_MASK = (int)0x20,
    V33_RG_HS_GATE_PU_EN_POS = 5,
    V33_RG_DCDC_PDPU_EN_MASK = (int)0x40,
    V33_RG_DCDC_PDPU_EN_POS = 6,
    V33_RG_DCDC_PDPU_MD_MASK = (int)0x80,
    V33_RG_DCDC_PDPU_MD_POS = 7,
};

enum V33_RG_REG_PWR_CTRL1_FIELD
{
    V33_RG_LSI_PD_EN_MASK = (int)0x1,
    V33_RG_LSI_PD_EN_POS = 0,
    V33_RG_LSE_PD_EN_MASK = (int)0x4,
    V33_RG_LSE_PD_EN_POS = 2,
    V33_RG_LSE_PU_EN_MASK = (int)0x8,
    V33_RG_LSE_PU_EN_POS = 3,
    V33_RG_HSI_PD_EN_MASK = (int)0x10,
    V33_RG_HSI_PD_EN_POS = 4,
    V33_RG_HSI_PU_EN_MASK = (int)0x20,
    V33_RG_HSI_PU_EN_POS = 5,
    V33_RG_HSE_PD_EN_MASK = (int)0x40,
    V33_RG_HSE_PD_EN_POS = 6,
    V33_RG_HSE_PU_EN_MASK = (int)0x80,
    V33_RG_HSE_PU_EN_POS = 7,
};

enum V33_RG_REG_EXTI_CTRL0_FIELD
{
    V33_RG_GPIOA_INTR_EN_L_MASK = (int)0xff,
    V33_RG_GPIOA_INTR_EN_L_POS = 0,
};

enum V33_RG_REG_EXTI_CTRL1_FIELD
{
    V33_RG_GPIOA_INTR_EN_H_MASK = (int)0xff,
    V33_RG_GPIOA_INTR_EN_H_POS = 0,
};

enum V33_RG_REG_EXTI_CTRL2_FIELD
{
    V33_RG_GPIOB_INTR_EN_L_MASK = (int)0xff,
    V33_RG_GPIOB_INTR_EN_L_POS = 0,
};

enum V33_RG_REG_EXTI_CTRL3_FIELD
{
    V33_RG_GPIOB_INTR_EN_H_MASK = (int)0xff,
    V33_RG_GPIOB_INTR_EN_H_POS = 0,
};

enum V33_RG_REG_EXTI_CTRL4_FIELD
{
    V33_RG_GPIOC_INTR_EN_L_MASK = (int)0xff,
    V33_RG_GPIOC_INTR_EN_L_POS = 0,
};

enum V33_RG_REG_EXTI_CTRL5_FIELD
{
    V33_RG_GPIOC_INTR_EN_H_MASK = (int)0x7,
    V33_RG_GPIOC_INTR_EN_H_POS = 0,
};

enum V33_RG_REG_EXTI_CTRL6_FIELD
{
    V33_RG_GPIOA_INTR_POL_L_MASK = (int)0xff,
    V33_RG_GPIOA_INTR_POL_L_POS = 0,
};

enum V33_RG_REG_EXTI_CTRL7_FIELD
{
    V33_RG_GPIOA_INTR_POL_H_MASK = (int)0xff,
    V33_RG_GPIOA_INTR_POL_H_POS = 0,
};

enum V33_RG_REG_EXTI_CTRL8_FIELD
{
    V33_RG_GPIOB_INTR_POL_L_MASK = (int)0xff,
    V33_RG_GPIOB_INTR_POL_L_POS = 0,
};

enum V33_RG_REG_EXTI_CTRL9_FIELD
{
    V33_RG_GPIOB_INTR_POL_H_MASK = (int)0xff,
    V33_RG_GPIOB_INTR_POL_H_POS = 0,
};

enum V33_RG_REG_EXTI_CTRL10_FIELD
{
    V33_RG_GPIOC_INTR_POL_L_MASK = (int)0xff,
    V33_RG_GPIOC_INTR_POL_L_POS = 0,
};

enum V33_RG_REG_EXTI_CTRL11_FIELD
{
    V33_RG_GPIOC_INTR_POL_H_MASK = (int)0x7,
    V33_RG_GPIOC_INTR_POL_H_POS = 0,
};

enum V33_RG_REG_EXTI_CTRL12_FIELD
{
    V33_RG_GPIOA_INTR_CLR_L_MASK = (int)0xff,
    V33_RG_GPIOA_INTR_CLR_L_POS = 0,
};

enum V33_RG_REG_EXTI_CTRL13_FIELD
{
    V33_RG_GPIOA_INTR_CLR_H_MASK = (int)0xff,
    V33_RG_GPIOA_INTR_CLR_H_POS = 0,
};

enum V33_RG_REG_EXTI_CTRL14_FIELD
{
    V33_RG_GPIOB_INTR_CLR_L_MASK = (int)0xff,
    V33_RG_GPIOB_INTR_CLR_L_POS = 0,
};

enum V33_RG_REG_EXTI_CTRL15_FIELD
{
    V33_RG_GPIOB_INTR_CLR_H_MASK = (int)0xff,
    V33_RG_GPIOB_INTR_CLR_H_POS = 0,
};

enum V33_RG_REG_EXTI_CTRL16_FIELD
{
    V33_RG_GPIOC_INTR_CLR_L_MASK = (int)0xff,
    V33_RG_GPIOC_INTR_CLR_L_POS = 0,
};

enum V33_RG_REG_EXTI_CTRL17_FIELD
{
    V33_RG_GPIOC_INTR_CLR_H_MASK = (int)0x7,
    V33_RG_GPIOC_INTR_CLR_H_POS = 0,
};

enum V33_RG_REG_EXTI_INTR_FIELD
{
    V33_RG_EXT_INTR_MASK = (int)0x1,
    V33_RG_EXT_INTR_POS = 0,
};

enum V33_RG_REG_WKUP_CTRL1_FIELD
{
    V33_RG_SWD_IO_WKUP_EN_MASK = (int)0x3,
    V33_RG_SWD_IO_WKUP_EN_POS = 0,
};

enum V33_RG_REG_PMU_SET_TGGL_FIELD
{
    V33_RG_PMU_SET_TGGL_MASK = (int)0x1,
    V33_RG_PMU_SET_TGGL_POS = 0,
};

enum V33_RG_REG_RTC_CNT0_FIELD
{
    V33_RG_RTC_CNT_SEC_MASK = (int)0x3f,
    V33_RG_RTC_CNT_SEC_POS = 0,
};

enum V33_RG_REG_RTC_CNT1_FIELD
{
    V33_RG_RTC_CNT_MIN_MASK = (int)0x3f,
    V33_RG_RTC_CNT_MIN_POS = 0,
};

enum V33_RG_REG_RTC_CNT2_FIELD
{
    V33_RG_RTC_CNT_HOUR_MASK = (int)0x1f,
    V33_RG_RTC_CNT_HOUR_POS = 0,
};

enum V33_RG_REG_RTC_CNT3_FIELD
{
    V33_RG_RTC_CNT_DAY_MASK = (int)0x1f,
    V33_RG_RTC_CNT_DAY_POS = 0,
};

enum V33_RG_REG_RTC_CNT4_FIELD
{
    V33_RG_RTC_CNT_WEEK_MASK = (int)0x7,
    V33_RG_RTC_CNT_WEEK_POS = 0,
};

enum V33_RG_REG_RTC_CNT5_FIELD
{
    V33_RG_RTC_CNT_MON_MASK = (int)0xf,
    V33_RG_RTC_CNT_MON_POS = 0,
};

enum V33_RG_REG_RTC_CNT6_FIELD
{
    V33_RG_RTC_CNT_YEAR_MASK = (int)0xff,
    V33_RG_RTC_CNT_YEAR_POS = 0,
};

enum V33_RG_REG_PMU_CTRL_FIELD
{
    V33_RG_PMU_CTRL_STAT_MASK = (int)0xff,
    V33_RG_PMU_CTRL_STAT_POS = 0,
};

#endif

