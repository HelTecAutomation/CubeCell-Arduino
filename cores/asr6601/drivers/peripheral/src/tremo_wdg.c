#include <stdio.h>
#include "tremo_rcc.h"
#include "tremo_wdg.h"

#define WDG_LOCK()                     \
    do {                               \
        WDG->LOCK = ~(WDG_LOCK_TOKEN); \
    } while (0)

#define WDG_UNLOCK()                \
    do {                            \
        WDG->LOCK = WDG_LOCK_TOKEN; \
    } while (0)

/**
 * @brief Deinitializes the WDG registers to the reset values
 * @param None
 * @retval None
 */
void wdg_deinit(void)
{
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_WDG, false);
    rcc_rst_peripheral(RCC_PERIPHERAL_WDG, true);
    rcc_rst_peripheral(RCC_PERIPHERAL_WDG, false);
}

/**
 * @brief Start the Watchdog
 * @note  If the WDG counter reaches 0, the WDG Irq will be asserted, and the counter reloaded. If the WDG counter reaches 0 again without feeding, the system will be reset
 * @param reload_value  The reload value of WDG (Max: 0xFFFFFFFF).
 * @retval None
 */
void wdg_start(uint32_t reload_value)
{
    WDG_UNLOCK();
    WDG->LOAD    = reload_value;
    WDG->CONTROL = WDG_RESEN | WDG_INTEN;
    WDG_LOCK();

    TREMO_REG_EN(RCC->RST_CR, RCC_RST_CR_WDG_RESET_REQ_EN_MASK, true);
}

/**
 * @brief Feed the Watchdog
 * @param None
 * @retval None
 */
void wdg_reload(void)
{
    WDG_UNLOCK();
    WDG->INTCLR = 0x1;
    WDG_LOCK();
}

/**
 * @brief Stop the Watchdog
 * @param None
 * @retval None
 */
void wdg_stop(void)
{
    TREMO_REG_EN(RCC->RST_CR, RCC_RST_CR_WDG_RESET_REQ_EN_MASK, false);

    WDG_UNLOCK();
    WDG->CONTROL = 0x0;
    WDG->LOAD    = 0xFFFFFFFF;
    WDG_LOCK();
}
