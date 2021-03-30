#include "tremo_rcc.h"
#include "tremo_flash.h"
#include "tremo_pwr.h"

#define PWR_LP_MODE_MASK    ((uint32_t)0x00000003)  /*!< The bits mask of the low power mode register */
#define PWR_LP_MODE_EXT_MASK ((uint32_t)1 << 24)    /*!< The bit mask of the low power mode ext register */

#define REG_AFEC_RAW_SR          ((uint32_t)0x40008208)
#define AFEC_RAW_SR_RCO48M_READY ((uint32_t)0x00000004)

void deepsleep(uint32_t mode, uint32_t wfi)
{
    if((TREMO_REG_RD(0x10002010) & 0x3) == 0)
        TREMO_REG_SET(PWR->CR1, (0xF<<20), (1<<20));
	
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    if (wfi)
        PWR->CR0 &= ~(1 << 5);
    else
        PWR->CR0 |= 1 << 5;

    if (mode < PWR_LP_MODE_STOP3)
        TREMO_REG_SET(PWR->CR0, PWR_LP_MODE_MASK, mode);
    else {
        uint32_t value = 0;
        //disable prefetch
        if(EFC->CR & EFC_CR_PREFETCH_EN_MASK) {
            FLASH_CR_UNLOCK();
            EFC->CR &= ~(EFC_CR_PREFETCH_EN_MASK);
            FLASH_CR_LOCK();
        }
		
        value = TREMO_ANALOG_RD(0x0C);
        if(!(value & (1<<14)))
            TREMO_ANALOG_WR(0x0C, (value | 1<<14));

        TREMO_REG_SET(PWR->CR0, PWR_LP_MODE_MASK, PWR_LP_MODE_STOP3);
        TREMO_REG_EN(PWR->CR1, PWR_LP_MODE_EXT_MASK, mode == PWR_LP_MODE_STOP3 ? true : false);
    }

    if (wfi) {
        NVIC_EnableIRQ(PWR_IRQn);
        __WFI();
    } else {
        __SEV();
        __WFE();
        __WFE();
    }
}


/**
 * @brief The WFI function of the deepsleep modes
 * @param mode The low power mode. 
 *          This parameter can be one of the following values:
 *           @arg PWR_LP_MODE_STOP0:  STOP0
 *           @arg PWR_LP_MODE_STOP1:  STOP1
 *           @arg PWR_LP_MODE_STOP2:  STOP2
 *           @arg PWR_LP_MODE_STOP3:  STOP3
 *           @arg PWR_LP_MODE_STANDBY:  STANDBY
 * @retval None
 */
void pwr_deepsleep_wfi(uint32_t mode)
{
    deepsleep(mode, 1);
}

/**
 * @brief The WFE function of the deepsleep modes
 * @param mode The low power mode. 
 *          This parameter can be one of the following values:
 *           @arg PWR_LP_MODE_STOP0:  STOP0
 *           @arg PWR_LP_MODE_STOP1:  STOP1
 *           @arg PWR_LP_MODE_STOP2:  STOP2
 *           @arg PWR_LP_MODE_STOP3:  STOP3
 *           @arg PWR_LP_MODE_STANDBY:  STANDBY
 * @retval None
 */
void pwr_deepsleep_wfe(uint32_t mode)
{
    deepsleep(mode, 0);
}

/**
 * @brief The WFI function of the sleep modes
 * @param lowpower The low power flag. 
 *          This parameter can be one of the following values:
 *           @arg true:  Low power sleep
 *           @arg false: Sleep
 * @retval None
 */
void pwr_sleep_wfi(bool lowpower)
{
    if (lowpower)
        pwr_enter_lprun_mode();

    NVIC_EnableIRQ(PWR_IRQn);
    __WFI();
}

/**
 * @brief The WFE function of the sleep modes
 * @param lowpower The low power flag. 
 *          This parameter can be one of the following values:
 *           @arg true:  Low power sleep
 *           @arg false: Sleep
 * @retval None
 */
void pwr_sleep_wfe(bool lowpower)
{
    if (lowpower)
        pwr_enter_lprun_mode();

    __WFE();
}

/**
 * @brief Enter the low power run mode
 * @param None
 * @retval None
 */
void pwr_enter_lprun_mode(void)
{
    TREMO_ANALOG_WR(0x06, (TREMO_ANALOG_RD(0x06) & (~(1<<19))));
    TREMO_ANALOG_WR(0x06, (TREMO_ANALOG_RD(0x06) | ((1<<20))));
}

/**
 * @brief Exit the low power run mode
 * @param None
 * @retval None
 */
void pwr_exit_lprun_mode(void)
{
    TREMO_ANALOG_WR(0x06, (TREMO_ANALOG_RD(0x06) & (~(1<<20))));
    TREMO_ANALOG_WR(0x06, (TREMO_ANALOG_RD(0x06) | ((1<<19))));
}

/**
 * @brief Enable/Disable the low power mode of the xo32k
 * @note This function must be called after xo32k is working.
 * @param new_state true or false
 * @retval None
 */
void pwr_xo32k_lpm_cmd(bool new_state)
{
    uint32_t value = TREMO_ANALOG_RD(0x03);
    TREMO_ANALOG_WR(0x03, new_state ? (value | (1 << 7)): (value & (~(1 << 7))));
}
