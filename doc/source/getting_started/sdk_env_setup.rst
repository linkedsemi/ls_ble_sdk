软件开发环境搭建
==================

我们支持两类开发环境：

#. Python 3 + VS Code + GCC(ARM)
#. Python 3 + Keil(MDK)

GCC(ARM) ：https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2019q4/gcc-arm-none-eabi-9-2019-q4-major-win32.zip?revision=20c5df9c-9870-47e2-b994-2a652fb99075&la=en&hash=347C07EEEB848CC8944F943D8E1EAAB55A6CA0BC

VS Code ：https://go.microsoft.com/fwlink/?Linkid=852157

Python 3 : https://www.python.org/ftp/python/3.8.2/python-3.8.2.exe

Python 3 + VS Code + GCC(ARM)
##############################

* 安装Python 3、VS Code，解压GCC

* 进入SDK根目录，打开命令行，执行:

    ::
        pip install -r requirements.txt

安装Python依赖库

*  编辑tools/arm-gcc.py，修改env['TOOLCHAIN_PATH']为GCC解压后可执行文件所在目录

* 打开VS Code，点击左侧Extensions，安装所有Workspace Recommendations插件







