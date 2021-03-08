.. _pdm_ref:

PDM
======

PDM（Pulse Density Modulation）是一种用数字信号表示模拟信号的调制方法。PDM只有1位数据输出，要么为0，要么为1。

一、初始化
------------

1. PDM模块的IO配置
++++++++++++++++++++++++

    | 调用IO 的初始化接口，可以将任意IO复用为pdm的colck data0或data1引脚。

.. code ::

    void pdm_clk_io_init(uint8_t pin);
    void pdm_data0_io_init(uint8_t pin);
    void pdm_data1_io_init(uint8_t pin);

.. note ::

 | 1. 芯片的IO 一共有34个，具体情况需根据封装图来定义。
 | 2. 为了避免不必要的bug，在使用pdm通信的时候，请先初始化IO，再进行下列参数的配置。

2. 初始化PDM模块
++++++++++++++++++++++++

2.1 PDM结构体参数原型如下：

.. code ::

    typedef struct __PDM_Init_TypeDef
    {
        const struct pdm_fir *fir;  /*!< pdm filter controller configure */
        PDM_CFG_TypeDef cfg;        /*!< pdm clock rate, capture delay, sampling rate, and data gain configure */
        PDM_MODE_TypeDef mode;      /*!< pdm channel mode configure */
    }PDM_Init_TypeDef;

2.2 调用初始化PDM模块函数接口

    通过初始化接口，应用程序可以对PDM进行参数配置。

.. code ::

    HAL_StatusTypeDef HAL_PDM_Init(PDM_HandleTypeDef *hpdm,PDM_Init_TypeDef *Init);

3.PDM初始化参考代码如下：
++++++++++++++++++++++++++

.. code ::

    #define PDM_CLK_KHZ 1024
    #define PDM_SAMPLE_RATE_HZ 16000
    void pdm_init()
    {
        pdm_clk_io_init(PB10);  /*!< PB10复用为pdm clk引脚 */
        pdm_data0_io_init(PB09);    /*!< PB09复用为pdm data0引脚 */  
        pdm.Instance = LSPDM;   /*!< PDM外设的基址 */
        PDM_Init_TypeDef Init = 
        {
            .fir = PDM_FIR_COEF_16KHZ,  /*!< 配置PDM的滤波控制器 */
            .cfg = {
                .clk_ratio = PDM_CLK_RATIO(PDM_CLK_KHZ),    /*!< 配置PDM的时钟频率为1.024MHZ */
                .sample_rate = PDM_SAMPLE_RATE(PDM_CLK_KHZ,PDM_SAMPLE_RATE_HZ), /*!< 配置PDM采样频率为16KHZ */
                .cap_delay = 30,    /*!< 配置捕获延迟为30 */
                .data_gain = 5,     /*!< 配置数据增益为5 */
            },
            .mode = PDM_MODE_Mono,  /*!< 配置PDM为单通道模式 */
        };
        HAL_PDM_Init(&pdm,&Init);   /*!< 调用PDM初始化函数 */
    }




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

3.6 在PDM中断处理函数中接收完FrameNum大小数据的回调函数

.. code ::

    void HAL_PDM_CpltCallback(PDM_HandleTypeDef *hpdm);

3.7 在DMA模式下接收完FrameNum大小pdm数据的回调函数

.. code ::

    void HAL_PDM_DMA_CpltCallback(PDM_HandleTypeDef *hpdm,uint8_t buf_idx);


