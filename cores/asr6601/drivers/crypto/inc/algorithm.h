/**
 ******************************************************************************
 * @file    algorithm.h
 * @author  ASR Tremo Team
 * @version V1.0
 * @date    2020-09-14
 * @brief   Header file of crypto module.
 * @addtogroup Tremo_Drivers
 * @{
 * @addtogroup CRYPTO
 * @{
 * @defgroup REGS
 * @{
 */

#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__
#include <string.h>
#include "type_define.h"
#include "sec_regs.h"
#include "drv.h"
/*---------------------------------------------------------------------------*/
/*                                  SAE                                      */
/*---------------------------------------------------------------------------*/

/**
 * @brief SAE
 */
typedef struct
{
  volatile   UINT32  SAECR      ;  /*!< common control register,Offset: 0x000*/
  volatile   UINT32  SAEHCR     ;  /*!< safe control register,Offset: 0x004*/
  volatile   UINT32  SAESR      ;  /*!< status register,Offset: 0x008*/
  volatile   UINT32  SAEGPR0    ;  /*!< general group register 0,Offset: 0x00C*/
  volatile   UINT32  SAEGPR1    ;  /*!< general group register 1,Offset: 0x010*/
  volatile   UINT32  SAEGPR2    ;  /*!< general group register 2,Offset: 0x014*/
  volatile   UINT32  SAEGPR3    ;  /*!< general group register 3,Offset: 0x018*/
  volatile   UINT32  SAEGPR4    ;  /*!< general group register 4,Offset: 0x01C*/
  volatile   UINT32  SAEGPR5    ;  /*!< general group register 5,Offset: 0x020*/
  volatile   UINT32  SAEMASKCR  ;  /*!< mask control register,Offset: 0x024*/
  volatile   UINT32  SAEMASKDAT0;  /*!< mask data register 0,Offset: 0x028*/
  volatile   UINT32  SAEMASKDAT1;  /*!< mask data register 1,Offset: 0x02C*/
  volatile   UINT32  SAEMASKDAT2;  /*!< mask data register 2,Offset: 0x030*/
  volatile   UINT32  SAEBUFCR   ;  /*!< buffer control register,Offset: 0x034*/
} sae_t;

#define CRYPTO_PKA          (0) /*!< crypto pka*/
#define CRYPTO_SHA          (1) /*!< crypto sha*/
#define CRYPTO_BLOCK        (2) /*!< crypto block,DES/AES/SM4*/

#define WORDWR              (1<<4) /*!< word or byte access*/
#define ENDIANTRANS         (1<<5) /*!< crypto block*/
#define DMAMODEEN           (1<<6) /*!< when word access enable converse byte-order*/
#define SAEINTEN            (1<<7) /*!< enable interrupt*/

#define RAMCLR              (1<<0)       /*!< clear RAM*/
#define KEYUPEN             (1<<1)       /*!< enable secret key update*/
#define CLKSCEN             (1<<2)       /*!< enable scrambler clock*/
#define SAECLKEN            (1<<3)       /*!< enable clock*/
#define SAESCL(level)       ((level)<<4) /*!< scrambler level*/
#define SAESCL_MASK         (0x0F<<4)    /*!< scrambler level mask*/

#define CRYPTO_DONE         (1<<0) /*!< crypto done*/
#define GLUEERR             (1<<1) /*!< SAE glutted logic error*/
#define CMDERR              (1<<2) /*!< command crypto error*/
#define ALGRUN              (1<<3) /*!< clear RAM done*/
#define FLOWBUSY            (1<<4) /*!< algorithm busy*/

#define MASKEN              (1<<0) /*!< enable SAE mask update*/


#define CLR_ARAM()  do\
{\
    SAE->SAEHCR |= RAMCLR;\
    while(SAE->SAEHCR & RAMCLR);\
} while (0) /*!< clear RAMA*/

#ifdef DES_DRV_EN /*!< DES*/
/*------------------------DES------------------------------*/
#define DESBLOCKCR              SAE->SAEGPR3 /*!< SAE SAEGPR3 register*/
#define DESALG                  (0<<4)       /*!< DES algorithm*/
#define DESECB                  (0)          /*!< DES ECB*/
#define DESCBC                  (1)          /*!< DES CBC*/
#define DESOFB                  (2)          /*!< DES OFB*/
#define DESCTR                  (3)          /*!< DES CTR*/
#define DESCR                   SAE->SAEGPR0 /*!< SAE SAEGPR0 register*/
#define DES_MODE_DEC            (1<<0) /*!< DES decode mode*/
#define TDES_EN                 (1<<2) /*!< DES enable*/
#define TDES_KEYN               (1<<3) /*!< DES key enable*/
#define DES_FAKEN               (1<<4) /*!< DES fake enable*/
#define DES_RUN                 (1<<7) /*!< DES run*/

