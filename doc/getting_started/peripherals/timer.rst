.. _timer_ref:

=======
TIMER
=======
 
我们有高级控制定时器(ADTIM)、 通用定时器(GPTIMA,GPTIMB,GPTIMC)与基础(BSTIM)定时器，他们是完全独立的，不共享任何资源，可以同步操作。下图主要从预装载寄存器（ARR）位数、预分频寄存器（PSC）、计数模式、是否支持重复计数、具有几路输出比较/输入捕获以及互补输出通道来区分不同timer之间的区别：

+---------+------------+-----------+----------------+--------------+--------------------+-------------------------+
| timer   |  ARR reg   |  PSC reg  |  Counter mode  |   RCR reg    |   Compare/Capture  |   Complementary output  |
+=========+============+===========+================+==============+====================+=========================+
|  BSTIM  |  16bit     |   16bit   |      UP        |      N       |         N          |        N                |
+---------+------------+-----------+----------------+--------------+--------------------+-------------------------+
|  GPTIMA |  32bit     |   16bit   |   UP/DOWN      |      N       |         4          |        N                |
+---------+------------+-----------+----------------+--------------+--------------------+-------------------------+
|  GPTIMB |  16bit     |   16bit   |   UP/DOWN      |      N       |         4          |        N                |
+---------+------------+-----------+----------------+--------------+--------------------+-------------------------+
|  GPTIMC |  16bit     |   16bit   |      UP        |      Y       |         2          |       ch1               |
+---------+------------+-----------+----------------+--------------+--------------------+-------------------------+
|  ADTIM  |  16bit     |   16bit   |   UP/DOWN      |      Y       |         4          |    ch1/ch2/ch3          |
+---------+------------+-----------+----------------+--------------+--------------------+-------------------------+

时基介绍
--------

时钟源
>>>>>>>>
定时器时钟来源于系统时钟，软件中可以通过宏 ``SDK_HCLK_MHZ`` 获取

计数器时钟
>>>>>>>>>>>
定时器时钟经过 PSC 预分频器之后，用来驱动计数器计数。 PSC 是一个16 位的预分频器，可以对定时器时钟进行 1~65536 之间的任何一个数进行分频。具体计算方式为：SDK_HCLK_MHZ/(PSC+1)

计数器
>>>>>>>>
除了BSTIM和GPTIMC只有递增计数模式外，其它定时器计数器都有三种计数模式，分别为递增计数模式、递减计数模式和递增/递减(中心对齐)计数模式。

#. 递增计数模式下，计数器从 0 开始计数，每来一个脉冲计数器就增加 1，直到计数器的值与自动重载寄存器 ARR 值相等，然后计数器又从 0 开始计数并生成计数器上溢事件，计数器总是如此循环计数。如果禁用重复计数器，在计数器生成上溢事件就马上生成更新事件；如果使能重复计数器，每生成一次上溢事件重复计数器内容就减 1，直到重复计数器内容为 0 时才会生成更新事件。

#. 递减计数模式下，计数器从自动重载寄存器 ARR 值开始计数，每来一个脉冲计数器就减 1，直到计数器值为 0，然后计数器又从自动重载寄存器 ARR 值开始递减计数并生成计数器下溢事件，计数器总是如此循环计数。如果禁用重复计数器，在计数器生成下溢事件就马上生成更新事件；如果使能重复计数器，每生成一次下溢事件重复计数器内容就减 1，直到重复计数器内容为 0 时才会生成更新事件。

#. 中心对齐模式下，计数器从 0 开始递增计数，直到计数值等于(ARR-1)值生成计数器上溢事件，然后从 ARR 值开始递减计数直到 1 生成计数器下溢事件。然后又从 0 开始计数，如此循环。每次发生计数器上溢和下溢事件都会生成更新事件。

重复计数器
>>>>>>>>>>
对于BSTIM、GPTIMA、GPTIMB发生上/下溢事件时直接就生成更新事件，而GPTIMC和ADTIM在硬件结构上多出了重复计数器，在定时器发生上溢或下溢事件是递减重复计数器的值，只有当重复计数器为 0 时才会生成更新事件。在发生 N+1 个上溢或下溢事件(N 为 RCR 的值)时产生更新事件。

自动重装载寄存器
>>>>>>>>>>>>>>>>
自动重装载寄存器里面装着计数器能计数的最大数值，最大值取决于寄存器的位数，除了GPTIMA是32位的外其余都是16位。当计数到这个值的时候，如果使能了中断的话，定时器就产生溢出中断。

