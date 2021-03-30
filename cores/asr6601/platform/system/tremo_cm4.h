/**
 ****************************************************************************************
 *
 * @file tremo_cm4.h
 *
 * @brief define arm cm4 SOC architecture
 *
 * Copyright (C) ASR
 *
 ****************************************************************************************
 */

/*************************   **************************************/
#ifndef __TREMO_CM4_H__
#define __TREMO_CM4_H__

#ifdef __cplusplus
extern "C" {
#endif

#define __MPU_PRESENT          0 /*!< do not provide MPU */
#define __FPU_PRESENT          1
#define __NVIC_PRIO_BITS       3
#define __Vendor_SysTickConfig 0 /*!< Set to 1 if different SysTick Config is used  */
#define configLIBRARY_NORMAL_INTERRUPT_PRIORITY 6

typedef enum IRQn {
    /**************   Processor Exceptions Numbers *************************************/
    NonMaskableInt_IRQn   = -14, /*!< 2 Non Maskable Interrupt                         */
    MemoryManagement_IRQn = -12, /*!< 4 Cortex-M3 Memory Management Interrupt          */
    BusFault_IRQn         = -11, /*!< 5 Cortex-M3 Bus Fault Interrupt                  */
    UsageFault_IRQn       = -10, /*!< 6 Cortex-M3 Usage Fault Interrupt                */
    SVCall_IRQn           = -5,  /*!< 11 Cortex-M3 SV Call Interrupt                   */
    DebugMonitor_IRQn     = -4,  /*!< 12 Cortex-M3 Debug Monitor Interrupt             */
    PendSV_IRQn           = -2,  /*!< 14 Cortex-M3 Pend SV Interrupt                   */
    SysTick_IRQn          = -1,  /*!< 15 Cortex-M3 System Tick Interrupt               */
    /******   Interrupt Numbers ********************************************************/
    SEC_IRQn    = 0,             /*!< SEC Interrupt                                    */
    RTC_IRQn    = 1,             /*!< RTC Interrupt                                    */
    WDG_IRQn    = 2,             /*!< WDG Interrupt                                    */
    EFC_IRQn    = 3,             /*!< EFC Interrupt                                    */
    UART3_IRQn  = 4,             /*!< UART3 Interrupt                                  */
    I2C2_IRQn   = 5,             /*!< I2C2 Interrupt                                   */
    UART0_IRQn  = 6,             /*!< UART0 Interrupt                                  */
    UART1_IRQn  = 7,             /*!< UART1 Interrupt                                  */
    UART2_IRQn  = 8,             /*!< UART2 Interrupt                                  */
    LPUART_IRQn = 9,             /*!< LPUART Interrupt                                 */
    SSP0_IRQn   = 10,            /*!< SSP0 Interrupt                                   */
    SSP1_IRQn   = 11,            /*!< SSP1 Interrupt                                   */
    QSPI_IRQn   = 12,            /*!< QSPI Interrupt                                   */
    I2C0_IRQn   = 13,            /*!< I2C0 Interrupt                                   */
    I2C1_IRQn   = 14,            /*!< I2C1 Interrupt                                   */
    SCC_IRQn    = 15,            /*!< SCC Interrupt                                    */
    ADC_IRQn    = 16,            /*!< ADC Interrupt                                    */
    AFEC_IRQn   = 17,            /*!< AFEC Interrupt                                   */
    SSP2_IRQn   = 18,            /*!< SSP2 Interrupt                                   */
    DMA1_IRQn   = 19,            /*!< DMA1 Interrupt                                   */
    DAC_IRQn    = 20,            /*!< DAC Interrupt                                    */
    LORA_IRQn  = 21,             /*!< LORA Interrupt                                   */
    GPIO_IRQn   = 22,            /*!< GPIO Interrupt                                   */
    TIMER0_IRQn = 23,            /*!< TIMER0 Interrupt                                 */
    TIMER1_IRQn = 24,            /*!< TIMER1 Interrupt                                 */
    TIMER2_IRQn = 25,            /*!< TIMER2 Interrupt                                 */
    TIMER3_IRQn = 26,            /*!< TIMER3 Interrupt                                 */
    BSTIMER0_IRQn  = 27,         /*!< BSTIMER0 Interrupt                               */
    BSTIMER1_IRQn  = 28,         /*!< BSTIMER1 Interrupt                               */
    LPTIMER0_IRQn  = 29,         /*!< LPTIMER0 Interrupt                               */
    SAC_IRQn    = 30,            /*!< SAC Interrupt                                    */
    DMA0_IRQn   = 31,            /*!< DMA0 Interrupt                                   */
    I2S_IRQn    = 32,            /*!< I2S Interrupt                                    */
    LCD_IRQn    = 33,            /*!< LCD Interrupt                                    */
    PWR_IRQn    = 34,            /*!< PWR Interrupt                                    */
    LPTIMER1_IRQn = 35,          /*!< LPTIMER1 Interrupt                               */
    IWDG_IRQn   = 36,            /*!< IWDG Interrupt                                   */
} IRQn_Type;

#include "core_cm4.h"
#include <stdint.h>

// ---------------------------------------------------------------------------
typedef enum { RESET = 0, SET = !RESET } flag_status_t, it_status_t;
typedef enum { DISABLE = 0, ENABLE = !DISABLE } functional_state_t;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#ifdef RUN_IN_RAM
#define RAM_FUNC_ATTR __attribute__((section(".ram_funcs")))
#else
#define RAM_FUNC_ATTR
#endif
// ---------------------------------------------------------------------------

#define ERRNO_OK      (0)
#define ERRNO_ERROR   (-1)
#define ERRNO_TIMEOUT (-2)

// ---------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif //__TREMO_CM4_H__
