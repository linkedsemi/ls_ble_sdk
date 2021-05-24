CRYPT设备使用示例
=================

例程路径：<install_file>/dev/examples/peripheral/crypt

操作简介:
-----------------
crypt测试文件夹中有两个测试例程，这个两个例程分别为阻塞模式与中断模式的例程，在这两个模式下分别测试了：

#. ECB模式下，密钥长度分别为128位、192位与256位加解密的例程。

#. CBC模式下，密钥长度分别为128位、192位与256位加解密的例程。

所有例程测试的流程均是先将明文进行加密然后与我们的密文进行对比，（住：我们的所有明文与密文均是在文档中进行复制过来的，也就是说这个密文都是经过验证过的，并且是绝对无误的）如果一致则打印加密成功，否则打印加密失败。

操作步骤:
-----------------

#. 将编译好的程序下载到测试的蓝牙芯片中，注意需要下载 info_sbl.hex, crypt_test.hex 这两个文件，或者只下载crypt_test_production.hex这一个文件也可以，（住：以“_production”这个文件名结尾的均为合并文件，只用下载这一个文件便可）。

#. 使用编译器仿真可以比较两个数组的值或者使用"LOG_I"函数在"J-Link RTT Viewer"软件中打印出来。

预期结果:
-----------------
在"J-Link RTT Viewer"软件中打印:
I/NO_TAG:CRYPT_AES_ECB_ENCRYPT_128_TEST_SUCCESS!
I/NO_TAG:CRYPT_AES_ECB_DECRYPT_128_TEST_SUCCESS!
I/NO_TAG:CRYPT_AES_CBC_ENCRYPT_128_TEST_SUCCESS!
I/NO_TAG:CRYPT_AES_CBC_DECRYPT_128_TEST_SUCCESS!
I/NO_TAG:CRYPT_AES_ECB_ENCRYPT_192_TEST_SUCCESS!
I/NO_TAG:CRYPT_AES_ECB_DECRYPT_192_TEST_SUCCESS!
I/NO_TAG:CRYPT_AES_CBC_ENCRYPT_192_TEST_SUCCESS!
I/NO_TAG:CRYPT_AES_CBC_DECRYPT_192_TEST_SUCCESS!
I/NO_TAG:CRYPT_AES_ECB_ENCRYPT_256_TEST_SUCCESS!
I/NO_TAG:CRYPT_AES_ECB_DECRYPT_256_TEST_SUCCESS!
I/NO_TAG:CRYPT_AES_CBC_ENCRYPT_256_TEST_SUCCESS!
I/NO_TAG:CRYPT_AES_CBC_DECRYPT_256_TEST_SUCCESS!
