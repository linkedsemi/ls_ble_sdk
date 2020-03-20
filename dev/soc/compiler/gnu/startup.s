	.syntax	unified
	.arch	armv6-m

	.section .stack
	.align	3
	.equ	Stack_Size, 0x1000
	.globl	__StackTop
	.globl	__StackLimit
__StackLimit:
	.space	Stack_Size
	.size	__StackLimit, . - __StackLimit
__StackTop:
	.size	__StackTop, . - __StackTop

    .section .heap
	.align	3
	.equ	Heap_Size, 0x100
	.globl	__HeapBase
	.globl	__HeapLimit
__HeapBase:
	.if	Heap_Size
	.space	Heap_Size
	.endif
	.size	__HeapBase, . - __HeapBase
__HeapLimit:
	.size	__HeapLimit, . - __HeapLimit

	.section .isr_vector
	.align 2
	.globl	__isr_vector
__isr_vector:
	.long	__StackTop            /* Top of Stack */
	.long	Reset_Handler         /* Reset Handler */
	.long	NMI_Handler           /* NMI Handler */
	.long	HardFault_Handler     /* Hard Fault Handler */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	SVC_Handler           /* SVCall Handler */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	PendSV_Handler        /* PendSV Handler */
	.long	SysTick_Handler       /* SysTick Handler */

	/* External interrupts */
	.long	WDT_IRQHandler
	.long	BLE_LP_IRQHandler
	.long	BLE_MAC_IRQHandler
	.long	RTC_IRQHandler
	.long	EXT_INTR_IRQHandler
	.long	ECC_IRQHandler
	.long	DMAC_IRQHandler
	.long	QSPI_IRQHandler
	.long	SPIM0_IRQHandler
	.long	SPIM1_IRQHandler
	.long	SPIS_IRQHandler
	.long	UART0_IRQHandler
	.long	UART1_IRQHandler
	.long	IIC0_IRQHandler
	.long	IIC1_IRQHandler
	.long	GPIO_IRQHandler
	.long	TIMER_IRQHandler
	.long	SFT_IRQHandler

	.size	__isr_vector, . - __isr_vector

	.thumb
	.thumb_func
	.align	1
	.globl	Reset_Handler
	.type	Reset_Handler, %function
Reset_Handler:
    ldr r4,=SystemInit
    blx r4

/*  Firstly it copies data from read only memory to RAM. There are two schemes
 *  to copy. One can copy more than one sections. Another can only copy
 *  one section.  The former scheme needs more instructions and read-only
 *  data to implement than the latter.
 *  Macro __STARTUP_COPY_MULTIPLE is used to choose between two schemes.  */


/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of triplets, each of which specify:
 *    offset 0: LMA of start of a section to copy from
 *    offset 4: VMA of start of a section to copy to
 *    offset 8: size of the section to copy. Must be multiply of 4
 *
 *  All addresses must be aligned to 4 bytes boundary.
 */
	ldr	r4, =__copy_table_start__
	ldr	r5, =__copy_table_end__

.L_loop0:
	cmp	r4, r5
	bge	.L_loop0_done
	ldr	r1, [r4]
	ldr	r2, [r4, #4]
	ldr	r3, [r4, #8]

.L_loop0_0:
	subs	r3, #4
	blt	.L_loop0_0_done
	ldr	r0, [r1, r3]
	str	r0, [r2, r3]
	b	.L_loop0_0

.L_loop0_0_done:
	adds	r4, #12
	b	.L_loop0

.L_loop0_done:



/*  This part of work usually is done in C library startup code. Otherwise,
 *  define this macro to enable it in this startup.
 *
 *  There are two schemes too. One can clear multiple BSS sections. Another
 *  can only clear one section. The former is more size expensive than the
 *  latter.
 *
 *  Define macro __STARTUP_CLEAR_BSS_MULTIPLE to choose the former.
 *  Otherwise efine macro __STARTUP_CLEAR_BSS to choose the later.
 */

/*  Multiple sections scheme.
 *
 *  Between symbol address __copy_table_start__ and __copy_table_end__,
 *  there are array of tuples specifying:
 *    offset 0: Start of a BSS section
 *    offset 4: Size of this BSS section. Must be multiply of 4
 */
	ldr	r3, =__zero_table_start__
	ldr	r4, =__zero_table_end__

.L_loop2:
	cmp	r3, r4
	bge	.L_loop2_done
	ldr	r1, [r3]
	ldr	r2, [r3, #4]
	movs	r0, 0

.L_loop2_0:
	subs	r2, #4
	blt	.L_loop2_0_done
	str	r0, [r1, r2]
	b	.L_loop2_0
.L_loop2_0_done:

	adds	r3, #8
	b	.L_loop2
.L_loop2_done:

	bl	main

	.pool
	.size	Reset_Handler, . - Reset_Handler

    .section .text.Default_Handler
	.align	1
	.thumb_func
	.weak	Default_Handler
	.type	Default_Handler, %function
Default_Handler:
	b	.
	.size	Default_Handler, . - Default_Handler

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
	.macro	def_irq_handler	handler_name
	.weak	\handler_name
	.set	\handler_name, Default_Handler
	.endm

	def_irq_handler	NMI_Handler
	def_irq_handler	HardFault_Handler
	def_irq_handler	SVC_Handler
	def_irq_handler	PendSV_Handler
	def_irq_handler	SysTick_Handler
	def_irq_handler	WDT_IRQHandler
    def_irq_handler BLE_LP_IRQHandler
	def_irq_handler	BLE_MAC_IRQHandler
    def_irq_handler RTC_IRQHandler
    def_irq_handler EXT_INTR_IRQHandler
    def_irq_handler ECC_IRQHandler
    def_irq_handler DMAC_IRQHandler
    def_irq_handler QSPI_IRQHandler
    def_irq_handler SPIM0_IRQHandler
    def_irq_handler SPIM1_IRQHandler
    def_irq_handler SPIS_IRQHandler
    def_irq_handler UART0_IRQHandler
    def_irq_handler UART1_IRQHandler
    def_irq_handler IIC0_IRQHandler
    def_irq_handler IIC1_IRQHandler
    def_irq_handler GPIO_IRQHandler
    def_irq_handler TIMER_IRQHandler
    def_irq_handler SFT_IRQHandler
    def_irq_handler PGSPY_IRQHandler
    .end
