#include "io_config.h"
#include "field_manipulate.h"
#include "per_func_mux.h"
#include "gemini.h"
#include "platform.h"
#include "reg_v33_rg.h"
#include "reg_sysc_awo.h"
gpio_pin_t uart1_txd;
gpio_pin_t uart1_rxd;
gpio_pin_t iic1_scl;
gpio_pin_t iic1_sda;

__attribute__((weak)) void io_exti_callback(uint8_t pin){}



void EXTI_Handler(void)
{

}

void io_init(void)
{
    //TODO
    arm_cm_set_int_isr(EXT_IRQn,EXTI_Handler);
    __NVIC_EnableIRQ(EXT_IRQn);
}

void io_cfg_output(uint8_t pin)
{
    
}

void io_cfg_input(uint8_t pin)
{

}

void io_write_pin(uint8_t pin,uint8_t val)
{
    if(val)
    {
        io_set_pin(pin);
    }else
    {
        io_clr_pin(pin);
    }
}

void io_set_pin(uint8_t pin)
{

}

void io_clr_pin(uint8_t pin)
{

}

void io_toggle_pin(uint8_t pin)
{

}

uint8_t io_get_output_val(uint8_t pin)
{
    return 0;
}

uint8_t io_read_pin(uint8_t pin)
{
    return 0;
}

void io_pull_write(uint8_t pin,io_pull_type_t pull)
{
    switch(pull)
    {
    case IO_PULL_DISABLE:

    break;
    case IO_PULL_UP:

    break;
    case IO_PULL_DOWN:

    break;
    }
}

io_pull_type_t io_pull_read(uint8_t pin)
{
    return 0;
}

void io_exti_config(uint8_t pin,exti_edge_t edge)
{

}

void io_exti_enable(uint8_t pin,bool enable)
{

}

static void uart_io_cfg(uint8_t txd,uint8_t rxd)
{
    io_set_pin(txd);
    io_cfg_output(txd);
    io_cfg_input(rxd);
}

static uint8_t pin2func_io(gpio_pin_t *x)
{
    return x->port*16+x->num;
}

static void per_func_enable(uint8_t func_io_num,uint8_t per_func)
{

}

static void per_func_disable(uint8_t func_io_num)
{

}

static void iic_io_cfg(uint8_t scl,uint8_t sda)
{
    io_cfg_input(scl);
    io_cfg_input(sda);
}

void iic1_io_init(uint8_t scl,uint8_t sda)
{
    *(uint8_t *)&iic1_scl = scl;
    *(uint8_t *)&iic1_sda = sda;
    iic_io_cfg(scl,sda);
    per_func_enable(pin2func_io((gpio_pin_t *)&scl), IIC1_SCL);
    per_func_enable(pin2func_io((gpio_pin_t *)&sda), IIC1_SDA);
}

void iic1_io_deinit(void)
{
    per_func_disable(pin2func_io((gpio_pin_t *)&iic1_scl));
    per_func_disable(pin2func_io((gpio_pin_t *)&iic1_sda));
}

void uart1_io_init(uint8_t txd,uint8_t rxd)
{
    *(uint8_t *)&uart1_txd = txd;
    *(uint8_t *)&uart1_rxd = rxd;
    uart_io_cfg(txd,rxd);
    per_func_enable(pin2func_io((gpio_pin_t *)&txd),UART1_TXD);
    per_func_enable(pin2func_io((gpio_pin_t *)&rxd),UART1_RXD);
}

void uart1_io_deinit(void)
{
    per_func_disable(pin2func_io((gpio_pin_t *)&uart1_txd));
    per_func_disable(pin2func_io((gpio_pin_t *)&uart1_rxd));
}

void qspi_flash_io_init(void)
{

}

void qspi_flash_io_deinit(void)
{

}

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
