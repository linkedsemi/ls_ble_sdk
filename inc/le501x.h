/* Copyright (c) 2011 - 2015 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


#ifndef LE501X_H
#define LE501X_H

#ifdef __cplusplus
extern "C" {
#endif


/* -------------------------  Interrupt Number Definition  ------------------------ */

typedef enum IRQn
{
/* -------------------  Cortex-M0 Processor Exceptions Numbers  ------------------- */
  NonMaskableInt_IRQn           = -14,      /*  2 Non Maskable Interrupt */
  HardFault_IRQn                = -13,      /*  3 HardFault Interrupt */



  SVCall_IRQn                   =  -5,      /* 11 SV Call Interrupt */

  PendSV_IRQn                   =  -2,      /* 14 Pend SV Interrupt */
  SysTick_IRQn                  =  -1,      /* 15 System Tick Interrupt */

/* ----------------------  ARMCM0 Specific Interrupt Numbers  --------------------- */
  WDT_IRQn                      =   0,      /* Watchdog Timer Interrupt */
  RTC_IRQn                      =   1,      /* Real Time Clock Interrupt */
  TIM0_IRQn                     =   2,      /* Timer0 / Timer1 Interrupt */
  TIM2_IRQn                     =   3,      /* Timer2 / Timer3 Interrupt */
  MCIA_IRQn                     =   4,      /* MCIa Interrupt */
  MCIB_IRQn                     =   5,      /* MCIb Interrupt */
  UART0_IRQn                    =   6,      /* UART0 Interrupt */
  UART1_IRQn                    =   7,      /* UART1 Interrupt */
  UART2_IRQn                    =   8,      /* UART2 Interrupt */
  UART4_IRQn                    =   9,      /* UART4 Interrupt */
  AACI_IRQn                     =  10,      /* AACI / AC97 Interrupt */
  CLCD_IRQn                     =  11,      /* CLCD Combined Interrupt */
  ENET_IRQn                     =  12,      /* Ethernet Interrupt */
  USBDC_IRQn                    =  13,      /* USB Device Interrupt */
  USBHC_IRQn                    =  14,      /* USB Host Controller Interrupt */
  CHLCD_IRQn                    =  15,      /* Character LCD Interrupt */
  FLEXRAY_IRQn                  =  16,      /* Flexray Interrupt */
  CAN_IRQn                      =  17,      /* CAN Interrupt */
  LIN_IRQn                      =  18,      /* LIN Interrupt */
  I2C_IRQn                      =  19,      /* I2C ADC/DAC Interrupt */
  CPU_CLCD_IRQn                 =  28,      /* CPU CLCD Combined Interrupt */
  UART3_IRQn                    =  30,      /* UART3 Interrupt */
  SPI_IRQn                      =  31       /* SPI Touchscreen Interrupt */
} IRQn_Type;


/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

/* --------  Configuration of the Cortex-M0 Processor and Core Peripherals  ------- */
#define __CM0_REV                 0x0000U   /* Core revision r0p0 */
#define __MPU_PRESENT             0         /* MPU present or not */
#define __VTOR_PRESENT            0         /* no VTOR present*/
#define __NVIC_PRIO_BITS          2         /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0         /* Set to 1 if different SysTick Config is used */

#include "core_cm0.h"                       /* Processor and core peripherals */
//#include "system_ARMCM0.h"                  /* System Header */

#ifdef __cplusplus
}
#endif

#endif
