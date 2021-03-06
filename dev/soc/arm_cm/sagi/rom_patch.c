#include "lspatch.h"
#include "instructions.h"

static void ke_timer_init_patch(uint8_t idx)
{
    lspatch_enable(idx,0x25b24,0xbd08b508);
}

static void ke_timer_set_patch(uint8_t idx)
{
    extern void ke_timer_set();
    uint32_t src_addr = 0x25b34;
    uint32_t dst_addr = (uint32_t)ke_timer_set;
    uint32_t code = b_bl_instruction_32_generate(src_addr,dst_addr,false);
    lspatch_enable(idx,src_addr,code);
}

static void ke_timer_clear_patch(uint8_t idx)
{
    extern void ke_timer_clear();
    uint32_t src_addr = 0x25c24;
    uint32_t dst_addr = (uint32_t)ke_timer_clear;
    uint32_t code = b_bl_instruction_32_generate(src_addr,dst_addr,false);
    lspatch_enable(idx,src_addr,code);
}

static void ke_timer_active_patch(uint8_t idx)
{
    extern void ke_timer_active();
    uint32_t src_addr = 0x25ca8;
    uint32_t dst_addr = (uint32_t)ke_timer_active;
    uint32_t code = b_bl_instruction_32_generate(src_addr,dst_addr,false);
    lspatch_enable(idx,src_addr,code);
}

void rom_patch_init()
{
    ke_timer_init_patch(1);
    ke_timer_set_patch(2);
    ke_timer_clear_patch(3);
    ke_timer_active_patch(4);
}