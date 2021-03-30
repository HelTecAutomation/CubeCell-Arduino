/**
 ******************************************************************************
 * @file    tremo_regs.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   Header file of tremo registers.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup REGISTERS
 * @{
 */
#ifndef _TREMO_REGS_H_
#define _TREMO_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "system_cm4.h"

/**
 * @brief Read the value of the register
 */
#define TREMO_REG_RD(addr)           (*(volatile uint32_t*)(addr))
    
/**
 * @brief Write the value into the register
 */
#define TREMO_REG_WR(addr, value)    (*(volatile uint32_t*)(addr)) = (value)

/**
 * @brief Read the value of the analog register
 */
#define TREMO_ANALOG_RD(addr)        (*(volatile uint32_t*)((AFEC_BASE) | ((addr) << 2)))

/**
 * @brief Write the value into the analog register
 */
#define TREMO_ANALOG_WR(addr, value) (*(volatile uint32_t*)((AFEC_BASE) | ((addr) << 2))) = (value)

/**
 * @brief Set the bit fields of the register
 */
#define TREMO_REG_SET(reg, mask, value) (reg = (reg & (~(mask))) | (value))

/**
 * @brief Enable or disable a bit of the register
 */
#define TREMO_REG_EN(reg, mask, enable) \
    do {                                \
        if (enable)                     \
            reg |= (mask);              \
        else                            \
            reg &= ~(mask);             \
    } while (0)

#define FLASH_BASE      ((uint32_t)0x08000000) /*!< The Flash base address */
#define FLASH_INFO_BASE ((uint32_t)0x10000000) /*!< The Flash info base address */
#define SRAM_BASE       ((uint32_t)0x20000000) /*!< The SYS RAM base address */
#define RET_SRAM_BASE   ((uint32_t)0x30000000) /*!< The Retention RAM base address */
#define PERIPH_BASE     ((uint32_t)0x40000000) /*!< The peripheral registers base address */
#define AFEC_BASE       ((uint32_t)0x40008000) /*!< The AFEC registers base address */


/******************************************************************************/
/**
 * @brief RCC
 */
typedef struct {
    __IO uint32_t CR0;    /*!< control register 0 */
    __IO uint32_t CR1;    /*!< control register 1 */
    __IO uint32_t CR2;    /*!< control register 2 */
    __IO uint32_t CGR0;   /*!< clock generation register 0 */
    __IO uint32_t CGR1;   /*!< clock generation register 1 */
    __IO uint32_t CGR2;   /*!< clock generation register 2 */
    __IO uint32_t RST0;   /*!< reset register 0 */
    __IO uint32_t RST1;   /*!< reset register 1 */
    __IO uint32_t RST_SR; /*!< reset status register 0 */
    __IO uint32_t RST_CR; /*!< reset control register 0 */
    __I uint32_t SR;      /*!< status register */
    __I uint32_t SR1;     /*!< status register 1 */
    __IO uint32_t CR3;    /*!< control register 3 */
} rcc_t;

#define RCC_BASE (PERIPH_BASE + 0x00000000) /*!< RCC base address */
#define RCC      ((rcc_t*)RCC_BASE) /*!< RCC peripheral */

/****************************RCC CR0 bit definition****************************/
#define RCC_CR0_STCLKEN_SEL_MASK   ((uint32_t)0x02000000)
#define RCC_CR0_STCLKEN_SEL_XO32K  ((uint32_t)0x00000000)
#define RCC_CR0_STCLKEN_SEL_RCO32K ((uint32_t)0x02000000)

#define RCC_CR0_MCO_CLK_DIV_MASK ((uint32_t)0x01C00000)
#define RCC_CR0_MCO_CLK_DIV_1    ((uint32_t)0x00000000)
#define RCC_CR0_MCO_CLK_DIV_2    ((uint32_t)0x01000000)
#define RCC_CR0_MCO_CLK_DIV_4    ((uint32_t)0x01400000)
#define RCC_CR0_MCO_CLK_DIV_8    ((uint32_t)0x01800000)
#define RCC_CR0_MCO_CLK_DIV_16   ((uint32_t)0x1C00000)

#define RCC_CR0_MCO_CLK_SEL_MASK    ((uint32_t)0x00380000)
#define RCC_CR0_MCO_CLK_SEL_RCO32K  ((uint32_t)0x00000000)
#define RCC_CR0_MCO_CLK_SEL_XO32K   ((uint32_t)0x00080000)
#define RCC_CR0_MCO_CLK_SEL_RCO4M   ((uint32_t)0x00100000)
#define RCC_CR0_MCO_CLK_SEL_XO24M   ((uint32_t)0x00180000)
#define RCC_CR0_MCO_CLK_SEL_XO32M   ((uint32_t)0x00200000)
#define RCC_CR0_MCO_CLK_SEL_RCO48M  ((uint32_t)0x00280000)
#define RCC_CR0_MCO_CLK_SEL_PLL     ((uint32_t)0x00300000)
#define RCC_CR0_MCO_CLK_SEL_SYSCLCK ((uint32_t)0x00380000)

#define RCC_CR0_MCO_CLK_OUT_EN_MASK ((uint32_t)0x00040000)

#define RCC_CR0_PCLK1_DIV_MASK ((uint32_t)0x00038000)
#define RCC_CR0_PCLK1_DIV_1    ((uint32_t)0x00000000)
#define RCC_CR0_PCLK1_DIV_2    ((uint32_t)0x00008000)
#define RCC_CR0_PCLK1_DIV_4    ((uint32_t)0x00010000)
#define RCC_CR0_PCLK1_DIV_8    ((uint32_t)0x00018000)
#define RCC_CR0_PCLK1_DIV_16   ((uint32_t)0x00020000)

#define RCC_CR0_SYSCLK_SEL_MASK        ((uint32_t)0x00007000)
#define RCC_CR0_SYSCLK_SEL_RCO48M_DIV2 ((uint32_t)0x00000000)
#define RCC_CR0_SYSCLK_SEL_RCO32K      ((uint32_t)0x00001000)
#define RCC_CR0_SYSCLK_SEL_XO32K       ((uint32_t)0x00002000)
#define RCC_CR0_SYSCLK_SEL_PLL         ((uint32_t)0x00003000)
#define RCC_CR0_SYSCLK_SEL_XO24M       ((uint32_t)0x00004000)
#define RCC_CR0_SYSCLK_SEL_XO32M       ((uint32_t)0x00005000)
#define RCC_CR0_SYSCLK_SEL_RCO4M       ((uint32_t)0x00006000)
#define RCC_CR0_SYSCLK_SEL_RCO48M      ((uint32_t)0x00007000)

#define RCC_CR0_HCLK_DIV_MASK ((uint32_t)0x00000F00)
#define RCC_CR0_HCLK_DIV_1    ((uint32_t)0x00000000)
#define RCC_CR0_HCLK_DIV_2    ((uint32_t)0x00000100)
#define RCC_CR0_HCLK_DIV_4    ((uint32_t)0x00000200)
#define RCC_CR0_HCLK_DIV_8    ((uint32_t)0x00000300)
#define RCC_CR0_HCLK_DIV_16   ((uint32_t)0x00000400)
#define RCC_CR0_HCLK_DIV_32   ((uint32_t)0x00000500)
#define RCC_CR0_HCLK_DIV_64   ((uint32_t)0x00000600)
#define RCC_CR0_HCLK_DIV_128  ((uint32_t)0x00000700)
#define RCC_CR0_HCLK_DIV_256  ((uint32_t)0x00000800)
#define RCC_CR0_HCLK_DIV_512  ((uint32_t)0x00000900)

#define RCC_CR0_PCLK0_DIV_MASK ((uint32_t)0x000000E0)
#define RCC_CR0_PCLK0_DIV_1    ((uint32_t)0x00000000)
#define RCC_CR0_PCLK0_DIV_2    ((uint32_t)0x00000020)
#define RCC_CR0_PCLK0_DIV_4    ((uint32_t)0x00000040)
#define RCC_CR0_PCLK0_DIV_8    ((uint32_t)0x00000060)
#define RCC_CR0_PCLK0_DIV_16   ((uint32_t)0x00000080)

/****************************RCC CR1 bit definition****************************/
#define RCC_CR1_LPTIMER1_EXTCLK_SEL_MASK ((uint32_t)0x00000800)

#define RCC_CR1_LPTIMER0_EXTCLK_SEL_MASK ((uint32_t)0x00000400)

#define RCC_CR1_LPTIMER1_CLK_SEL_MASK   ((uint32_t)0x00000300)
#define RCC_CR1_LPTIMER1_CLK_SEL_PCLK0  ((uint32_t)0x00000000)
#define RCC_CR1_LPTIMER1_CLK_SEL_RCO4M  ((uint32_t)0x00000100)
#define RCC_CR1_LPTIMER1_CLK_SEL_XO32K  ((uint32_t)0x00000200)
#define RCC_CR1_LPTIMER1_CLK_SEL_RCO32K ((uint32_t)0x00000300)

#define RCC_CR1_LPTIMER0_CLK_SEL_MASK   ((uint32_t)0x000000C0)
#define RCC_CR1_LPTIMER0_CLK_SEL_PCLK0  ((uint32_t)0x00000000)
#define RCC_CR1_LPTIMER0_CLK_SEL_RCO4M  ((uint32_t)0x00000040)
#define RCC_CR1_LPTIMER0_CLK_SEL_XO32K  ((uint32_t)0x00000080)
#define RCC_CR1_LPTIMER0_CLK_SEL_RCO32K ((uint32_t)0x000000C0)

#define RCC_CR1_LCD_CLK_SEL_MASK   ((uint32_t)0x00000030)
#define RCC_CR1_LCD_CLK_SEL_XO32K  ((uint32_t)0x00000000)
#define RCC_CR1_LCD_CLK_SEL_RCO32K ((uint32_t)0x00000010)
#define RCC_CR1_LCD_CLK_SEL_RCO4M  ((uint32_t)0x00000020)

#define RCC_CR1_LPUART_CLK_SEL_MASK   ((uint32_t)0x0000000C)
#define RCC_CR1_LPUART_CLK_SEL_XO32K  ((uint32_t)0x00000000)
#define RCC_CR1_LPUART_CLK_SEL_RCO32K ((uint32_t)0x00000004)
#define RCC_CR1_LPUART_CLK_SEL_RCO4M  ((uint32_t)0x00000008)

