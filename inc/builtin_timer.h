#ifndef BUILTIN_TIMER_H_
#define BUILTIN_TIMER_H_
#include <stdint.h>
#include "linked_buffer.h"

struct builtin_timer
{
    void *hdr;
    void (*timer_cb)(void *);
    void *param;
};

#define DEF_BUILTIN_TIMER_ENV(num) \
    DEF_LINKED_BUF(builtin_timer_env,struct builtin_timer,(num))

#define INIT_BUILTIN_TIMER_ENV() \
    do{ \
        INIT_LINKED_BUF(builtin_timer_env);\
        builtin_timer_env_register(&builtin_timer_env);\
    }while(0)

void builtin_timer_env_register(linked_buffer_t *env);
#endif

