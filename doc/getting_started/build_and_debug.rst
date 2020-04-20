编译构建、调试简介
===================

编译构建
-------------

通过VS Code打开SDK根目录，通过快捷键 ``Ctrl + ``` 打开Terminal，执行::

    $ scons
    
会编译所有示例程序，生成文件保存在 ``build/examples/`` 目录下


调试
-------------

.. note:: 在调试前，请将JLink安装路径（JLinkGDBServerCL.exe 所在目录）添加到系统环境变量。

打开VS Code调试视图（快捷键 ``Ctrl + Shift + D`` ），可以选择需要调试的程序。

**Debug {example 1}**
    表示复位系统，下载 ``{example 1}`` 程序到Flash中，并从头运行


**Attach {example 1}**
    表示以 ``{example 1}`` 为程序，调试器直接连接到当前运行现场
