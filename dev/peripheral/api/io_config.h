#ifndef IO_CONFIG_H_
#define IO_CONFIG_H_
#include <stdint.h>
#include <stdbool.h>

/** \addtogroup PERIPHERAL
 *  @{
 */

/** \addtogroup GPIO
 *  @{
 */
 
/**@brief IO pull type */
typedef enum 
{
    IO_PULL_DISABLE,
    IO_PULL_UP,
    IO_PULL_DOWN,
}io_pull_type_t;

typedef enum
{
    INT_EDGE_FALLING,
    INT_EDGE_RISING,
}exti_edge_t;

typedef struct
{
    uint8_t num:4,
            port:4;
}gpio_pin_t;

//lsgpioa
#define PA00 ((uint8_t)0x00)   /* Pin A0 selected    */
#define PA01 ((uint8_t)0x01)   /* Pin A1 selected    */
#define PA02 ((uint8_t)0x02)   /* Pin A2 selected    */
#define PA03 ((uint8_t)0x03)   /* Pin A3 selected    */
#define PA04 ((uint8_t)0x04)   /* Pin A4 selected    */
#define PA05 ((uint8_t)0x05)   /* Pin A5 selected    */
#define PA06 ((uint8_t)0x06)   /* Pin A6 selected    */
#define PA07 ((uint8_t)0x07)   /* Pin A7 selected    */
#define PA08 ((uint8_t)0x08)   /* Pin A8 selected    */
#define PA09 ((uint8_t)0x09)   /* Pin A9 selected    */
#define PA10 ((uint8_t)0x0A)  /* Pin A10 selected   */
#define PA11 ((uint8_t)0x0B)  /* Pin A11 selected   */
#define PA12 ((uint8_t)0x0C)  /* Pin A12 selected   */
#define PA13 ((uint8_t)0x0D)  /* Pin A13 selected   */
#define PA14 ((uint8_t)0x0E)  /* Pin A14 selected   */
#define PA15 ((uint8_t)0x0F)  /* Pin A15 selected   */
//lsgpiob
#define PB00 ((uint8_t)0X10)   /* Pin B0 selected    */
#define PB01 ((uint8_t)0X11)   /* Pin B1 selected    */
#define PB02 ((uint8_t)0X12)   /* Pin B2 selected    */
#define PB03 ((uint8_t)0X13)   /* Pin B3 selected    */
#define PB04 ((uint8_t)0X14)   /* Pin B4 selected    */
#define PB05 ((uint8_t)0X15)   /* Pin B5 selected    */
#define PB06 ((uint8_t)0X16)   /* Pin B6 selected    */
#define PB07 ((uint8_t)0X17)   /* Pin B7 selected    */
#define PB08 ((uint8_t)0X18)   /* Pin B8 selected    */
#define PB09 ((uint8_t)0X19)   /* Pin B9 selected    */
#define PB10 ((uint8_t)0X1A)   /* Pin B10 selected   */
#define PB11 ((uint8_t)0X1B)   /* Pin B11 selected   */
#define PB12 ((uint8_t)0X1C)   /* Pin B12 selected   */
#define PB13 ((uint8_t)0X1D)   /* Pin B13 selected   */
#define PB14 ((uint8_t)0X1E)   /* Pin B14 selected   */
#define PB15 ((uint8_t)0X1F)   /* Pin B15 selected   */
//lsgpioc
#define PC00 ((uint8_t)0X20)   /* Pin C0 selected    */
#define PC01 ((uint8_t)0X21)   /* Pin C1 selected    */
#define PC02 ((uint8_t)0X22)   /* Pin C2 selected    */
#define PC03 ((uint8_t)0X23)   /* Pin C3 selected    */
#define PC04 ((uint8_t)0X24)   /* Pin C4 selected    */
#define PC05 ((uint8_t)0X25)   /* Pin C5 selected    */
#define PC06 ((uint8_t)0X26)   /* Pin C6 selected    */
#define PC07 ((uint8_t)0X27)   /* Pin C7 selected    */
#define PC08 ((uint8_t)0X28)   /* Pin C8 selected    */
#define PC09 ((uint8_t)0X29)   /* Pin C9 selected    */
#define PC10 ((uint8_t)0X2A)   /* Pin C10 selected   */
/**
  * @brief GPIO Init
  */
void io_init(void);

