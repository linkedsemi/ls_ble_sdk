编译构建、调试简介
===================

编译构建
-------------

通过VS Code打开SDK **根目录** ，通过快捷键 ``Ctrl + ``` 打开Terminal，执行::

    $ scons
    
会编译所有示例程序，生成文件保存在SDK ``build/examples/`` 目录下

生成文件
~~~~~~~~~
一般情况，每个示例工程会生成.asm，.elf，.hex，.map四个文件。

**.elf**
    包含调试信息的工程编译链接输出

**.asm**
    由.elf文件导出的反汇编

**.map**
    编译链接生成的符号和交叉引用信息

**.hex**
    由.elf文件导出的Intel Hex格式程序镜像，用于烧录


调试
-------------

.. note:: 在调试前，请将JLink安装路径（JLinkGDBServerCL.exe 所在目录） ``{JLINK_SETUP_DIR}`` 添加到系统环境变量PATH。并将SDK ``tools/prog/`` 目录下所有内容拷贝到 ``{JLINK_SETUP_DIR}`` 覆盖原有文件。

打开VS Code调试视图（快捷键 ``Ctrl + Shift + D`` ），可以选择需要调试的程序。

**Debug {example 1}**
    表示复位系统，下载 ``{example 1}`` 程序到Flash中，并从头运行


**Attach {example 1}**
    表示以 ``{example 1}`` 为程序，调试器直接连接到当前运行现场
