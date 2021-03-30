#include "tremo_regs.h"
#include "tremo_rcc.h"
#include "tremo_delay.h"


//static uint8_t fac_us  = 0;
static uint16_t fac_ms = 0;
static float fac_us  = 0;

/**
 * @brief Init the registers used in delay function
 * @param None
 * @retval None
 */
void delay_init(void)
{
    uint32_t reload;
    uint32_t tick_rate = 1000;

    uint32_t clk_freq = rcc_get_clk_freq(RCC_HCLK);
    if (clk_freq < 1000000)
        return;

    fac_us = 1.0011*clk_freq / 1000000 ;
    reload = 1.0011*clk_freq / tick_rate;

    fac_ms = 1000 / tick_rate;

    SysTick_Config(reload);
}

/**
 * @brief Delay some microseconds
 * @param nus The delay in microsecond 
 * @retval None
 */
void delayMicroseconds(uint32_t nus)
{
    uint32_t ticks;
    uint32_t tpre, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;

    if (!fac_us)
        return;

    ticks = (uint32_t)((float)nus * fac_us);
    tpre  = SysTick->VAL;
    while (1) {
        tnow = SysTick->VAL;
        if (tnow != tpre) {
            if (tnow < tpre)
                tcnt += tpre - tnow;
            else
                tcnt += reload - tnow + tpre;

            tpre = tnow;

            if (tcnt >= ticks)
                break;
        }
    };
}

/**
 * @brief Delay some milliseconds
 * @param nms The delay in millisecond 
 * @retval None
 */
void delay(uint32_t nms)
{
    delayMicroseconds((uint32_t)(nms * 1000)-1);//sys irq’º”√1
}