/**
  * @brief GPIO config output
  * @param pin Specific GPIO pin
  */
void io_cfg_output(uint8_t pin);

/**
  * @brief GPIO config open drain output mode
  * @param  pin  Specific GPIO pin
  */
void io_cfg_opendrain(uint8_t pin);

/**
  * @brief GPIO config pushpull output mode
  * @param  pin  Specific GPIO pin
  */
void io_cfg_pushpull(uint8_t pin);

/**
  * @brief GPIO config input
  * @param  pin  Specific GPIO pin
  */
void io_cfg_input(uint8_t pin);

/**
  * @brief GPIO input disable
  * @param  pin  Specific GPIO pin
  */
void io_cfg_disable(uint8_t pin);

/**
  * @brief GPIO config output
  * @param  pin  Specific GPIO pin
  * @param  val  GPIO level status
  *              0 means low level
  *              1 means high level
  */
void io_write_pin(uint8_t pin,uint8_t val);

/**
  * @brief set GPIO high level
  * @param  pin  Specific GPIO pin
  */
void io_set_pin(uint8_t pin);

/**
  * @brief set GPIO low level
  * @param  pin  Specific GPIO pin
  */
void io_clr_pin(uint8_t pin);

/**
  * @brief toggle GPIO 
  * @param  pin  Specific GPIO pin
  */
void io_toggle_pin(uint8_t pin);

/**
  * @brief get GPIO output level
  * @param  pin  Specific GPIO pin
  * @retval GPIO output level
  *              0 means low level
  *              1 means high level
  */
uint8_t io_get_output_val(uint8_t pin);

/**
  * @brief get GPIO input level
  * @param  pin  Specific GPIO pin
  * @retval GPIO output level
  *              0 means low level
  *              1 means high level
  */
uint8_t io_read_pin(uint8_t pin);

/**
  * @brief set GPIO pullup or pulldwon or nullpull
  * @param  pin  Specific GPIO pin
  * @param pull Configure the GPIO pull up and down     
  */
void io_pull_write(uint8_t pin,io_pull_type_t pull);

/**
  * @brief read GPIO pull states
  * @param  pin  Specific GPIO pin 
  * @retval GPIO pullup and pulldown state    
  */
io_pull_type_t io_pull_read(uint8_t pin);

/**
  * @brief GPIO external interrupt enable 
  * @param  pin  Specific GPIO pin
  */
void io_ext_intrp_enable(uint8_t pin);

/**
  * @brief GPIO external interrupt disable
  * @param  pin  Specific GPIO pin
  */
void io_ext_intrp_disable(uint8_t pin);

/**
  * @brief Sets the trigger edge for IO interrupt 
  * @param  pin  Specific GPIO pin 
  * @param  edge edge for IO interrupts 
  */
void io_exti_config(uint8_t pin,exti_edge_t edge);

/**
  * @brief GPIO external interrupt enable or disable
  * @param  pin  Specific GPIO pin
  * @param  enable
  */
void io_exti_enable(uint8_t pin,bool enable);

/**
  * @brief GPIO external interrupt callback
  * @param pin specific GPIO pin
  */
void io_exti_callback(uint8_t pin);


void qspi_flash_io_init(void);
void qspi_flash_io_deinit(void);

void ssi_clk_io_init(uint8_t clk);
void ssi_nss0_io_init(uint8_t nss0);
void ssi_nss1_io_init(uint8_t nss1);
void ssi_dq0_io_init(uint8_t dq0);
void ssi_dq1_io_init(uint8_t dq1);
void ssi_dq2_io_init(uint8_t dq2);
void ssi_dq3_io_init(uint8_t dq3);

void spi2_clk_io_init(uint8_t clk);
void spi2_nss_io_init(uint8_t nss);
void spi2_mosi_io_init(uint8_t mosi);
void spi2_miso_io_init(uint8_t miso);
void spi2_clk_io_deinit(void);
void spi2_nss_io_deinit(void);
void spi2_mosi_io_deinit(void);
void spi2_miso_io_deinit(void);

void iic1_io_init(uint8_t scl,uint8_t sda);
void iic1_io_deinit(void);
void iic2_io_init(uint8_t scl,uint8_t sda);
void iic2_io_deinit(void);

void uart1_io_init(uint8_t txd,uint8_t rxd);
void uart1_io_deinit(void);

