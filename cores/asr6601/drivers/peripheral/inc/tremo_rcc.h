/**
 ******************************************************************************
 * @file    tremo_rcc.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   This file contains all the functions prototypes for the RCC firmware
 *          library.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup RCC
 * @{
 */

#ifndef __TREMO_RCC_H
#define __TREMO_RCC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "tremo_regs.h"

extern uint32_t systime ;

#define RCC_FREQ_48M      ((uint32_t)48000000)
#define RCC_FREQ_40M      ((uint32_t)40000000)	
#define RCC_FREQ_32M      ((uint32_t)32000000)
#define RCC_FREQ_30M      ((uint32_t)30000000)	
#define RCC_FREQ_24M      ((uint32_t)24000000)		
#define RCC_FREQ_4M       ((uint32_t)4000000)	
#define RCC_FREQ_32768    ((uint32_t)32768)	
#define RCC_FREQ_32000    ((uint32_t)32000)

#define RCC_WAIT() \
    do {           \
        __NOP();   \
        __NOP();   \
        __NOP();   \
    } while (0)

/**
 * @brief RCC CLK type
 */
typedef enum {
    RCC_SYS_CLK,  /*!< SYS CLK */
    RCC_HCLK,     /*!< HCLK */
    RCC_PCLK0,    /*!< PCLK0 */
    RCC_PCLK1,    /*!< PCLK1 */
} rcc_clk_t;

/**
 * @brief RCC oscillator type
 */
typedef enum {
    RCC_OSC_RCO48M,  /*!< RCO48M */
    RCC_OSC_RCO32K,  /*!< RCO32K */
    RCC_OSC_XO32K,   /*!< XO32K */
    RCC_OSC_XO24M,   /*!< XO24M */
    RCC_OSC_XO32M,   /*!< XO32M */
    RCC_OSC_RCO4M,   /*!< RCO4M */
} rcc_oscillator_t;

/**
 * @brief The source of the SYSCLK
 */
typedef enum {
    RCC_SYS_CLK_SOURCE_RCO48M_DIV2 = RCC_CR0_SYSCLK_SEL_RCO48M_DIV2, /*!< SYSCLK source is RCO48M/2 */
    RCC_SYS_CLK_SOURCE_RCO48M      = RCC_CR0_SYSCLK_SEL_RCO48M,      /*!< SYSCLK source is RCO48M */
    RCC_SYS_CLK_SOURCE_RCO32K      = RCC_CR0_SYSCLK_SEL_RCO32K,      /*!< SYSCLK source is RCO32K*/
    RCC_SYS_CLK_SOURCE_XO32K       = RCC_CR0_SYSCLK_SEL_XO32K,       /*!< SYSCLK source is XO32K*/
    RCC_SYS_CLK_SOURCE_XO24M       = RCC_CR0_SYSCLK_SEL_XO24M,       /*!< SYSCLK source is XO24M*/
    RCC_SYS_CLK_SOURCE_XO32M       = RCC_CR0_SYSCLK_SEL_XO32M,       /*!< SYSCLK source is XO32M*/
    RCC_SYS_CLK_SOURCE_RCO4M       = RCC_CR0_SYSCLK_SEL_RCO4M,       /*!< SYSCLK source is RCO4M*/
} rcc_sys_clk_source_t;

/**
 * @brief The source of the SYSTICK
 */
typedef enum {
    RCC_SYSTICK_SOURCE_XO32K  = RCC_CR0_STCLKEN_SEL_XO32K,  /*!< SYSTICK source is XO32K */
    RCC_SYSTICK_SOURCE_RCO32K = RCC_CR0_STCLKEN_SEL_RCO32K, /*!< SYSTICK source is RCO32K */
    RCC_SYSTICK_SOURCE_HCLK,                                /*!< SYSTICK source is HCLK */
} rcc_systick_source_t;

/**
 * @brief The source of the MCOCLK
 */
