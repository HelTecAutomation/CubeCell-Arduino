  .syntax unified
  .cpu cortex-m4
  .thumb

.global  g_pfnVectors
.global  Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word  _sidata
/* start address for the .data section. defined in linker script */
.word  _sdata
/* end address for the .data section. defined in linker script */
.word  _edata
/* start address for the .bss section. defined in linker script */
.word  _sbss
/* end address for the .bss section. defined in linker script */
.word  _ebss
/* stack used for SystemInit_ExtMemCtl; always internal RAM used */

/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called.
 * @param  None
 * @retval : None
*/

  .section  .text.Reset_Handler
  .weak  Reset_Handler
  .type  Reset_Handler, %function
Reset_Handler:
/* Copy the data segment initializers from flash to SRAM */
  movs  r1, #0
  b  LoopCopyDataInit

CopyDataInit:
  ldr  r3, =_sidata
  ldr  r3, [r3, r1]
  str  r3, [r0, r1]
  adds  r1, r1, #4

LoopCopyDataInit:
  ldr  r0, =_sdata
  ldr  r3, =_edata
  adds  r2, r0, r1
  cmp  r2, r3
  bcc  CopyDataInit
  ldr  r2, =_sbss
  b  LoopFillZerobss
/* Zero fill the bss segment. */
FillZerobss:
  movs  r3, #0
  str  r3, [r2], #4

LoopFillZerobss:
  ldr  r3, = _ebss
  cmp  r2, r3
  bcc  FillZerobss

/* Call the clock system intitialization function.*/
  bl  system_init
  
  bl __libc_init_array
/* Call the application's entry point.*/
  bl  main
  
LoopForever:
  b LoopForever
	
.size  Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 * @param  None
 * @retval None
*/
    .section  .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b  Infinite_Loop
  .size  Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M3. Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
*******************************************************************************/
  .section  .isr_vector,"a",%progbits
  .type  g_pfnVectors, %object
  .size  g_pfnVectors, .-g_pfnVectors


g_pfnVectors:
  .word  _estack
  .word  Reset_Handler
  .word  NMI_Handler
  .word  HardFault_Handler
  .word  MemManage_Handler
  .word  BusFault_Handler
  .word  UsageFault_Handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  SVC_Handler
  .word  0
  .word  0
  .word  PendSV_Handler
  .word  SysTick_Handler

  /* External Interrupts */
  .word     SEC_IRQHandler
  .word     RTC_IRQHandler
  .word     WDG_IRQHandler
  .word     EFC_IRQHandler
  .word     UART3_IRQHandler
  .word     I2C2_IRQHandler
  .word     UART0_IRQHandler
  .word     UART1_IRQHandler
  .word     UART2_IRQHandler
  .word     LPUART_IRQHandler
  .word     SSP0_IRQHandler
  .word     SSP1_IRQHandler
  .word     QSPI_IRQHandler
  .word     I2C0_IRQHandler
  .word     I2C1_IRQHandler
  .word     SCC_IRQHandler
  .word     ADC_IRQHandler
  .word     AFEC_IRQHandler
  .word     SSP2_IRQHandler
  .word     DMA1_IRQHandler
  .word     DAC_IRQHandler
  .word     LORA_IRQHandler
  .word     GPIO_IRQHandler
  .word     TIMER0_IRQHandler
  .word     TIMER1_IRQHandler
  .word     TIMER2_IRQHandler
  .word     TIMER3_IRQHandler
  .word     BSTIMER0_IRQHandler
  .word     BSTIMER1_IRQHandler
  .word     LPTIMER0_IRQHandler
  .word     SAC_IRQHandler
  .word     DMA0_IRQHandler
  .word     I2S_IRQHandler
  .word     LCD_IRQHandler
  .word     PWR_IRQHandler
  .word     LPTIMER1_IRQHandler
  .word     IWDG_IRQHandler


