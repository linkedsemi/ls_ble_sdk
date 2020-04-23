#include <stdlib.h>
#include <string.h>
#include "common.h"

void rand_gen_by_word(uint8_t *ptr,uint8_t word_num)
{
    while(word_num--)
    {
        uint32_t x = rand();
        memcpy(ptr,&x,sizeof(uint32_t));    
        ptr += sizeof(uint32_t);
    }
}

