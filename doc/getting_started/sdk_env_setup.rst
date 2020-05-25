.. _env_setup:

软件开发环境搭建
================= 

我们支持如下开发环境：

#. Python 3 + VS Code + GCC(ARM)

#. MDK-KEIL

下载地址： 

`GCC(ARM) 9.2.1 20191025 <https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2019q4/gcc-arm-none-eabi-9-2019-q4-major-win32.zip?revision=20c5df9c-9870-47e2-b994-2a652fb99075&la=en&hash=347C07EEEB848CC8944F943D8E1EAAB55A6CA0BC>`_ 

`VS Code (64bit) <https://go.microsoft.com/fwlink/?Linkid=852157>`_ 

`Python 3.8.2 (64bit) <https://www.python.org/ftp/python/3.8.2/python-3.8.2-amd64.exe>`_

`MDK_KEIL <http://www.keil.com/fid/1u1q11wb9c9j1w9braw1mmudm139b2p1sd2wd1/files/eval/mdk530.exe>`_

Python 3 + VS Code + GCC(ARM)
##############################

* 安装Python 3(勾选安装pip模块、添加Python到PATH)、VS Code

* 解压GCC，将 ``{GCC_SETUP_DIR}/bin`` 路径添加到系统环境变量PATH中

* 进入SDK根目录，打开命令行，执行下述命令，安装Python依赖库::

    pip install -r requirements.txt

* 设置后缀名为.py的文件默认打开方式为Python，设置完成后，在命令行中执行::

    hexmerge.py -h

  若得到hexmerge帮助信息，则设置成功

* 用VS Code打开SDK目录，点击左侧Extensions（快捷键 ``Ctrl + Shift + X`` ），在搜索框输入 ``@recommended`` ,安装所有Workspace Recommendations插件

MDK-KEIL环境搭建
##############################

* keil版本需要安装5.25以上

* 打开下载的 `ls_ble_sdk <https://github.com/linkedsemi/ls_ble_sdk>`_ 目录，将当前目录下的 tools\\prog\\LinkedSemi\\flash_prog.elf 文件复制一份并修改文件名为flash_prog.FLM,并将flash_prog.FLM文件拷贝到keil安装目录下的 ARM\\Flash 路径中 

* 将fromelf执行文件的所在目录添加系统环境变量中，否则在使用keil编译时会报“fromelf不是内部或外部命令，也不是可运行的程序或批处理文件”的警告

* 仍需要安装 `Python 3.8.2 (64bit) <https://www.python.org/ftp/python/3.8.2/python-3.8.2-amd64.exe>`_ 及Python依赖库
