/**
 ******************************************************************************
 * @file    ecc.h
 * @author  ASR Tremo Team
 * @version V1.0
 * @date    2020-09-14
 * @brief   Header file of crypto module.
 * @addtogroup Tremo_Drivers
 * @{
 * @addtogroup CRYPTO
 * @{
 * @defgroup ECC
 * @{
 */

#ifndef __ECCBASIC_H__
#define __ECCBASIC_H__

#include "type_define.h"

/**
 * @brief DES status
 */
enum
{
    ECCSUCCESS = 0,   /*!< success*/
    ECCBufferNull,    /*!< buffer is null*/
    ECCNotInCurve,    /*!< not in curve*/
    ECCZeroPoint,     /*!< zero point*/
    ECCRandomTooLong, /*!< random number too long*/
    ECCZeroALL,       /*!< all is zero*/
    ECCPError,        /*!< ECCP error*/
    ECCKError,        /*!< K error*/
    ECCNOMODINV       /*!< no inverse mode*/
};

/**
 * @brief curve point
 */
typedef struct
{
    U8* x; /*!< x value*/
    U8* y; /*!< y value*/
} ECC_POINT;

/**
 * @brief ECC  parameters
 */
typedef struct
{
    U16  ECC_NUMBIT; /*!< the size of elliptic curve domain parameters,can be 192/224/256/384/521*/
    U8*  ECC_p;      /*!< pointer to P value*/
    U8*  ECC_a;      /*!< pointer to A value*/
    U8*  ECC_b;      /*!< pointer to B value*/
    U8*  ECC_G0x;    /*!< pointer to x coordinate of generator point*/
    U8*  ECC_G0y;    /*!< pointer to y coordinate of generator point*/
    U8*  ECC_n;      /*!< pointer to n value*/
} ECC_PARA;

/**
 * @brief  ECC initialize
 * @param  ECC_PARA0 ECC elliptic curve paramters
 * @retval ECCSUCCESS success
 * @retval ECCBufferNull buffer is null
 * @retval ECCNotInCurve not in curve
 * @retval ECCZeroPoint zero point
 * @retval ECCRandomTooLong random number too long
 * @retval ECCZeroALL all is zero
 * @retval ECCPError ECCP error
 * @retval ECCKError K error
 * @retval ECCNOMODINV no inverse mode
 */
U8 ecc_init(ECC_PARA* ECC_PARA0);

/**
 * @brief  ECC close
 * @retval ECCSUCCESS success
 * @retval ECCBufferNull buffer is null
 * @retval ECCNotInCurve not in curve
 * @retval ECCZeroPoint zero point
 * @retval ECCRandomTooLong random number too long
 * @retval ECCZeroALL all is zero
 * @retval ECCPError ECCP error
 * @retval ECCKError K error
 * @retval ECCNOMODINV no inverse mode
 */
U8 ecc_close(void);

/**
 * @brief  ECC get bit length
 * @param  a A value
 * @param  abytelen A value bytes length
 * @return bit length
 */
 U16 ecc_getbitlen(U8* a, U16 abytelen);

/**
 * @brief  ECC compare the data a and b
 * @param  a A value
 * @param  abytelen A value bytes length
 * @param  b B value
 * @param  bbytelen B value bytes length
 * @return compare result
 */
 U8 ecc_compare(U8* a, U16 abytelen, U8* b, U16 bbytelen );
 

/**
 * @brief  ECC point multiple
 * @param  k the scalar value of the multiplication
 * @param  kWordLen K value bytes length
 * @param  px point that will be multiplied
 * @param  out output,result of the multiplication
 * @retval ECCSUCCESS success
 * @retval ECCBufferNull buffer is null
 * @retval ECCNotInCurve not in curve
 * @retval ECCZeroPoint zero point
 * @retval ECCRandomTooLong random number too long
 * @retval ECCZeroALL all is zero
 * @retval ECCPError ECCP error
 * @retval ECCKError K error
 * @retval ECCNOMODINV no inverse mode
 */
U8 ecc_pointmul(U8* k, U16 kWordLen, ECC_POINT* px, ECC_POINT* out);

/**
 * @brief  ECC point add
 * @param  x X point value
 * @param  y Y point value
 * @param  out output,result of the addition
 * @retval ECCSUCCESS success
 * @retval ECCBufferNull buffer is null
 * @retval ECCNotInCurve not in curve
 * @retval ECCZeroPoint zero point
 * @retval ECCRandomTooLong random number too long
 * @retval ECCZeroALL all is zero
 * @retval ECCPError ECCP error
 * @retval ECCKError K error
 * @retval ECCNOMODINV no inverse mode
 */
U8 ecc_pointadd(ECC_POINT* x, ECC_POINT* y, ECC_POINT* out);

/**
 * @brief  ECC point double
 * @param  x X point value
 * @param  out output,result of the point double
 * @retval ECCSUCCESS success
 * @retval ECCBufferNull buffer is null
 * @retval ECCNotInCurve not in curve
 * @retval ECCZeroPoint zero point
 * @retval ECCRandomTooLong random number too long
 * @retval ECCZeroALL all is zero
 * @retval ECCPError ECCP error
 * @retval ECCKError K error
 * @retval ECCNOMODINV no inverse mode
 */