#define DES_ARAM_BASE           WYSEC_ARAM_ALONG_BASE               /*!< DES RAMA address*/
#define DES_K0ADR               ((uint8_t  *)(DES_ARAM_BASE+0x640)) /*!< DES key0 address*/
#define DES_K1ADR               ((uint8_t  *)(DES_ARAM_BASE+0x648)) /*!< DES key1 address*/
#define DES_K2ADR               ((uint8_t  *)(DES_ARAM_BASE+0x650)) /*!< DES key2 address*/
#define DES_ONEADR              ((uint8_t  *)(DES_ARAM_BASE+0x600)) /*!< DES counter address*/
#define DES_IVADR               ((uint8_t  *)(DES_ARAM_BASE+0x610)) /*!< DES IV input address*/
#define DES_XADR                ((uint8_t  *)(DES_ARAM_BASE+0x000)) /*!< DES input data address*/
#define DES_YADR                ((uint8_t  *)(DES_ARAM_BASE+0x020)) /*!< DES output data address*/
#endif //#ifdef DES_DRV_EN
#ifdef AES_DRV_EN /*!< AES*/
/*------------------------AES------------------------------*/

#define AESCR                  SAE->SAEGPR0 /*!< SAE SAEGPR0 register*/

#define AES_MODE_MASK          (0x03) /*!< mode mask*/
#define AES_MODE_ENC           (0)    /*!< encode mode*/
#define AES_MODE_DEC           (1)    /*!< decode mode*/
#define AES_MODE_KEY           (2)    /*!< key mode*/
#define AES_KEY128             (0<<2) /*!< 128 key*/
#define AES_KEY192             (1<<2) /*!< 192 key*/
#define AES_KEY256             (2<<2) /*!< 256 key*/
#define AES_RUN                (1<<7) /*!< AES run*/

#define AESBLOCKCR              SAE->SAEGPR3 /*!< SAE SAEGPR3 register*/
#define AESALG                  (1<<4)       /*!< AES algorithm*/
#define AESECB                  (0)          /*!< AES ECB*/
#define AESCBC                  (1)          /*!< AES CBC*/
#define AESOFB                  (2)          /*!< AES OFB*/
#define AESCTR                  (3)          /*!< AES CTR*/

#define AES_ARAM_BASE          WYSEC_ARAM_ALONG_BASE          /*!< AES RAMA address*/
#define AES_KADR               ((U8 *)(AES_ARAM_BASE+0x0640)) /*!< AES key address*/
#define AES_IVADR              ((U8 *)(AES_ARAM_BASE+0x0610)) /*!< AES IV input address*/
#define AES_ONEADR             ((U8 *)(AES_ARAM_BASE+0x600))  /*!< AES counter address*/
#define AES_XADR               ((U8 *)(AES_ARAM_BASE+0x000))  /*!< AES input data address*/
#define AES_YADR               ((U8 *)(AES_ARAM_BASE+0x020))  /*!< AES output data address*/
#endif //#ifdef AES_DRV_EN
#ifdef SM4_DRV_EN /*!< SM4*/
/*------------------------SM4------------------------------*/

#define SM4CR                   SAE->SAEGPR0 /*!< SAE SAEGPR0 register*/

#define SM4_MODE_MASK           (0x03) /*!< SM4 mode mask*/
#define SM4_MODE_ENC            (0)    /*!< SM4 encode mode*/
#define SM4_MODE_DEC            (1)    /*!< SM4 decode mode*/
#define SM4_MODE_KEY            (2)    /*!< SM4 key mode*/

#define SM4_RUN                 (1<<7)       /*!< SM4 run*/
#define SM4BLOCKCR              SAE->SAEGPR3 /*!< SAE SAEGPR3 register*/
#define SM4ALG                  (2<<4)       /*!< SM4 algorithm*/
#define SM4ECB                  (0)          /*!< SM4 ECB*/
#define SM4CBC                  (1)          /*!< SM4 CBC*/
#define SM4OFB                  (2)          /*!< SM4 OFB*/
#define SM4CTR                  (3)          /*!< SM4 CTR*/

