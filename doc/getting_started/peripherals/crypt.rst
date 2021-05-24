.. _crypt_ref:

CRYPT
==============

一、CRYPT简介
--------------

硬件加密模块主要用于由硬件对数据进行加密或解密操作，支持的标准有AES、DES。
AES（Advanced Encryption Standard）是最新的分组对称密码算法，兼容联邦信息处理标准出版物（FIPSPUB197，2001年11月26日）规定的高级加密标准（AES）。
DES（Data Encryption Standard）是应用非常广泛的对称密码算法，兼容联邦信息处理标准出版物（FIPS PUB 46-3，1999年10月25日）规定的数据加密标准（DES）和三重DES（TDES），遵循美国国家标准协会（ANSI）X9.52标准。

..  image:: crypt_01.png

AES
    支持128、192和256位的密钥
    支持ECB、CBC模式
    支持1、8、16或32位数据交换

DES/TDES
    支持ECB、CBC模式
    支持64、128和192位密钥
    支持在CBC模式下使用的2×32位初始化向量（IV）
    支持1、8、16或32位数据交换

AES-ECB模式加密:
在 AES-ECB 加密中，执行位/字节/半字交换后，128位明文数据块将用作输入块。输入块通过AEA在加密状态下使用128位、192位或256位密钥进行处理。执行位/字节/半字交换后，生成的128位输出块将用作密文。该密文随后给到结果寄存器。

AES-ECB模式解密:
要在ECB模式下执行AES解密，需要准备密钥（需要针对加密执行完整的密钥计划），方法为:收集最后一个轮密钥并将其用作解密密文的第一个轮密钥。该准备过程通过AES内核计算完成。
在 AES-ECB 解密中，执行位/字节/半字交换后，128位密文块将用作输入块。该密钥序列与加密处理中的密钥序列相反。执行位/字节/半字交换，生成的128位输出块将产生明文。

AES-CBC模式加密:
在 AES-CBC 加密中，执行位/字节/半字交换后所获得的第一个输入块是通过一个128位初始化向量IV与第一个明文数据块进行异或运算形成的。输入块通过AEA在加密状态下使用128位、192位或256位密钥进行处理。生成的128位输出块将直接用作密文。然后，第一个密文块与第二个明文数据块进行异或运算，从而生成第二个输入块。第二个输入块通过AEA处理而生成第二个密文块。此加密处理会不断将后续密文块和明文块链接到一起，直到消息中最后一个明文块得到加密为止。如果消息中包含的数据块数不是整数，则应按照应用程序指定的方式对最后的不完整数据块进行加密。
在 CBC 模式下（如 ECB 模式），必须准备密钥才可以执行 AES 解密。

AES-CBC 模式解密:
在 AES-CBC 解密中，第一个128位密文块将直接用作输入块。输入块通过AEA在解密状态下使用128位、192位或256位密钥进行处理。生成的输出块与128位初始化向量 IV（必须与加密期间使用的相同）进行异或运算，从而生成第一个明文块。然后，第二个密文块将用作下一个输入块，并由AEA进行处理。生成的输出块与第一个密文块进行异或运算，从而生成第二个明文数据块。AES-CBC解密过程将以此方式继续进行，直到最后一个完整密文块得到解密为止。必须按照应用程序指定的方式对不完整数据块密文进行解密。

二、CRYPT接口介绍
----------------------
2.1 初始化:
----------------------
首先需要进行CRYPT模块初始化。


.. code ::

    HAL_LSCRYPT_Init(void);
    

由于在进行加密解密的过程中需要使用到一个相同的key，
这个key用来加密明文的密码，在对称加密算法中，加密与解密的密钥是相同的。
密钥为接收方与发送方协商产生，但不可以直接在网络上传输，否则会导致密钥泄漏，最终导致明文被还原。
所以还需要输入一个key。如下代码所示：

.. code ::

    HAL_LSCRYPT_AES_Key_Config(const uint32_t *key, enum aes_key_type key_size);

    
在加密中我们可以有三种key的类型，如下代码所示：

.. code ::

    enum aes_key_type
    {
        AES_KEY_128 = 0,
        AES_KEY_192,
        AES_KEY_256,
    };

2.2 CRYPT加解密:
------------------

**参数描述**

.. note ::

    #. plaintext:明文数据，没有经过加密过后的数据。
    #. ciphertext:密文数据，经过加密过后的数据
    #. iv:CBC模式下使用的2×32位初始化向量（IV）

2.2.1 轮询模式
......................

.. code ::

    HAL_LSCRYPT_AES_ECB_Encrypt(const uint8_t *plaintext,uint32_t length,uint8_t *ciphertext);
    HAL_LSCRYPT_AES_ECB_Decrypt(const uint8_t *ciphertext,uint32_t length,uint8_t *plaintext);
    /*ECB模式下以轮询模式对数据进行加密与解密*/
    HAL_LSCRYPT_AES_CBC_Encrypt(const uint32_t iv[4],const uint8_t *plaintext,uint32_t length,uint8_t *ciphertext);
    HAL_LSCRYPT_AES_CBC_Decrypt(const uint32_t iv[4],const uint8_t *ciphertext,uint32_t length,uint8_t *plaintext);
    /*CBC模式下以轮询模式对数据进行加密与解密*/

2.2.2 中断模式
......................

.. code ::

    HAL_LSCRYPT_AES_ECB_Encrypt_IT(const uint8_t *plaintext,uint32_t length,uint8_t *ciphertext);
    HAL_LSCRYPT_AES_ECB_Decrypt_IT(const uint8_t *ciphertext,uint32_t length,uint8_t *plaintext);
    /*ECB模式下以中断模式对数据进行加密与解密*/
    HAL_LSCRYPT_AES_CBC_Encrypt_IT(const uint32_t iv[4],const uint8_t *plaintext,uint32_t length,uint8_t *ciphertext);
    HAL_LSCRYPT_AES_CBC_Decrypt_IT(const uint32_t iv[4],const uint8_t *ciphertext,uint32_t length,uint8_t *plaintext);
    /*CBC模式下以中断模式对数据进行加密与解密*/

2.2.3 回调函数 
......................
.. code ::

    HAL_LSCRYPT_AES_Complete_Callback(bool Encrypt,bool CBC);
    {

    }
.. note ::
    
    | 函数中的“Encrypt”为true时选择的是加密模式（Encrypt），当为false时选择的是解密模式（Decrypt）。
    | 函数中的“CBC”为true时选择的是CBC模式，当为false时选择的是ECB模式。
    | 这个函数属于弱定义，用户可以自行定义，并完成相应的逻辑处理。

2.3 反初始化
---------------

反初始化CRYPT模块
.........................

通过反初始化接口，应用程序可以关闭CRYPT外设，从而在运行BLE的程序的时候，降低系统的功耗。

.. code ::

    HAL_LSCRYPT_DeInit(void);