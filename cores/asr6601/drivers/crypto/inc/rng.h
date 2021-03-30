/**
 ******************************************************************************
 * @file    rng.h
 * @author  ASR Tremo Team
 * @version V1.0
 * @date    2020-09-14
 * @brief   The header file for rng
 * @addtogroup Tremo_Drivers
 * @{
 * @addtogroup CRYPTO
 * @{
 * @defgroup RNG
 * @{
 */

#ifndef __RNG_H__
#define __RNG_H__
#include "algorithm.h"

/**
 * @brief RNG ERRNO
 */
enum {
    RNGSuccess =0,   /*!< success */
    RNGCKError,      /*!< error */
    RNGInitFail      /*!< init failed */
};

/**
 * @brief rng initialization
 *
 * @param div 
 * @param mode
 * @retval None
 */
void rng_init(UINT8 div, UINT8 mode);

/**
 * @brief rng close
 *
 * @param None 
 * @retval None
 */
void rng_close(void);

/**
 * @brief rng get random values
 *
 * @param buf the output buffer
 * @param len the length of the buffer
 * @retval None
 */
void rng_get_rand(UINT8 *buf, UINT16 len);
#endif /* ifndef __RNG_H__ */

/**
 * @} 
 * @}
 * @}
 */