BLE_UART_SERVER（串口透传）示例说明
===================================

例程路径：<install_file>/dev/examples/ble/ble_uart_server

一、示例基本配置、流程及说明:
-----------------------------
BLE_UART_SERVER（以下简称uart_server）是具备蓝牙串口透传功能且无安全要求的单连接示例。串口透传，指的是作为无线数据传输通道，蓝牙芯片将Uart上收到的数据不经任何处理直接发送给蓝牙对端，同时也将蓝牙收到的数据推送到Uart上。

..  image:: ble_uart_server_flowchart.png

1.1 准备工作之一：串口相关初始化
++++++++++++++++++++++++++++++++
串口透传，首先需要初始化串口相关的功能模块，主要包括是能串口硬件模块，以及创建一个用于数据查询和处理的软件定时器。调用的相关接口如下：

.. code ::

        ls_uart_init(); // 串口模块初始化
        HAL_UART_Receive_IT(&UART_Server_Config, &uart_server_rx_byte, 1);  // 串口接收使能，每次接收1byte，存放到uart_server_rx_byte内
        ls_uart_server_init(); // 初始化软件定时器
		
串口参数默认配置IO为PB00/PB01，波特率为115200，具体可以参考ls_uart_init()的实现。软件定时器周期配置为50ms。

1.2 准备工作之二：服务添加及注册
++++++++++++++++++++++++++++++++
串口透传需要添加uart_server服务，这里主要包括服务定义和服务内部的特征值/描述符定义。

.. code ::

    static const struct att_decl ls_uart_server_att_decl[UART_SVC_ATT_NUM] =
    {
        [UART_SVC_IDX_RX_CHAR] = {
            .uuid = att_decl_char_array,
            .s.max_len = 0,
            .s.uuid_len = UUID_LEN_16BIT,
            .s.read_indication = 1,   
            .char_prop.rd_en = 1,
        },
        [UART_SVC_IDX_RX_VAL] = {
            .uuid = ls_uart_rx_char_uuid_128,
            .s.max_len = UART_SVC_RX_MAX_LEN,
            .s.uuid_len = UUID_LEN_128BIT,
            .s.read_indication = 1,
            .char_prop.wr_cmd = 1,
            .char_prop.wr_req = 1,
        },
        [UART_SVC_IDX_TX_CHAR] = {
            .uuid = att_decl_char_array,
            .s.max_len = 0,
            .s.uuid_len = UUID_LEN_16BIT,
            .s.read_indication = 1,
            .char_prop.rd_en = 1, 
        },
        [UART_SVC_IDX_TX_VAL] = {
            .uuid = ls_uart_tx_char_uuid_128,
            .s.max_len = UART_SVC_TX_MAX_LEN,
            .s.uuid_len = UUID_LEN_128BIT,
            .s.read_indication = 1,
            .char_prop.ntf_en = 1,
        },
        [UART_SVC_IDX_TX_NTF_CFG] = {
            .uuid = att_desc_client_char_cfg_array,
            .s.max_len = 0,
            .s.uuid_len = UUID_LEN_16BIT,
            .s.read_indication = 1,
            .char_prop.rd_en = 1,
            .char_prop.wr_req = 1,
        },
    };
    static const struct svc_decl ls_uart_server_svc =
    {
        .uuid = ls_uart_svc_uuid_128,
        .att = (struct att_decl*)ls_uart_server_att_decl,
        .nb_att = UART_SVC_ATT_NUM,
        .uuid_len = UUID_LEN_128BIT,
    };

这里的定义大多符合SIG关于服务/特征值/描述符的规范的，具体可参照代码实现及蓝牙官方core协议。这里重点提一下一些自定义配置：
1、max_len为该特征值操作的最大长度，单位为byte，对于改特征值的操作不可以超出max_len，否则多余的部分会被丢弃
2、read_indication表示收到对方的读请求时，是否将该请求发送至应用层，通常配置都为1

添加服务需要首先调用

.. code ::

    dev_manager_add_service((struct svc_decl *)&ls_uart_server_svc);

之后会上SERVICE_ADDED消息，再调用

.. code ::

    gatt_manager_svc_register(evt->service_added.start_hdl, UART_SVC_ATT_NUM, &ls_uart_server_svc_env);
    
1.3 发广播包/建立连接
+++++++++++++++++++++
在uart_server服务注册完成后，所有准备工作已经完成，此时可以调用create_adv_obj()创建广播对象。函数具体实现为：