#define RCC_CR1_RTC_CLK_SEL_MASK   ((uint32_t)0x00000002)
#define RCC_CR1_RTC_CLK_SEL_XO32K  ((uint32_t)0x00000000)
#define RCC_CR1_RTC_CLK_SEL_RCO32K ((uint32_t)0x00000002)

#define RCC_CR1_IWDG_CLK_SEL_MASK   ((uint32_t)0x00000001)
#define RCC_CR1_IWDG_CLK_SEL_XO32K  ((uint32_t)0x00000000)
#define RCC_CR1_IWDG_CLK_SEL_RCO32K ((uint32_t)0x00000001)

/****************************RCC CR2 bit definition****************************/
#define RCC_CR2_UART0_CLK_SEL_MASK  ((uint32_t)0x00018000)
#define RCC_CR2_UART0_CLK_SEL_PCLK0 ((uint32_t)0x00000000)
#define RCC_CR2_UART0_CLK_SEL_RCO4M ((uint32_t)0x00008000)
#define RCC_CR2_UART0_CLK_SEL_XO32K ((uint32_t)0x00010000)
#define RCC_CR2_UART0_CLK_SEL_XO24M ((uint32_t)0x00018000)

#define RCC_CR2_UART1_CLK_SEL_MASK  ((uint32_t)0x00006000)
#define RCC_CR2_UART1_CLK_SEL_PCLK0 ((uint32_t)0x00000000)
#define RCC_CR2_UART1_CLK_SEL_RCO4M ((uint32_t)0x00002000)
#define RCC_CR2_UART1_CLK_SEL_XO32K ((uint32_t)0x00004000)
#define RCC_CR2_UART1_CLK_SEL_XO24M ((uint32_t)0x00006000)

#define RCC_CR2_UART2_CLK_SEL_MASK  ((uint32_t)0x00001800)
#define RCC_CR2_UART2_CLK_SEL_PCLK1 ((uint32_t)0x00000000)
#define RCC_CR2_UART2_CLK_SEL_RCO4M ((uint32_t)0x00000800)
#define RCC_CR2_UART2_CLK_SEL_XO32K ((uint32_t)0x00001000)
#define RCC_CR2_UART2_CLK_SEL_XO24M ((uint32_t)0x00001800)

#define RCC_CR2_UART3_CLK_SEL_MASK  ((uint32_t)0x00000600)
#define RCC_CR2_UART3_CLK_SEL_PCLK1 ((uint32_t)0x00000000)
#define RCC_CR2_UART3_CLK_SEL_RCO4M ((uint32_t)0x00000200)
#define RCC_CR2_UART3_CLK_SEL_XO32K ((uint32_t)0x00000400)
#define RCC_CR2_UART3_CLK_SEL_XO24M ((uint32_t)0x00000600)

#define RCC_CR2_SCC_CLK_SEL_MASK   ((uint32_t)0x00000180)
#define RCC_CR2_SCC_CLK_SEL_PCLK1  ((uint32_t)0x00000000)
#define RCC_CR2_SCC_CLK_SEL_SYSCLK ((uint32_t)0x00000080)
#define RCC_CR2_SCC_CLK_SEL_PLL    ((uint32_t)0x00000100)
#define RCC_CR2_SCC_CLK_SEL_EXT    ((uint32_t)0x00000180)

#define RCC_CR2_ADC_CLK_SEL_MASK   ((uint32_t)0x00000060)
#define RCC_CR2_ADC_CLK_SEL_PCLK1  ((uint32_t)0x00000000)
#define RCC_CR2_ADC_CLK_SEL_SYSCLK ((uint32_t)0x00000020)
#define RCC_CR2_ADC_CLK_SEL_PLL    ((uint32_t)0x00000040)
#define RCC_CR2_ADC_CLK_SEL_RCO48M ((uint32_t)0x00000060)

#define RCC_CR2_I2S_CLK_SEL_MASK    ((uint32_t)0x0000001C)
#define RCC_CR2_I2S_CLK_SEL_PCLK0   ((uint32_t)0x00000000)
#define RCC_CR2_I2S_CLK_SEL_XO24M   ((uint32_t)0x00000004)
#define RCC_CR2_I2S_CLK_SEL_PLL     ((uint32_t)0x00000008)
#define RCC_CR2_I2S_CLK_SEL_XO32M   ((uint32_t)0x0000000C)
#define RCC_CR2_I2S_CLK_SEL_EXT_CLK ((uint32_t)0x00000010)

#define RCC_CR2_QSPI_CLK_SEL_MASK   ((uint32_t)0x00000003)
#define RCC_CR2_QSPI_CLK_SEL_HCLK   ((uint32_t)0x00000000)
#define RCC_CR2_QSPI_CLK_SEL_SYSCLK ((uint32_t)0x00000001)
#define RCC_CR2_QSPI_CLK_SEL_PLL    ((uint32_t)0x00000002)

/****************************RCC CR3 bit definition****************************/
#define RCC_CR3_I2S_MCLK_DIV_MASK ((uint32_t)0x0000FF00)

#define RCC_CR3_I2S_SCLK_DIV_MASK ((uint32_t)0x000000FF)

/****************************RCC CGR0 bit definition***************************/
#define RCC_CGR0_PWR_CLK_EN_MASK    ((uint32_t)0x80000000)
#define RCC_CGR0_DMAC0_CLK_EN_MASK  ((uint32_t)0x40000000)
#define RCC_CGR0_DMAC1_CLK_EN_MASK  ((uint32_t)0x20000000)
#define RCC_CGR0_CRC_CLK_EN_MASK    ((uint32_t)0x10000000)
#define RCC_CGR0_BSTIMER0_CLK_EN_MASK  ((uint32_t)0x08000000)
#define RCC_CGR0_BSTIMER1_CLK_EN_MASK  ((uint32_t)0x04000000)
#define RCC_CGR0_IOM0_CLK_EN_MASK   ((uint32_t)0x02000000)
#define RCC_CGR0_IOM1_CLK_EN_MASK   ((uint32_t)0x01000000)
#define RCC_CGR0_IOM2_CLK_EN_MASK   ((uint32_t)0x00800000)
#define RCC_CGR0_IOM3_CLK_EN_MASK   ((uint32_t)0x00400000)
#define RCC_CGR0_SYSCFG_CLK_EN_MASK ((uint32_t)0x00200000)
#define RCC_CGR0_UART0_CLK_EN_MASK  ((uint32_t)0x00100000)
#define RCC_CGR0_UART1_CLK_EN_MASK  ((uint32_t)0x00080000)
#define RCC_CGR0_UART2_CLK_EN_MASK  ((uint32_t)0x00040000)
#define RCC_CGR0_UART3_CLK_EN_MASK  ((uint32_t)0x00020000)
#define RCC_CGR0_LPUART_CLK_EN_MASK ((uint32_t)0x00010000)
#define RCC_CGR0_SSP0_CLK_EN_MASK   ((uint32_t)0x00008000)
#define RCC_CGR0_SSP1_CLK_EN_MASK   ((uint32_t)0x00004000)
#define RCC_CGR0_SSP2_CLK_EN_MASK   ((uint32_t)0x00002000)
#define RCC_CGR0_I2C0_CLK_EN_MASK   ((uint32_t)0x00001000)
#define RCC_CGR0_I2C1_CLK_EN_MASK   ((uint32_t)0x00000800)
#define RCC_CGR0_I2C2_CLK_EN_MASK   ((uint32_t)0x00000400)
#define RCC_CGR0_SCC_CLK_EN_MASK    ((uint32_t)0x00000200)
#define RCC_CGR0_ADC_CLK_EN_MASK    ((uint32_t)0x00000100)
#define RCC_CGR0_AFEC_CLK_EN_MASK   ((uint32_t)0x00000080)
#define RCC_CGR0_LCD_CLK_EN_MASK    ((uint32_t)0x00000040)
#define RCC_CGR0_DAC_CLK_EN_MASK    ((uint32_t)0x00000020)
#define RCC_CGR0_LORA_CLK_EN_MASK   ((uint32_t)0x00000010)
#define RCC_CGR0_TIMER0_CLK_EN_MASK  ((uint32_t)0x00000008)
#define RCC_CGR0_TIMER1_CLK_EN_MASK  ((uint32_t)0x00000004)
#define RCC_CGR0_TIMER2_CLK_EN_MASK  ((uint32_t)0x00000002)
#define RCC_CGR0_TIMER3_CLK_EN_MASK  ((uint32_t)0x00000001)

/****************************RCC CGR1 bit definition***************************/
#define RCC_CGR1_LPTIMER1_PCLK_EN_MASK ((uint32_t)0x00001000)
#define RCC_CGR1_LPTIMER1_CLK_EN_MASK  ((uint32_t)0x00000800)
#define RCC_CGR1_RNGC_CLK_EN_MASK    ((uint32_t)0x00000400)
#define RCC_CGR1_LPTIMER0_PCLK_EN_MASK  ((uint32_t)0x00000200)
#define RCC_CGR1_I2S_CLK_EN_MASK     ((uint32_t)0x00000100)
#define RCC_CGR1_SAC_CLK_EN_MASK     ((uint32_t)0x00000080)
#define RCC_CGR1_WDG_CNT_CLK_EN_MASK ((uint32_t)0x00000040)
#define RCC_CGR1_QSPI_CLK_EN_MASK    ((uint32_t)0x00000020)
#define RCC_CGR1_LPTIMER0_CLK_EN_MASK   ((uint32_t)0x00000010)
#define RCC_CGR1_IWDG_CLK_EN_MASK    ((uint32_t)0x00000008)
#define RCC_CGR1_WDG_CLK_EN_MASK     ((uint32_t)0x00000004)
#define RCC_CGR1_RTC_CLK_EN_MASK     ((uint32_t)0x00000002)
#define RCC_CGR1_SEC_CLK_EN_MASK     ((uint32_t)0x00000001)

/****************************RCC CGR2 bit definition***************************/
#define RCC_CGR2_LPTIMER1_AON_CLK_EN_MASK ((uint32_t)0x00000020)
#define RCC_CGR2_LPTIMER0_AON_CLK_EN_MASK  ((uint32_t)0x00000010)
#define RCC_CGR2_LCD_AON_CLK_EN_MASK    ((uint32_t)0x00000008)
#define RCC_CGR2_LPUART_AON_CLK_EN_MASK ((uint32_t)0x00000004)
#define RCC_CGR2_RTC_AON_CLK_EN_MASK    ((uint32_t)0x00000002)
#define RCC_CGR2_IWDG_CLK_EN_MASK       ((uint32_t)0x00000001)

