#include "platform.h"
#include "calc_div.h"
#include "cpu.h"

void stack_var_ptr_init(void);

void main_task_app_init(void);

void main_task_itf_init(void);

void sys_init(bool app)
{
    stack_var_ptr_init();
    if(app)
    {
        main_task_app_init();
    }else
    {
        main_task_itf_init();
    }
    //TODO
}

void uart_eif_read(uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy)
{


}

void uart_eif_write(uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy)
{


}

void uart_eif_flow_on(void)
{

}

bool uart_eif_flow_off(void)
{
    return true;
}

void platform_reset(uint32_t error)
{

}

void ecc_calc_start(const uint8_t* secret_key,const uint8_t* pub_x,const uint8_t* pub_y,uint8_t* result_x,uint8_t* result_y,void *param)
{

}


void true_rand_init(unsigned int seed)
{


}

int true_rand_gen(void)
{

    return 0;
}

uint64_t idiv_acc(uint32_t dividend,uint32_t divisor,bool signed_int)
{
    uint64_t retval;
    struct {
        uint32_t r0;
        uint32_t r1;
    }*ret_ptr = (void *)&retval;
    enter_critical();
    calc_div(dividend, divisor, signed_int,&ret_ptr->r0,&ret_ptr->r1);
    exit_critical();
    return retval;
}

void SystemInit()
{

}

uint32_t plf_get_reset_error()
{

    return 0;
}