#include "lsgpio.h"
#include "reg_lsgpio.h"
#include "io_config.h"
#include "reg_syscfg.h"
#include "sleep.h"
typedef struct
{
    uint8_t num:4,
            port:4;
}gpio_pin_t;

gpio_pin_t uart1_txd;
gpio_pin_t uart1_rxd;
gpio_pin_t uart2_txd;
gpio_pin_t uart2_rxd;
gpio_pin_t uart3_txd;
gpio_pin_t uart3_rxd;

reg_lsgpio_t* GPIO_GetPort(uint8_t Pin_port)
{
    uint8_t       PortID = Pin_port;
    reg_lsgpio_t* Port = {0};

   switch (PortID)
    {
    case 0:
        Port = LSGPIOA;
        break;
    case 1:
        Port = LSGPIOB;
        break;
    case 2:
        Port = LSGPIOC;
        break;
    default:
        break;
    }
    return Port;
}

static void uart_gpio_init(uint8_t txd,uint8_t rxd)
{
    io_set_pin(txd);
    io_cfg_output(txd);
    io_cfg_input(rxd);
}

static void af_io_init(gpio_pin_t *pin,enum GPIO_AF af)
{
    reg_lsgpio_t *port = GPIO_GetPort(pin->port);
    switch(pin->num)
    {
    case 0: case 1: case 2: case 3:
        MODIFY_REG(port->AF0, GPIO_IO0_AF_MASK << pin->num * 8, af << pin->num * 8);
    break;
    case 4: case 5: case 6: case 7:
        MODIFY_REG(port->AF1, GPIO_IO4_AF_MASK << (pin->num - 4) * 8, af << (pin->num - 4) * 8);
    break;
    case 8: case 9: case 10: case 11:
        MODIFY_REG(port->AF2, GPIO_IO8_AF_MASK << (pin->num - 8) * 8, af << (pin->num - 8) * 8);
    break;
    case 12: case 13: case 14: case 15:
        MODIFY_REG(port->AF3, GPIO_IO12_AF_MASK << (pin->num - 12) * 8, af << (pin->num - 12) * 8);
    break;
    }
    MODIFY_REG(port->MODE, GPIO_MODE0_MASK << (pin->num << 1u), SET_GPIO_MODE_AF << (pin->num << 1u));
}

static void set_gpio_mode(gpio_pin_t *pin)
{
    reg_lsgpio_t *gpiox = GPIO_GetPort(pin->port);
    MODIFY_REG(gpiox->MODE, GPIO_MODE0_MASK << (pin->num << 1u), SET_GPIO_MODE_GPIO << (pin->num << 1u));
}

void uart1_io_init(uint8_t txd,uint8_t rxd)
{
    uart_gpio_init(txd,rxd);
    af_io_init((gpio_pin_t *)&txd,AF_UART1_TXD);
    af_io_init((gpio_pin_t *)&rxd,AF_UART1_RXD);
}

void uart1_io_deinit()
{
    set_gpio_mode((gpio_pin_t *)&uart1_txd);
    set_gpio_mode((gpio_pin_t *)&uart1_rxd);
}

void uart2_io_init(uint8_t txd,uint8_t rxd)
{
    uart_gpio_init(txd,rxd);
    af_io_init((gpio_pin_t *)&txd,AF_UART2_TXD);
    af_io_init((gpio_pin_t *)&rxd,AF_UART2_RXD);
}

void uart2_io_deinit()
{
    set_gpio_mode((gpio_pin_t *)&uart2_txd);
    set_gpio_mode((gpio_pin_t *)&uart2_rxd);
}

void uart3_io_init(uint8_t txd,uint8_t rxd)
{
    uart_gpio_init(txd,rxd);
    af_io_init((gpio_pin_t *)&txd,AF_UART3_TXD);
    af_io_init((gpio_pin_t *)&rxd,AF_UART3_RXD);
}

void uart3_io_deinit()
{
    set_gpio_mode((gpio_pin_t *)&uart3_txd);
    set_gpio_mode((gpio_pin_t *)&uart3_rxd);
}