#define SM4_ARAM_BASE            WYSEC_ARAM_ALONG_BASE         /*!< SM4 RAMA address*/
#define SM4_XADR                ((U8 *)(SM4_ARAM_BASE+0x000))  /*!< SM4 input data address*/
#define SM4_YADR                ((U8 *)(SM4_ARAM_BASE+0x020))  /*!< SM4 output data address*/
#define SM4_CKADR               ((U8 *)(SM4_ARAM_BASE+0x080))  /*!< SM4 extension secret key address*/
#define SM4_FKADR               ((U8 *)(SM4_ARAM_BASE+0x030))  /*!< SM4 extension fake key address*/
#define SM4_ONEADR              ((U8 *)(SM4_ARAM_BASE+0x600))  /*!< SM4 counter address*/
#define SM4_MKADR               ((U8 *)(SM4_ARAM_BASE+0x0640)) /*!< SM4 secret key address*/
#define SM4_IVADR               ((U8 *)(SM4_ARAM_BASE+0x0610)) /*!< SM4 IV input address*/
#endif //#ifdef SM4_DRV_EN

/*------------------------PKA------------------------------*/

#define PKACR                   SAE->SAEGPR0 /*!< PKA control register*/
#define PKASR                   SAE->SAEGPR1 /*!< PKA status register*/
#define PKAPLENH                SAE->SAEGPR2 /*!< PKA PKAPLEN high register*/
#define PKAPLENL                SAE->SAEGPR3 /*!< PKA PKAPLEN low register*/
#define PKAELENH                SAE->SAEGPR4 /*!< PKA PKAELEN high register*/
#define PKAELENL                SAE->SAEGPR5 /*!< PKA PKAELEN low register*/

/*PKACR bitmap*/
#define PKA_RUN                 (1<<7) /*!< PKA run*/
#define LONGADD                 (0x00) /*!< PKA add*/
#define LONGSUB                 (0x01) /*!< PKA subtraction*/
#define LONGLSHIFT              (0x02) /*!< left shift*/
#define LONGRSHIFT              (0x03) /*!< right shift*/
#define LONGCMP                 (0x04) /*!< compare*/
#define MODADD                  (0x05) /*!< modulo add*/
#define MODSUB                  (0x06) /*!< modulo subtraction*/
#define MODPSUB                 (0x07) /*!< modulo subtraction,less than p*/
#define LONGMUL                 (0x08) /*!< multiple*/
#define MODINV                  (0x09) /*!< modulo inverse*/
#define MODINVINIT              (0x0a) /*!< modulo inverse when initialize*/
#define MODMULPRE               (0x10) /*!< modulo multiple precomputation*/
#define MODMULINIT              (0x11) /*!< modulo multiple initialize*/
#define INTMODMUL               (0x20) /*!< integ modulo multiple*/
#define MGMODMUL                (0x21) /*!< montgomery modulo multiple*/
#define MGMODSQR                (0x22) /*!< montgomery modulo square*/
#define QMODE                   (0x24) /*!< quick modulo power*/
#define S1MODE                  (0x25) /*!< security modulo powerm,check 1 time*/
#define S2MODE                  (0x26) /*!< quick modulo powerm,check 2 times*/
#define ECCPADD                 (0x30) /*!< ECC p region add*/
#define ECCPDOUBLE              (0x31) /*!< ECC p region double*/
#define ECCPSMUL                (0x32) /*!< ECC p region security multiple*/
#define ECCPQMUL                (0x33) /*!< ECC p region quick multiple*/
#define DIV                     (0x40) /*!< division*/

