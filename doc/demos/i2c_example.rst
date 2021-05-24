IIC设备使用示例
=================

例程路径：<install_file>/dev/examples/peripheral/i2c

操作步骤:
-----------------
#. iic测试例程是使用我们的蓝牙芯片和EEPROM（AT24CXX）进行通信，整个流程为：首先将数组"aTxBuffer"的数据写入EEPROM中，然后再从EEPROM中读出来并写入数组"aRxBuffer"。

#. 将编译好的程序下载到测试的蓝牙芯片中，注意需要下载 fw.hex, info_sbl.hex, i2c_test.hex 这三个文件。

#. 将蓝牙芯片的iic接口（程序中设置的IO是PB12(SDA)\PB13(SCL)）接到EEPROM模块的SDA/SCL上。给EEPROM模块供电，同时两者的地线要接到一起。

#. 使用编译器仿真可以比较两个数组的值或者使用"LOG_I"函数在"J-Link RTT Viewer"软件中打印出来。

#. 比较结果。

预期结果:
-----------------
#. aTxBuffer[12] = {0x00,0x10,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,}注:0x00与0x10为iic写入数据的地址

#. aRxBuffer[10] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,}经比较数据部分完全吻合。