typedef enum {
    RCC_MCO_CLK_SOURCE_RCO32K = RCC_CR0_MCO_CLK_SEL_RCO32K,  /*!< MCOCLK source is RCO32K */
    RCC_MCO_CLK_SOURCE_XO32K  = RCC_CR0_MCO_CLK_SEL_XO32K,   /*!< MCOCLK source is XO32K */
    RCC_MCO_CLK_SOURCE_RCO4M  = RCC_CR0_MCO_CLK_SEL_RCO4M,   /*!< MCOCLK source is RCO4M */
    RCC_MCO_CLK_SOURCE_XO24M  = RCC_CR0_MCO_CLK_SEL_XO24M,   /*!< MCOCLK source is XO24M */
    RCC_MCO_CLK_SOURCE_XO32M  = RCC_CR0_MCO_CLK_SEL_XO32M,   /*!< MCOCLK source is XO32M */
    RCC_MCO_CLK_SOURCE_RCO48M = RCC_CR0_MCO_CLK_SEL_RCO48M,  /*!< MCOCLK source is RCO48M */
    RCC_MCO_CLK_SOURCE_SYSCLK = RCC_CR0_MCO_CLK_SEL_SYSCLCK, /*!< MCOCLK source is SYSCLK */
} rcc_mco_clk_source_t;

/**
 * @brief The source of the UART0 CLK
 */
typedef enum {
    RCC_UART0_CLK_SOURCE_PCLK0 = RCC_CR2_UART0_CLK_SEL_PCLK0, /*!< UART0 CLK source is PCLK0 */
    RCC_UART0_CLK_SOURCE_RCO4M = RCC_CR2_UART0_CLK_SEL_RCO4M, /*!< UART0 CLK source is RCO4M */
    RCC_UART0_CLK_SOURCE_XO32K = RCC_CR2_UART0_CLK_SEL_XO32K, /*!< UART0 CLK source is XO32K */
    RCC_UART0_CLK_SOURCE_XO24M = RCC_CR2_UART0_CLK_SEL_XO24M, /*!< UART0 CLK source is XO24M */
} rcc_uart0_clk_source_t;

/**
 * @brief The source of the UART1 CLK
 */
typedef enum {
    RCC_UART1_CLK_SOURCE_PCLK0 = RCC_CR2_UART1_CLK_SEL_PCLK0, /*!< UART1 CLK source is PCLK0 */
    RCC_UART1_CLK_SOURCE_RCO4M = RCC_CR2_UART1_CLK_SEL_RCO4M, /*!< UART1 CLK source is RCO4M */
    RCC_UART1_CLK_SOURCE_XO32K = RCC_CR2_UART1_CLK_SEL_XO32K, /*!< UART1 CLK source is XO32K */
    RCC_UART1_CLK_SOURCE_XO24M = RCC_CR2_UART1_CLK_SEL_XO24M, /*!< UART1 CLK source is XO24M */
} rcc_uart1_clk_source_t;

/**
 * @brief The source of the UART2 CLK
 */
typedef enum {
    RCC_UART2_CLK_SOURCE_PCLK1 = RCC_CR2_UART2_CLK_SEL_PCLK1, /*!< UART2 CLK source is PCLK1 */
    RCC_UART2_CLK_SOURCE_RCO4M = RCC_CR2_UART2_CLK_SEL_RCO4M, /*!< UART2 CLK source is RCO4M */
    RCC_UART2_CLK_SOURCE_XO32K = RCC_CR2_UART2_CLK_SEL_XO32K, /*!< UART2 CLK source is XO32K */
    RCC_UART2_CLK_SOURCE_XO24M = RCC_CR2_UART2_CLK_SEL_XO24M, /*!< UART2 CLK source is XO24M */
} rcc_uart2_clk_source_t;

/**
 * @brief The source of the UART3 CLK
 */
