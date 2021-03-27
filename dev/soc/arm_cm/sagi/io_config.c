#include "io_config.h"
#include "reg_sysc_awo.h"
#include "field_manipulate.h"
#include "per_func_mux.h"
#include "reg_sysc_per.h"
#include "ARMCM3.h"
#include "platform.h"
#include "reg_v33_rg.h"
gpio_pin_t uart1_txd;
gpio_pin_t uart1_rxd;
gpio_pin_t iic1_scl;
gpio_pin_t iic1_sda;

__attribute__((weak)) void io_exti_callback(uint8_t pin){}

static void exti_io_handler(uint8_t port,uint8_t num)
{
    uint8_t pin = port<<4 | num;
    io_exti_callback(pin);
}

void EXTI_Handler(void)
{
    uint8_t i;
    uint8_t gpio_status = V33_RG->GPIOA_INTR_L;
    if(gpio_status != 0)
    {
        for(i=0;i<8;++i)
        {
            if(1<<i & gpio_status)
            {
                V33_RG->GPIOA_INTR_CLR_L = 1<<i;
                V33_RG->GPIOA_INTR_CLR_L = 0;
                exti_io_handler(0,i);
            }
        }
    }
    gpio_status = V33_RG->GPIOA_INTR_H;
    if(gpio_status != 0)
    {
        for(i=0;i<8;++i)
        {
            if(1<<i & gpio_status)
            {
                V33_RG->GPIOA_INTR_CLR_H = 1<<i;
                V33_RG->GPIOA_INTR_CLR_H = 0;
                exti_io_handler(0,i+8);
            }
        }
    }
    gpio_status = V33_RG->GPIOB_INTR_L;
    if(gpio_status != 0)
    {
        for(i=0;i<8;++i)
        {
            if(1<<i & gpio_status)
            {
                V33_RG->GPIOB_INTR_CLR_L = 1<<i;
                V33_RG->GPIOB_INTR_CLR_L = 0;
                exti_io_handler(1,i);
            }
        }
    }
    gpio_status = V33_RG->GPIOB_INTR_H;
    if(gpio_status != 0)
    {
        for(i=0;i<8;++i)
        {
            if(1<<i & gpio_status)
            {
                V33_RG->GPIOB_INTR_CLR_H = 1<<i;
                V33_RG->GPIOB_INTR_CLR_H = 0;
                exti_io_handler(1,i+8);
            }
        }
    }
    gpio_status = V33_RG->GPIOC_INTR_L;
    if(gpio_status != 0)
    {
        for(i=0;i<8;++i)
        {
            if(1<<i & gpio_status)
            {
                V33_RG->GPIOC_INTR_CLR_L = 1<<i;
                V33_RG->GPIOC_INTR_CLR_L = 0;
                exti_io_handler(2,i);
            }
        }
    }
    gpio_status = V33_RG->GPIOC_INTR_H;
    if(gpio_status != 0)
    {
        for(i=0;i<8;++i)
        {
            if(1<<i & gpio_status)
            {
                V33_RG->GPIOC_INTR_CLR_H = 1<<i;
                V33_RG->GPIOC_INTR_CLR_H = 0;
                exti_io_handler(2,i+8);
            }
        }
    }
}

void io_init(void)
{
    SYSC_AWO->IO[0].IE_OD = 0;
    SYSC_AWO->IO[0].OE_DOT= 0;
    SYSC_AWO->IO[1].IE_OD = 0;
    SYSC_AWO->IO[1].OE_DOT = 0;
    arm_cm_set_int_isr(EXT_IRQn,EXTI_Handler);
    __NVIC_EnableIRQ(EXT_IRQn);
}

void io_cfg_output(uint8_t pin)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    SYSC_AWO->IO[x->port].OE_DOT |= 1<<16<<x->num;
}

void io_cfg_input(uint8_t pin)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    SYSC_AWO->IO[x->port].OE_DOT &= ~(1<<16<<x->num);
    SYSC_AWO->IO[x->port].IE_OD &= ~(1<<16<<x->num); 
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
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    SYSC_AWO->IO[x->port].OE_DOT |= 1<<x->num;
}

