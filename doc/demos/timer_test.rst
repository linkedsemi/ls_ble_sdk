TIMER 使用实例
==================

测试例程说明：
----------------
SDK提供了Timer常用到的4种场景：

多通道输出PWM：<install_file>/dev/examples/peripheral/timer/Basic_PWM

基本定时功能：<install_file>/dev/examples/peripheral/timer/Basic_TIM

带死区嵌入的互补PWM输出：<install_file>/dev/examples/peripheral/timer/DTC_PWM

输入捕获功能：<install_file>/dev/examples/peripheral/timer/Input_Capture

多通道输出PWM
>>>>>>>>>>>>>>>

该例程演示了LSGPTIMB的四个通道输出PWM波形,频率4KHz，占空比不一样

#. 将编译出来的程序烧录到芯片中，运行程序

#. 将芯片的PA00、PA01、PB14、PB15四个IO接到逻辑分析仪或者示波器

#. 四个IO输出的波形，如下图所示

 .. image:: MultichannelPWM.png

基本定时
>>>>>>>>>>>>>>>

该例程演示了基本定时器的功能，每250us会上一次定时器中断，我们在中断里面翻转了PA00 IO

#. 将编译出来的程序烧录到芯片中，运行程序

#. 将芯片的PA00 IO接到逻辑分析仪或者示波器

#. PA00 电平翻转波形，如下图所示

 .. image:: BaseTIM.png

带死区嵌入的互补输出
>>>>>>>>>>>>>>>>>>>>

#. 将编译出来的程序烧录到芯片中，运行程序

#. 将芯片的PA00、PA01两个IO接到逻辑分析仪或者示波器

#. 输出的波形，如下图所示

 .. image:: DTCPWM.png

脉宽测量输入捕获
>>>>>>>>>>>>>>>>>>

该例程中我们选用的是LSGPTIMC的CH1,使用PA00这个GPIO来测量信号的脉宽，测试模块上PA00接一个按键，接到VDD，当按键按下的时候IO口会被拉高，这个时候我们可以利用定时器的输入捕获功能来测量按键按下的这段高电平的时间。另外按键按下和松开的时候也会Toggle PA01 GPIO.

#. 将编译出来的程序烧录到芯片中，运行程序

#. 通过LOG信息可以查看捕获到的高电平时间
