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

    
    .thumb
    .thumb_func
    .align 1
    .globl hardfault_env_save
    .type hardfault_env_save,%function
hardfault_env_save:
    STR R1,[R0,#0]
    MRS R3,PSP
    STR R3,[R0,#4]
    STR R2,[R0,#8]
    STR R4,[R0,#12]
    STR R5,[R0,#16]
    STR R6,[R0,#20]
    STR R7,[R0,#24]
    BX LR

    .end
    
