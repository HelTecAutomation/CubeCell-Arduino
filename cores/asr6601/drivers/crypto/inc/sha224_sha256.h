/**
 ******************************************************************************
 * @file    sha224_sha256.h
 * @author  ASR Tremo Team
 * @version V1.0
 * @date    2020-09-14
 * @brief   The header file for sha224_sha256
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup CRYPTO
 * @{
 * @defgroup SHA224_SHA256
 * @{
 */

#ifndef __SHA224_SHA256_H__
#define __SHA224_SHA256_H__
#include "type_define.h"

/**
 * @brief SHA256 context
 */
typedef struct SHA256Context
{
    U32 byteLen[2];            /*!< message length, byteLen[0] is the high 32 bits, byteLen[1] is the low 32 bits */
    U8 Message_Block[64];      /*!< message block */
    U8 Message_Block_Index;    /*!< message block index */
    U8 Corrupted;              /*!< corrupted flag */
} SHA256Context;

/**
 * @brief SHA224 context
 */
typedef SHA256Context SHA224Context;

/**
 * @brief SHA256 ERRNO
 */
enum
{
    HashSuccess = 0,     /*!< success */
    HashNull,            /*!< input data is NULL */
    HashInputTooLong,    /*!< input data length is too long */
    HashStateError,      /*!< state is wrong */
    HashLenError         /*!< length is wrong */
};

/**
 * @brief Initialize a new SHA256 context
 *
 * @param context The context that will be initialized
 * @retval HashSuccess Success
 * @retval Others Fail
 */
U8 sha256_init(SHA256Context *context);

/**
 * @brief Process input data and update the SHA256 context
 *
 * @param context The context of SHA256
 * @param message The message data that will be processed using SHA256
 * @param byteLen The length of input message
 * @retval HashSuccess success
 * @retval HashNull input data is NULL
 * @retval HashInputTooLong input data length is too long
 * @retval HashStateError the state is wrong
 * @retval HashLenError the length is wrong
 */
U8 sha256_input(SHA256Context *context, U8 *message, U32 byteLen);

/**
 * @brief Produce the output SHA256 digest
 *
 * @param context The context of SHA256
 * @param digest The result of the SHA256
 * @retval HashSuccess success
 * @retval HashNull input data is NULL
 * @retval HashInputTooLong input data length is too long
 * @retval HashStateError the state is wrong
 * @retval HashLenError the length is wrong
 */
U8 sha256_result(SHA256Context *context, U8 digest[32]);

/**
 * @brief Use the SHA256 algorithm to generate the digest of the input message directly
 *
 * @param in The input message data that will be processed using SHA256
 * @param inByteLen The length of the input message data
 * @param digest The digest of the SHA256 hash function
 * @retval HashSuccess success
 * @retval HashNull input data is NULL
 * @retval HashInputTooLong input data length is too long
 * @retval HashStateError the state is wrong
 * @retval HashLenError the length is wrong
 */
U8 sha256_hash(U8 *in, U32 inByteLen, U8 digest[32]);

/**
 * @brief Initialize a new SHA224 context
 *
 * @param context The context that will be initialized
 * @retval HashSuccess Success
 * @retval Others Fail
 */
U8 sha224_init(SHA224Context *context);

/**
 * @brief Process input data and update the SHA224 context
 *
 * @param context The context of SHA224
 * @param message The message data that will be prcessed using SHA224
 * @param byteLen The length of input message
 * @retval HashSuccess success
 * @retval HashNull input data is NULL
 * @retval HashInputTooLong input data length is too long
 * @retval HashStateError the state is wrong
 * @retval HashLenError the length is wrong
 */
U8 sha224_input(SHA224Context *context, U8 *message, U32 byteLen);

/**
 * @brief Produce the output SHA224 digest
 *
 * @param context The context of SHA224
 * @param digest The result of the SHA224
 * @retval HashSuccess success
 * @retval HashNull input data is NULL
 * @retval HashInputTooLong input data length is too long
 * @retval HashStateError the state is wrong
 * @retval HashLenError the length is wrong
 */
U8 sha224_result(SHA224Context *context, U8 digest[28]);

/**
 * @brief Use the SHA224 algorithm to generate the digest of the input message directly
 *
 * @param in The input message data that will be processed using SHA224
 * @param inByteLen The length of the input message data
 * @param digest The digest of the SHA224 hash function
 * @retval HashSuccess success
 * @retval HashNull input data is NULL
 * @retval HashInputTooLong input data length is too long
 * @retval HashStateError the state is wrong
 * @retval HashLenError the length is wrong
 */
U8 sha224_hash(U8 *in, U32 inByteLen, U8 digest[28]);


#endif /* ifndef __SHA224_SHA256_H__ */

/**
 * @} 
 * @}
 * @}
 */