typedef enum {
    RCC_UART3_CLK_SOURCE_PCLK1 = RCC_CR2_UART3_CLK_SEL_PCLK1, /*!< UART3 CLK source is PCLK1 */
    RCC_UART3_CLK_SOURCE_RCO4M = RCC_CR2_UART3_CLK_SEL_RCO4M, /*!< UART3 CLK source is RCO4M */
    RCC_UART3_CLK_SOURCE_XO32K = RCC_CR2_UART3_CLK_SEL_XO32K, /*!< UART3 CLK source is XO32K */
    RCC_UART3_CLK_SOURCE_XO24M = RCC_CR2_UART3_CLK_SEL_XO24M, /*!< UART3 CLK source is XO24M */
} rcc_uart3_clk_source_t;

/**
 * @brief The source of the LPTIMER0 CLK
 */
typedef enum {
    RCC_LPTIMER0_CLK_SOURCE_PCLK0  = RCC_CR1_LPTIMER0_CLK_SEL_PCLK0,  /*!< LPTIMER0 CLK source is PCLK0 */
    RCC_LPTIMER0_CLK_SOURCE_RCO4M  = RCC_CR1_LPTIMER0_CLK_SEL_RCO4M,  /*!< LPTIMER0 CLK source is RCO4M */
    RCC_LPTIMER0_CLK_SOURCE_XO32K  = RCC_CR1_LPTIMER0_CLK_SEL_XO32K,  /*!< LPTIMER0 CLK source is XO32K */
    RCC_LPTIMER0_CLK_SOURCE_RCO32K = RCC_CR1_LPTIMER0_CLK_SEL_RCO32K, /*!< LPTIMER0 CLK source is RCO32K */
    RCC_LPTIMER0_CLK_SOURCE_EXTCLK,                                /*!< LPTIMER0 CLK source is EXT CLK */
} rcc_lptimer0_clk_source_t;

/**
 * @brief The source of the LPTIMER1 CLK
 */
typedef enum {
    RCC_LPTIMER1_CLK_SOURCE_PCLK0  = RCC_CR1_LPTIMER1_CLK_SEL_PCLK0,  /*!< LPTIMER1 CLK source is PCLK0 */
    RCC_LPTIMER1_CLK_SOURCE_RCO4M  = RCC_CR1_LPTIMER1_CLK_SEL_RCO4M,  /*!< LPTIMER1 CLK source is RCO4M */
    RCC_LPTIMER1_CLK_SOURCE_XO32K  = RCC_CR1_LPTIMER1_CLK_SEL_XO32K,  /*!< LPTIMER1 CLK source is XO32K */
    RCC_LPTIMER1_CLK_SOURCE_RCO32K = RCC_CR1_LPTIMER1_CLK_SEL_RCO32K, /*!< LPTIMER1 CLK source is RCO32K */
    RCC_LPTIMER1_CLK_SOURCE_EXTCLK,                                 /*!< LPTIMER1 CLK source is EXT CLK */
} rcc_lptimer1_clk_source_t;

/**
 * @brief The source of the LCD CLK
 */
typedef enum {
    RCC_LCD_CLK_SOURCE_XO32K  = RCC_CR1_LCD_CLK_SEL_XO32K,  /*!< LCD CLK source is XO32K */
    RCC_LCD_CLK_SOURCE_RCO32K = RCC_CR1_LCD_CLK_SEL_RCO32K, /*!< LCD CLK source is RCO32K */
    RCC_LCD_CLK_SOURCE_RCO4M  = RCC_CR1_LCD_CLK_SEL_RCO4M,  /*!< LCD CLK source is RCO4M */
} rcc_lcd_clk_source_t;

/**
 * @brief The source of the LPUART CLK
 */
typedef enum {
    RCC_LPUART_CLK_SOURCE_XO32K  = RCC_CR1_LPUART_CLK_SEL_XO32K,  /*!< LPUART CLK source is XO32K */
    RCC_LPUART_CLK_SOURCE_RCO32K = RCC_CR1_LPUART_CLK_SEL_RCO32K, /*!< LPUART CLK source is RCO32K */
    RCC_LPUART_CLK_SOURCE_RCO4M  = RCC_CR1_LPUART_CLK_SEL_RCO4M,  /*!< LPUART CLK source is RCO4M */
} rcc_lpuart_clk_source_t;