void io_init(void)
{
    RCC->AHBEN |= RCC_GPIOA_MASK | RCC_GPIOB_MASK | RCC_GPIOC_MASK;
    LSGPIOA->MODE = 0;
    LSGPIOA->IE = 0;
    LSGPIOA->OE = 0;
    LSGPIOA->PUPD = 0;
    LSGPIOB->MODE = 0x2800;
    LSGPIOB->IE = 0;
    LSGPIOB->OE = 0;
    LSGPIOB->PUPD = 0x2800;
}

void io_cfg_output(uint8_t pin)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    switch(x->port)
    {
    case 0:
        LSGPIOA->OE |= 1<< x->num;
    break;
    case 1:
        LSGPIOB->OE |= 1<< x->num;
    break;
    }
}

void io_cfg_input(uint8_t pin)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    switch(x->port)
    {
    case 0:
        LSGPIOA->OE &= ~(1<< x->num);
        LSGPIOA->IE |= 1<< x->num;
    break;
    case 1:
        LSGPIOB->OE &= ~(1<< x->num);
        LSGPIOB->IE |= 1<< x->num;
    break;
    }

}

void io_set_pin(uint8_t pin)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    switch(x->port)
    {
    case 0:
        LSGPIOA->DOUT |= 1<< x->num;
    break;
    case 1:
        LSGPIOB->DOUT |= 1<< x->num;
    break;
    }
}

void io_clr_pin(uint8_t pin)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    switch(x->port)
    {
    case 0:
        LSGPIOA->DOUT &= ~(1<< x->num);
    break;
    case 1:
        LSGPIOB->DOUT &= ~(1<< x->num);
    break;
    }
}

void io_write_pin(uint8_t pin, uint8_t val)
{
    if(val)
    {
        io_set_pin(pin);
    }else
    {
        io_clr_pin(pin);
    }
}

uint8_t io_read_pin(uint8_t pin)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    uint8_t val = 0;
    switch(x->port)
    {
    case 0:
        val = (LSGPIOA->DIN >> x->num) & 0x1;
    break;
    case 1:
        val = (LSGPIOB->DIN >> x->num) & 0x1;
    break;
    }
    return val;
}

void io_toggle_pin(uint8_t pin)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    switch(x->port)
    {
    case 0:
        LSGPIOA->DOUT ^= 1<< x->num;
    break;
    case 1:
        LSGPIOB->DOUT ^= 1<< x->num;
    break;
    }
}

void io_pull_write(uint8_t pin,io_pull_type_t pull)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    switch(x->port)
    {
    case 0:
        LSGPIOA->PUPD = pull << 2 * x->num;
    break;
    case 1:
        LSGPIOB->PUPD = pull << 2 * x->num;
    break;
    }
}

io_pull_type_t io_pull_read(uint8_t pin)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    io_pull_type_t pull = IO_PULL_DISABLE;
    switch(x->port)
    {
    case 0:
        pull = (LSGPIOA->PUPD >> 2 * x->num ) & 0x3;
    break;
    case 1:
        pull = (LSGPIOB->PUPD >> 2 * x->num ) & 0x3;
    break;
    }
    return pull;
}

void io_exti_config(uint8_t pin,exti_edge_t edge)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    if(x->num <= 7)
    {
        MODIFY_REG(EXTI->EICFG0, 0xf<< 4 * x->num,x->port << 4 * x->num);
    }else
    {
        MODIFY_REG(EXTI->EICFG1, 0xf<< 4 * (x->num - 8),x->port << 4 * (x->num - 8));
    }
    if(edge == INT_EDGE_FALLING)
    {
        EXTI->ERTS &= ~(1 << x->num);
        EXTI->EFTS |= 1 << x->num;
        switch(pin)
        {
        case PA00:
            CLEAR_BIT(SYSCFG->PMU_WKUP,PA00_IO_WKUP<<WKUP_EDGE_POS);
        break;
        case PA07:
            CLEAR_BIT(SYSCFG->PMU_WKUP,PA07_IO_WKUP<<WKUP_EDGE_POS);
        break;
        case PB11:
            CLEAR_BIT(SYSCFG->PMU_WKUP,PB11_IO_WKUP<<WKUP_EDGE_POS);
        break;
        case PB15:
            CLEAR_BIT(SYSCFG->PMU_WKUP,PB15_IO_WKUP<<WKUP_EDGE_POS);
        break;
        }
    }else
    {
        EXTI->EFTS &= ~(1 << x->num);
        EXTI->ERTS |= 1 << x->num;
        switch(pin)
        {
        case PA00:
            SET_BIT(SYSCFG->PMU_WKUP,PA00_IO_WKUP<<WKUP_EDGE_POS);
        break;
        case PA07:
            SET_BIT(SYSCFG->PMU_WKUP,PA07_IO_WKUP<<WKUP_EDGE_POS);
        break;
        case PB11:
            SET_BIT(SYSCFG->PMU_WKUP,PB11_IO_WKUP<<WKUP_EDGE_POS);
        break;
        case PB15:
            SET_BIT(SYSCFG->PMU_WKUP,PB15_IO_WKUP<<WKUP_EDGE_POS);
        break;
        }
    }
}

