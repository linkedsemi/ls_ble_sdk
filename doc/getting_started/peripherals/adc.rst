.. _adc_ref:

ADC
======

ADC是Analog-to-Digital Converter的缩写。指模/数转换器或者模拟/数字转换器。是指将连续变量的模拟信号转换为离散的数字信号的器件。

一、初始化
--------------

1.1 设置ADC模块的IO
........................

    调用IO 的初始化接口，可以配置具体的IO的ADC模拟功能。

.. code:: c

   void adc12b_in0_io_init(void);
   void adc12b_in1_io_init(void);
   void adc12b_in2_io_init(void);
   void adc12b_in3_io_init(void);
   void adc12b_in4_io_init(void);
   void adc12b_in5_io_init(void);
   void adc12b_in6_io_init(void);
   void adc12b_in7_io_init(void);
   void adc12b_in8_io_init(void);

.. note::

    每一个ADC的通道都有其对应的IO，不可随意映射。具体的对应关系如下：

    ========= =============
    GPIO      ANA_FUNC1
    ========= =============
    GPIO_PB12 ADC12B_AIN[0]
    GPIO_PB13 ADC12B_AIN[1]
    GPIO_PC00 ADC12B_AIN[2]
    GPIO_PC01 ADC12B_AIN[3]
    GPIO_PA00 ADC12B_AIN[4]
    GPIO_PA01 ADC12B_AIN[5]
    GPIO_PA02 ADC12B_AIN[6]
    GPIO_PA03 ADC12B_AIN[7]
    GPIO_PA04 ADC12B_AIN[8]
    ========= =============

    另外ADC模块还有对应的内部通道，分布如下：

    ============================= ==============
    内部输入信号                  内部输入通道
    ============================= ==============
    芯片工作温度                  ADC12B_AIN[9]
    芯片工作电压 Vbat信号         ADC12B_AIN[10]
    芯片内部ADC参考电压(标准1.4v) ADC12B_AIN[11]
    ============================= ==============

1.2 设置ADC模块的参数变量
...........................

设置ADC模块的参数变量，其结构体的参数原型如下：

.. code:: c

    /**
      * @struct __ADC_HandleTypeDef
      * @brief  ADC handle type Structure definition  
      */
    typedef struct __ADC_HandleTypeDef
    {
        reg_adc_t       *Instance; /*!< Register base address */
        ADC_InitTypeDef Init; 
        void            *DMAC_Instance;
        union{
            struct AdcDMAEnv DMA;
            struct AdcInterruptEnv Interrupt;
        }Env;
        HAL_LockTypeDef Lock; 
        volatile uint32_t State; 
        volatile uint32_t ErrorCode; 
 
    } ADC_HandleTypeDef;


**参数说明**

（1） ADC寄存器结构化处理(Instance)

    -  目前LE5010/LE5110 仅支持一个ADC

    -  ADC 基地址 0x40012400

（2） ADC初始化处理(Init)

    -  请参见ADC初始化 1.2.1

（3） DMA寄存器结构化处理(DMAC_Instance)

    -  请参见DMA初始化 1.2.2

（4） ADC转换的触发方式(Env)

    -  ADC-DMA触发方式

       设置DMA通道和Callback函数(规则组转换)

       .. code:: c

          /**
            * @struct AdcDMAEnv
            * @brief  ADC DMA Structure definition  
            */
           struct AdcDMAEnv
          {
              void                          (*Callback)();
              uint8_t                       DMA_Channel;    
          };

    -  ADC中断单次触发方式

       设置读取ADC数据的变量(单次转换)

       .. code:: c

          /**
            * @struct AdcInterruptEnv
            * @brief  ADC Interrupt Structure definition  
            */
          struct AdcInterruptEnv
          {
              uint8_t                       *pBuffPtr;      /*!< Pointer to ADC data Buffer */
              uint16_t                      XferCount;      /*!< UART ADC data Counter */
          };

1.2.1ADC初始化
+++++++++++++++++
.. code:: c

    /** 
      * @struct  ADC_InitTypeDef
      * @brief  Structure definition of ADC and regular group initialization 
      * @note   Parameters of this structure are shared within 2 scopes:
      *          - Scope entire ADC (affects regular and injected groups): DataAlign, ScanConvMode.
      *          - Scope regular group: ContinuousConvMode, NbrOfConversion, DiscontinuousConvMode, NbrOfDiscConversion,ExternalTrigConv.
      * @note   The setting of these parameters with function HAL_ADC_Init() is conditioned to ADC state.
      *         ADC can be either disabled or enabled without conversion on going on regular group.
      */
    typedef struct
    {
        uint32_t DataAlign;            
        uint32_t ScanConvMode;               
        FunctionalState ContinuousConvMode;  
        uint32_t NbrOfConversion;           
        FunctionalState DiscontinuousConvMode; 
        uint32_t NbrOfDiscConversion;        
        uint32_t TrigType;  
        uint32_t VrefType;
        uint32_t AdcDriveType;
        uint32_t AdcCkDiv;
    } ADC_InitTypeDef;
 
 -  参数说明：
 
