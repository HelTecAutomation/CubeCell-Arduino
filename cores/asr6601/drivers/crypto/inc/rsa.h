/**
 ******************************************************************************
 * @file    rsa.h
 * @author  ASR Tremo Team
 * @version V1.0
 * @date    2020-09-14
 * @brief   The header file for rsa
 * @addtogroup Tremo_Drivers
 * @{
 * @addtogroup CRYPTO
 * @{
 * @addtogroup RSA
 * @{
 */
 
#ifndef __RSA_H__
#define __RSA_H__

#include "algorithm.h"

/**
 * @brief rsa_init rsa engine
 * @retval 0 successfully
 */
U8 rsa_init(void);

/**
 * @brief generate the RSA key pair
 *
 * @param e e part of the public key(e,n)
 * @param d d part of the private key(d,n)
 * @param n n part of common part of the public key and private key
 * @param ebitlen size of data e expressed in bits
 * @param nbitlen size of data n expressed in bits
 * @retval 0 successfully
 */
U8 rsa_getkey(U8* e, U8* d, U8* n, U16 ebitlen, U16 nbitlen);

/**
 * @brief generate the CRT RSA key pair
 * @param p p part of the private key(p, q, dp, dq, u)
 * @param q q part of the private key(p, q, d, dq, u)
 * @param dp dp part of the private key(p, q, d, dq, u)
 * @param dq dq part of the private key(p, q, d, dq, u)
 * @param u u part of the private key(p, q, d, dq, u)
 * @param e e part of the public key(e,n)
 * @param n n part of the public key(e,n)
 * @param ebitlen the bytes size of data e
 * @param nbitlen the bytes size of data n
 * @param mode
 *        @arg 0 e is output
 *        @arg 1 e is input
 *
 * @retval 0 successfully
 */
U8 rsa_getcrtkey( U8* p, U8* q, U8* dp, U8* dq, U8* u, U8* e,
                  U8* n, U16 ebitlen, U16 nbitlen);

				  
/**
 * @brief Modular exponentiation
 * @param a input buffer
 * @param e data e
 * @param n data n
 * @param out output buffer,out=a^e mod n
 * @param ebitlen the size of data e in bits
 * @param nbitlen the size of data n in bits
 * @retval 0 successfully
 */
U8 rsa_modexp(U8* a, U8* e, U8* n, U8* out, U16 ebitlen, U16 nbitlen);


/**
 * @brief Modular exponentiation use RSA CRT private key
 * @param in input buffer
 * @param p p part of the private key(p, q, dp, dq, u)
 * @param q q part of the private key(p, q, d, dq, u)
 * @param dp dp part of the private key(p, q, d, dq, u)
 * @param dq dq part of the private key(p, q, d, dq, u)
 * @param u u part of the private key(p, q, d, dq, u)
 * @param out output buffer,out=in^dq mod q + (u * (in^dp mod p - in^dq mod q) mod p)*q
 * @param nbitlen size of data n expressed in bits,must not more than 2048
 * @retval 0 successfully
 */
U8 rsa_modexpwithcrt(U8* in, U8* p, U8* q, U8* dp, U8* dq, U8* u, U8* out, U16 nbitlen);

/**
 * @brief  Modular multiplication
 * @param  a input data a
 * @param  b input data b
 * @param  n input data n
 * @param  out output buffer,out=(a*b)mod n
 * @param  nbitlen size of data n expressed in bits
 * @retval None
 */
void rsa_mod_mul( U8* a, U8* b, U8* n, U8* out, U16 nbitlen);

/**
 * @brief Modular inversion
 * @param a input data a
 * @param n input data n
 * @param out output buffer,out=(1/a)mod n
 * @param abitlen size of data a expressed in bits
 * @param nbitlen size of data n expressed in bits
 * @retval 0 successfully
 */
U8 rsa_mod_inv(U8* a, U8* n, U8* out, U16 abitlen, U16 nbitlen);	


/**
 * @brief Modular multiplication of large numbers
 * @param a input data a
 * @param b input data b
 * @param out output buffer,out=(a*b)
 * @param abitlen size of data a expressed in bits
 * @param bbitlen size of data b expressed in bits
 * @retval None
 */
void rsa_mul(U8* a, U8* b, U8* out, U16 abitlen, U16 bbitlen);

/**
 * @brief comparison of large numbers
 * @param a input data a
 * @param abytelen size of data a expressed in bytes
 * @param b input data b
 * @param bbytelen size of data b expressed in bytes
 * @retval 0 a==b
 * @retval 1 a>b
 * @retval 2 a<b
 */
U8 rsa_compare(U8* a, U16 abytelen, U8* b, U16 bbytelen);

/**
 * @brief generate a prime number
 * @param prime prime number buffer
 * @param bitLen size of prime number expressed in bits
 * @retval None
 */
void rsa_get_prime(U8* prime, U16 bitLen);

/**
 * @brief get the bit length of input data
 * @param a input data
 * @param abytelen size of data a expressed in bytes
 * @return the bit length of input data
 */
U16 rsa_getbitlen(U8* a, U16 abytelen);
				  
/**
 * @brief close rsa engine
 * @retval 0 successfully
 */
U8 rsa_close(void);

#endif /* ifndef __RSA_H__ */

/**
 * @} 
 * @}
 * @}
 */
