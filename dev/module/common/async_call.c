#include "async_call.h"
#include "ls_dbg.h"
#include "compile_flag.h"

extern uint32_t (*enter_critical_fn)(void);
extern void (*exit_critical_fn)(uint32_t);

ROM_SYMBOL LL_PKT_ISR void async_call(void (*func)(void *),void *param,struct fifo_env *fifo)
{
    struct async_call_param env;
    env.func = func;
    env.param = param;
    uint32_t cpu_stat = enter_critical_fn();
    bool not_full = dword_fifo_put(fifo, &env);
    exit_critical_fn(cpu_stat);
    LS_ASSERT(not_full);
    (void)(not_full);
}

ROM_SYMBOL bool async_process(struct fifo_env *fifo)
{
    struct async_call_param env;
    uint32_t cpu_stat = enter_critical_fn();
    bool not_empty = dword_fifo_get(fifo,&env); 
    exit_critical_fn(cpu_stat);
    if(not_empty)
    {
        env.func(env.param);
    }
    return not_empty;
}
