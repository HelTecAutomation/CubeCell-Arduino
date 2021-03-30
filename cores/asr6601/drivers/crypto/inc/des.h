/**
 ******************************************************************************
 * @file    des.h
 * @author  ASR Tremo Team
 * @version V1.0
 * @date    2020-09-14
 * @brief   Header file of crypto module.
 * @addtogroup Tremo_Drivers
 * @{
 * @addtogroup CRYPTO
 * @{
 * @defgroup DES
 * @{
 */

#ifndef __DES_H__
#define __DES_H__
#include "type_define.h"
#include "algorithm.h"

/**
 * @brief DES
 */
enum DES_ENUM
{
    DESSuccess = 0, /*!< seccess*/
    DESNull,        /*!< input null*/
    DESLengthError, /*!< length error,not integer multiples of 8*/
    DESLengthZero,  /*!< length is zero*/
    DESStateError,	/*!< state error*/
    DESSeedError,   /*!< seed error*/
    DESCloseError, 	/*!< close clock error*/
    DESAttacked     /*!< attacked*/
};

/**
 * @brief TDES
 */
enum TDES_ENUM
{
    TDESSuccess = 0, /*!< seccess*/
    TDESNull,        /*!< input null*/
    TDESLengthError, /*!< length error,not integer multiples of 8*/
    TDESLengthZero,  /*!< length is zero*/
    TDESStateError,	 /*!< state error*/
    TDESSeedError,   /*!< seed error*/
    TDESKeyLenError, /*!< key length error*/
    TDESCloseError,  /*!< close clock error*/
    TDESAttacked     /*!< attacked*/
};

/**
 * @brief DES code mode
 */
enum DES_ENC_DEC{
    DES_ENC_MODE =0, /*!< encode mode*/
    DES_DEC_MODE,    /*!< decode mode*/
};

/**
 * @brief DES mode
 */
enum DES_ECB_CBC{
    DES_ECB_MODE = 0, /*!< ECB mode*/
    DES_CBC_MODE ,    /*!< CBC mode*/
};

/**
 * @brief  DES initialize
 * @param  key secret key,must be 8 bytes
 * @param  mode DES mode
 *            @arg 0: ECB
 *            @arg 1: CBC
 *            @arg 2: OFB
 *            @arg 3: CTR
 * @param IVorNonce IV or nonce, must be 8 bytes,only needed in CBC mode
 * @retval DESNull input key is null
 * @retval DESSuccess seccessfully
 */
U8 des_init(U8* key,U8 mode ,U8* IVorNONCE);

/**
 * @brief  DES encryption/decryption
 * @param  in input buffer
 * @param  inByteLen size of input data in bytes,it must be a multiple of 8
 * @param  En_De encryption or decryption
 *            @arg 0: encryption
 *            @arg 1: decode
 * @param out output decryption
 * @retval DESNull input¡¢output or IV is NULL
 * @retval DESLengthZero length is zero
 * @retval DESLengthError length isn't integer tims of 8
 * @retval DESSuccess seccessfully
 */
U8 des_crypto(U8* in, U32 inByteLen, U8 En_De, U8*  out);

/**
 * @brief  DES close
 * @retval DESCloseError close error
 * @retval DESSuccess seccessfully
 */
U8 des_close(void);

/**
 * @brief  TDES initialize
 * @param  key secret key,must be 16 or 24 bytes,
 * @param  KeyBytelen secret key length
 * @param  mode TDES mode
 *            @arg 0: ECB
 *            @arg 1: CBC
 *            @arg 2: OFB
 *            @arg 3: CTR
 * @param IVorNonce IV or nonce,must be 8 bytes,only needed in CBC mode
 * @retval TDESNull input key is null
 * @retval TDESKeyLenError key length isn't 16 or 24
 * @retval DESSuccess seccessfully
 */
U8 tdes_init(U8* key, U8 KeyBytelen,U8 mode,U8* IVorNONCE);

/**
 * @brief  TDES encryption/decryption
 * @param  in input buffer
 * @param  inByteLen input data length in bytes,must be a multiple of 8
 * @param  En_De encryption or decryption
 *            @arg 0: encryption
 *            @arg 1: decryption
 * @param out output buffer
 * @retval TDESNull input¡¢output or IV is NULL
 * @retval TDESLengthZero length is zero
 * @retval TDESLengthError length isn't integer tims of 8
 * @retval TDESSuccess seccessfully
 */
U8 tdes_crypto(U8* in, U32 inByteLen, U8 En_De,  U8* out);

/**
 * @brief  TDES close
 * @retval TDESSuccess seccessfully
 */
U8 tdes_close(void);

#endif /* ifndef __DES_H__ */

/**
 * @} 
 * @}
 * @}
 */