#define PKA_ARAM_BASE           WYSEC_ARAM_MIX_BASE          /*!< PKA RAMA address*/
#define RSA_PADR                (U8  *)(PKA_ARAM_BASE+0x0)   /*!< RSA P address*/
#define RSA_EADR                (U8  *)(PKA_ARAM_BASE+0x100) /*!< RSA E address*/
#define RSA_AADR                (U8  *)(PKA_ARAM_BASE+0x200) /*!< RSA A address*/
#define RSA_HADR                (U8  *)(PKA_ARAM_BASE+0x600) /*!< RSA H address*/
#define RSA_R0ADR               (U8  *)(PKA_ARAM_BASE+0x700) /*!< RSA R0 address*/
#define RSA_DADR                (U8  *)(PKA_ARAM_BASE+0x72C) /*!< RSA D address*/
#define RSA_R1ADR               (U8  *)(PKA_ARAM_BASE+0x800) /*!< RSA R1 address*/
#define RSA_BADR                (U8  *)(PKA_ARAM_BASE+0x800) /*!< RSA B address*/
#define RSA_J0ADR               (U8  *)(PKA_ARAM_BASE+0x800) /*!< RSA J0 address*/
#define RSA_RADR                (U8  *)(PKA_ARAM_BASE+0xA00) /*!< RSA R address*/
#define RSA_EBKADR              (U8  *)(PKA_ARAM_BASE+0x800) /*!< RSA EBK address*/
#define RSA_UADR                (U8  *)(PKA_ARAM_BASE+0x384) /*!< RSA U address*/     
#define RSA_VADR                (U8  *)(PKA_ARAM_BASE+0x984) /*!< RSA V address*/	

#define ECCPADDR                (U8  *)(PKA_ARAM_BASE+0x000) /*!< ECC P address*/
#define ECCAADDR                (U8  *)(PKA_ARAM_BASE+0x044) /*!< ECC A address*/
#define ECCKADDR                (U8  *)(PKA_ARAM_BASE+0x088) /*!< ECC K address*/
#define ECCQ0XADDR              (U8  *)(PKA_ARAM_BASE+0x0CC) /*!< ECC Q0X address*/
#define ECCQ0YADDR              (U8  *)(PKA_ARAM_BASE+0x6CC) /*!< ECC Q0Y address*/
#define ECCQ1XADDR              (U8  *)(PKA_ARAM_BASE+0x110) /*!< ECC Q1X address*/
#define ECCQ1YADDR              (U8  *)(PKA_ARAM_BASE+0x710) /*!< ECC Q1Y address*/
#define ECCMASKADDR             (U8  *)(PKA_ARAM_BASE+0x644) /*!< ECC mask address*/

#define DIV_ARAM_BASE            WYSEC_ARAM_ALONG_BASE       /*!< DIV RAMA address*/
#define DVSADDR                 (U8  *)(DIV_ARAM_BASE+0x000) /*!< DIV S address*/
#define DVDADDR                 (U8  *)(DIV_ARAM_BASE+0x004) /*!< DIV D address*/
#define RMDADDR                 (U8  *)(DIV_ARAM_BASE+0x300) /*!< DIV RMD address*/
#define QUAADDR                 (U8  *)(DIV_ARAM_BASE+0x304) /*!< DIV QUA address*/

/*---------------------------------------------------------------------------*/
/*                                  HASH                                      */
/*---------------------------------------------------------------------------*/
#define HASHCR                  SAE->SAEGPR0 /*!< HASH control register*/

#define HASH_ALG_MASK           (0x0F) /*!< HASH algorithm mask*/
#define HASH_SHA1               (0)    /*!< HASH SHA1*/
#define HASH_SHA224             (1)    /*!< HASH SHA224*/
#define HASH_SHA256             (2)    /*!< HASH SHA256*/
#define HASH_SHA384             (3)    /*!< HASH SHA384*/
#define HASH_SHA512             (4)    /*!< HASH SHA512*/
#define HASH_SM3                (7)    /*!< HASH SM3*/
#define HASH_RUN                (1<<7) /*!< HASH run*/

#ifdef SM3_DRV_EN /*!< SM3*/
/*------------------------SM3------------------------------*/
#define SM3_ARAM_BASE        WYSEC_ARAM_ALONG_BASE          /*!< SM3 RAMA address*/
#define SM3_VADADR           ((U8  *)(SM3_ARAM_BASE+0x300)) /*!< SM3 VA address*/
#define SM3_VEADR            ((U8  *)(SM3_ARAM_BASE+0x0))   /*!< SM3 VE address*/
#define SM3_WDADR            ((U8  *)(SM3_ARAM_BASE+0x10))  /*!< SM3 WD address*/
#define SM3_KADR             ((U8  *)(SM3_ARAM_BASE+0x310)) /*!< SM3 K address*/
#endif //#ifdef SM3_DRV_EN

