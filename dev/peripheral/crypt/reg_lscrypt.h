#ifndef REG_LSCRYPT_H_
#define REG_LSCRYPT_H_
#include "reg_lscrypt_type.h"

#define LSCRYPT ((reg_crypt_t *)(0x40002800))

void lscrpt_init(void);
#endif //(REG_LSCRYPT_H_)