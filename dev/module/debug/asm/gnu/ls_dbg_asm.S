    .syntax unified
    .arch armv6-m

    .thumb
    .thumb_func
    .align 1
    .globl stack_assert_asm
    .type stack_assert_asm,%function
stack_assert_asm:

    PUSH {LR}
    MOV R3,LR
    BL stack_assert_c
    POP {PC}

    .thumb_func
    .align 1
    .globl ls_delay_asm
    .type ls_delay_asm,%function
ls_delay_asm:
    SUBS r0,r0,0x1
    CMP r0,0x0
    BNE ls_delay_asm
    BX LR

    .end
    
