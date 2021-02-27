
PDM
======

PDM（Pulse Density Modulation）是一种用数字信号表示模拟信号的调制方法。PDM只有1位数据输出，要么为0，要么为1。

一、初始化
------------

1. 为PDM模块配置IO

    | 调用IO 的初始化接口，可以将任意IO复用为pdm的colck data0或data1引脚。

.. code ::

    void pdm_clk_io_init(uint8_t pin);
    void pdm_data0_io_init(uint8_t pin);
    void pdm_data1_io_init(uint8_t pin);

.. note ::

 | 1. 芯片的IO 一共有34个，具体情况需根据封装图来定义。
 | 2. 为了避免不必要的bug，在使用pdm通信的时候，请先初始化IO，再进行下列参数的配置。

2. 初始化PDM模块

2.1 PDM结构体参数原型如下：

.. code ::

    typedef struct __PDM_Init_TypeDef
    {
        onst struct pdm_fir *fir;  /*!< pdm filter controller configure */
        PDM_CFG_TypeDef cfg;        /*!< pdm clock rate, capture delay, sampling rate, and data gain configure */
        PDM_MODE_TypeDef mode;      /*!< pdm channel mode configure */
    }PDM_Init_TypeDef;


2.2 配置参数的赋值如下定义：

.. code ::

    //pdm filter controller configure
        #define PDM_FIR_COEF_8KHZ ((const struct pdm_fir *)fir_coef_8khz)
        #define PDM_FIR_COEF_16KHZ ((const struct pdm_fir *)fir_coef_16khz)
    //pdm clock rate
        #define PDM_CLK_RATIO(kHz) (128000/(kHz) - 1)
    //capture delay
        default 30
    //sampling rate
        #define PDM_SAMPLE_RATE(Clk_kHz,Sample_Rate_Hz) ((Clk_kHz)*1000/(Sample_Rate_Hz) - 1)
    //data gain configure
        defualt 5
    //pdm channel mode
        PDM_MODE_Mono or PDM_MODE_Stereo


| 2.3 调用初始化PDM模块函数接口

    通过初始化接口，应用程序可以对PDM进行参数配置。

.. code ::

    HAL_StatusTypeDef HAL_PDM_Init(PDM_HandleTypeDef *hpdm,PDM_Init_TypeDef *Init);



二、反初始化
----------------

1. 反初始化PDM模块

    通过反初始化接口，应用程序可以关闭PDM外设，在运行BLE程序时降低系统的功耗。

.. code ::

    HAL_StatusTypeDef HAL_PDM_DeInit(PDM_HandleTypeDef *hpdm);


2. 反初始化PDM IO

    反初始化IO接口的主要目的是为了避免在进入低功耗模式时，IO上产生漏电。

.. code ::

    void pdm_clk_io_deinit(void);
    void pdm_data0_io_deinit(void);
    void pdm_data1_io_deinit(void);


.. note ::

    PDM初始化动作会向系统注册PDM进入工作状态，当系统检测到有任一外设处于工作状态时，都不会进入低功耗休眠。
    因此，PDM使用完毕，需要进入低功耗状态之前，必须反初始化PDM。



三、PDM相关函数接口
-----------------------

.. note ::

    收PDM数据的模式分为 2 种：中断模式 和 DMA 模式。

3.1 收PDM数据——中断方式

.. code ::

    HAL_StatusTypeDef HAL_PDM_Transfer_Config_IT(PDM_HandleTypeDef *hpdm,uint16_t *pFrameBuffer0,uint16_t *pFrameBuffer1,uint16_t FrameNum);


3.2 收PDM数据——DMA方式

     | 以DMA方式(基本模式和乒乓模式)收PDM数据如下所示：

.. code ::

    HAL_StatusTypeDef HAL_PDM_Transfer_Config_DMA(PDM_HandleTypeDef *hpdm,uint16_t *pFrameBuffer0,uint16_t *pFrameBuffer1,uint16_t FrameNum);
    HAL_StatusTypeDef HAL_PDM_PingPong_Transfer_Config_DMA(PDM_HandleTypeDef *hpdm,struct PDM_PingPong_Bufptr *CH0_Buf,struct PDM_PingPong_Bufptr *CH1_Buf,uint16_t FrameNum);


3.3 使能PDM

.. code ::

    HAL_StatusTypeDef HAL_PDM_Start(PDM_HandleTypeDef *hpdm);


3.4 失能PDM

.. code ::

    HAL_StatusTypeDef HAL_PDM_Stop(PDM_HandleTypeDef *hpdm);


3.5 PDM中断处理函数

.. code ::

    void HAL_PDM_IRQHandler(PDM_HandleTypeDef *hpdm);

3.6 在PDM中断处理函数中接受完FrameNum大小数据的回调函数

.. code ::

    void HAL_PDM_CpltCallback(PDM_HandleTypeDef *hpdm);

3.7 在DMA模式下接受完FrameNum大小pdm数据的回调函数

.. code ::

    void HAL_PDM_DMA_CpltCallback(PDM_HandleTypeDef *hpdm,uint8_t buf_idx);


