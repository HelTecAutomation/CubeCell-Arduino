#include "system_cm4.h"
#include "tremo_rcc.h"
#include "tremo_delay.h"
#include "tremo_regs.h"
#include "tremo_pwr.h"
#include "rtc-board.h"
#include "ASR_Arduino.h"

void boardInitMcu( void )
{
    pinMode(1,ANALOG);
    pinMode(6,ANALOG);
    pinMode(7,ANALOG);
    rcc_enable_oscillator(RCC_OSC_XO32K, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOA, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOB, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOC, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOD, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_PWR, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_SYSCFG, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_RTC, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_SAC, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_LORA, true);
    delay(100);
    pinMode(Vext,OUTPUT);
    digitalWrite(Vext,HIGH);
    pinMode(VBAT_ADC_CTL, OUTPUT);
    digitalWrite(VBAT_ADC_CTL,HIGH);
    pwr_xo32k_lpm_cmd(true);
    RtcInit();
    systime = (uint32_t)TimerGetCurrentTime();
    srand(analogRead(11)+analogRead(4)+analogRead(5)+analogRead(8)+analogRead(47));
    SX126xInit();
    sx126xSleep();
}


void nvic_init()
{
    NVIC_SetPriority(PendSV_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
    
    for(int i=0; i<=IWDG_IRQn; i++)
        NVIC_SetPriority(i, configLIBRARY_NORMAL_INTERRUPT_PRIORITY);
}

void system_init(void)
{
    rcc_set_sys_clk_source(RCC_SYS_CLK_SOURCE_RCO48M);
    // FPU enable
#if (__FPU_PRESENT == 1)
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));
#endif

    // enable afec clk
    TREMO_REG_EN(RCC->CGR0, RCC_CGR0_AFEC_CLK_EN_MASK, true);

    // set flash read number to 1
    EFC->TIMING_CFG  =  (EFC->TIMING_CFG & (~0xF0000)) | (1<<16);
    while(!(EFC->SR&0x2));

    nvic_init();
    delay_init();
}

/********END OF FILE ***********/