/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/
   .weak      NMI_Handler
   .thumb_set NMI_Handler,Default_Handler

   .weak      HardFault_Handler
   .thumb_set HardFault_Handler,Default_Handler

   .weak      MemManage_Handler
   .thumb_set MemManage_Handler,Default_Handler

   .weak      BusFault_Handler
   .thumb_set BusFault_Handler,Default_Handler

   .weak      UsageFault_Handler
   .thumb_set UsageFault_Handler,Default_Handler

   .weak      SVC_Handler
   .thumb_set SVC_Handler,Default_Handler

   .weak      PendSV_Handler
   .thumb_set PendSV_Handler,Default_Handler

   .weak      SysTick_Handler
   .thumb_set SysTick_Handler,Default_Handler

   .weak      SEC_IRQHandler
   .thumb_set SEC_IRQHandler,Default_Handler
   
   .weak      RTC_IRQHandler
   .thumb_set RTC_IRQHandler,Default_Handler
   
   .weak      WDG_IRQHandler
   .thumb_set WDG_IRQHandler,Default_Handler
   
   .weak      EFC_IRQHandler
   .thumb_set EFC_IRQHandler,Default_Handler
   
   .weak      UART3_IRQHandler
   .thumb_set UART3_IRQHandler,Default_Handler
   
   .weak      I2C2_IRQHandler
   .thumb_set I2C2_IRQHandler,Default_Handler
   
   .weak      UART0_IRQHandler
   .thumb_set UART0_IRQHandler,Default_Handler
   
   .weak      UART1_IRQHandler
   .thumb_set UART1_IRQHandler,Default_Handler
   
   .weak      UART2_IRQHandler
   .thumb_set UART2_IRQHandler,Default_Handler
   
   .weak      LPUART_IRQHandler
   .thumb_set LPUART_IRQHandler,Default_Handler
   
   .weak      SSP0_IRQHandler
   .thumb_set SSP0_IRQHandler,Default_Handler
   
   .weak      SSP1_IRQHandler
   .thumb_set SSP1_IRQHandler,Default_Handler
   
   .weak      QSPI_IRQHandler
   .thumb_set QSPI_IRQHandler,Default_Handler
   
   .weak      I2C0_IRQHandler
   .thumb_set I2C0_IRQHandler,Default_Handler
   
   .weak      I2C1_IRQHandler
   .thumb_set I2C1_IRQHandler,Default_Handler
   
   .weak      SCC_IRQHandler
   .thumb_set SCC_IRQHandler,Default_Handler
   
   .weak      ADC_IRQHandler
   .thumb_set ADC_IRQHandler,Default_Handler
   
   .weak      AFEC_IRQHandler
   .thumb_set AFEC_IRQHandler,Default_Handler
   
   .weak      SSP2_IRQHandler
   .thumb_set SSP2_IRQHandler,Default_Handler
   
   .weak      DMA1_IRQHandler
   .thumb_set DMA1_IRQHandler,Default_Handler
   
   .weak      DAC_IRQHandler
   .thumb_set DAC_IRQHandler,Default_Handler
   
   .weak      LORA_IRQHandler
   .thumb_set LORA_IRQHandler,Default_Handler
   
   .weak      GPIO_IRQHandler
   .thumb_set GPIO_IRQHandler,Default_Handler
   
   .weak      TIMER0_IRQHandler
   .thumb_set TIMER0_IRQHandler,Default_Handler
   
   .weak      TIMER1_IRQHandler
   .thumb_set TIMER1_IRQHandler,Default_Handler
   
   .weak      TIMER2_IRQHandler
   .thumb_set TIMER2_IRQHandler,Default_Handler
   
   .weak      TIMER3_IRQHandler
   .thumb_set TIMER3_IRQHandler,Default_Handler
   
   .weak      BSTIMER0_IRQHandler
   .thumb_set BSTIMER0_IRQHandler,Default_Handler
   
   .weak      BSTIMER1_IRQHandler
   .thumb_set BSTIMER1_IRQHandler,Default_Handler
   
   .weak      LPTIMER0_IRQHandler
   .thumb_set LPTIMER0_IRQHandler,Default_Handler
   
   .weak      SAC_IRQHandler
   .thumb_set SAC_IRQHandler,Default_Handler
   
   .weak      DMA0_IRQHandler
   .thumb_set DMA0_IRQHandler,Default_Handler
   
   .weak      I2S_IRQHandler
   .thumb_set I2S_IRQHandler,Default_Handler
   
   .weak      LCD_IRQHandler
   .thumb_set LCD_IRQHandler,Default_Handler
   
   .weak      PWR_IRQHandler
   .thumb_set PWR_IRQHandler,Default_Handler
   
   .weak      LPTIMER1_IRQHandler
   .thumb_set LPTIMER1_IRQHandler,Default_Handler
   
   .weak      IWDG_IRQHandler
   .thumb_set IWDG_IRQHandler,Default_Handler


/*****************END OF FILE*********************/