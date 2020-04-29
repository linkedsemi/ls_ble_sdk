#ifndef FIELD_MANIPULATE_H_
#define FIELD_MANIPULATE_H_
#include <stdint.h>

#define REG_FIELD_WR(reg,field,val)\
        do{                                 \
        uint32_t old_val = (reg);\
        uint32_t new_val = ((unsigned int)(val)<<(field##_POS)&(unsigned int)(field##_MASK))|(old_val &~(unsigned int)(field##_MASK));\
        (reg)= new_val;\
    }while(0)

#define REG_FIELD_RD(reg,field)\
    (((reg)&(unsigned int)(field##_MASK))>>(field##_POS))

#define FIELD_BUILD(field,val) \
    ((unsigned int)(val)<<(field##_POS)&(unsigned int)(field##_MASK))


#endif
