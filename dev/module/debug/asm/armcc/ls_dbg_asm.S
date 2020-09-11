    PRESERVE8
    THUMB
    AREA    |STACK_ASSERT_ASM|, CODE, READONLY
    FRAME UNWIND ON
stack_assert_asm PROC
    EXPORT stack_assert_asm
    IMPORT stack_assert_c
    PUSH {LR}
    FRAME PUSH {LR}
    MOV R3,LR
    BL stack_assert_c
    POP {PC}
    ENDP

    ALIGN
    AREA    |LS_DELAY_ASM|, CODE, READONLY
    FRAME UNWIND ON
ls_delay_asm PROC
    EXPORT ls_delay_asm
    SUBS R0,R0,#1
    CMP R0,#0
    BNE ls_delay_asm
    BX LR
    ENDP
                ALIGN
                AREA    |HARDFAULT_ENV_SAVE|, CODE, READONLY
                FRAME UNWIND ON
hardfault_env_save\
                PROC
                EXPORT  hardfault_env_save
                STR R1,[R0,#0]
                MRS R3,PSP
                STR R3,[R0,#4]
                STR R2,[R0,#8]
                STR R4,[R0,#12]
                STR R5,[R0,#16]
                STR R6,[R0,#20]
                STR R7,[R0,#24]
                BX LR
                ENDP
                

    END
    
