/**
 ******************************************************************************
 * @file    drv.h
 * @author  ASR Tremo Team
 * @version V1.0
 * @date    2020-09-14
 * @brief   Header file of crypto module.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup CRYPTO
 * @{
 */

#ifndef __DRV_H__
#define __DRV_H__

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "type_define.h"
#include "sec_regs.h"

/*-------------------------
Driver Enable Define
-------------------------*/
#define RNG_DRV_EN  /*!< RNG enable*/
#define DES_DRV_EN  /*!< DES enable*/
#define AES_DRV_EN  /*!< AES enable*/
#define SHA1_DRV_EN /*!< SHA1 enable*/
#define SHA2_DRV_EN /*!< SHA2 enable*/
#define SHA3_DRV_EN /*!< SHA3 enable*/
#define ECC_DRV_EN  /*!< ECC enable*/
#define RSA_DRV_EN  /*!< RSA enable*/

#define DEF_TEST_REG(regname, value)\
{\
    if(regname != (value))\
    {\
       printf("%s default Err = 0x%x !\r\n", (#regname), (unsigned int)regname);\
			 SIM_FAIL();\
    }\
    else\
    {\
      printf("%s default OK !\r\n", (#regname));\
    }\
} /*!< Register default value test*/

#define RW_TEST_REG(regname, valueFF, valueAA, value55, value00)\
{\
    regname = 0xFF;\
    delay(0x01);\
    if(regname != (valueFF))\
    {\
       printf("%s 0xFF Err = 0x%x !\r\n", (#regname), (unsigned int)regname);\
			 SIM_FAIL();\
    }\
\
    regname = 0x00;\
    delay(0x01);\
    if(regname != (value00))\
    {\
       printf("%s 0x00 Err = 0x%x !\r\n", (#regname), (unsigned int)regname);\
			 SIM_FAIL();\
    }\
\
    regname = 0xAA;\
    delay(0x01);\
    if(regname != (valueAA))\
    {\
       printf("%s 0xAA Err = 0x%x !\r\n", (#regname), (unsigned int)regname);\
			 SIM_FAIL();\
    }\
\
    regname = 0x55;\
    delay(0x01);\
    if(regname != (value55))\
    {\
       printf("%s 0x55 Err = 0x%x !\r\n", (#regname), (unsigned int)regname);\
			 SIM_FAIL();\
    }\
\
    printf("%s check OK !\r\n", (#regname)); \
          \
} /*!< Register write read test*/

#endif /* ifndef __DRV_H__ */

/**
 * @} 
 * @}
 */