#ifdef SHA1_DRV_EN /*!< SHA1*/
/*------------------------SHA1------------------------------*/
#define SHA1_ARAM_BASE          WYSEC_ARAM_ALONG_BASE           /*!< SHA1 RAMA address*/
#define SHA1_VEADR              ((U8  *)(SHA1_ARAM_BASE+0x0))   /*!< SHA1 VE address*/
#define SHA1_VADADR             ((U8  *)(SHA1_ARAM_BASE+0x300)) /*!< SHA1 VA address*/
#define SHA1_WDADR              ((U8  *)(SHA1_ARAM_BASE+0x10))  /*!< SHA1 WD address*/
#define SHA1_KADR               ((U8  *)(SHA1_ARAM_BASE+0x310)) /*!< SHA1 K address*/

#endif //#ifdef SHA1_DRV_EN

#ifdef SHA2_DRV_EN /*!< SHA2*/
/*------------------------SHA2------------------------------*/
#define SHA2_ARAM_BASE           WYSEC_ARAM_ALONG_BASE          /*!< SHA2 RAMA address*/
#define SHA2_VEADR              ((U8  *)(SHA2_ARAM_BASE+0x0))   /*!< SHA2 VE address*/
#define SHA2_VADADR             ((U8  *)(SHA2_ARAM_BASE+0x300)) /*!< SHA2 VA address*/
#define SHA2_WDADR              ((U8  *)(SHA2_ARAM_BASE+0x10))  /*!< SHA2 WD address*/
#define SHA2_KADR               ((U8  *)(SHA2_ARAM_BASE+0x310)) /*!< SHA2 K address*/
#endif //#ifdef SHA2_DRV_EN

#ifdef SHA3_DRV_EN /*!< SHA3*/
/*------------------------SHA3------------------------------*/
#define SHA512_ARAM_BASE        WYSEC_ARAM_MIX_BASE              /*!< SHA3 RAMA address*/
#define SHA512_VAHADR           ((U8  *)(SHA512_ARAM_BASE+0x0))  /*!< SHA3 VA address*/
#define SHA512_WDADR            ((U8  *)(SHA512_ARAM_BASE+0x40)) /*!< SHA3 WD address*/
#define SHA512_KADR             ((U8  *)(SHA512_ARAM_BASE+0xC0)) /*!< SHA3 K address*/
#endif //#ifdef SHA3_DRV_EN

/*---------------------------------------------------------------------------*/
/*                                  RNG                                      */
/*---------------------------------------------------------------------------*/
/**
 * @brief RNG
 */
typedef struct
{
  volatile   UINT32  RNGDET;  	 /*!< control register,offset: 0x000(R/W) */
  volatile   UINT32  RNGSR;  	 /*!< status register,offset: 0x004(R/W) */
  volatile   UINT32  RNGCLK;  	 /*!< clock register,offset: 0x008(R/W) */	
  volatile   UINT32  RNGCR;  	 /*!< control register,offset: 0x00c(R/W) */
  volatile   UINT32  RNGRESEED;  /*!< FIFO register,offset: 0x010(R/W) */
  volatile   UINT32  RNGDATA;    /*!< data register,offset: 0x014(R/W) */
  volatile   UINT32  RNGPKNUM;   /*!< SPI read register,offset: 0x018(R/W) */	
  volatile   UINT32  RNGPKRES0;  /*!< SPI write register,offset: 0x01C(R/W) */
  volatile   UINT32  RNGPKRES1;  /*!< interrupt wakeup register,offset: 0x020(R/W) */
  volatile   UINT32  RNGPKRES2;  /*!< chip selection register,offset: 0x024(R/W) */		
} rng_t;

#define FLOWREINI               (1<<3) /*!< flow initialize begin*/
#define POKERSRC_DAT            (1<<2) /*!< source data*/
#define POKEREN                 (1<<1) /*!< poker enalbe*/
#define DATACKEN                (1<<0) /*!< entropy enable*/

#define POKENCKEND              (1<<2) /*!< poker end*/
#define DATACKERR               (1<<1) /*!< clock error*/
#define DATARDY                 (1<<0) /*!< data ready*/

#define RNGCLKEN                (1<<7) /*!< clock enable*/

#define RNGEN                   (1<<7)     /*!< RNG enable*/
#define TRUEMODE                (1<<6)     /*!< flow mode*/
#define ENTROPYSEL(val)         ((val)<<4) /*!< entropy selection*/
#define PRECODESEL(val)         ((val)<<2) /*!< pre-process selection*/
#define POSTPROSEL(val)         (val)      /*!< post-process selection*/

#endif /* ifndef __ALGORITHM_H__ */

/**
 * @} 
 * @}
 * @}
 */