void uart1_7816_io_init(uint8_t txd,uint8_t ck);
void uart1_7816_io_deinit(void);

void uart2_io_init(uint8_t txd,uint8_t rxd);
void uart2_io_deinit(void);

void uart3_io_init(uint8_t txd,uint8_t rxd);
void uart3_io_deinit(void);

void adtim1_ch1_io_init(uint8_t pin,bool output,uint8_t default_val);
void adtim1_ch1_io_deinit(void);
void adtim1_ch2_io_init(uint8_t pin,bool output,uint8_t default_val);
void adtim1_ch2_io_deinit(void);
void adtim1_ch3_io_init(uint8_t pin,bool output,uint8_t default_val);
void adtim1_ch3_io_deinit(void);
void adtim1_ch4_io_init(uint8_t pin,bool output,uint8_t default_val);
void adtim1_ch4_io_deinit(void);
void adtim1_ch1n_io_init(uint8_t pin);
void adtim1_ch1n_io_deinit(void);
void adtim1_ch2n_io_init(uint8_t pin);
void adtim1_ch2n_io_deinit(void);
void adtim1_ch3n_io_init(uint8_t pin);
void adtim1_ch3n_io_deinit(void);
void adtim1_etr_io_init(uint8_t pin);
void adtim1_etr_io_deinit(void);
void adtim1_bk_io_init(uint8_t pin);
void adtim1_bk_io_deinit(void);


void gptima1_ch1_io_init(uint8_t pin,bool output,uint8_t default_val);
void gptima1_ch1_io_deinit(void);
void gptima1_ch2_io_init(uint8_t pin,bool output,uint8_t default_val);
void gptima1_ch2_io_deinit(void);
void gptima1_ch3_io_init(uint8_t pin,bool output,uint8_t default_val);
void gptima1_ch3_io_deinit(void);
void gptima1_ch4_io_init(uint8_t pin,bool output,uint8_t default_val);
void gptima1_ch4_io_deinit(void);
void gptima1_etr_io_init(uint8_t pin);
void gptima1_etr_io_deinit(void);

void gptimb1_ch1_io_init(uint8_t pin,bool output,uint8_t default_val);
void gptimb1_ch1_io_deinit(void);
void gptimb1_ch2_io_init(uint8_t pin,bool output,uint8_t default_val);
void gptimb1_ch2_io_deinit(void);
void gptimb1_ch3_io_init(uint8_t pin,bool output,uint8_t default_val);
void gptimb1_ch3_io_deinit(void);
void gptimb1_ch4_io_init(uint8_t pin,bool output,uint8_t default_val);
void gptimb1_ch4_io_deinit(void);
void gptimb1_etr_io_init(uint8_t pin);
void gptimb1_etr_io_deinit(void);

void gptimc1_ch1_io_init(uint8_t pin,bool output,uint8_t default_val);
void gptimc1_ch1_io_deinit(void);
void gptimc1_ch1n_io_init(uint8_t pin);
void gptimc1_ch1n_io_deinit(void);
void gptimc1_ch2_io_init(uint8_t pin,bool output,uint8_t default_val);
void gptimc1_ch2_io_deinit(void);
void gptimc1_bk_io_init(uint8_t pin);
void gptimc1_bk_io_deinit(void);

void adc12b_in0_io_init(void);
void adc12b_in0_io_deinit(void);
void adc12b_in1_io_init(void);
void adc12b_in1_io_deinit(void);
void adc12b_in2_io_init(void);
void adc12b_in2_io_deinit(void);
void adc12b_in3_io_init(void);
void adc12b_in3_io_deinit(void);
void adc12b_in4_io_init(void);
void adc12b_in4_io_deinit(void);
void adc12b_in5_io_init(void);
void adc12b_in5_io_deinit(void);
void adc12b_in6_io_init(void);
void adc12b_in6_io_deinit(void);
void adc12b_in7_io_init(void);
void adc12b_in7_io_deinit(void);
void adc12b_in8_io_init(void);
void adc12b_in8_io_deinit(void);

void pdm_clk_io_init(uint8_t pin);
void pdm_clk_io_deinit(void);
void pdm_data0_io_init(uint8_t pin);
void pdm_data0_io_deinit(void);
void pdm_data1_io_init(uint8_t pin);
void pdm_data1_io_deinit(void);

/** @}*/


/** @}*/

#endif

