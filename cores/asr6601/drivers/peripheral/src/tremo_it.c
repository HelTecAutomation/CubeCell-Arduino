#include "tremo_gpio.h"
#include "tremo_it.h"
#include "tremo_timer.h"
#include "ASR_Arduino.h"


extern void RadioOnDioIrq(void);

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{

    /* Go to infinite loop when Hard Fault exception occurs */
    while (1) { }
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1) { }
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1) { }
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1) { }
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
uint32_t systime = 0;
void SysTick_Handler(void)
{
	systime ++ ;
}

/**
 * @brief  This function handles PWR Handler.
 * @param  None
 * @retval None
 */
void PWR_IRQHandler()
{
}

void IWDG_IRQHandler()
{
	iwdg_clear_interrupt();
	idwgIrqCnt++;
	if(idwgIrqCnt >=3 )
		NVIC_SystemReset();

	if(autoFeedIdwg)
		iwdg_reload();
}
/******************************************************************************/ 
/*                 Tremo Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_cm4.S).                                               */
/******************************************************************************/

/*
void TIMER0_IRQHandler(void)
{
    bool state;
    timer_get_status(TIMER0, TIMER_SR_CC0IF, &state);
    if (state) {
        timer_clear_status(TIMER0, TIMER_SR_CC0IF);
    }

}

*/
void __attribute__((weak)) RadioOnDioIrq()
{
	printf("RadioOnDioIrq");
}
extern bool uart0started;

void DMA1_IRQHandler(void)
{
    dma1_IRQHandler();
}

void LORA_IRQHandler()
{
    if(uart0started)
        iomux(UART0_TX,1);
    RadioOnDioIrq();
}



void RTC_IRQHandler(void)
{
    if(uart0started)
        iomux(UART0_TX,1);
    RtcOnIrq();
}