U8 ecc_pointdouble(ECC_POINT* x, ECC_POINT* out);

/**
 * @brief  ECC modular multiple
 * @param  a A value
 * @param  b B value
 * @param  p P value
 * @param  pbytelen P value length
 * @param  out output
 * @return
 */
void ecc_modmul( U8* a, U8* b, U8* p, U16 pbytelen, U8* out);

/**
 * @brief  ECC modular add
 * @param  a A value
 * @param  b B value
 * @param  p P value
 * @param  wordLen size of data p expressed in bytes
 * @param  out output
 * @return
 */
void ecc_modadd(U8* a, U8* b, U8* p, U16 wordLen, U8* out);

/**
 * @brief  ECC modular subtraction
 * @param  a A value
 * @param  b B value
 * @param  p P value
 * @param  pbytelen size of data p expressed in bytes
 * @param  out output
 * @return
 */
void ecc_modsub(U8* a, U8* b, U8* p, U16 pbytelen, U8* out);

/**
 * @brief  ECC modular inverse
 * @param  a A value
 * @param  p P value
 * @param  pbitlen size of data p expressed in bytes
 * @param  out output
 * @return
 */
U8 ecc_modinv(U8* a, U8* p, U16 pbitlen, U8* out);

/**
 * @brief  ECC check test point is on an elliptic curve
 * @param  x tested point
 * @retval 1 in curve
 * @retval 0 not in curve
 */
U8 ecc_testpoint(ECC_POINT* x);

/**
 * @brief  ECC check private key
 * @param  d D value
 * @param  dbytelen byte length
 * @retval 1 OK
 * @retval 0 ERROR
 */
U8 ecp_check_privkey( U8 *d,U8 dbytelen);

/**
 * @brief  for data converse endian
 * @param  in input data
 * @param  out output data
 * @param  dbytelen input data length
 * @retval ECCSUCCESS success
 * @retval ECCBufferNull buffer is null
 * @retval ECCNotInCurve not in curve
 * @retval ECCZeroPoint zero point
 * @retval ECCRandomTooLong random number too long
 * @retval ECCZeroALL all is zero
 * @retval ECCPError ECCP error
 * @retval ECCKError K error
 * @retval ECCNOMODINV no inverse mode
 */
U8 ecc_swith_endian_cpy(U8* in ,U8* out,U8 dbytelen);

/**
 * @brief  ecies xor function
 * @param  in input data
 * @param  message message data
 * @param  out output data
 * @param  dbytelen byte length
 * @retval ECCSUCCESS success
 * @retval ECCBufferNull buffer is null
 * @retval ECCNotInCurve not in curve
 * @retval ECCZeroPoint zero point
 * @retval ECCRandomTooLong random number too long
 * @retval ECCZeroALL all is zero
 * @retval ECCPError ECCP error
 * @retval ECCKError K error
 * @retval ECCNOMODINV no inverse mode
 */
U8 ecies_xor_meg(U8* in,U8* message,U8* out ,U8 bytelen);

/**
 * @brief  ECC gen prikey pubkey pair
 * @param  prikey private key
 * @param  pubkey public key
 * @retval ECCSUCCESS success
 * @retval ECCBufferNull buffer is null
 * @retval ECCNotInCurve not in curve
 * @retval ECCZeroPoint zero point
 * @retval ECCRandomTooLong random number too long
 * @retval ECCZeroALL all is zero
 * @retval ECCPError ECCP error
 * @retval ECCKError K error
 * @retval ECCNOMODINV no inverse mode
 */
U8 ecc_genkey(U8* prikey,ECC_POINT* pubkey);

/**
 * @brief  ecc ecdsa signature verification with a digest input
 * @param  pubkey public key
 * @param  digest digest value
 * @param  rs the resut of the signature
 * @retval ECCSUCCESS success
 * @retval ECCBufferNull buffer is null
 * @retval ECCNotInCurve not in curve
 * @retval ECCZeroPoint zero point
 * @retval ECCRandomTooLong random number too long
 * @retval ECCZeroALL all is zero
 * @retval ECCPError ECCP error
 * @retval ECCKError K error
 * @retval ECCNOMODINV no inverse mode
 */
U8 ecdsa_verify(ECC_POINT* pubkey, U8* digest, ECC_POINT* rs);

/**
 * @brief  ecc ecdsa signature generation
 * @param  prikey private key
 * @param  digest digest value
 * @param  rs the result of the signature
 * @retval ECCSUCCESS success
 * @retval ECCBufferNull buffer is null
 * @retval ECCNotInCurve not in curve
 * @retval ECCZeroPoint zero point
 * @retval ECCRandomTooLong random number too long
 * @retval ECCZeroALL all is zero
 * @retval ECCPError ECCP error
 * @retval ECCKError K error
 * @retval ECCNOMODINV no inverse mode
 */
U8 ecdsa_sign(U8* prikey, U8* digest, ECC_POINT* rs);



#endif /* ifndef __ECCBASIC_H__ */

/**
 * @} 
 * @}
 * @}
 */
