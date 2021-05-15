#ifndef LSCRYPT_H_
#define LSCRYPT_H_
#include <stdbool.h>
#include <stdint.h>
#include "HAL_def.h"
#include "reg_base_addr.h"
#include "reg_lscrypt_type.h"

#ifndef LSCRYPT
#define LSCRYPT ((reg_crypt_t *)LSCRYPT_BASE_ADDR)
#endif

enum aes_key_type
{
    AES_KEY_128 = 0,
    AES_KEY_192,
    AES_KEY_256,
};

HAL_StatusTypeDef HAL_LSCRYPT_Init(void);
HAL_StatusTypeDef HAL_LSCRYPT_DeInit(void);

HAL_StatusTypeDef HAL_LSCRYPT_AES_Key_Config(uint32_t *key,enum aes_key_type key_size);

HAL_StatusTypeDef HAL_LSCRYPT_AES_ECB_Encrypt(uint8_t *plaintext,uint32_t length,uint8_t *ciphertext);
HAL_StatusTypeDef HAL_LSCRYPT_AES_ECB_Decrypt(uint8_t *ciphertext,uint32_t length,uint8_t *plaintext);

HAL_StatusTypeDef HAL_LSCRYPT_AES_CBC_Encrypt(uint32_t iv[4],uint8_t *plaintext,uint32_t length,uint8_t *ciphertext);
HAL_StatusTypeDef HAL_LSCRYPT_AES_CBC_Decrypt(uint32_t iv[4],uint8_t *ciphertext,uint32_t length,uint8_t *plaintext);

HAL_StatusTypeDef HAL_LSCRYPT_AES_ECB_Encrypt_IT(uint8_t *plaintext,uint32_t length,uint8_t *ciphertext);
HAL_StatusTypeDef HAL_LSCRYPT_AES_ECB_Decrypt_IT(uint8_t *ciphertext,uint32_t length,uint8_t *plaintext);

HAL_StatusTypeDef HAL_LSCRYPT_AES_CBC_Encrypt_IT(uint32_t iv[4],uint8_t *plaintext,uint32_t length,uint8_t *ciphertext);
HAL_StatusTypeDef HAL_LSCRYPT_AES_CBC_Decrypt_IT(uint32_t iv[4],uint8_t *ciphertext,uint32_t length,uint8_t *plaintext);

void HAL_LSCRYPT_AES_Complete_Callback(bool Encrypt,bool CBC);

void HAL_LSCRYPT_IRQHandler(void);

#endif