/****************************RCC RST0 bit definition***************************/
#define RCC_RST0_UART0_RST_N_MASK  ((uint32_t)0x80000000)
#define RCC_RST0_UART1_RST_N_MASK  ((uint32_t)0x40000000)
#define RCC_RST0_UART2_RST_N_MASK  ((uint32_t)0x20000000)
#define RCC_RST0_UART3_RST_N_MASK  ((uint32_t)0x10000000)
#define RCC_RST0_LPUART_RST_N_MASK ((uint32_t)0x08000000)
#define RCC_RST0_SSP0_RST_N_MASK   ((uint32_t)0x04000000)
#define RCC_RST0_SSP1_RST_N_MASK   ((uint32_t)0x02000000)
#define RCC_RST0_SSP2_RST_N_MASK   ((uint32_t)0x01000000)
#define RCC_RST0_QSPI_RST_N_MASK   ((uint32_t)0x00800000)
#define RCC_RST0_I2C0_RST_N_MASK   ((uint32_t)0x00400000)
#define RCC_RST0_I2C1_RST_N_MASK   ((uint32_t)0x00200000)
#define RCC_RST0_I2C2_RST_N_MASK   ((uint32_t)0x00100000)
#define RCC_RST0_SCC_RST_N_MASK    ((uint32_t)0x00080000)
#define RCC_RST0_ADC_RST_N_MASK    ((uint32_t)0x00040000)
#define RCC_RST0_AFEC_RST_N_MASK   ((uint32_t)0x00020000)
#define RCC_RST0_LCD_RST_N_MASK    ((uint32_t)0x00010000)
#define RCC_RST0_DAC_RST_N_MASK    ((uint32_t)0x00008000)
#define RCC_RST0_LORA_RST_N_MASK   ((uint32_t)0x00004000)
#define RCC_RST0_IOM_RST_N_MASK    ((uint32_t)0x00002000)
#define RCC_RST0_TIMER0_RST_N_MASK ((uint32_t)0x00001000)
#define RCC_RST0_TIMER1_RST_N_MASK ((uint32_t)0x00000800)
#define RCC_RST0_TIMER2_RST_N_MASK ((uint32_t)0x00000400)
#define RCC_RST0_TIMER3_RST_N_MASK ((uint32_t)0x00000200)
#define RCC_RST0_BSTIMER0_RST_N_MASK  ((uint32_t)0x00000100)
#define RCC_RST0_BSTIMER1_RST_N_MASK  ((uint32_t)0x00000080)
#define RCC_RST0_LPTIMER0_RST_N_MASK  ((uint32_t)0x00000040)
#define RCC_RST0_IWDG_RST_N_MASK   ((uint32_t)0x00000020)
#define RCC_RST0_WDG_RST_N_MASK    ((uint32_t)0x00000010)
#define RCC_RST0_RTC_RST_N_MASK    ((uint32_t)0x00000008)
#define RCC_RST0_CRC_RST_N_MASK    ((uint32_t)0x00000004)
#define RCC_RST0_SEC_RST_N_MASK    ((uint32_t)0x00000002)
#define RCC_RST0_SAC_RST_N_MASK    ((uint32_t)0x00000001)

/****************************RCC RST1 bit definition***************************/
#define RCC_RST1_LPTIMER1_RST_N_MASK ((uint32_t)0x00000010)
#define RCC_RST1_RNGC_RST_N_MASK   ((uint32_t)0x00000008)
#define RCC_RST1_I2S_RST_N_MASK    ((uint32_t)0x00000004)
#define RCC_RST1_DMAC0_RST_N_MASK  ((uint32_t)0x00000002)
#define RCC_RST1_DMAC1_RST_N_MASK  ((uint32_t)0x00000001)

/****************************RCC RST SR bit definition*************************/
#define RCC_RST_SR_BOR_RESET_SR     ((uint32_t)0x00000040)
#define RCC_RST_SR_IWDG_RESET_SR    ((uint32_t)0x00000020)
#define RCC_RST_SR_WDG_RESET_SR     ((uint32_t)0x00000010)
#define RCC_RST_SR_EFC_RESET_SR     ((uint32_t)0x00000008)
#define RCC_RST_SR_CPU_RESET_SR     ((uint32_t)0x00000004)
#define RCC_RST_SR_SEC_RESET_SR     ((uint32_t)0x00000002)
#define RCC_RST_SR_STANDBY_RESET_SR ((uint32_t)0x00000001)

/****************************RCC RST CR bit definition*************************/
#define RCC_RST_CR_RESET_REQ_EN_MASK      ((uint32_t)0x0000003E)
#define RCC_RST_CR_IWDG_RESET_REQ_EN_MASK ((uint32_t)0x00000020)
#define RCC_RST_CR_WDG_RESET_REQ_EN_MASK  ((uint32_t)0x00000010)
#define RCC_RST_CR_EFC_RESET_REQ_EN_MASK  ((uint32_t)0x00000008)
#define RCC_RST_CR_CPU_RESET_REQ_EN_MASK  ((uint32_t)0x00000004)
#define RCC_RST_CR_SEC_RESET_REQ_EN_MASK  ((uint32_t)0x00000002)

/****************************RCC SR bit definition*****************************/
#define RCC_SR_ALL_DONE               ((uint32_t)0x0000003F)
#define RCC_SR_LPTIMER1_AON_CLK_EN_DONE ((uint32_t)0x00000020)
#define RCC_SR_LPTIM_AON_CLK_EN_DONE  ((uint32_t)0x00000010)
#define RCC_SR_LCD_AON_CLK_EN_DONE    ((uint32_t)0x00000008)
#define RCC_SR_LPUART_AON_CLK_EN_DONE ((uint32_t)0x00000004)
#define RCC_SR_RTC_AON_CLK_EN_DONE    ((uint32_t)0x00000002)
#define RCC_SR_IWDG_AON_CLK_EN_DONE   ((uint32_t)0x00000001)

/****************************RCC SR1 bit definition***************************/
#define RCC_SR1_LPTIMER1_CLK_EN_SYNC     ((uint32_t)0x00100000)
#define RCC_SR1_LPTIMER1_AON_CLK_EN_SYNC ((uint32_t)0x00080000)
#define RCC_SR1_UART0_CLK_EN_SYNC      ((uint32_t)0x00040000)
#define RCC_SR1_UART1_CLK_EN_SYNC      ((uint32_t)0x00020000)
#define RCC_SR1_UART2_CLK_EN_SYNC      ((uint32_t)0x00010000)
#define RCC_SR1_UART3_CLK_EN_SYNC      ((uint32_t)0x00008000)
#define RCC_SR1_SCC_CLK_EN_SYNC        ((uint32_t)0x00004000)
#define RCC_SR1_ADC_CLK_EN_SYNC        ((uint32_t)0x00002000)
#define RCC_SR1_LPTIMER0_CLK_EN_SYNC      ((uint32_t)0x00001000)
#define RCC_SR1_QSPI_CLK_EN_SYNC       ((uint32_t)0x00000800)
#define RCC_SR1_LPUART_CLK_EN_SYNC     ((uint32_t)0x00000400)
#define RCC_SR1_LCD_CLK_EN_SYNC        ((uint32_t)0x00000200)
#define RCC_SR1_IWDG_CLK_EN_SYNC       ((uint32_t)0x00000100)
#define RCC_SR1_RTC_CLK_EN_SYNC        ((uint32_t)0x00000080)
#define RCC_SR1_MCO_CLK_EN_SYNC        ((uint32_t)0x00000040)
#define RCC_SR1_I2S_CLK_EN_SYNC        ((uint32_t)0x00000020)
#define RCC_SR1_LPTIMER0_AON_CLK_EN_SYNC  ((uint32_t)0x00000010)
#define RCC_SR1_LCD_AON_CLK_EN_SYNC    ((uint32_t)0x00000008)
#define RCC_SR1_LPUART_AON_CLK_EN_SYNC ((uint32_t)0x00000004)
#define RCC_SR1_RTC_AON_CLK_EN_SYNC    ((uint32_t)0x00000002)
#define RCC_SR1_IWDG_AON_CLK_EN_SYNC   ((uint32_t)0x00000001)


/******************************************************************************/
/**
 * @brief SSP
 */
typedef struct __SSP {
    __IO uint32_t CR0;       /*!< control register 0,offset 0x0*/
    __IO uint32_t CR1;       /*!< control register 1,offset 0x4*/
    __IO uint32_t DR;        /*!< data register,offset 0x8*/
    __I uint32_t SR;         /*!< status register,offset 0xC*/
    __IO uint32_t CPSR;      /*!< clock prescale register,offset 0x10*/
    __IO uint32_t IMSC;      /*!< interrupt mask set or clear register,offset 0x14*/
    __I uint32_t RIS;        /*!< raw interrupt status register,offset 0x18*/
    __I uint32_t MIS;        /*!< masked interrupt status register,offset 0x1C*/
    __IO uint32_t ICR;       /*!< interrupt clear register,offset 0x20*/
    __IO uint32_t DMA_CR;    /*!< DMA control register,offset 0x24*/
    __I uint32_t RESV[1006]; /*!< reserved,offset 0x28 ~ 0xFDC*/
    __I uint32_t PERIPH_ID0; /*!< peripheral identification register 0,offset 0xFE0*/
    __I uint32_t PERIPH_ID1; /*!< peripheral identification register 1,offset 0xFE4*/
    __I uint32_t PERIPH_ID2; /*!< peripheral identification register 2,offset 0xFE8*/
    __I uint32_t PERIPH_ID3; /*!< peripheral identification register 3,offset 0xFEC*/
    __I uint32_t PCELL_ID0;  /*!< prime cell identification register 0,offset 0xFF0*/
    __I uint32_t PCELL_ID1;  /*!< prime cell identification register 1,offset 0xFF4*/
    __I uint32_t PCELL_ID2;  /*!< prime cell identification register 2,offset 0xFF8*/
    __I uint32_t PCELL_ID3;  /*!< prime cell identification register 3,offset 0xFFC*/
} ssp_typedef_t,spi_t;

#define SSP0_BASE (PERIPH_BASE + 0x00006000) /*!< SSP0 base address*/
#define SSP1_BASE (PERIPH_BASE + 0x00012000) /*!< SSP1 base address*/
#define SSP2_BASE (PERIPH_BASE + 0x00013000) /*!< SSP2 base address*/
#define SSP0 ((ssp_typedef_t*)SSP0_BASE)     /*!< SSP0 peripheral*/
#define SSP1 ((ssp_typedef_t*)SSP1_BASE)     /*!< SSP1 peripheral*/
#define SSP2 ((ssp_typedef_t*)SSP2_BASE)     /*!< SSP2 peripheral*/