void io_clr_pin(uint8_t pin)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    SYSC_AWO->IO[x->port].OE_DOT &= ~(1<<x->num);
}

void io_toggle_pin(uint8_t pin)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    SYSC_AWO->IO[x->port].OE_DOT ^= 1<<x->num;
}

uint8_t io_get_output_val(uint8_t pin)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    return SYSC_AWO->IO[x->port].OE_DOT >> x->num & 0x1;
}

uint8_t io_read_pin(uint8_t pin)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    return SYSC_AWO->IO[x->port].DIN >> x->num & 0x1;
}

void io_pull_write(uint8_t pin,io_pull_type_t pull)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    switch(pull)
    {
    case IO_PULL_DISABLE:
        SYSC_AWO->IO[x->port].PUPD &= ~(1<<16<<x->num | 1<<x->num);
    break;
    case IO_PULL_UP:
        MODIFY_REG(SYSC_AWO->IO[x->port].PUPD,1<<16<<x->num,1<<x->num);
    break;
    case IO_PULL_DOWN:
        MODIFY_REG(SYSC_AWO->IO[x->port].PUPD,1<<x->num,1<<16<<x->num);
    break;
    }
}

io_pull_type_t io_pull_read(uint8_t pin)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    if((SYSC_AWO->IO[x->port].PUPD>>x->num)&0x1)
    {
        return IO_PULL_DOWN;
    }else if((SYSC_AWO->IO[x->port].PUPD>>x->num>>16)&0x1)
    {
        return IO_PULL_UP;
    }else
    {
        return IO_PULL_DISABLE;
    }
}

static void io_intr_flag_clr(gpio_pin_t *x)
{
    switch (x->port)
    {
    case 0:
        if(x->num<=7)
        {
            V33_RG->GPIOA_INTR_CLR_L = 1<<(x->num);
            V33_RG->GPIOA_INTR_CLR_L = 0;
        }
        else
        {
            V33_RG->GPIOA_INTR_CLR_H = 1<<(x->num -8);
            V33_RG->GPIOA_INTR_CLR_H = 0;
        }
        break;
    case 1:
        if(x->num<=7)
        {
            V33_RG->GPIOB_INTR_CLR_L = 1<<(x->num);
            V33_RG->GPIOB_INTR_CLR_L  = 0;
        }
        else
        {
            V33_RG->GPIOB_INTR_CLR_H = 1<<(x->num -8);
            V33_RG->GPIOB_INTR_CLR_H  = 0;
        }
        break;
    case 2:
        if(x->num<=7)
        {
            V33_RG->GPIOC_INTR_CLR_L = 1<<(x->num);
            V33_RG->GPIOC_INTR_CLR_L  = 0;
        }
        else
        {
            V33_RG->GPIOC_INTR_CLR_H = 1<<(x->num -8);
            V33_RG->GPIOC_INTR_CLR_H  = 0;
        }
        break;
    default:
        break;
    }
}

