/**
 ******************************************************************************
 * @file    sha384_sha512.h
 * @author  ASR Tremo Team
 * @version V1.0
 * @date    2020-09-14
 * @brief   The header file for sha384_sha512
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup CRYPTO
 * @{
 * @defgroup SHA384_SHA512
 * @{
 */

#ifndef __SHA384_SHA512_H__
#define __SHA384_SHA512_H__
#include "TypeDefine.h"

/**
 * @brief SHA512 context
 */
typedef struct SHA512Context
{
    U32 byteLen[4];            /*!< message length, byteLen[0] is the high 32 bits, byteLen[1] is the low 32 bits */
    U8 Message_Block[128];     /*!< message block */
    U8 Message_Block_Index;    /*!< message block index */
    S8 Corrupted;              /*!< corrupted flag */
} SHA512Context;

/**
 * @brief SHA384 context
 */
typedef SHA512Context SHA384Context;

/**
 * @brief SHA384 ERRNO
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
 * @brief Initialize a new SHA512 context
 *
 * @param context The context that will be initialized
 * @retval HashSuccess Success
 * @retval Others Fail
 */
U8 sha512_init(SHA512Context *context);

/**
 * @brief Process input data and update the SHA512 context
 *
 * @param context The context of SHA512
 * @param message The message data that will be processed using SHA512
 * @param byteLen The length of input message
 * @retval HashSuccess success
 * @retval HashNull input data is NULL
 * @retval HashInputTooLong input data length is too long
 * @retval HashStateError the state is wrong
 * @retval HashLenError the length is wrong
 */
U8 sha512_input(SHA512Context *context, U8 *message, U32 byteLen);

/**
 * @brief Produce the output SHA512 digest
 *
 * @param context The context of SHA512
 * @param digest The result of the SHA512
 * @retval HashSuccess success
 * @retval HashNull input data is NULL
 * @retval HashInputTooLong input data length is too long
 * @retval HashStateError the state is wrong
 * @retval HashLenError the length is wrong
 */
U8 sha512_result(SHA512Context *context, U8 digest[64]);

/**
 * @brief Use the SHA512 algorithm to generate the digest of the input message directly
 *
 * @param in The input message data that will be processed using SHA512
 * @param inByteLen The length of the input message data
 * @param digest The digest of the SHA512 hash function
 * @retval HashSuccess success
 * @retval HashNull input data is NULL
 * @retval HashInputTooLong input data length is too long
 * @retval HashStateError the state is wrong
 * @retval HashLenError the length is wrong
 */
U8 sha512_hash(U8 *in, U32 inByteLen, U8 digest[64]);

/**
 * @brief Initialize a new SHA384 context
 *
 * @param context The context that will be initialized
 * @retval HashSuccess Success
 * @retval Others Fail
 */
U8 sha384_init(SHA384Context *context);

/**
 * @brief Process input data and update the SHA384 context
 *
 * @param context The context of SHA384
 * @param message The input message that will be processed using SHA384
 * @param byteLen The length of input message
 * @retval HashSuccess success
 * @retval HashNull input data is NULL
 * @retval HashInputTooLong input data length is too long
 * @retval HashStateError the state is wrong
 * @retval HashLenError the length is wrong
 */
U8 sha384_input(SHA384Context *context, U8 *message, U32 byteLen);

/**
 * @brief Produce the ouput SHA384 digest
 *
 * @param context The context of SHA384
 * @param digest The digest of the SHA384
 * @retval HashSuccess success
 * @retval HashNull input data is NULL
 * @retval HashInputTooLong input data length is too long
 * @retval HashStateError the state is wrong
 * @retval HashLenError the length is wrong
 */
U8 sha384_result(SHA384Context *context, U8 digest[48]);

/**
 * @brief Use the SHA384 algorithm to generate the digest of the input message directly
 *
 * @param in The input message data that will be processed using SHA384
 * @param inByteLen The length of the input message data
 * @param digest The digest of the SHA384 hash function
 * @retval HashSuccess success
 * @retval HashNull input data is NULL
 * @retval HashInputTooLong input data length is too long
 * @retval HashStateError the state is wrong
 * @retval HashLenError the length is wrong
 */
U8 sha384_hash(U8 *in, U32 inByteLen, U8 digest[48]);


#endif /* ifndef __SHA384_SHA512_H__ */

/**
 * @} 
 * @}
 * @}
 */