#define SPI_0 ((spi_t*)SSP0_BASE)     /*!< SSP0 peripheral*/
#define SPI_1 ((spi_t*)SSP1_BASE)     /*!< SSP1 peripheral*/
#define SPI_2 ((spi_t*)SSP2_BASE)     /*!< SSP2 peripheral*/

#define SSP_NUM_PORTS 3 /*!< SSP number*/


/******************************************************************************/
/**
 * @brief GPIO
 */
typedef struct {
    __IO uint32_t OER;        /*!< output enable register */
    __IO uint32_t OTYPER;     /*!< output type register */
    __IO uint32_t IER;        /*!< input enable register */
    __IO uint32_t PER;        /*!< pull enable register */
    __IO uint32_t PSR;        /*!< pull select register */
    __I uint32_t IDR;         /*!< input data register */
    __IO uint32_t ODR;        /*!< output data register */
    __O uint32_t BRR;         /*!< bit reset register */
    __O uint32_t BSR;         /*!< bit set register */
    __IO uint32_t DSR;        /*!< dirve set register */
    __IO uint32_t ICR;        /*!< interrupt control register */
    __IO uint32_t IFR;        /*!< interrupt flag register */
    __IO uint32_t WUCR;       /*!< wakeup control register */
    __IO uint32_t WULVL;      /*!< wakeup level register */
    __IO uint32_t AFRL;       /*!< alternate function low register */
    __IO uint32_t AFRH;       /*!< alternate function high register */
    __IO uint32_t STOP3_WUCR; /*!< stop3 wakeup control register */
} gpio_t;

#define GPIO_BASE (0x4001F000)                   /*!< GPIO base address */
#define GPIOA     ((gpio_t*)(GPIO_BASE))         /*!< GPIOA peripheral */
#define GPIOB     ((gpio_t*)(GPIO_BASE + 0x400)) /*!< GPIOB peripheral */
#define GPIOC     ((gpio_t*)(GPIO_BASE + 0x800)) /*!< GPIOC peripheral */
#define GPIOD     ((gpio_t*)(GPIO_BASE + 0xC00)) /*!< GPIOD peripheral */


/******************************************************************************/
/**
 * @brief RTC
 */
typedef struct __RTC {
    __IO uint32_t CTRL;             /*!< control register*/
    __IO uint32_t ALARM0;           /*!< alarm 0*/
    __IO uint32_t ALARM1;           /*!< alarm 1*/
    __IO uint32_t PPM_ADJUST;       /*!< ppm adjust value*/
    __IO uint32_t CALENDAR;         /*!< time hour/minute/second*/
    __IO uint32_t CALENDAR_H;       /*!< time year/month/date*/
    __IO uint32_t CYC_MAX;          /*!< cyc max value*/
    __IO uint32_t SR;               /*!< status register*/
    __I uint32_t SYN_DATA;          /*!< syncronization time hour/minute/second*/
    __I uint32_t SYN_DATA_H;        /*!< syncronization time year/month/date*/
    __IO uint32_t CR1;              /*!< control register 1*/
    __IO uint32_t SR1;              /*!< status register 1*/
    __IO uint32_t CR2;              /*!< control register 2*/
    __I uint32_t SUB_SECOND_CNT;    /*!< subsecond counter*/
    __I uint32_t CYC_CNT;           /*!< cyc counter*/
    __IO uint32_t ALARM0_SUBSECOND; /*!< alarm0 subsecond*/
    __IO uint32_t ALARM1_SUBSECOND; /*!< alarm1 subsecond*/
    __IO uint32_t CALENDAR_R;       /*!< read time hour/minute/second*/
    __IO uint32_t CALENDAR_R_H;     /*!< read time year/month/date*/
} rtc_t;

#define RTC_REG_BASE 0x4000E000               /*!< RTC base address*/
#define RTC          ((rtc_t*)(RTC_REG_BASE)) /*!< RTC peripheral*/

#define CHECK_FUNC_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE)) /*!< check state*/


/******************************************************************************/
/** 
 * @brief UART
 */
typedef struct {
    __IO uint32_t DR;       /*!< data register */
    __IO uint32_t RSC_ECR;  /*!< receive status register / error clear register */
    __I uint32_t RSV0[4];   /*!< reserved */
    __I uint32_t FR;        /*!< flag register */
    __I uint32_t RSV1;      /*!< reserved */
    __IO uint32_t ILPR;     /*!< IRDA low power counter register */
    __IO uint32_t IBRD;     /*!< integer baudrate register */
    __IO uint32_t FBRD;     /*!< fractional baudrate register */
    __IO uint32_t LCR_H;    /*!< line control register */
    __IO uint32_t CR;       /*!< control register */
    __IO uint32_t IFLS;     /*!< interrupt fifo level select register */
    __IO uint32_t IMSC;     /*!< interrupt mask set/clear register */
    __I uint32_t RIS;       /*!< raw interrupt status register */
    __I uint32_t MIS;       /*!< masked interrupt status register */
    __O uint32_t ICR;       /*!< interrupt clear register */
    __IO uint32_t DMACR;    /*!< DMA control register */
    __I uint32_t RSV2[997]; /*!< reserved */
    __I uint32_t ID[8];     /*!< ID registers */
} uart_t;

#define UART0_BASE (PERIPH_BASE + 0x3000)  /*!< UART0 base address */
#define UART1_BASE (PERIPH_BASE + 0x4000)  /*!< UART1 base address */
#define UART2_BASE (PERIPH_BASE + 0x10000) /*!< UART2 base address */
#define UART3_BASE (PERIPH_BASE + 0x11000) /*!< UART3 base address */

#define UART0 ((uart_t*)UART0_BASE) /*!< UART0 peripheral */
#define UART1 ((uart_t*)UART1_BASE) /*!< UART1 peripheral */
#define UART2 ((uart_t*)UART2_BASE) /*!< UART2 peripheral */
#define UART3 ((uart_t*)UART3_BASE) /*!< UART3 peripheral */

/****************************UART CR bit definition***************************/
#define UART_CR_SIR_LPIRDA_EN ((uint32_t)0x00000004)
#define UART_CR_SIR_EN        ((uint32_t)0x00000002)

#define UART_CR_UART_EN ((uint32_t)0x00000001)

#define UART_CR_UART_MODE      ((uint32_t)0x00000030)
#define UART_CR_UART_MODE_NONE ((uint32_t)0x00000000)
#define UART_CR_UART_MODE_RX   ((uint32_t)0x00000020)
#define UART_CR_UART_MODE_TX   ((uint32_t)0x00000010)
#define UART_CR_UART_MODE_TXRX ((uint32_t)0x00000030)

#define UART_CR_FLOW_CTRL         ((uint32_t)0x0000C000)
#define UART_CR_FLOW_CTRL_NONE    ((uint32_t)0x00000000)
#define UART_CR_FLOW_CTRL_CTS     ((uint32_t)0x00008000)
#define UART_CR_FLOW_CTRL_RTS     ((uint32_t)0x00004000)
#define UART_CR_FLOW_CTRL_CTS_RTS ((uint32_t)0x0000C000)

/****************************UART LCR_H bit definition*************************/
#define UART_LCR_H_PEN ((uint32_t)0x00000002)

#define UART_LCR_H_EPS_EVEN ((uint32_t)0x00000004)

#define UART_LCR_H_STOP   ((uint32_t)0x00000008)
#define UART_LCR_H_STOP_1 ((uint32_t)0x00000000)
#define UART_LCR_H_STOP_2 ((uint32_t)0x00000008)

#define UART_LCR_H_FEN ((uint32_t)0x00000010)

#define UART_LCR_H_WLEN   ((uint32_t)0x00000060)
#define UART_LCR_H_WLEN_5 ((uint32_t)0x00000000)
#define UART_LCR_H_WLEN_6 ((uint32_t)0x00000020)
#define UART_LCR_H_WLEN_7 ((uint32_t)0x00000040)
#define UART_LCR_H_WLEN_8 ((uint32_t)0x00000060)

/****************************UART IFLS bit definition**************************/
#define UART_IFLS_TX     ((uint32_t)0x00000007)
#define UART_IFLS_TX_1_8 ((uint32_t)0x00000000)
#define UART_IFLS_TX_1_4 ((uint32_t)0x00000001)
#define UART_IFLS_TX_1_2 ((uint32_t)0x00000002)
#define UART_IFLS_TX_3_4 ((uint32_t)0x00000003)
#define UART_IFLS_TX_7_8 ((uint32_t)0x00000004)

#define UART_IFLS_RX     ((uint32_t)0x00000038)
#define UART_IFLS_RX_1_8 ((uint32_t)0x00000000)
#define UART_IFLS_RX_1_4 ((uint32_t)0x00000008)
#define UART_IFLS_RX_1_2 ((uint32_t)0x00000010)
#define UART_IFLS_RX_3_4 ((uint32_t)0x00000018)
#define UART_IFLS_RX_7_8 ((uint32_t)0x00000020)

/****************************UART DMACR bit definition*************************/
#define UART_DMACR_ONERR_EN_MASK ((uint32_t)0x00000004)

#define UART_DMACR_TX_EN_MASK ((uint32_t)0x00000002)

#define UART_DMACR_RX_EN_MASK ((uint32_t)0x00000001)

/**
 * @brief LPUART
 */
typedef struct __LPUART {
    __IO uint32_t CR0;    /*!< control register 0,offset 0x0*/
    __IO uint32_t CR1;    /*!< control register 1,offset 0x4*/
    __IO uint32_t SR0;    /*!< status register 0,offset 0x8*/
    __IO uint32_t SR1;    /*!< status register 1,offset 0xC*/
    __IO uint32_t DATA;   /*!< data register,offset 0x10*/
} lpuart_t;

#define LPUART_BASE (PERIPH_BASE + 0x5000) /*!< LPUART base address*/
#define LPUART ((lpuart_t*)LPUART_BASE)    /*!< LPUART peripheral*/


/******************************************************************************/
/**
 * @brief EFC
 */
