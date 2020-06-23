# Timer Driver Notes

##
基于STM32F10x HAL driver移植

## 需订正
内部设计文档，GPTIMA/B->SMCR寄存器，需要增加ETR_CTRL[7:0]

1.5.2 GPTIMA/B->CR寄存器，寄存器位少了TI1S (表格中没有少)

## Timer分类

包含了advanced timer, general purpose timer A/B/C, basic timer.

不同Timer功能基本相似，但存在一些功能上差异。

LSADTIM1 - advanced timer, 16 bit, up, down, up/down auto-reload, dead-time and break, quad encoder, hall-sensor. 

LSGPTIM1 - general purpose timer, 16 bit, 2 channel, with dead-time and break

LSGPTIM2 - general purpose timer, 32 bit, 4 channel, no dead-time 

LSGPTIM3 - general purpose timer, 16 bit, 4 channel, no dead-time

LSBSTIM1 - basic timer, 16 bit, 

LSGPTIM2/LSGPTIM3 唯一差别是32bit vs 16bit

## 函数

HAL_TIM_ConfigOCrefClear() - 支持adtim, GPTIM2/3


## 与STM32F1xx Timer差异

~ 提供了重复捕捉/比较的中断信号生成 (CR2)


## Timer分类
| INSTANCE类别                         |   Feature           | ADTIMER(16B4C) | GPTIM1(16B2C) | GPTIM2(32B4C) | GPTIM3(16B4C) | BSTIM       |
| :--------------------------------:   | :------------:      | :------------: | :-----------: | :----------:  | :----------:  | :---------: |
|IS_TIM_INSTANCE                       |  timer              | V              |         V     |    V          |       V       |      V      |
|IS_TIM_ADVANCED_INSTANCE              |  ADV                | V              |               |               |               |             |
|IS_TIM_CC1_INSTANCE                   |  cap/cmp chn1       | V              |         v     |       v       |       v       |             |
|IS_TIM_CC2_INSTANCE                   |  cap/cmp chn2       | V              |         v     |       v       |       v       |             |
|IS_TIM_CC3_INSTANCE                   |  cap/cmp chn3       | V              |               |       v       |       v       |             |
|IS_TIM_CC4_INSTANCE                   |  cap/cmp chn4       | V              |               |       v       |       v       |             |
|IS_TIM_CLOCKSOURCE_ETRMODE1_INSTANCE  |  extrig mode1       | V              |               |       v       |       v       |             |
|IS_TIM_CLOCKSOURCE_ETRMODE2_INSTANCE  |  extrig mode2 ETRF  | V              |               |       v       |       v       |             |
|IS_TIM_CLOCKSOURCE_TIX_INSTANCE       |  exclk mode 1       | V              |        v      |       v       |       v       |             |
|IS_TIM_CLOCKSOURCE_ITRX_INSTANCE      |                     | V              |        v      |       v       |       v       |             |
|IS_TIM_OCXREF_CLEAR_INSTANCE          |  OCxREF clr ETRF    | V              |               |       v       |       v       |             |
|IS_TIM_ENCODER_INTERFACE_INSTANCE     |  quad encoder       | V              |               |       v       |       v       |             |
|IS_TIM_XOR_INSTANCE                   |  XOR ch2/3/4        | V              |               |       v       |       v       |             |
|IS_TIM_MASTER_INSTANCE                |  master             | V              |       v       |       v       |       v       |    v        |
|IS_TIM_SLAVE_INSTANCE                 |  slave              | V              |       v       |       v       |       v       |             |
|IS_TIM_SYNCHRO_INSTANCE               | master/slave/TRGO   | V              |       v       |       v       |       v       |             |
|IS_TIM_BREAK_INSTANCE                 |  break              | V              |       v       |               |               |             |
|IS_TIM_CCX_INSTANCE                   |  cap/cmp            | V              |       v       |      v        |      v        |             |
sc|IS_TIM_CCXN_INSTANCE                  |  cap/cmp w/ comp    | V              |       v       |               |               |             |
|IS_TIM_COUNTER_MODE_SELECT_INSTANCE   |  cnt up/down        | V              |       v       |      v        |      v        |             |
|IS_TIM_REPETITION_COUNTER_INSTANCE    |  repeat             | V              |       v       |               |               |             |
|IS_TIM_CLOCK_DIVISION_INSTANCE        |  clk div            | V              |       v       |      v        |      v        |             |
|IS_TIM_COMMUTATION_EVENT_INSTANCE     |  compl has          | V              |       v       |               |               |             |
|IS_TIM_ETR_INSTANCE                   |  ext trig in        | V              |       v       |      v        |     v         |             |
|IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE |  hall               | V              |               |      v        |     v         |             |
|IS_TIM_32B_COUNTER_INSTANCE           |   32bit counter     |                |               |       v       |               |             |
|IS_TIM_DMABURST_INSTANCE              |                     | V              |      v        |      v        |     v         |             |
|IS_TIM_DMA_INSTANCE                   |                     | V              |      v        |      v        |      v        |             |
|IS_TIM_DMA_CC_INSTANCE                |                     | V              |      v        |      v        |      v        |             |
