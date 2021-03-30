/***************************************************************************//**
* \file .h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component in I2C mode.
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

#if !defined(CY_SCB_I2C_PVT_I2C_H)
#define CY_SCB_I2C_PVT_I2C_H

#include "I2C_I2C.h"


/***************************************
*     Private Global Vars
***************************************/

extern volatile uint8 I2C_state; /* Current state of I2C FSM */


#if(I2C_I2C_MASTER_CONST)
    extern volatile uint16 I2C_mstrStatus;      /* Master Status byte  */
    extern volatile uint8  I2C_mstrControl;     /* Master Control byte */

    /* Receive buffer variables */
    extern volatile uint8 * I2C_mstrRdBufPtr;   /* Pointer to Master Read buffer */
    extern volatile uint32  I2C_mstrRdBufSize;  /* Master Read buffer size       */
    extern volatile uint32  I2C_mstrRdBufIndex; /* Master Read buffer Index      */

    /* Transmit buffer variables */
    extern volatile uint8 * I2C_mstrWrBufPtr;   /* Pointer to Master Write buffer */
    extern volatile uint32  I2C_mstrWrBufSize;  /* Master Write buffer size       */
    extern volatile uint32  I2C_mstrWrBufIndex; /* Master Write buffer Index      */
    extern volatile uint32  I2C_mstrWrBufIndexTmp; /* Master Write buffer Index Tmp */
#endif /* (I2C_I2C_MASTER_CONST) */

/***************************************
*     Private Function Prototypes
***************************************/

#if(I2C_SCB_MODE_I2C_CONST_CFG)
    void I2C_I2CInit(void);
#endif /* (I2C_SCB_MODE_I2C_CONST_CFG) */

void I2C_I2CStop(void);
void I2C_I2CFwBlockReset(void);

void I2C_I2CSaveConfig(void);
void I2C_I2CRestoreConfig(void);

#if(I2C_I2C_MASTER_CONST)
    void I2C_I2CReStartGeneration(void);
#endif /* (I2C_I2C_MASTER_CONST) */

#endif /* (CY_SCB_I2C_PVT_I2C_H) */


/* [] END OF FILE */