typedef struct {
    __IO uint32_t CR;               /*!< control register, offset 0x0 */
    __IO uint32_t INT_EN;           /*!< interrupt enable register, offset 0x4 */
    __IO uint32_t SR;               /*!< status register, offset 0x8 */
    __IO uint32_t PROGRAM_DATA0;    /*!< program data0 register, offset 0xC */
    __IO uint32_t PROGRAM_DATA1;    /*!< program data1 register, offset 0x10 */
    __IO uint32_t TIMING_CFG;       /*!< timing config register, offset 0x14 */
    __O uint32_t PROTECT_SEQ;       /*!< protect sequence register, offset 0x18 */
    __IO uint32_t RSV0;             /*!< reserved, offset 0x1C */
    __I uint32_t CHIP_PATTERN;      /*!< chip pattern register, offset 0x20 */
    __I uint32_t IP_TRIM_L;         /*!< analog ip trimming low register, offset 0x24 */
    __I uint32_t IP_TRIM_H;         /*!< analog ip trimming high register, offset 0x28 */
    __I uint32_t SN_L;              /*!< serial number low register, offset 0x2C */
    __I uint32_t SN_H;              /*!< serial number high register, offset 0x30 */
    __I uint32_t TEST_INFO_L;       /*!< test info low register, offset 0x34 */
    __I uint32_t TEST_INFO_H;       /*!< test info high register, offset 0x38 */
    __I uint32_t OPTION_CSR_BYTES;  /*!< option control and status register, offset 0x3C */
    __I uint32_t OPTION_EO_BYTES;   /*!< option exe-only bytes register, offset 0x40 */
    __I uint32_t OPTION_WP_BYTES;   /*!< option write-protect bytes register, offset 0x44 */
    __I uint32_t OPTION_SEC_BYTES0; /*!< option secure byte 0 register, offset 0x48 */
    __I uint32_t OPTION_SEC_BYTES1; /*!< option secure byte 1 register, offset 0x4C */
} efc_t;

#define EFC_BASE (PERIPH_BASE + 0x20000)  /*!< EFC base address */

#define EFC ((efc_t*)EFC_BASE)            /*!< EFC peripheral */

/****************************EFC CR bit definition*****************************/
#define EFC_CR_INFO_LOAD_MASK ((uint32_t)0x80000000)

#define EFC_CR_ECC_DISABLE_MASK ((uint32_t)0x00000200)

#define EFC_CR_OPTION_OP_EN_MASK ((uint32_t)0x00000100)

#define EFC_CR_FACTORY_OP_EN_MASK ((uint32_t)0x00000080)

#define EFC_CR_WRITE_RELEASE_EN_MASK ((uint32_t)0x00000040)

#define EFC_CR_PREFETCH_EN_MASK ((uint32_t)0x00000020)

#define EFC_CR_READ_ACC_EN_MASK ((uint32_t)0x00000010)

#define EFC_CR_PROG_MODE_MASK  ((uint32_t)0x00000008)
#define EFC_CR_PROG_MODE_DWORD ((uint32_t)0x00000000)
#define EFC_CR_PROG_MODE_WLINE ((uint32_t)0x00000008)

#define EFC_CR_PROG_EN_MASK ((uint32_t)0x00000004)

#define EFC_CR_PAGE_ERASE_EN_MASK ((uint32_t)0x00000002)

#define EFC_CR_MASS_ERASE_EN_MASK ((uint32_t)0x00000001)

/****************************EFC INT_EN bit definition*************************/

/****************************EFC TIMING_CFG bit definition*********************/
#define EFC_TIMING_CFG_READ_NUM_MASK ((uint32_t)0x000F0000)

/****************************EFC SR bit definition*****************************/
#define EFC_SR_OPTION_WRITE_ERROR ((uint32_t)0x00000010)

#define EFC_SR_PROGRAM_DATA_WAIT ((uint32_t)0x00000004)

#define EFC_SR_READ_NUM_DONE ((uint32_t)0x00000002)

#define EFC_SR_OPERATION_DONE ((uint32_t)0x00000001)


/**
 * @brief LORAC
 */
typedef struct {
    __IO uint32_t SSP_CR0;    /*!< ssp control register 0, offset 0x0 */
    __IO uint32_t SSP_CR1;    /*!< ssp control register 1, offset 0x4 */
    __IO uint32_t SSP_DR;     /*!< ssp data register, offset 0x8 */
    __I uint32_t SSP_SR;      /*!< ssp status register, offset 0xC */
    __IO uint32_t SSP_CPSR;   /*!< ssp clock prescale register, offset 0x10 */
    __IO uint32_t SSP_IMSC;   /*!< ssp interrupt mask set or clear register, offset 0x14 */
    __I uint32_t SSP_RIS;     /*!< ssp raw interrupt status register, offset 0x18 */
    __I uint32_t SSP_MIS;     /*!< ssp masked interrupt status register, offset 0x1C */
    __IO uint32_t SSP_ICR;    /*!< ssp interrupt clear register, offset 0x20 */
    __IO uint32_t SSP_DMA_CR; /*!< ssp DMA control register, offset 0x24 */
    __I uint32_t RESV[54];    /*!< reserved, offset 0x28-0xFC */
    __IO uint32_t CR0;        /*!< control register 0, offset 0x100 */
    __IO uint32_t CR1;        /*!< control register 1, offset 0x104 */
    __I uint32_t SR;          /*!< status register, offset 0x108 */
    __IO uint32_t NSS_CR;     /*!< nss control register, offset 0x10C */
    __IO uint32_t SCK_CR;     /*!< sck control register, offset 0x110 */
    __IO uint32_t MOSI_CR;    /*!< mosi control register, offset 0x114 */
    __IO uint32_t MISO_SR;    /*!< miso control register, offset 0x118 */
} lorac_t;

#define LORAC_BASE (PERIPH_BASE + 0x9000)  /*!< LORAC base address */

#define LORAC ((lorac_t*)LORAC_BASE)       /*!< LORAC peripheral */


/**
 * @brief AFEC
 */
typedef struct {
    __IO uint32_t CR;     /*!< control register, offset 0x0 */
    __IO uint32_t INT_SR; /*!< interrupt status register, offset 0x4 */
    __I uint32_t RAW_SR;  /*!< raw status register, offset 0x8 */
} afec_t;

#define AFEC ((afec_t*)(AFEC_BASE + 0x200)) /*!< AFEC peripheral */

/****************************AFEC RAW_SR bit definition*************************/

#define AFEC_RAW_SR_RCO4M_READY_MASK ((uint32_t)0x80000000)

#define AFEC_RAW_SR_PLL_UNLOCK_MASK ((uint32_t)0x40000000)

#define AFEC_RAW_SR_RCO24M_READY_MASK ((uint32_t)0x00000004)


/******************************************************************************/
/**
 * @brief IWDG
 */
typedef struct {
    __IO uint32_t CR;  /*!< control register, offset 0x0 */
    __IO uint32_t MAX; /*!< max value register, offset 0x4 */
    __IO uint32_t WIN; /*!< window value register, offset 0x8 */
    __I uint32_t SR;   /*!< status register, offset 0xC */
    __I uint32_t SR1;  /*!< status register 1, offset 0x10 */
    __IO uint32_t CR1; /*!< control register 1, offset 0x14 */
    __IO uint32_t SR2; /*!< status register 2, offset 0x18 */
} iwdg_t;

#define IWDG_BASE (PERIPH_BASE + 0x1D000) /*!< IWDG base address */
#define IWDG      ((iwdg_t*)IWDG_BASE)    /*!< IWDG peripheral */

/****************************IWDG CR bit definition*************************/
#define IWDG_CR_RSTEN_MASK ((uint32_t)0x00000020)
#define IWDG_CR_WKEN_MASK  ((uint32_t)0x00000010)

#define IWDG_CR_PREDIV_MASK ((uint32_t)0x0000000E)
#define IWDG_CR_PREDIV_4    ((uint32_t)0x00000000)
#define IWDG_CR_PREDIV_8    ((uint32_t)0x00000002)
#define IWDG_CR_PREDIV_16   ((uint32_t)0x00000004)
#define IWDG_CR_PREDIV_32   ((uint32_t)0x00000006)
#define IWDG_CR_PREDIV_64   ((uint32_t)0x00000008)
#define IWDG_CR_PREDIV_128  ((uint32_t)0x0000000A)
#define IWDG_CR_PREDIV_256  ((uint32_t)0x0000000C)

#define IWDG_CR_START_MASK ((uint32_t)0x00000001)

/****************************IWDG SR bit definition*************************/
#define IWDG_SR_WRITE_SR2_DONE ((uint32_t)0x00000008)
#define IWDG_SR_WIN_SET_DONE   ((uint32_t)0x00000004)
#define IWDG_SR_MAX_SET_DONE   ((uint32_t)0x00000002)
#define IWDG_SR_WRITE_CR_DONE  ((uint32_t)0x00000001)

/****************************IWDG SR1 bit definition************************/
#define IWDG_SR1_RESET_REQ_SYNC ((uint32_t)0x00001000)

/****************************IWDG CR1 bit definition************************/
#define IWDG_CR1_RESET_REQ_RST_EN_MASK ((uint32_t)0x00000002)
#define IWDG_CR1_RESET_REQ_INT_EN_MASK ((uint32_t)0x00000001)

/****************************IWDG SR2 bit definition************************/
#define IWDG_SR2_RESET_REQ_SR_MASK ((uint32_t)0x00000001)


/******************************************************************************/
/**
 * @brief WDG
 */
typedef struct {
    __IO uint32_t LOAD;         /*!< load register, offset 0x0 */
    __I uint32_t VALUE;         /*!< value register, offset 0x4 */
    __IO uint32_t CONTROL;      /*!< control register, offset 0x8 */
    __O uint32_t INTCLR;        /*!< clear interrupt register, offset 0xC */
    __I uint32_t RIS;           /*!< raw interrupt status register, offset 0x10 */
    __I uint32_t MIS;           /*!< interrupt status register, offset 0x14 */
    __I uint32_t DUMMY0[0x2FA]; /*!< dummy register, offset 0x18 */
    __IO uint32_t LOCK;         /*!< lock register, offset 0xC00 */
    __I uint32_t DUMMY1[0xBF];  /*!< dummy register, offset 0xC00 */
    __IO uint32_t ITCR;         /*!< integration test control register, offset 0xF00 */
    __O uint32_t ITOP;          /*!< integration test output set register, offset 0xF04 */
    __I uint32_t DUMMY2[0x32];  /*!< dummy register, offset 0xF08 */
    __I uint32_t PERIPHID4;     /*!< peripheral id register 4, offset 0xFD0 */
    __I uint32_t PERIPHID5;     /*!< peripheral id register 5, offset 0xFD4 */
    __I uint32_t PERIPHID6;     /*!< peripheral id register 6, offset 0xFD8 */
    __I uint32_t PERIPHID7;     /*!< peripheral id register 7, offset 0xFDC */
    __I uint32_t PERIPHID0;     /*!< peripheral id register 0, offset 0xFE0 */
    __I uint32_t PERIPHID1;     /*!< peripheral id register 1, offset 0xFE4 */
    __I uint32_t PERIPHID2;     /*!< peripheral id register 2, offset 0xFE8 */
    __I uint32_t PERIPHID3;     /*!< peripheral id register 3, offset 0xFEC */
    __I uint32_t PCELLID0;      /*!< component id register 0, offset 0xFF0 */
    __I uint32_t PCELLID1;      /*!< component id register 1, offset 0xFF4 */
    __I uint32_t PCELLID2;      /*!< component id register 2, offset 0xFF8 */
    __I uint32_t PCELLID3;      /*!< component id register 3, offset 0xFFC */
} wdg_t;