void io_exti_config(uint8_t pin,exti_edge_t edge)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    switch (x->port)
    {
    case 0:
        if(x->num<=7)
        {
            if(edge == INT_EDGE_RISING)
            {
                V33_RG->GPIOA_INTR_POL_L &= ~(1<<(x->num));
            }
            else
            {
                V33_RG->GPIOA_INTR_POL_L |= 1<<(x->num);
            }
        }
        else
        {
            if(edge == INT_EDGE_RISING)
            {
                V33_RG->GPIOA_INTR_POL_H &=~(1<<(x->num-8));
            }
            else
            {
                V33_RG->GPIOA_INTR_POL_H |= 1<<(x->num-8);
            }
        }
        break;
    case 1:
        if(x->num<=7)
        {
            if(edge == INT_EDGE_RISING)
            {
                V33_RG->GPIOB_INTR_POL_L &= ~(1<<(x->num));
            }
            else
            {
                V33_RG->GPIOB_INTR_POL_L |= 1<<(x->num);
            }
        }
        else
        {
            if(edge == INT_EDGE_RISING)
            {
                V33_RG->GPIOB_INTR_POL_H &= ~(1<<(x->num-8));
            }
            else
            {
                V33_RG->GPIOB_INTR_POL_H |= 1<<(x->num-8);
            }
        }
        break;
    case 2:
        if(x->num<=7)
        {
            if(edge == INT_EDGE_RISING)
            {
               V33_RG->GPIOC_INTR_POL_L &= ~(1<<(x->num));
            }
            else
            {
                V33_RG->GPIOC_INTR_POL_L |= 1<<(x->num);
            }
        }
        else
        {
            if(edge == INT_EDGE_RISING)
            {
                V33_RG->GPIOC_INTR_POL_H &= ~(1<<(x->num-8));
            }
            else
            {
                V33_RG->GPIOC_INTR_POL_H |= 1<<(x->num-8);
            }
        }
        break;
    default:
        break;
    }
}

void io_exti_enable(uint8_t pin,bool enable)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    io_intr_flag_clr(x);
    switch (x->port)
    {
    case 0:
        if(x->num<=7)
        {
            if(enable)
            {
                V33_RG->GPIOA_INTR_EN_L |= 1<<(x->num);
            }
            else
            {
                V33_RG->GPIOA_INTR_EN_L &= ~(1<<(x->num));
            }
        }
        else
        {
            if(enable)
            {
                V33_RG->GPIOA_INTR_EN_H |= 1<<(x->num-8);
            }
            else
            {
                V33_RG->GPIOA_INTR_EN_H &= ~(1<<(x->num - 8));
            }
        }
        break;
    case 1:
        if(x->num<=7)
        {
            if(enable)
            {
                V33_RG->GPIOB_INTR_EN_L |= 1<<(x->num);
            }
            else
            {
                V33_RG->GPIOB_INTR_EN_L &= ~(1<<(x->num));
            }
        }
        else
        {
            if(enable)
            {
                V33_RG->GPIOB_INTR_EN_H |= 1<<(x->num-8);
            }
            else
            {
                V33_RG->GPIOB_INTR_EN_H &= ~(1<<(x->num - 8));
            }
        }
        break;
    case 2:
        if(x->num<=7)
        {
            if(enable)
            {
                V33_RG->GPIOC_INTR_EN_L |= 1<<(x->num);
            }
            else
            {
                V33_RG->GPIOC_INTR_EN_L &= ~(1<<(x->num));
            }
        }
        else
        {
            if(enable)
            {
                V33_RG->GPIOC_INTR_EN_H |= 1<<(x->num-8);
            }
            else
            {
                V33_RG->GPIOC_INTR_EN_H &= ~(1<<(x->num - 8));
            }
        }
        break;
    default:
        break;
    }
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
    MODIFY_REG(SYSC_PER->FUNC_SEL[func_io_num/4],0xff<<8*(func_io_num%4),per_func<<8*(func_io_num%4));
    if(func_io_num >= 32)
    {
        SYSC_AWO->PIN_SEL2 |= 1<<(func_io_num-32);
    }else
    {
        SYSC_AWO->PIN_SEL1 |= 1<<func_io_num;
    }
}

static void per_func_disable(uint8_t func_io_num)
{
    if(func_io_num >= 32)
    {
        SYSC_AWO->PIN_SEL2 &= ~(1<<(func_io_num-32));
    }else
    {
        SYSC_AWO->PIN_SEL1 &= ~(1<<func_io_num);
    }
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
    REG_FIELD_WR(SYSC_AWO->PIN_SEL0,SYSC_AWO_QSPI0_EN,0x3f);
}

void qspi_flash_io_deinit(void)
{
    REG_FIELD_WR(SYSC_AWO->PIN_SEL0,SYSC_AWO_QSPI0_EN,0);
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