/**
 * @brief The source of the RTC CLK
 */
typedef enum {
    RCC_RTC_CLK_SOURCE_XO32K  = RCC_CR1_RTC_CLK_SEL_XO32K,  /*!< RTC CLK source is XO32K */
    RCC_RTC_CLK_SOURCE_RCO32K = RCC_CR1_RTC_CLK_SEL_RCO32K, /*!< RTC CLK source is RCO32K */
} rcc_rtc_clk_source_t;

/**
 * @brief The source of the IWDG CLK
 */
typedef enum {
    RCC_IWDG_CLK_SOURCE_XO32K  = RCC_CR1_IWDG_CLK_SEL_XO32K,  /*!< IWDG CLK source is XO32K */
    RCC_IWDG_CLK_SOURCE_RCO32K = RCC_CR1_IWDG_CLK_SEL_RCO32K, /*!< IWDG CLK source is RCO32K */
} rcc_iwdg_clk_source_t;

/**
 * @brief The source of the ADC CLK
 */
typedef enum {
    RCC_ADC_CLK_SOURCE_PCLK1  = RCC_CR2_ADC_CLK_SEL_PCLK1,  /*!< ADC CLK source is PCLK1 */
    RCC_ADC_CLK_SOURCE_SYSCLK = RCC_CR2_ADC_CLK_SEL_SYSCLK, /*!< ADC CLK source is SYSCLK */
    RCC_ADC_CLK_SOURCE_RCO48M = RCC_CR2_ADC_CLK_SEL_RCO48M, /*!< ADC CLK source is RCO48M */
} rcc_adc_clk_source_t;

/**
 * @brief The source of the QSPI CLK
 */
typedef enum {
    RCC_QSPI_CLK_SOURCE_HCLK   = RCC_CR2_QSPI_CLK_SEL_HCLK,   /*!< QSPI CLK source is HCLK */
    RCC_QSPI_CLK_SOURCE_SYSCLK = RCC_CR2_QSPI_CLK_SEL_SYSCLK, /*!< QSPI CLK source is SYSCLK */
} rcc_qspi_clk_source_t;

/**
 * @brief The source of the I2S CLK
 */
typedef enum {
    RCC_I2S_CLK_SOURCE_PCLK0   = RCC_CR2_I2S_CLK_SEL_PCLK0,   /*!< I2S CLK source is PCLK0 */
    RCC_I2S_CLK_SOURCE_XO24M   = RCC_CR2_I2S_CLK_SEL_XO24M,   /*!< I2S CLK source is XO24M */
    RCC_I2S_CLK_SOURCE_XO32M   = RCC_CR2_I2S_CLK_SEL_XO32M,   /*!< I2S CLK source is XO32M */
    RCC_I2S_CLK_SOURCE_EXT_CLK = RCC_CR2_I2S_CLK_SEL_EXT_CLK, /*!< I2S CLK source is EXT CLK */
} rcc_i2s_clk_source_t;

/**
 * @brief The divider of the HCLK
 */
typedef enum {
    RCC_HCLK_DIV_1   = RCC_CR0_HCLK_DIV_1,    /*!< HCLK divider is 1 */
    RCC_HCLK_DIV_2   = RCC_CR0_HCLK_DIV_2,    /*!< HCLK divider is 2 */
    RCC_HCLK_DIV_4   = RCC_CR0_HCLK_DIV_4,    /*!< HCLK divider is 4 */
    RCC_HCLK_DIV_8   = RCC_CR0_HCLK_DIV_8,    /*!< HCLK divider is 8 */
    RCC_HCLK_DIV_16  = RCC_CR0_HCLK_DIV_16,   /*!< HCLK divider is 16 */
    RCC_HCLK_DIV_32  = RCC_CR0_HCLK_DIV_32,   /*!< HCLK divider is 32 */
    RCC_HCLK_DIV_64  = RCC_CR0_HCLK_DIV_64,   /*!< HCLK divider is 64 */
    RCC_HCLK_DIV_128 = RCC_CR0_HCLK_DIV_128,  /*!< HCLK divider is 128 */
    RCC_HCLK_DIV_256 = RCC_CR0_HCLK_DIV_256,  /*!< HCLK divider is 256 */
    RCC_HCLK_DIV_512 = RCC_CR0_HCLK_DIV_512   /*!< HCLK divider is 512 */
} rcc_hclk_div_t;

