
Keil+JLink 构建、烧录、调试
============================

构建
-------------

完成keil环境安装后，以ble_dis工程为例，打开ls_ble_sdk\\dev\\examples\\ble_dis\\mdk路径下的工程文件

1. 选择 J-Link 作为调试工具

 .. image:: MDK_01.png

2. 选择SW作为调试

 .. image:: MDK_02.png

3. 在 flash download 选项卡中配置下载选项，不能选择 ``Erase Full Chip`` 选项

 .. image:: MDK_03.png

4. 设置使用 Debug Drive 进行 flash 的烧录 

 .. image:: MDK_04.png

5. 选择使用创建 Hex 文件 

 .. image:: OutputHex.png

烧录
--------------
#. **务必** 先完成 :ref:`vscode_debug` 中的JLINK烧录、调试的准备工作，以及 :ref:`env_setup` 中Python 3 + MDK-KEIL的环境搭建；

#. 打开 ``ble_sdk_app\dev\examples\sbl\mdk`` 路径下的工程,编译生成 sbl\\mdk\\UVBuild\\info_sbl.hex 文件；

#. 使用JFlash工具将 info_sbl.hex 和 dev\\soc\\arm_cm\\le501x\\bin\\fw.hex 分别烧录到芯片内；

#. 在通过Keil烧录之前,必须要先确保芯片内已经烧录了 dev\\soc\\arm_cm\\le501x\\bin\\fw.hex 和 info_sbl.hex文件。如果已经烧录了这两个文件并且没有执行过flash全部擦除的指令,则可以不用重复烧录 fw.hex 和 info_sbl.hex； 

#. 使用keil图形界面的DownLoad选项,将编译后的文件下载到flash中
 .. image:: DownLoad.png

调试
-------------
#. 使用keil图形界面的 Start/Stop Debug Session选项进入调试模式
 .. image:: MDK_Debug.png