定时时间计算
>>>>>>>>>>>>>
定时器的定时时间等于计数器的周期乘以计数次数。计一个数的时间则是计数器时钟的倒数，等于： 1/（SDK_HCLK_MHZ*1000000/(PSC+1)），可以计算出我们需要的定时时间就等于：1/（SDK_HCLK_MHZ*1000000/(PSC+1)）*(ARR+1)。

PWM输出频率计算
>>>>>>>>>>>>>>>>
假设需要配置频率为X的脉冲，通过公式 ARR = （SDK_HCLK_MHZ*1000000/(PSC+1)）/ X  - 1； 可以确定需要配置的预分频寄存器（PSC）和自动重装载值寄存器 (ARR) 的值。

定时功能
---------
初始化
>>>>>>>
基本的定时器功能只需要对结构体 ``TIM_HandleTypeDef`` 进行初始化即可

#. 初始化一个全局的 ``TIM_HandleTypeDef`` 结构体变量::

    TIM_HandleTypeDef TimHandle;

#. 选择需要使用的硬件Timer，例如需要使用LSBSTIM::
    
    TimHandle.Instance = LSBSTIM;

#. 根据应用需求填写相应的预分频系数、自动重装载值的大小、选择计数模式::

    TimHandle.Init.Prescaler     = TIM_PRESCALER;
    TimHandle.Init.Period        = TIM_PERIOD;
    TimHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;

#. 将初始化号的结构变量值配置到相应寄存器中::

    HAL_TIM_Init(&TimHandle);

打开定时器
>>>>>>>>>>

#. 需要产生中断的话，执行下述接口，定时器开始工作::

    HAL_TIM_Base_Start_IT(&TimHandle);

#. 只要计数功能调用以下接口，定时器开始工作::

    HAL_TIM_Base_Start(&TimHandle);

事件回调
>>>>>>>>>>>>

#. 如果使能了中断，定时事件到了之后，会进入回调函数 ``void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);`` 用户可以在回调函数内处理应用逻辑

#. 如果单纯使用计数功能，用户可以使用宏 ``__HAL_TIM_GET_COUNTER(&TimHandle)`` 来获取当前计数值

关闭定时器
>>>>>>>>>>

#. 如果使能了中断，需要关闭定时器时，调用函数::

    HAL_StatusTypeDef HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef *htim)

#. 如果只使用计数功能，需要关闭定时器时，调用函数:: 

    HAL_StatusTypeDef HAL_TIM_Base_Start(TIM_HandleTypeDef *htim)

反初始化
>>>>>>>>

反初始化Timer功能::

    HAL_StatusTypeDef HAL_TIM_DeInit(TIM_HandleTypeDef *htim);

PWM输出
--------
初始化
>>>>>>
#. 与定时功能里初始化步骤一样，需要先对定时器的时基部分进行配置

#. 根据选择使用的硬件Timer，调用相应的接口函数初始化PWM使用到的相关GPIO，比如需要使用LSGPTIMB的四个通道同时输出::

    gptimb1_ch1_io_init(PA00, true, 0);
    gptimb1_ch2_io_init(PA01, true, 0);
    gptimb1_ch3_io_init(PB14, true, 0);
    gptimb1_ch4_io_init(PB15, true, 0);

#. 初始化输出比较结构体 ``TIM_OC_InitTypeDef``,对指定定时器输出通道进行初始化配置

#. 调用以下接口完成输出通道的初始化配置::

    HAL_StatusTypeDef HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *htim,TIM_OC_InitTypeDef *sConfig,uint32_t Channel);

开始产生PWM脉冲
>>>>>>>>>>>>>>>
初始化配置完成之后，需要执行下述函数才会开始输出PWM波形::

    HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel);

停止PWM输出
>>>>>>>>>>>
需要停止PWM时，调用以下函数接口::

    HAL_StatusTypeDef HAL_TIM_PWM_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel);

反初始化
>>>>>>>>

#. 反初始化Timer功能::

    HAL_StatusTypeDef HAL_TIM_DeInit(TIM_HandleTypeDef *htim);

#. 调用相应的接口，对配置过的IO进行反初始化，比如对配置过的LSGPTIMB的四个通道的IO进行反初始化::

    gptimb1_ch1_io_deinit();
    gptimb1_ch2_io_deinit();
    gptimb1_ch3_io_deinit();
    gptimb1_ch4_io_deinit();
