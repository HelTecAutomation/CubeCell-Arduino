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

#if !defined(CY_SCB_PVT_SPI_2_H)
#define CY_SCB_PVT_SPI_2_H

#include "SPI_2.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define SPI_2_SetI2CExtClkInterruptMode(interruptMask) SPI_2_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define SPI_2_ClearI2CExtClkInterruptSource(interruptMask) SPI_2_CLEAR_INTR_I2C_EC(interruptMask)
#define SPI_2_GetI2CExtClkInterruptSource()                (SPI_2_INTR_I2C_EC_REG)
#define SPI_2_GetI2CExtClkInterruptMode()                  (SPI_2_INTR_I2C_EC_MASK_REG)
#define SPI_2_GetI2CExtClkInterruptSourceMasked()          (SPI_2_INTR_I2C_EC_MASKED_REG)

#if (!SPI_2_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define SPI_2_SetSpiExtClkInterruptMode(interruptMask) \
                                                                SPI_2_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define SPI_2_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                SPI_2_CLEAR_INTR_SPI_EC(interruptMask)
    #define SPI_2_GetExtSpiClkInterruptSource()                 (SPI_2_INTR_SPI_EC_REG)
    #define SPI_2_GetExtSpiClkInterruptMode()                   (SPI_2_INTR_SPI_EC_MASK_REG)
    #define SPI_2_GetExtSpiClkInterruptSourceMasked()           (SPI_2_INTR_SPI_EC_MASKED_REG)
#endif /* (!SPI_2_CY_SCBIP_V1) */

#if(SPI_2_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void SPI_2_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (SPI_2_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (SPI_2_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_SPI_2_CUSTOM_INTR_HANDLER)
    extern cyisraddress SPI_2_customIntrHandler;
#endif /* !defined (CY_REMOVE_SPI_2_CUSTOM_INTR_HANDLER) */
#endif /* (SPI_2_SCB_IRQ_INTERNAL) */

extern SPI_2_BACKUP_STRUCT SPI_2_backup;

#if(SPI_2_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 SPI_2_scbMode;
    extern uint8 SPI_2_scbEnableWake;
    extern uint8 SPI_2_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 SPI_2_mode;
    extern uint8 SPI_2_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * SPI_2_rxBuffer;
    extern uint8   SPI_2_rxDataBits;
    extern uint32  SPI_2_rxBufferSize;

    extern volatile uint8 * SPI_2_txBuffer;
    extern uint8   SPI_2_txDataBits;
    extern uint32  SPI_2_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 SPI_2_numberOfAddr;
    extern uint8 SPI_2_subAddrSize;
#endif /* (SPI_2_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (SPI_2_SCB_MODE_I2C_CONST_CFG || \
        SPI_2_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 SPI_2_IntrTxMask;
#endif /* (! (SPI_2_SCB_MODE_I2C_CONST_CFG || \
              SPI_2_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(SPI_2_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define SPI_2_SCB_MODE_I2C_RUNTM_CFG     (SPI_2_SCB_MODE_I2C      == SPI_2_scbMode)
    #define SPI_2_SCB_MODE_SPI_RUNTM_CFG     (SPI_2_SCB_MODE_SPI      == SPI_2_scbMode)
    #define SPI_2_SCB_MODE_UART_RUNTM_CFG    (SPI_2_SCB_MODE_UART     == SPI_2_scbMode)
    #define SPI_2_SCB_MODE_EZI2C_RUNTM_CFG   (SPI_2_SCB_MODE_EZI2C    == SPI_2_scbMode)
    #define SPI_2_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (SPI_2_SCB_MODE_UNCONFIG == SPI_2_scbMode)

    /* Defines wakeup enable */
    #define SPI_2_SCB_WAKE_ENABLE_CHECK       (0u != SPI_2_scbEnableWake)
#endif /* (SPI_2_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!SPI_2_CY_SCBIP_V1)
    #define SPI_2_SCB_PINS_NUMBER    (7u)
#else
    #define SPI_2_SCB_PINS_NUMBER    (2u)
#endif /* (!SPI_2_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_SPI_2_H) */


/* [] END OF FILE */
