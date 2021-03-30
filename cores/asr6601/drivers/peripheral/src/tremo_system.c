#include "tremo_regs.h"
#include "tremo_system.h"

/**
 * @brief Reset the chip
 * @param None
 * @retval None
 */
void system_reset(void)
{
    NVIC_SystemReset();
}

/**
 * @brief Read the chip id of the chip  
 * @param id The chip id read from the chip
 * @retval ERRNO_OK Read chip id successfully 
 * @retval ERRNO_ERROR The input parameter is NULL
 */
int32_t system_get_chip_id(uint32_t* id)
{
    if (!id)
        return ERRNO_ERROR;

    id[0] = EFC->SN_L;
    id[1] = EFC->SN_H;

    return ERRNO_OK;
}

