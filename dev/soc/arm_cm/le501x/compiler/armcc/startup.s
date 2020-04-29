;/**************************************************************************//**
; * @file     startup.s
; * @brief    CMSIS Cortex-M0 Core Device Startup File
; *           
; * @version  V3.10
; * @date     23. November 2012
; *
; * @note
; *
; ******************************************************************************/
;/* Copyright (c) 2012 ARM LIMITED
;
;   All rights reserved.
;   Redistribution and use in source and binary forms, with or without
;   modification, are permitted provided that the following conditions are met: 
;   - Redistributions of source code must retain the above copyright
;     notice, this list of conditions and the following disclaimer.
;   - Redistributions in binary form must reproduce the above copyright
;     notice, this list of conditions and the following disclaimer in the
;     documentation and/or other materials provided with the distribution.
;   - Neither the name of ARM nor the names of its contributors may be used
;     to endorse or promote products derived from this software without
;     specific prior written permission.
;   *
;   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
;   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
;   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
;   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
;   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
;   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
;   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
;   POSSIBILITY OF SUCH DAMAGE.
;   ---------------------------------------------------------------------------*/
;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00001000

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp



; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000800

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts

                DCD    EXTI_Handler
                DCD    WWDT_Handler
                DCD    LPWKUP_Handler
                DCD    BLE_Handler
                DCD    RTC_Handler
                DCD    DMA_Handler
                DCD    QSPI_Handler
                DCD    ECC_Handler
                DCD    CACHE_Handler
                DCD    TRNG_Handler
                DCD    IWDT_Handler
                DCD    CRYPT_Handler
                DCD    PDM_Handler
                DCD    BLE_Handler
                DCD    ADC_Handler
                DCD    ADTIM1_Handler
                DCD    BSTIM1_Handler
                DCD    GPTIMA1_Handler
                DCD    GPTIMB1_Handler
                DCD    BLE_Handler
                DCD    LVD33_Handler
                DCD    GPTIMC1_Handler
                DCD    LPTIM_Handler
                DCD    I2C1_Handler
                DCD    I2C2_Handler
                DCD    SPI1_Handler
                DCD    SPI2_Handler
                DCD    UART1_Handler
                DCD    UART2_Handler
                DCD    UART3_Handler
                DCD    BLE_Handler
                DCD    BLE_Handler

__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R4, =SystemInit
                BLX     R4
                LDR     R4, =__main
                BX      R4
                ENDP

            AREA    |.text.Default_Handler|, CODE, READONLY

; Dummy Exception Handlers (infinite loops which can be modified)

Default_Handler PROC
                EXPORT  NMI_Handler               [WEAK]
                EXPORT  HardFault_Handler         [WEAK]
                EXPORT  SVC_Handler               [WEAK]
                EXPORT  PendSV_Handler            [WEAK]
                EXPORT  SysTick_Handler           [WEAK]
                
                EXPORT  EXTI_Handler              [WEAK]
                EXPORT  WWDT_Handler              [WEAK]
                EXPORT  LPWKUP_Handler            [WEAK]
                EXPORT  BLE_Handler               [WEAK]
                EXPORT  RTC_Handler               [WEAK]
                EXPORT  DMA_Handler               [WEAK]
                EXPORT  QSPI_Handler              [WEAK]
                EXPORT  ECC_Handler               [WEAK]
                EXPORT  CACHE_Handler             [WEAK]
                EXPORT  TRNG_Handler              [WEAK]
                EXPORT  IWDT_Handler              [WEAK]
                EXPORT  CRYPT_Handler             [WEAK]
                EXPORT  PDM_Handler               [WEAK]
                EXPORT  BLE_Handler               [WEAK]
                EXPORT  ADC_Handler               [WEAK]
                EXPORT  ADTIM1_Handler            [WEAK]
                EXPORT  BSTIM1_Handler            [WEAK]
                EXPORT  GPTIMA1_Handler           [WEAK]
                EXPORT  GPTIMB1_Handler           [WEAK]
                EXPORT  BLE_Handler               [WEAK]
                EXPORT  LVD33_Handler             [WEAK]
                EXPORT  GPTIMC1_Handler           [WEAK]
                EXPORT  LPTIM_Handler             [WEAK]
                EXPORT  I2C1_Handler              [WEAK]
                EXPORT  I2C2_Handler              [WEAK]
                EXPORT  SPI1_Handler              [WEAK]
                EXPORT  SPI2_Handler              [WEAK]
                EXPORT  UART1_Handler             [WEAK]
                EXPORT  UART2_Handler             [WEAK]
                EXPORT  UART3_Handler             [WEAK]
                EXPORT  BLE_Handler               [WEAK]
                EXPORT  BLE_Handler               [WEAK]
NMI_Handler      
HardFault_Handler
SVC_Handler      
PendSV_Handler   
SysTick_Handler  
EXTI_Handler
WWDT_Handler
LPWKUP_Handler
BLE_Handler
RTC_Handler
DMA_Handler
QSPI_Handler
ECC_Handler
CACHE_Handler
TRNG_Handler
IWDT_Handler
CRYPT_Handler
PDM_Handler
ADC_Handler
ADTIM1_Handler
BSTIM1_Handler
GPTIMA1_Handler
GPTIMB1_Handler
LVD33_Handler
GPTIMC1_Handler
LPTIM_Handler
I2C1_Handler
I2C2_Handler
SPI1_Handler
SPI2_Handler
UART1_Handler
UART2_Handler
UART3_Handler
                B       .
                ENDP
                
                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF


                END