#define WDG_BASE (PERIPH_BASE + 0x1E000) /*!< WDG base address */
#define WDG      ((wdg_t*)WDG_BASE)      /*!< WDG peripheral */


/******************************************************************************/
/**
 * @brief CRC
 */
typedef struct {
    __IO uint32_t CR;    /*!< control register, offset 0x0 */
    __IO uint32_t DR;    /*!< data register, offset 0x4 */
    __IO uint32_t INIT;  /*!< init value register, offset 0x8 */
    __IO uint32_t POLY;  /*!< polynomial register, offset 0xC */
} crc_t;

#define CRC_BASE (PERIPH_BASE + 0x22000) /*!< CRC base address */
#define CRC      ((crc_t*)CRC_BASE)      /*!< CRC peripheral */

/****************************CRC CR bit definition*****************************/
#define CRC_CR_CALC_FLAG ((uint32_t)0x00000040)

#define CRC_CR_CALC_INIT ((uint32_t)0x00000020)

#define CRC_CR_POLY_SIZE_MASK ((uint32_t)0x00000018)
#define CRC_CR_POLY_SIZE_7    ((uint32_t)0x00000018)
#define CRC_CR_POLY_SIZE_8    ((uint32_t)0x00000010)
#define CRC_CR_POLY_SIZE_16   ((uint32_t)0x00000008)
#define CRC_CR_POLY_SIZE_32   ((uint32_t)0x00000000)

#define CRC_CR_REVERSE_IN_MASK  ((uint32_t)0x00000006)
#define CRC_CR_REVERSE_IN_NONE  ((uint32_t)0x00000000)
#define CRC_CR_REVERSE_IN_BYTE  ((uint32_t)0x00000002)
#define CRC_CR_REVERSE_IN_HWORD ((uint32_t)0x00000004)
#define CRC_CR_REVERSE_IN_WORD  ((uint32_t)0x00000006)

#define CRC_CR_REVERSE_OUT_EN ((uint32_t)0x00000001)


/******************************************************************************/
/**
 * @brief I2C
 */
typedef struct {
    __IO uint32_t CR;          /*!< control register, offset 0x00 */
    __IO uint32_t SR;          /*!< status register, offset 0x04 */
    __IO uint32_t SAR;         /*!< slave address register, offset 0x08 */
    __IO uint32_t DBR;         /*!< data buffer register, offset 0x0C */
    __IO uint32_t LCR;         /*!< load count register, offset 0x10 */
    __IO uint32_t WCR;         /*!< wait count register, offset 0x14 */
    __IO uint32_t RST_CYCL;    /*!< reset cycle register, offset 0x18 */
    __I uint32_t BMR;          /*!< bus monitor register, offset 0x1C */
    __IO uint32_t WFIFO;       /*!< write fifo register, offset 0x20 */
    __IO uint32_t WFIFO_WPTR;  /*!< write fifo write pointer register, offset 0x24 */
    __IO uint32_t WFIFO_RPTR;  /*!< write fifo read pointer register, offset 0x28 */
    __IO uint32_t RFIFO;       /*!< read fifo register, offset 0x2C */
    __IO uint32_t RFIFO_WPTR;  /*!< read fifo write pointer register, offset 0x30 */
    __IO uint32_t RFIFO_RPTR;  /*!< read fifo read pointer register, offset 0x34 */
    __IO uint32_t RESV[2];     /*!< reserved, offset 0x28-0x3C */
    __I uint32_t WFIFO_STATUS; /*!< write fifo status register, offset 0x40 */
    __I uint32_t RFIFO_STATUS; /*!< read fifo status register, offset 0x44 */
} i2c_t;

#define I2C0_BASE (PERIPH_BASE + 0x7000)   /*!< I2C0 base address */
#define I2C1_BASE (PERIPH_BASE + 0x14000)  /*!< I2C2 base address */
#define I2C2_BASE (PERIPH_BASE + 0x15000)  /*!< I2C2 base address */
#define I2C0      ((i2c_t*)I2C0_BASE)      /*!< I2C0 peripheral */
#define I2C1      ((i2c_t*)I2C1_BASE)      /*!< I2C1 peripheral */
#define I2C2      ((i2c_t*)I2C2_BASE)      /*!< I2C2 peripheral */

/****************************I2C CR bit definition*****************************/
#define I2C_CR_RFIFO_OVERRUN_INTR_EN_MASK   ((uint32_t)0x80000000)
#define I2C_CR_RFIFO_FULL_INTR_EN_MASK      ((uint32_t)0x40000000)
#define I2C_CR_RFIFO_HALFFULL_INTR_EN_MASK  ((uint32_t)0x20000000)
#define I2C_CR_TFIFO_EMPTY_INTR_EN_MASK     ((uint32_t)0x10000000)
#define I2C_CR_TRANS_DONE_INTR_EN_MASK      ((uint32_t)0x08000000)
#define I2C_CR_MASTER_STOP_DET_EN_MASK      ((uint32_t)0x04000000)
#define I2C_CR_MASTER_STOP_DET_INTR_EN_MASK ((uint32_t)0x02000000)
#define I2C_CR_SLAVE_STOP_DET_INTR_EN_MASK  ((uint32_t)0x01000000)
#define I2C_CR_SLAVE_ADDR_DET_INTR_EN_MASK  ((uint32_t)0x00800000)
#define I2C_CR_BUS_ERROR_INTR_EN_MASK       ((uint32_t)0x00400000)
#define I2C_CR_GENERAL_CALL_DIS_MASK        ((uint32_t)0x00200000)
#define I2C_CR_DBR_FULL_INTR_EN_MASK        ((uint32_t)0x00100000)
#define I2C_CR_IDBR_EMPTY_INTR_EN_MASK      ((uint32_t)0x00080000)
#define I2C_CR_ARB_LOSS_DET_INTR_EN_MASK    ((uint32_t)0x00040000)

#define I2C_CR_TWSI_UNIT_EN_MASK      ((uint32_t)0x00004000)
#define I2C_CR_SCL_EN_MASK            ((uint32_t)0x00002000)
#define I2C_CR_MASTER_ABORT_MASK      ((uint32_t)0x00001000)
#define I2C_CR_BUS_RESET_REQUEST_MASK ((uint32_t)0x00000800)
#define I2C_CR_UNIT_RESET_MASK        ((uint32_t)0x00000400)

#define I2C_CR_BUS_MODE_MASK     ((uint32_t)0x00000300)
#define I2C_CR_BUS_MODE_STANDARD ((uint32_t)0x00000000)
#define I2C_CR_BUS_MODE_FAST     ((uint32_t)0x00000100)
#define I2C_CR_BUS_MODE_HIGH     ((uint32_t)0x00000200)

#define I2C_CR_DMA_EN_MASK ((uint32_t)0x00000080)

#define I2C_CR_FIFO_EN_MASK ((uint32_t)0x00000020)

#define I2C_CR_TRANS_BEGIN_MASK ((uint32_t)0x00000010)
#define I2C_CR_TRANS_BYTE_MASK  ((uint32_t)0x00000008)
#define I2C_CR_ACKNAK_MASK      ((uint32_t)0x00000004)
#define I2C_CR_STOP_MASK        ((uint32_t)0x00000002)
#define I2C_CR_START_MASK       ((uint32_t)0x00000001)

/****************************I2C SR bit definition*****************************/
#define I2C_SR_RFIFO_OVERRUN_MASK   ((uint32_t)0x80000000)
#define I2C_SR_RFIFO_FULL_MASK      ((uint32_t)0x40000000)
#define I2C_SR_RFIFO_HALFFULL_MASK  ((uint32_t)0x20000000)
#define I2C_SR_TFIFO_EMPTY_MASK     ((uint32_t)0x10000000)
#define I2C_SR_TRANS_DONE_MASK      ((uint32_t)0x08000000)
#define I2C_SR_MASTER_STOP_DET_MASK ((uint32_t)0x02000000)
#define I2C_SR_SLAVE_STOP_DET_MASK  ((uint32_t)0x01000000)
#define I2C_SR_SLAVE_ADDR_DET_MASK  ((uint32_t)0x00800000)
#define I2C_SR_BUS_ERROR_MASK       ((uint32_t)0x00400000)
#define I2C_SR_GENERAL_CALL_MASK    ((uint32_t)0x00200000)
#define I2C_SR_DBR_FULL_MASK        ((uint32_t)0x00100000)
#define I2C_SR_IDBR_EMPTY_MASK      ((uint32_t)0x00080000)
#define I2C_SR_ARB_LOSS_DET_MASK    ((uint32_t)0x00040000)

#define I2C_SR_BUS_BUSY_MASK   ((uint32_t)0x00010000)
#define I2C_SR_UNIT_BUSY_MASK  ((uint32_t)0x00008000)
#define I2C_SR_ACK_STATUS_MASK ((uint32_t)0x00004000)
#define I2C_SR_RW_MODE_MASK    ((uint32_t)0x00002000)

/****************************I2C WFIFO bit definition************************/
#define I2C_WFIFO_CONTROL_TB_MASK     ((uint32_t)0x00000800)
#define I2C_WFIFO_CONTROL_ACKNAK_MASK ((uint32_t)0x00000400)
#define I2C_WFIFO_CONTROL_STOP_MASK   ((uint32_t)0x00000200)
#define I2C_WFIFO_CONTROL_START_MASK  ((uint32_t)0x00000100)

/****************************I2C WFIFO_STATUS bit definition*****************/
#define I2C_WFIFO_STATUS_SIZE_MASK  ((uint32_t)0x0000003C)
#define I2C_WFIFO_STATUS_EMPTY_MASK ((uint32_t)0x00000002)
#define I2C_WFIFO_STATUS_FULL_MASK  ((uint32_t)0x00000001)

