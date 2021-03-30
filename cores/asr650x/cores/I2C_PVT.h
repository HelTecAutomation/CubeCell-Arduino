/***************************************************************************//**
* \file .h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PVT_I2C_H)
#define CY_SCB_PVT_I2C_H

#include "I2C.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define I2C_SetI2CExtClkInterruptMode(interruptMask) I2C_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define I2C_ClearI2CExtClkInterruptSource(interruptMask) I2C_CLEAR_INTR_I2C_EC(interruptMask)
#define I2C_GetI2CExtClkInterruptSource()                (I2C_INTR_I2C_EC_REG)
#define I2C_GetI2CExtClkInterruptMode()                  (I2C_INTR_I2C_EC_MASK_REG)
#define I2C_GetI2CExtClkInterruptSourceMasked()          (I2C_INTR_I2C_EC_MASKED_REG)


    /* APIs to service INTR_SPI_EC register */
    #define I2C_SetSpiExtClkInterruptMode(interruptMask) \
                                                                I2C_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define I2C_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                I2C_CLEAR_INTR_SPI_EC(interruptMask)
    #define I2C_GetExtSpiClkInterruptSource()                 (I2C_INTR_SPI_EC_REG)
    #define I2C_GetExtSpiClkInterruptMode()                   (I2C_INTR_SPI_EC_MASK_REG)
    #define I2C_GetExtSpiClkInterruptSourceMasked()           (I2C_INTR_SPI_EC_MASKED_REG)



/***************************************
*     Vars with External Linkage
***************************************/

extern cyisraddress I2C_customIntrHandler;

extern I2C_BACKUP_STRUCT I2C_backup;

#if (! (I2C_SCB_MODE_I2C_CONST_CFG || \
        I2C_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 I2C_IntrTxMask;
#endif /* (! (I2C_SCB_MODE_I2C_CONST_CFG || \
              I2C_SCB_MODE_EZI2C_CONST_CFG)) */


/* Defines maximum number of SCB pins */
#define I2C_SCB_PINS_NUMBER    (7u)

#endif /* (CY_SCB_PVT_I2C_H) */


/* [] END OF FILE */
