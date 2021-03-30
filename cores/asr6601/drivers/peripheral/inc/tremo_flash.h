/**
 ******************************************************************************
 * @file    tremo_flash.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   This file contains all the functions for the FLASH firmware library.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup FLASH
 * @{
 */

#ifndef __TREMO_FLASH_H
#define __TREMO_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "tremo_regs.h"

#define FLASH_LINE_SIZE       (0x200)                  /*!< The size of the flash word line */
#define FLASH_PAGE_SIZE       (0x1000)                 /*!< The size of the flash page */
#define FLASH_CR_PROTECT_SEQ0 ((uint32_t)0x8C9DAEBF)   /*!< The flash protect sequence0 to unlock the flash CR access */
#define FLASH_CR_PROTECT_SEQ1 ((uint32_t)0x13141516)   /*!< The flash protect sequence1 to unlock the flash CR access */

#define FLASH_OTP_ADDR_START ((uint32_t)0x10001C00)                      /*!< The start address of the flash OTP area */
#define FLASH_OTP_ADDR_END   ((uint32_t)0x10002000)                      /*!< The end address of the flash OTP area */
#define FLASH_OTP_SIZE       (FLASH_OTP_ADDR_END - FLASH_OTP_ADDR_START) /*!< The size of the flash OTP area */

#define ERRNO_FLASH_INVALID_ADDR       (-101)
#define ERRNO_FLASH_INVALID_SIZE       (-102)
#define ERRNO_FLASH_OTP_REFLASH        (-103)
#define ERRNO_FLASH_SEC_ERROR          (-104)

#define FLASH_CR_LOCK()                           \
    do {                                          \
        EFC->PROTECT_SEQ = FLASH_CR_PROTECT_SEQ0; \
        EFC->PROTECT_SEQ = 0;                     \
    } while (0)

#define FLASH_CR_UNLOCK()                         \
    do {                                          \
        EFC->PROTECT_SEQ = FLASH_CR_PROTECT_SEQ0; \
        EFC->PROTECT_SEQ = FLASH_CR_PROTECT_SEQ1; \
    } while (0)

int32_t flash_erase_all(void);
int32_t flash_erase_page(uint32_t addr);

int32_t flash_program_bytes(uint32_t addr, uint8_t* data, uint32_t size);
RAM_FUNC_ATTR int32_t flash_program_line(uint32_t addr, uint8_t* data);

int32_t flash_otp_program_data(uint32_t addr, uint8_t* data, uint32_t size);

#ifdef __cplusplus
}
#endif
#endif //__TREMO_FLASH_H

/**
 * @} 
 * @}
 */