/****************************I2C RFIFO_STATUS bit definition*****************/
#define I2C_RFIFO_STATUS_SIZE_MASK   ((uint32_t)0x000000F0)
#define I2C_RFIFO_STATUS_EMPTY_MASK  ((uint32_t)0x00000004)
#define I2C_RFIFO_STATUS_FULL_MASK   ((uint32_t)0x00000003)
#define I2C_RFIFO_STATUS_HFULL_MASK  ((uint32_t)0x00000002)
#define I2C_RFIFO_STATUS_OVERUN_MASK ((uint32_t)0x00000001)


/******************************************************************************/
/**
 * @brief SYSCFG
 */
typedef struct {
    __IO uint32_t CR0;  /*!< control register 0, offset 0x00 */
    __IO uint32_t CR1;  /*!< control register 1, offset 0x04 */
    __IO uint32_t CR2;  /*!< control register 2, offset 0x08 */
    __IO uint32_t CR3;  /*!< control register 3, offset 0x0C */
    __IO uint32_t CR4;  /*!< control register 4, offset 0x10 */
    __IO uint32_t CR5;  /*!< control register 5, offset 0x14 */
    __IO uint32_t CR6;  /*!< control register 6, offset 0x18 */
    __IO uint32_t CR7;  /*!< control register 7, offset 0x1C */
    __IO uint32_t CR8;  /*!< control register 8, offset 0x20 */
    __IO uint32_t CR9;  /*!< control register 9, offset 0x24 */
    __IO uint32_t CR10; /*!< control register 10, offset 0x28 */
} syscfg_t;

#define SYSCFG_BASE (PERIPH_BASE + 0x1000)   /*!< SYSCFG base address */
#define SYSCFG      ((syscfg_t*)SYSCFG_BASE) /*!< SYSCFG peripheral */


/******************************************************************************/
/**
 * @brief PWR
 */
typedef struct {
    __IO uint32_t CR0;  /*!< control register 0, offset 0x00 */
    __IO uint32_t CR1;  /*!< control register 1, offset 0x04 */
    __IO uint32_t SR0;  /*!< status register 0, offset 0x08 */
    __IO uint32_t SR1;  /*!< status register 2, offset 0x0C */
    __IO uint32_t CR2;  /*!< control register 3, offset 0x10 */
    __IO uint32_t CR3;  /*!< control register 4, offset 0x14 */
    __IO uint32_t CR4;  /*!< control register 5, offset 0x18 */
    __IO uint32_t CR5;  /*!< control register 6, offset 0x1C */
} pwr_t;

#define PWR_BASE (PERIPH_BASE + 0x1800) /*!< PWR base address */
#define PWR      ((pwr_t*)PWR_BASE)     /*!< PWR peripheral */


/******************************************************************************/
/**
 * @brief TIMER
 */
typedef struct {
    __IO uint32_t CR1;   /*!< TIMER control register 1, Address offset: 0x00 */
    __IO uint32_t CR2;   /*!< TIMER control register 2, Address offset: 0x04 */
    __IO uint32_t SMCR;  /*!< TIMER slave Mode Control register, Address offset: 0x08 */
    __IO uint32_t DIER;  /*!< TIMER DMA/interrupt enable register, Address offset: 0x0C */
    __IO uint32_t SR;    /*!< TIMER status register, Address offset: 0x10 */
    __O uint32_t EGR;    /*!< TIMER event generation register, Address offset: 0x14 */
    __IO uint32_t CCMR1; /*!< TIMER  capture/compare mode register 1, Address offset: 0x18 */
    __IO uint32_t CCMR2; /*!< TIMER  capture/compare mode register 2, Address offset: 0x1C */
    __IO uint32_t CCER;  /*!< TIMER capture/compare enable register, Address offset: 0x20 */
    __IO uint32_t CNT;   /*!< TIMER counter register, Address offset: 0x24 */
    __IO uint32_t PSC;   /*!< TIMER prescaler register, Address offset: 0x28 */
    __IO uint32_t ARR;   /*!< TIMER auto-reload register, Address offset: 0x2C */
    __I uint32_t RESV1;  /*!< Reserved Address offset: 0x30 */
    __IO uint32_t CCR0;  /*!< TIMER capture/compare register 0, Address offset: 0x34 */
    __IO uint32_t CCR1;  /*!< TIMER capture/compare register 1, Address offset: 0x38 */
    __IO uint32_t CCR2;  /*!< TIMER capture/compare register 2, Address offset: 0x3C */
    __IO uint32_t CCR3;  /*!< TIMER capture/compare register 3, Address offset: 0x40 */
    __I uint32_t RESV2;  /*!< Reserved, Address offset: 0x44 */
    __IO uint32_t DCR;   /*!< TIMER DMA control register, Address offset: 0x48 */
    __IO uint32_t DMAR;  /*!< TIMER DMA address for full transfer register, Address offset: 0x4C */
    __IO uint32_t OR;    /*!< TIMER option register, Address offset: 0x50 */
} timer_gp_t;

#define TIMER0_SFR_BASE 0x4000A000            /*!< TIMER0 base address*/
#define TIMER1_SFR_BASE 0x4001A000            /*!< TIMER1 base address*/
#define TIMER2_SFR_BASE 0x4000B000            /*!< TIMER2 base address*/
#define TIMER3_SFR_BASE 0x4001B000            /*!< TIMER3 base address*/
#define TIMER0 ((timer_gp_t*)TIMER0_SFR_BASE) /*!< TIMER0 peripheral*/
#define TIMER1 ((timer_gp_t*)TIMER1_SFR_BASE) /*!< TIMER1 peripheral*/
#define TIMER2 ((timer_gp_t*)TIMER2_SFR_BASE) /*!< TIMER2 peripheral*/
#define TIMER3 ((timer_gp_t*)TIMER3_SFR_BASE) /*!< TIMER3 peripheral*/


/******************************************************************************/
/**
 * @brief LPTIMER
 */
typedef struct {
    __I uint32_t ISR;   /*!< LPTIMER flag and status register*/
    __O uint32_t ICR;   /*!< LPTIMER flag clear register*/
    __IO uint32_t IER;  /*!< LPTIMER interrupt enable register*/
    __IO uint32_t CFGR; /*!< LPTIMER configuration register*/
    __IO uint32_t CR;   /*!< LPTIMER control register*/
    __IO uint32_t CMP;  /*!< LPTIMER compare register*/
    __IO uint32_t ARR;  /*!< LPTIMER autoreload register*/
    __I uint32_t CNT;   /*!< LPTIMER counter register*/
    __I uint32_t CSR;   /*!< LPTIMER CSR register*/
    __I uint32_t SR1;   /*!< LPTIMER SR1 register*/
} lptimer_t;

#define LPTIMER0_SFR_BASE  0x4000D000           /*!< LPTIMER0 base address*/
#define LPTIMER1_SFR_BASE 0x4000D800           /*!< LPTIMER1 base address*/
#define LPTIMER0  ((lptimer_t*)LPTIMER0_SFR_BASE)  /*!< LPTIMER0 peripheral*/
#define LPTIMER1 ((lptimer_t*)LPTIMER1_SFR_BASE) /*!< LPTIMER1 peripheral*/


/******************************************************************************/
/**
 * @brief I2S
 */
typedef struct {
    /* channel-common registers */
    __IO uint32_t IER;             /*!< enable register, offset 0x00*/
    __IO uint32_t IRER;            /*!< receiver block enable register, offset 0x04*/
    __IO uint32_t ITER;            /*!< transmitter block enable register, offset 0x08*/
    __IO uint32_t CER;             /*!< clock enable register, offset 0x0c*/
    __IO uint32_t CCR;             /*!< clock configuration register, offset 0x10*/
    __O uint32_t RXFFR;            /*!< receiver block FIFO reset register, offset 0x14*/
    __O uint32_t TXFFR;            /*!< transmitter block FIFO reset register, offset 0x18*/
    __I uint32_t RESV0;            /*!< reserved*/

    /* channel-specific registers */
    __IO uint32_t LRBR_LTHR;       /*!< right receive buffer register, offset 0x20*/
    __IO uint32_t RRBR_RTHR;       /*!< right transmit holding register, offset 0x24*/
    __IO uint32_t RER;             /*!< receiver enable register, offset 0x28*/
    __IO uint32_t TER;             /*!< transmitter enable register, offset 0x2c*/
    __IO uint32_t RCR;             /*!< receiver configuration register, offset 0x30*/
    __IO uint32_t TCR;             /*!< transmitter configuration register, offset 0x34*/
    __I uint32_t ISR;              /*!< interrupt status register, offset 0x38*/
    __IO uint32_t IMR;             /*!< interrupt mask register, offset 0x3c*/
    __I uint32_t ROR;              /*!< receiver overrun register, offset 0x40*/
    __I uint32_t TOR;              /*!< transmitter overrun register, offset 0x44*/
    __IO uint32_t RFCR;            /*!< receiver FIFO configuration register, offset 0x48*/
    __IO uint32_t TFCR;            /*!< transmitter FIFO configuration register, offset 0x4c*/
    __O uint32_t RFF;              /*!< receiver FIFO flush register, offset 0x50*/
    __O uint32_t TFF;              /*!< transmitter FIFO flush register, offset 0x54*/
    __I uint32_t RESV1[0x5a];      /*!< reserved*/
    __IO uint32_t RXDMA;           /*!< receiver block dma register, offset 0x1c0*/
    __O uint32_t RRXDMA;           /*!< reset receiver block dma register, offset 0x1c4*/
    __IO uint32_t TXDMA;           /*!< transmitter block dma register, offset 0x1c8*/
    __O uint32_t RTXDMA;           /*!< reset transmitter block dma register, offset 0x1cc*/
    __I uint32_t RESV2[8];         /*!< reserved*/
    __I uint32_t I2S_COMP_PARAM_2; /*!< component parameter register 2, offset 0x1f0*/
    __I uint32_t I2S_COMP_PARAM_1; /*!< component parameter register 1, offset 0x1f4*/
    __I uint32_t I2S_COMP_VERSION; /*!< component version register, offset 0x1f8*/
    __I uint32_t I2S_COMP_TYPE;    /*!< component type register, offset 0x1fc*/
} i2s_t;

#define I2S_BASE (0x40002000)       /*!< I2S base address*/
#define I2S      ((i2s_t*)I2S_BASE) /*!< I2S peripheral*/