/**
 * @brief The divider of the PCLK0
 */
typedef enum {
    RCC_PCLK0_DIV_1  = RCC_CR0_PCLK0_DIV_1,  /*!< PCLK0 divider is 1 */
    RCC_PCLK0_DIV_2  = RCC_CR0_PCLK0_DIV_2,  /*!< PCLK0 divider is 2 */
    RCC_PCLK0_DIV_4  = RCC_CR0_PCLK0_DIV_4,  /*!< PCLK0 divider is 4 */
    RCC_PCLK0_DIV_8  = RCC_CR0_PCLK0_DIV_8,  /*!< PCLK0 divider is 8 */
    RCC_PCLK0_DIV_16 = RCC_CR0_PCLK0_DIV_16  /*!< PCLK0 divider is 16 */
} rcc_pclk0_div_t;

/**
 * @brief The divider of the PCLK1
 */
typedef enum {
    RCC_PCLK1_DIV_1  = RCC_CR0_PCLK1_DIV_1,  /*!< PCLK1 divider is 1 */
    RCC_PCLK1_DIV_2  = RCC_CR0_PCLK1_DIV_2,  /*!< PCLK1 divider is 2 */
    RCC_PCLK1_DIV_4  = RCC_CR0_PCLK1_DIV_4,  /*!< PCLK1 divider is 4 */
    RCC_PCLK1_DIV_8  = RCC_CR0_PCLK1_DIV_8,  /*!< PCLK1 divider is 8 */
    RCC_PCLK1_DIV_16 = RCC_CR0_PCLK1_DIV_16  /*!< PCLK1 divider is 16 */
} rcc_pclk1_div_t;

/**
 * @brief The divider of the MCO CLK
 */
typedef enum {
    RCC_MCO_CLK_DIV_1  = RCC_CR0_MCO_CLK_DIV_1,  /*!< MCO CLK divider is 1 */
    RCC_MCO_CLK_DIV_2  = RCC_CR0_MCO_CLK_DIV_2,  /*!< MCO CLK divider is 2 */
    RCC_MCO_CLK_DIV_4  = RCC_CR0_MCO_CLK_DIV_4,  /*!< MCO CLK divider is 4 */
    RCC_MCO_CLK_DIV_8  = RCC_CR0_MCO_CLK_DIV_8,  /*!< MCO CLK divider is 8 */
    RCC_MCO_CLK_DIV_16 = RCC_CR0_MCO_CLK_DIV_16, /*!< MCO CLK divider is 16 */
} rcc_mco_clk_div_t;

/**
 * @brief RCC peripherals
 */
