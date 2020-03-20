#ifndef TINYFS_NVM_H_
#define TINYFS_NVM_H_
#include <stdint.h>

extern uint8_t (*const nvm_program)(uint32_t,uint16_t,uint8_t *);
extern uint8_t (*const nvm_read)(uint32_t, uint32_t, uint8_t *);
extern uint8_t (*const nvm_sector_ease)(uint32_t);

#endif
