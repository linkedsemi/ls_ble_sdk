.. _uart_ref:

UART
======


初始化
-------

#. 为UART模块配置IO

#. 设置UART模块参数变量

#. 初始化UART模块

反初始化
---------

#. 反初始化UART模块

#. 反初始化UART IO

.. code ::

    #include "io_config.h"
    #include "lsuart.h"
    UART_HandleTypeDef UART_Config; 

    void uart1_init()
    {
        uart1_io_init(PB00,PB01);                                       // init step 1
        UART_Config.UARTX = UART1;
        UART_Config.Init.BaudRate = UART_BAUDRATE_115200;
        UART_Config.Init.MSBEN = 0;
        UART_Config.Init.Parity = UART_NOPARITY;
        UART_Config.Init.StopBits = UART_STOPBITS1;
        UART_Config.Init.WordLength = UART_BYTESIZE8;                  // init step 2
        HAL_UART_Init(&UART_Config);                                   // init step 3
    }

    void uart1_deinit()
    {
        HAL_UART_DeInit(&UART_Config);                                // deinit step 1
        uart1_io_deinit();                                            // deinit step 2
    }

.. note ::

    UART初始化动作会向系统注册UART进入工作状态，当系统检测到有任一外设处于工作状态时，都不会进入低功耗休眠。
    因此，UART使用完毕，需要进入低功耗状态之前，必须反初始化UART。

数据收发——非阻塞（中断）方式
-----------------------------





数据收发——阻塞方式
---------------------------