typedef enum {
    RCC_PERIPHERAL_SAC      = 0x00,  /*!< RCC peripheral SAC */
    RCC_PERIPHERAL_SEC      = 0x01,  /*!< RCC peripheral SEC */
    RCC_PERIPHERAL_CRC      = 0x02,  /*!< RCC peripheral CRC */
    RCC_PERIPHERAL_RTC      = 0x03,  /*!< RCC peripheral RTC */
    RCC_PERIPHERAL_WDG      = 0x04,  /*!< RCC peripheral WDG */
    RCC_PERIPHERAL_IWDG     = 0x05,  /*!< RCC peripheral IWDG */
    RCC_PERIPHERAL_LPTIMER0 = 0x06,  /*!< RCC peripheral LPTIMER0 */
    RCC_PERIPHERAL_BSTIMER1 = 0x07,  /*!< RCC peripheral BSTIMER1 */
    RCC_PERIPHERAL_BSTIMER0 = 0x08,  /*!< RCC peripheral BSTIMER0 */
    RCC_PERIPHERAL_TIMER3   = 0x09,  /*!< RCC peripheral TIMER3 */
    RCC_PERIPHERAL_TIMER2   = 0x0A,  /*!< RCC peripheral TIMER2 */
    RCC_PERIPHERAL_TIMER1   = 0x0B,  /*!< RCC peripheral TIMER1 */
    RCC_PERIPHERAL_TIMER0   = 0x0C,  /*!< RCC peripheral TIMER0 */
    RCC_PERIPHERAL_GPIOA    = 0x0D,  /*!< RCC peripheral GPIOA */
    RCC_PERIPHERAL_GPIOB    = 0x20,  /*!< RCC peripheral GPIOB */
    RCC_PERIPHERAL_GPIOC    = 0x21,  /*!< RCC peripheral GPIOC */
    RCC_PERIPHERAL_GPIOD    = 0x22,  /*!< RCC peripheral GPIOD */
    RCC_PERIPHERAL_LORA     = 0x0E,  /*!< RCC peripheral LORA */
    RCC_PERIPHERAL_DAC      = 0x0F,  /*!< RCC peripheral DAC */
    RCC_PERIPHERAL_LCD      = 0x10,  /*!< RCC peripheral LCD */
    RCC_PERIPHERAL_AFEC     = 0x11,  /*!< RCC peripheral AFEC */
    RCC_PERIPHERAL_ADC      = 0x12,  /*!< RCC peripheral ADC */
    RCC_PERIPHERAL_I2C2     = 0x14,  /*!< RCC peripheral I2C2 */
    RCC_PERIPHERAL_I2C1     = 0x15,  /*!< RCC peripheral I2C1 */
    RCC_PERIPHERAL_I2C0     = 0x16,  /*!< RCC peripheral I2C0 */
    RCC_PERIPHERAL_QSPI     = 0x17,  /*!< RCC peripheral QSPI */
    RCC_PERIPHERAL_SSP2     = 0x18,  /*!< RCC peripheral SSP2 */
    RCC_PERIPHERAL_SSP1     = 0x19,  /*!< RCC peripheral SSP1 */
    RCC_PERIPHERAL_SSP0     = 0x1A,  /*!< RCC peripheral SSP0 */
    RCC_PERIPHERAL_LPUART   = 0x1B,  /*!< RCC peripheral LPUART */
    RCC_PERIPHERAL_UART3    = 0x1C,  /*!< RCC peripheral UART3 */
    RCC_PERIPHERAL_UART2    = 0x1D,  /*!< RCC peripheral UART2 */
    RCC_PERIPHERAL_UART1    = 0x1E,  /*!< RCC peripheral UART1 */
    RCC_PERIPHERAL_UART0    = 0x1F,  /*!< RCC peripheral UART0 */

    RCC_PERIPHERAL_DMA1     = 0x30,  /*!< RCC peripheral DMA1 */
    RCC_PERIPHERAL_DMA0     = 0x31,  /*!< RCC peripheral DMA0 */
    RCC_PERIPHERAL_I2S      = 0x32,  /*!< RCC peripheral I2S */
    RCC_PERIPHERAL_RNGC     = 0x33,  /*!< RCC peripheral RNGC */
    RCC_PERIPHERAL_LPTIMER1 = 0x34,  /*!< RCC peripheral LPTIMER1 */

    RCC_PERIPHERAL_SYSCFG   = 0x40,  /*!< RCC peripheral SYSCFG */
    RCC_PERIPHERAL_PWR      = 0x41,  /*!< RCC peripheral PWR */
} rcc_peripheral_t;

/**
 * @brief RCC reset mask
 */
typedef enum {
    RCC_RESET_MASK_SEC = 0x02,  /*!< SEC reset mask */
    RCC_RESET_MASK_CPU = 0x04,  /*!< CPU reset mask */
    RCC_RESET_MASK_EFC = 0x08,  /*!< EFC reset mask */
    RCC_RESET_MASK_WDG = 0x10,  /*!< WDG reset mask */
    RCC_RESET_MASK_IWDG = 0x20, /*!< IWDG reset mask */
    RCC_RESET_MASK_ALL = 0x3E,  /*!< all reset mask */
}rcc_reset_mask_t;