.. code ::

    static void create_adv_obj()
    {
        struct legacy_adv_obj_param adv_param = {
            .adv_intv_min = 0x20,
            .adv_intv_max = 0x20,
            .own_addr_type = PUBLIC_OR_RANDOM_STATIC_ADDR,
            .filter_policy = 0,
            .ch_map = 0x7,
            .disc_mode = ADV_MODE_GEN_DISC,
            .prop = {
                .connectable = 1,
                .scannable = 1,
                .directed = 0,
                .high_duty_cycle = 0,
            },
        };
        dev_manager_create_legacy_adv_object(&adv_param);
    }
    
1、adv_intv_min/adv_intv_max分别表示广播包的最小和最大周期，单位为625us，一般配置成同一个值

2、ch_map表示每组广播包的个数，默认为7，表示在37/38/39这3个channel上都会发送

3、uart_server示例里，connectable必须为1，否则为不可连接广播包，后续无法建立连接

在广播对象创建完成后，调用start_adv()开启广播。在这一步需要特别注意，组建advertising_data和scan_response_data内容需要通过使用宏ADV_DATA_PACK，返回值为最终的长度，作为参数之一传入dev_manager_start_adv()。如果没有advertising_data或scan_response_data，对应的length需要填0。**不可以填如与实际内容不匹配的length，例如sizeof(advertising_data)，否则协议栈的解析有可能出错！**
广播包发出来之后，可以通过手机APP（例如nRF Connect）扫描该设备的广播包，并建立连接，成功后本地能在gap_manager_callback()里收到CONNECTED消息，手机端也会自动进行服务发现流程，通常如下：

..  image:: nRF_Connect_connection_screen_capture.png

二、示例验证步骤及结果:
-------------------------
串口发送68 17 00 43 05 66 55 44 33 22 11 00 1B 6E 05 01 00 F3 01 02，可以在手机APP上收到该数据；同样的，手机APP推送0x12345678，也可以在串口上打印出来，如下图：

..  image:: nRF_Connect_rx_tx_result.png

..  image:: SSCom_rx_tx_result.png

三、特别说明:
-------------------------

1、关于数据从串口接收到蓝牙发送的处理
++++++++++++++++++++++++++++++++++++++++++

为什么使用软件定时器轮询处理数据收发？由于uart_server是单连接数据透传，因此串口上的数据是没有固定格式的，无法预知后后续会有多少字节的数据会被收到，因此每次只能接收1字节。接收1字节完成后，串口接收完成callback函数HAL_UART_RxCpltCallback()会被调用，将收到的数据保存到全局buffer里。而将收到的串口数据推送的蓝牙对端的动作在定时器里做，不在HAL_UART_RxCpltCallback()里做的原因，主要是考虑到中断回调函数里不宜有过多逻辑处理，且每次收到1字节就启动蓝牙发送会导致数据收发效率低下，因此在软件定时器里周期性检查、处理接收的数据是更为合理的选择。

2、关于MTU
++++++++++++++++++

MTU是BLE ATT的概念，它定义了在ATT层Client与Server之间任意数据包的最大长度。另外，由于send_notification和write cmd在GATT层不需要对端回复response，这导致调用这两个接口进行数据发送时，一旦传入的length超出了一定范围（MTU - 3），超出该范围的数据会被丢弃。所以在uart server里，会对串口接收到的数据进行处理，每次调用send_notification接口进行数据发送时都会保证传入的数据都会被协议栈接收并处理。
至于为何不选择有回复的send_indication/write request，是因为这两个命令要求数据接收方在GATT层必须回复response（空包，无实际数据内容），这会降低数据通信的效率，因此使用较少。

3、关于通信速率评估
+++++++++++++++++++++++

既然是透传，某些特殊场景下可能就需要考虑数据通信速率。uart server的实际通信速率会受若干因素影响，比如串口波特率、CPU处理速度、MTU长度、蓝牙射频性能、设备距离远近以及外部环境干扰等等，
因此应用对通信速率如果有要求，需要事先评估。例如，115200的波特率，串口通信最大速率在100Kb/s左右，那么整个系统的透传速率就不可能超过这个理论值；MTU默认23字节时的通信速率，也肯定会低于更大MTU配置时的值，因此执行MTU Exchange也是提高通信速率的方式之一，等等。

4、关于更新广播间隔
+++++++++++++++++++++++++++

更新广播间隔并非uart_server必备的功能，只是放在这个demo里作为一个简单的示例。在软件定时器里，会去检查RTT input，如果检测到有字符输入，且是‘1’-‘9’之间，同时又有广播包在发送，则会调用ls_uart_server_update_adv_interval()更新广播包间隔。例如输入字符‘5’，则广播间隔会被更新为500ms。