/******************************************************************************/
/**
 * @brief BSTIMER
 */
typedef struct {
    __IO uint32_t CR1;     /*!< control register 1*/
    __IO uint32_t CR2;     /*!< control register 2*/
    __I uint32_t RESV1;    /*!< reserved*/
    __IO uint32_t DIER;    /*!< DMA/interrupt enable register*/
    __I uint32_t SR;       /*!< status register*/
    __O uint32_t EGR;      /*!< event generation register*/
    __I uint32_t RESV2[3]; /*!< reserved*/
    __IO uint32_t CNT;     /*!< counter*/
    __IO uint32_t PSC;     /*!< prescaler*/
    __IO uint32_t ARR;     /*!< auto-reload register*/
} bstimer_t;

#define BSTIMER0_SFR_BASE 0x4000C000                    /*!< BSTIMER0 base address*/
#define BSTIMER1_SFR_BASE 0x4001C000                    /*!< BSTIMER1 base address*/
#define BSTIMER0          ((bstimer_t*)BSTIMER0_SFR_BASE) /*!< BSTIMER0 peripheral*/
#define BSTIMER1          ((bstimer_t*)BSTIMER1_SFR_BASE) /*!< BSTIMER1 peripheral*/


/******************************************************************************/
/**
 * @brief SEC
 */
typedef struct {
    __IO uint32_t INT;     /*!< interrupt enable register*/
    __IO uint32_t RST;     /*!< reset enable register*/
    __IO uint32_t SR;      /*!< status*/
    __IO uint32_t FILTER0; /*!< filter0*/
    __IO uint32_t FILTER1; /*!< filter1*/
    __IO uint32_t FILTER2; /*!< filter2*/
    __IO uint32_t FILTER3; /*!< filter3*/
} sec_t;

#define SEC_BASE (0x4000F000)       /*!< SEC base address*/
#define SEC      ((sec_t*)SEC_BASE) /*!< SEC peripheral*/

#define SEC_SR_FLASH_ACCESS_ERROR_MASK ((uint32_t)0x00001000) /*!< SEC flash access error status*/


/******************************************************************************/
/**
 * @brief QSPI
 */
typedef struct {
    uint32_t QSPI_CR;       /*!< control register,offset 0x00*/
    uint32_t QSPI_DCR;      /*!< device configuration register,offset 0x04*/
    uint32_t QSPI_SR;       /*!< status register,offset 0x08*/
    uint32_t QSPI_FCR;      /*!< flag clear register,offset 0x0C*/
    uint32_t QSPI_DLR;      /*!< data length register,offset 0x10*/
    uint32_t QSPI_CCR;      /*!< communication configuration register,offset 0x14*/
    uint32_t QSPI_AR;       /*!< address register,offset 0x18*/
    uint32_t QSPI_ABR;      /*!< alternate byte register,offset 0x1C*/
    uint32_t QSPI_DR;       /*!< data register,offset 0x20*/
    uint32_t QSPI_PSMKR;    /*!< polling status mask register,offset 0x24*/
    uint32_t QSPI_PSMAR;    /*!< polling status match register,offset 0x28*/
    uint32_t QSPI_PIR;      /*!< polling interval register,offset 0x2C*/
    uint32_t QSPI_TOR;      /*!< timeout register,offset 0x30*/
    uint32_t RESERVERD[19]; /*!< reserved*/
    uint32_t QSPI_HIT0R;    /*!< hit0 times accumulator register,offset 0x80*/
    uint32_t QSPI_HIT1R;    /*!< hit1 times accumulator register,offset 0x84*/
    uint32_t QSPI_MIR;      /*!< miss times accumulator register,offset 0x88*/
    uint32_t QSPI_CFGR;     /*!< configuration register,offset 0x8C*/
    uint32_t SBUS_START;    /*!< start register,offset 0x90*/
} qspi_t;

#define QSPI_BASE 0x40021000      /*!< QSPI base address*/
#define QSPI ((qspi_t*)QSPI_BASE) /*!< QSPI peripheral*/


/******************************************************************************/
/**
 * @brief DAC
 */
typedef struct {
    __IO uint32_t CR;      /*!< control register,offset 0x00*/
    __O  uint32_t SWTRIGR; /*!< software trigger register,offset 0x04*/
    __IO uint32_t DHR;     /*!< data holding register,offset 0x08*/
    __I  uint32_t DOR;     /*!< data output register,offset 0x0C*/
    __IO uint32_t SR;      /*!< status register,offset 0x10*/
} dac_t;

#define DAC_BASE 0x40019000          /*!< DAC base address*/
#define DAC      ((dac_t*) DAC_BASE) /*!< DAC peripheral*/


#define DAC_CR_INTR_EMPTY_EN_MASK                ((uint32_t)0x00010000)
#define DAC_CR_INTR_UNDERFLOW_EN_MASK            ((uint32_t)0x00008000)

#define DAC_CR_DMA_EN_MASK                       ((uint32_t)0x00004000)

#define DAC_CR_MASK_AMP_SEL_MASK                 ((uint32_t)0x00003C00)
#define DAC_CR_MASK_AMP_SEL_1                    ((uint32_t)0x00000000)
#define DAC_CR_MASK_AMP_SEL_3                    ((uint32_t)0x00000400)
#define DAC_CR_MASK_AMP_SEL_7                    ((uint32_t)0x00000800)
#define DAC_CR_MASK_AMP_SEL_15                   ((uint32_t)0x00000C00)
#define DAC_CR_MASK_AMP_SEL_31                   ((uint32_t)0x00001000)
#define DAC_CR_MASK_AMP_SEL_63                   ((uint32_t)0x00001400)
#define DAC_CR_MASK_AMP_SEL_127                  ((uint32_t)0x00001800)
#define DAC_CR_MASK_AMP_SEL_255                  ((uint32_t)0x00001C00)
#define DAC_CR_MASK_AMP_SEL_511                  ((uint32_t)0x00002000)
#define DAC_CR_MASK_AMP_SEL_1023                 ((uint32_t)0x00002400)

#define DAC_CR_WAVE_SEL_MASK                     ((uint32_t)0x00000300)
#define DAC_CR_WAVE_SEL_NONE                     ((uint32_t)0x00000000)
#define DAC_CR_WAVE_SEL_NOISE                    ((uint32_t)0x00000100)
#define DAC_CR_WAVE_SEL_TRIANGLE                 ((uint32_t)0x00000200)

#define DAC_CR_TRIG_TYPE_SEL_MASK                ((uint32_t)0x000000C0)
#define DAC_CR_TRIG_TYPE_SEL_RISING_EDGE         ((uint32_t)0x00000000)
#define DAC_CR_TRIG_TYPE_SEL_FALLING_EDGE        ((uint32_t)0x00000040)
#define DAC_CR_TRIG_TYPE_SEL_RISING_FALLING_EDGE ((uint32_t)0x00000080)

#define DAC_CR_TRIG_SRC_SEL_MASK                 ((uint32_t)0x00000038)
#define DAC_CR_TRIG_SRC_SEL_GPTIMER1_TRGO        ((uint32_t)0x00000000)
#define DAC_CR_TRIG_SRC_SEL_GPTIMER0_TRGO        ((uint32_t)0x00000008)
#define DAC_CR_TRIG_SRC_SEL_BSTIMER1_TRGO        ((uint32_t)0x00000010)
#define DAC_CR_TRIG_SRC_SEL_BSTIMER0_TRGO        ((uint32_t)0x00000018)
#define DAC_CR_TRIG_SRC_SEL_GPIO6                ((uint32_t)0x00000020)
#define DAC_CR_TRIG_SRC_SEL_GPIO24               ((uint32_t)0x00000028)
#define DAC_CR_TRIG_SRC_SEL_GPIO43               ((uint32_t)0x00000030)
#define DAC_CR_TRIG_SRC_SEL_SOFTWARE             ((uint32_t)0x00000038)

#define DAC_CR_TRIG_EN_MASK                      ((uint32_t)0x00000004)

#define DAC_CR_DAC_EN_MASK                       ((uint32_t)0x00000001)


/******************************************************************************/
/**
 * @brief ADC
 */
typedef struct {
    __IO uint32_t CR;        /*!< control register,offset 0x00*/
    __IO uint32_t CFGR;      /*!< configuration register,offset 0x04*/
    __IO uint32_t SEQR0;     /*!< sequence0 register,offset 0x08*/
    __IO uint32_t SEQR1;     /*!< sequence1 register,offset 0x0c*/
    __IO uint32_t DIFFSEL;   /*!< difference register,offset 0x10*/
    __IO uint32_t ISR;       /*!< interrupt status register,offset 0x14*/
    __IO uint32_t IER;       /*!< interrupt enable register,offset 0x18*/
    __I uint32_t DR;         /*!< data register,offset 0x1c*/
    __IO uint32_t AWD0_CFGR; /*!< AWD0 register,offset 0x20*/
    __IO uint32_t AWD1_CFGR; /*!< AWD1 register,offset 0x24*/
    __IO uint32_t AWD2_CFGR; /*!< AWD2 register,offset 0x28*/
} adc_t;

#define ADC_BASE 0x40017000       /*!< ADC base address*/
#define ADC ((adc_t *)(ADC_BASE)) /*!< ADC peripheral*/


/******************************************************************************/
/**
 * @brief LCD
 */

typedef struct {
    __IO uint32_t CR0; /*!< control register 0,offset 0x00*/
    __IO uint32_t CR1; /*!< control register 1,offset 0x04*/
    __IO uint32_t DR0; /*!< data register 0,offset 0x08*/
    __IO uint32_t DR1; /*!< data register 1,offset 0x0c*/
    __IO uint32_t DR2; /*!< data register 2,offset 0x10*/
    __IO uint32_t DR3; /*!< data register 3,offset 0x14*/
    __IO uint32_t DR4; /*!< data register 4,offset 0x18*/
    __IO uint32_t DR5; /*!< data register 5,offset 0x1c*/
    __IO uint32_t DR6; /*!< data register 6,offset 0x20*/
    __IO uint32_t DR7; /*!< data register 7,offset 0x24*/
    __I uint32_t SR;   /*!< status register,offset 0x28*/
    __IO uint32_t CR2; /*!< control register 2,offset 0x2c*/
} lcd_t;

#define LCD_BASE 0x40018000       /*!< LCD base address*/
#define LCD ((lcd_t *)(LCD_BASE)) /*!< LCD peripheral*/



/******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif //_TREMO_REGS_H_

/**
 * @} 
 * @}
 */
