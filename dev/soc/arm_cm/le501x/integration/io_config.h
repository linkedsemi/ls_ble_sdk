#ifndef IO_CONFIG_H_
#define IO_CONFIG_H_
//lsgpioa
#define LSGPIOA_0 ((uint8_t)0x00)   /* Pin A0 selected    */
#define LSGPIOA_1 ((uint8_t)0x01)   /* Pin A1 selected    */
#define LSGPIOA_2 ((uint8_t)0x02)   /* Pin A2 selected    */
#define LSGPIOA_3 ((uint8_t)0x03)   /* Pin A3 selected    */
#define LSGPIOA_4 ((uint8_t)0x04)   /* Pin A4 selected    */
#define LSGPIOA_5 ((uint8_t*)0x05)   /* Pin A5 selected    */
#define LSGPIOA_6 ((uint8_t*)0x06)   /* Pin A6 selected    */
#define LSGPIOA_7 ((uint8_t)0x07)   /* Pin A7 selected    */
#define LSGPIOA_8 ((uint8_t)0x08)   /* Pin A8 selected    */
#define LSGPIOA_9 ((uint8_t)0x09)   /* Pin A9 selected    */
#define LSGPIOA_10 ((uint8_t)0x0A)  /* Pin A10 selected   */
#define LSGPIOA_11 ((uint8_t)0x0B)  /* Pin A11 selected   */
#define LSGPIOA_12 ((uint8_t)0x0C)  /* Pin A12 selected   */
#define LSGPIOA_13 ((uint8_t)0x0D)  /* Pin A13 selected   */
#define LSGPIOA_14 ((uint8_t)0x0E)  /* Pin A14 selected   */
#define LSGPIOA_15 ((uint8_t)0x0F)  /* Pin A15 selected   */
//lsgpiob
#define LSGPIOB_0 ((uint8_t)0X10)   /* Pin B0 selected    */
#define LSGPIOB_1 ((uint8_t)0X11)   /* Pin B1 selected    */
#define LSGPIOB_2 ((uint8_t)0X12)   /* Pin B2 selected    */
#define LSGPIOB_3 ((uint8_t)0X13)   /* Pin B3 selected    */
#define LSGPIOB_4 ((uint8_t)0X14)   /* Pin B4 selected    */
#define LSGPIOB_5 ((uint8_t)0X15)   /* Pin B5 selected    */
#define LSGPIOB_6 ((uint8_t)0X16)   /* Pin B6 selected    */
#define LSGPIOB_7 ((uint8_t)0X17)   /* Pin B7 selected    */
#define LSGPIOB_8 ((uint8_t)0X18)   /* Pin B8 selected    */
#define LSGPIOB_9 ((uint8_t)0X19)   /* Pin B9 selected    */
#define LSGPIOB_10 ((uint8_t)0X1A)  /* Pin B10 selected   */
#define LSGPIOB_11 ((uint8_t)0X1B)  /* Pin B11 selected   */
#define LSGPIOB_12 ((uint8_t)0X1C)  /* Pin B12 selected   */
#define LSGPIOB_13 ((uint8_t)0X1D)  /* Pin B13 selected   */
#define LSGPIOB_14 ((uint8_t)0X1E)  /* Pin B14 selected   */
#define LSGPIOB_15 ((uint8_t)0X1F)  /* Pin B15 selected   */
//lsgpioc
#define LSGPIOC_0 ((uint8_t)0X20)   /* Pin C0 selected    */
#define LSGPIOC_1 ((uint8_t)0X21)   /* Pin C1 selected    */

typedef struct
{
    uint8_t part:4,
            pin_num:4;
}peripheral_gpio_pin;


void uart1_io_init(uint8_t txd,uint8_t rxd);
void uart1_io_deinit();

void uart2_io_init(uint8_t txd,uint8_t rxd);
void uart2_io_deinit();

void uart3_io_init(uint8_t txd,uint8_t rxd);
void uart3_io_deinit();

#endif