（1） 数据对齐(DataAlign)
 
        -  默认情况，ADC 转换后的数据采用右对齐方式(bit11:0)。
 
        -  设置左对齐方式，ADC 转换后的数据：
 
        -  规则组转换数据寄存器(ADC_RDR)
 
        -  注入组转换数据寄存器((ADC_JDRx)： *Raw Converted Data +
           JnjectOffset*
 
（2） 扫描模式(ScanConvMode)
 
        -  禁止
 
        ..
 
           单通道单次转换
 
           参数：NbrOfConversion无效
 
           参数：NbrOfDiscConversion无效
 
        -  使能
 
        ..
 
           规则组序列转换或注入组序列转换的条件下需要使能
 
           序列连续转换模式
 
           序列间断转换模式
 
           参数：NbrOfConversion有效
 
           参数：NbrOfDiscConversion有效
 
（3） 连续转换模式(ContinuousConvMode)
 
        -  禁止
 
        ..
 
           单通道单次转换
 
        -  使能
 
        ..
 
           连续多通道转换
 
（4） 连续转换的次数(NbrOfConversion)
 
        -  规则组序列转换
 
        -  转换次数范围：1～12
 
        -  参数：ScanConvMode 必须使能
 
（5） 间断转换模式(DiscontinuousConvMode)
 
        -  规则组子序列转换
 
        -  参数：ScanConvMode 必须使能
 
        -  参数：ContinuousConvMode 必须禁止
 
（6） 间断转换的次数(NbrOfDiscConversion)
 
        -  规则组转换子序列数
 
        -  转换次数范围：≤9
 
        -  参数：DiscontinuousConvMode 必须使能
 
（7） 触发转换的方式(TrigType)
 
        -  规则组转换的触发方式
 
        -  注入组转换的触发方式
 
（8） 选择参考电压(VrefType)
 
        -  默认芯片内部1.4V为参考电压
 
        -  GPIO输入参考电压
 
        -  芯片工作电压AVDD为参考电压
 
（9） ADC通道的驱动方式(AdcDriveType)
 
        -  输入信号经过输入buf运放驱动ADC
 
        -  输入信号1/3分压，并经过输入buf运放驱动ADC
 
        -  默认关闭输入buf运放，输入信号直接驱动ADC
 
（10） ADC时钟分频系数(AdcCkDiv)
 
        -  系统时钟按AdcCkDiv分频获得ADC运行时钟


1.3 初始化ADC模块
..................

.. code:: c

   HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef *hadc);

二、反初始化
---------------

2.1 反初始化ADC模块
....................

通过反初始化函数，根据场景需求可以关闭ADC模块，可以降低系统的功耗。

.. code:: c

   HAL_StatusTypeDef HAL_ADC_DeInit(ADC_HandleTypeDef *hadc);

2.2 反初始化ADC IO
......................
   
根据场景需求通过反初始化函数，可以关闭ADC模块，对应的模拟IO反初始为普通GPIO。

.. code:: c

       
   void adc12b_in0_io_deinit(void);
   void adc12b_in1_io_deinit(void);
   void adc12b_in2_io_deinit(void);
   void adc12b_in3_io_deinit(void);
   void adc12b_in4_io_deinit(void);
   void adc12b_in5_io_deinit(void);
   void adc12b_in6_io_deinit(void);
   void adc12b_in7_io_deinit(void);
   void adc12b_in8_io_deinit(void);

.. note::

    由于ADC外部输入电压的不确定性，不好配置内部IO的状态，所以在使用ADC功能，在进入休眠之后，IO内部电平状态与外部输入电压易产生压差，导致出现部分漏电。

三、ADC模块采集数据

ADC模块采集数据我们一共提供了三种接口，规则通道采集，注入通道采集和DMA采集。在配置完初始化相关信息之后，需要调用相应的API接口，让ADC模块开始工作。

3.1 数据采集——规则通道
..........................



.. code ::
    
    HAL_StatusTypeDef HAL_ADC_Start_IT(ADC_HandleTypeDef *hadc);

3.2 数据采集——注入通道
.......................

.. code ::
    
    HAL_StatusTypeDef HAL_ADCEx_InjectedStart_IT(ADC_HandleTypeDef* hadc);

3.3 数据采集——DMA模式
..........................

.. code ::
    
    HAL_StatusTypeDef HAL_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint16_t* pData, uint32_t Length,void (*Callback)());

示例代码：
 
参考：<install_file>/dev/examples/adc_test/adc_single_channel
