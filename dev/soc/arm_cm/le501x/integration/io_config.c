#include "lsgpio.h"
#include "reg_lsgpio.h"
#include "io_config.h"
peripheral_gpio_pin uart1_txd;
peripheral_gpio_pin uart1_rxd;
peripheral_gpio_pin uart2_txd;
peripheral_gpio_pin uart2_rxd;
peripheral_gpio_pin uart3_txd;
peripheral_gpio_pin uart3_rxd;
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

static void uart_io_init(peripheral_gpio_pin txd,peripheral_gpio_pin rxd)
{
    reg_lsgpio_t* gpiox;
    uint8_t position = txd.pin_num;
    gpiox = GPIO_GetPort(txd.part);
    MODIFY_REG(gpiox->MODE, (GPIO_MODE0_MASK << (position << 1u)), (SET_GPIO_MODE_AF << (position << 1u)));
    MODIFY_REG(gpiox->OD, (GPIO_OD0_MASK << (position << 1u)), (GPIO_OUTPUT_MAX_DRIVER << (position << 1u)));
    MODIFY_REG(gpiox->OE, (GPIO_OE0_MASK << position), (GPIO_OUTPUT_EN << position));
    MODIFY_REG(gpiox->IE, (GPIO_IE0_MASK << position), (0U << position));

    position = rxd.pin_num;
    gpiox = GPIO_GetPort(rxd.part);
    MODIFY_REG(gpiox->MODE, (GPIO_MODE0_MASK << (position << 1u)), (SET_GPIO_MODE_AF << (position << 1u)));
    MODIFY_REG(gpiox->OD, (GPIO_OD0_MASK << (position << 1u)), (GPIO_OUTPUT_MAX_DRIVER << (position << 1u)));
    MODIFY_REG(gpiox->OE, (GPIO_OE0_MASK << position), (GPIO_OUTPUT_EN << position));
    MODIFY_REG(gpiox->IE, (GPIO_IE0_MASK << position), (0U << position));
}

static void uart_io_deinit(peripheral_gpio_pin txd,peripheral_gpio_pin rxd)
{
    reg_lsgpio_t* gpiox;
    uint8_t position = txd.pin_num;
    gpiox = GPIO_GetPort(txd.part);
    MODIFY_REG(gpiox->MODE, (GPIO_MODE0_MASK << (position << 1u)), (SET_GPIO_MODE_GPIO << (position << 1u)));
    HAL_GPIO_WritePin(gpiox,0x01<<position,1);

    position = rxd.pin_num;
    gpiox = GPIO_GetPort(rxd.part);
    MODIFY_REG(gpiox->MODE, (GPIO_MODE0_MASK << (position << 1u)), (SET_GPIO_MODE_GPIO << (position << 1u)));
    HAL_GPIO_WritePin(gpiox,0x01<<position,1);
}
void uart1_io_init(uint8_t txd,uint8_t rxd)
{
    uart1_rxd.part = rxd>>4;
    uart1_rxd.pin_num = rxd;
    uart1_txd.part = txd>>4;
    uart1_txd.pin_num = txd;
    uart_io_init(uart1_rxd,uart1_txd);
}

void uart1_io_deinit()
{
    uart_io_deinit(uart1_rxd,uart1_txd);
}

void uart2_io_init(uint8_t txd,uint8_t rxd)
{
    uart2_rxd.part = rxd>>4;
    uart2_rxd.pin_num = rxd;
    uart2_txd.part = txd>>4;
    uart2_txd.pin_num = txd;
    uart_io_init(uart2_rxd,uart2_txd);
}

void uart2_io_deinit()
{
    uart_io_deinit(uart2_rxd,uart2_txd);
}

void uart3_io_init(uint8_t txd,uint8_t rxd)
{
    uart3_rxd.part = rxd>>4;
    uart3_rxd.pin_num = rxd;
    uart3_txd.part = txd>>4;
    uart3_txd.pin_num = txd;
    uart_io_init(uart3_rxd,uart3_txd);
}

void uart3_io_deinit()
{
    uart_io_deinit(uart3_rxd,uart3_txd);
}