uint32_t rcc_get_clk_freq(rcc_clk_t clk);

void rcc_enable_oscillator(rcc_oscillator_t osc, bool new_state);

void rcc_set_sys_clk_source(rcc_sys_clk_source_t clk_source);
void rcc_set_systick_source(rcc_systick_source_t clk_source);
void rcc_set_uart0_clk_source(rcc_uart0_clk_source_t clk_source);
void rcc_set_uart1_clk_source(rcc_uart1_clk_source_t clk_source);
void rcc_set_uart2_clk_source(rcc_uart2_clk_source_t clk_source);
void rcc_set_uart3_clk_source(rcc_uart3_clk_source_t clk_source);
void rcc_set_lptimer0_clk_source(rcc_lptimer0_clk_source_t clk_source);
void rcc_set_lptimer1_clk_source(rcc_lptimer1_clk_source_t clk_source);
void rcc_set_lcd_clk_source(rcc_lcd_clk_source_t clk_source);
void rcc_set_lpuart_clk_source(rcc_lpuart_clk_source_t clk_source);
void rcc_set_rtc_clk_source(rcc_rtc_clk_source_t clk_source);
void rcc_set_iwdg_clk_source(rcc_iwdg_clk_source_t clk_source);
void rcc_set_adc_clk_source(rcc_adc_clk_source_t clk_source);
void rcc_set_qspi_clk_source(rcc_qspi_clk_source_t clk_source);
void rcc_set_i2s_clk_source(rcc_i2s_clk_source_t clk_source);

rcc_sys_clk_source_t rcc_get_sys_clk_source(void);
rcc_systick_source_t rcc_get_systick_source(void);
rcc_uart0_clk_source_t rcc_get_uart0_clk_source(void);
rcc_uart1_clk_source_t rcc_get_uart1_clk_source(void);
rcc_uart2_clk_source_t rcc_get_uart2_clk_source(void);
rcc_uart3_clk_source_t rcc_get_uart3_clk_source(void);
rcc_lptimer0_clk_source_t rcc_get_lptimer0_clk_source(void);
rcc_lptimer1_clk_source_t rcc_get_lptimer1_clk_source(void);
rcc_lcd_clk_source_t rcc_get_lcd_clk_source(void);
rcc_lpuart_clk_source_t rcc_get_lpuart_clk_source(void);
rcc_rtc_clk_source_t rcc_get_rtc_clk_source(void);
rcc_iwdg_clk_source_t rcc_get_iwdg_clk_source(void);
rcc_adc_clk_source_t rcc_get_adc_clk_source(void);
rcc_qspi_clk_source_t rcc_get_qspi_clk_source(void);
rcc_i2s_clk_source_t rcc_get_i2s_clk_source(void);

void rcc_set_hclk_div(rcc_hclk_div_t hclk_div);
void rcc_set_pclk_div(rcc_pclk0_div_t pclk0_div, rcc_pclk1_div_t pclk1_div);

void rcc_enable_peripheral_clk(uint8_t peripheral, bool new_state);

void rcc_set_mco_clk_source(rcc_mco_clk_source_t clk_source);
rcc_mco_clk_source_t rcc_get_mco_clk_source(void);
void rcc_set_mco_clk_div(rcc_mco_clk_div_t mco_clk_div);
void rcc_enable_mco_clk_output(bool new_state);

void rcc_set_i2s_mclk_div(uint8_t div);
void rcc_set_i2s_sclk_div(uint8_t div);

void rcc_rst_peripheral(uint8_t peripheral, bool new_state);

void rcc_set_reset_mask(uint32_t reset_mask);
uint32_t rcc_get_reset_mask(void);

#ifdef __cplusplus
}
#endif
#endif //__TREMO_RCC_H

/**
 * @} 
 * @}
 */
