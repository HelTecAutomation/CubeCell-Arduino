/**
 ******************************************************************************
 * @file    sha1.h
 * @author  ASR Tremo Team
 * @version V1.0
 * @date    2020-09-14
 * @brief   The header file for sha1
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup CRYPTO
 * @{
 * @defgroup SHA1
 * @{
 */

#ifndef __SHA1_H__
#define __SHA1_H__
#include "type_define.h"

/**
 * @brief SHA1 context
 */
typedef struct SHA1Context
{
    U32 bitLen[2];            /*!< message length */
    U8 Message_Block[64];     /*!< message block */
    U8 Message_Block_Index;   /*!< message block index */
    U8 Corrupted;             /*!< corrupted flag */
}SHA1Context;

/**
 * @brief SHA1 ERRNO
 */
enum
{
    HashSuccess = 0,   /*!< success */
    HashNull,          /*!< input data is NULL */
    HashInputTooLong,  /*!< input data length is too long */
    HashStateError,    /*!< state is wrong */
    HashLenError       /*!< length is wrong */
};

/**
 * @brief Initialize a new SHA1 context
 *
 * @param context The context that will be initialized
 * @retval HashSuccess Success
 * @retval Others Fail
 */
U8 sha1_init(SHA1Context* context);

/**
 * @brief Process input data and update the SHA1 context
 *
 * @param context The context of SHA1
 * @param message The message data that will be processed using SHA1
 * @param byteLen The length of input message
 * @retval HashSuccess success
 * @retval HashNull input data is NULL
 * @retval HashInputTooLong input data length is too long
 * @retval HashStateError the state is wrong
 * @retval HashLenError the length is wrong
 */
U8 sha1_input(SHA1Context* context, U8* message, U32 byteLen);

/**
 * @brief Produce the output SHA1 digest
 *
 * @param context The context of SHA1
 * @param digest The result of the SHA1
 * @retval HashSuccess success
 * @retval HashNull input data is NULL
 * @retval HashInputTooLong input data length is too long
 * @retval HashStateError the state is wrong
 * @retval HashLenError the length is wrong
 */
U8 sha1_result(SHA1Context* context, U8 digest[20]);

/**
 * @brief Use the SHA1 algorithm to generate the digest of the input message directly
 *
 * @param in The input message data that will be processed using SHA1
 * @param inByteLen The length of the input message data
 * @param digest The digest of the SHA1 hash function
 * @retval HashSuccess success
 * @retval HashNull input data is NULL
 * @retval HashInputTooLong input data length is too long
 * @retval HashStateError the state is wrong
 * @retval HashLenError the length is wrong
 */
U8 sha1_hash(U8* in, U32 inByteLen, U8 digest[20]);

/**
 * @brief SHA1 iterator output
 *
 * @param tmp_V tmp_V
 * @retval None
 */
void sha1_iterator_output(U8 tmp_V[20]);

/**
 * @brief SHA1 iterator input
 *
 * @param context The context of SHA1
 * @param tmp_V tmp_V
 * @param byteLen byteLen
 * @retval None
 */
void sha1_iterator_input(SHA1Context* context, U8 tmp_V[20], U32 byteLen[2]);
#endif /* ifndef __SHA1_H__ */

/**
 * @} 
 * @}
 * @}
 */