void io_exti_enable(uint8_t pin,bool enable)
{
    gpio_pin_t *x = (gpio_pin_t *)&pin;
    if(enable==false)
    {
        EXTI->EIDR =  1 << x->num;
        switch(pin)
        {
        case PA00:
            CLEAR_BIT(SYSCFG->PMU_WKUP,PA00_IO_WKUP<<WKUP_EN_POS);
        break;
        case PA07:
            CLEAR_BIT(SYSCFG->PMU_WKUP,PA07_IO_WKUP<<WKUP_EN_POS);
        break;
        case PB11:
            CLEAR_BIT(SYSCFG->PMU_WKUP,PB11_IO_WKUP<<WKUP_EN_POS);
        break;
        case PB15:
            CLEAR_BIT(SYSCFG->PMU_WKUP,PB15_IO_WKUP<<WKUP_EN_POS);
        break;
        }
    }else
    {
        switch(pin)
        {
        case PA00:
            SET_BIT(SYSCFG->PMU_WKUP,PA00_IO_WKUP<<WKUP_EN_POS);
        break;
        case PA07:
            SET_BIT(SYSCFG->PMU_WKUP,PA07_IO_WKUP<<WKUP_EN_POS);
        break;
        case PB11:
            SET_BIT(SYSCFG->PMU_WKUP,PB11_IO_WKUP<<WKUP_EN_POS);
        break;
        case PB15:
            SET_BIT(SYSCFG->PMU_WKUP,PB15_IO_WKUP<<WKUP_EN_POS);
        break;
        }
        EXTI->EIER =  1 << x->num;
    }
}

void io_wkup_en_clr_set(uint8_t pin)
{
    switch(pin)
    {
    case PA00:
        CLEAR_BIT(SYSCFG->PMU_WKUP,PA00_IO_WKUP<<WKUP_EN_POS);
        SET_BIT(SYSCFG->PMU_WKUP,PA00_IO_WKUP<<WKUP_EN_POS);
    break;
    case PA07:
        CLEAR_BIT(SYSCFG->PMU_WKUP,PA07_IO_WKUP<<WKUP_EN_POS);
        SET_BIT(SYSCFG->PMU_WKUP,PA07_IO_WKUP<<WKUP_EN_POS);
    break;
    case PB11:
        CLEAR_BIT(SYSCFG->PMU_WKUP,PB11_IO_WKUP<<WKUP_EN_POS);
        SET_BIT(SYSCFG->PMU_WKUP,PB11_IO_WKUP<<WKUP_EN_POS);
    break;
    case PB15:
        CLEAR_BIT(SYSCFG->PMU_WKUP,PB15_IO_WKUP<<WKUP_EN_POS);
        SET_BIT(SYSCFG->PMU_WKUP,PB15_IO_WKUP<<WKUP_EN_POS);
    break;
    }
}

__attribute__((weak)) void io_exti_callback(uint8_t pin){}

static void exti_io_handler(uint8_t port,uint8_t num)
{
    uint8_t pin = port<<4 | num;
    EXTI->EICR = 1<<num;
    io_wkup_en_clr_set(pin);
    io_exti_callback(pin);
}

void EXTI_Handler()
{
    uint16_t int_stat = EXTI->EEIFM;
    uint8_t i;
    uint8_t port;
    for(i=0;i<8;++i)
    {
        if(1<<i & int_stat)
        {
            port = EXTI->EICFG0 >> (4*i);
            exti_io_handler(port,i);
        }
    }
    for(i=8;i<16;++i)
    {
        if(1<<i & int_stat)
        {
            port = EXTI->EICFG1 >> (4*(i-8));
            exti_io_handler(port,i);
        }
    }
}

