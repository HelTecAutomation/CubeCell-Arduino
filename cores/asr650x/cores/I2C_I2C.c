/***************************************************************************//**
* \file I2C_I2C.c
* \version 4.0
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  I2C mode.
*
* Note:
*
*******************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "I2C_PVT.h"
#include "I2C_I2C_PVT.h"



/***************************************
*      I2C Private Vars
***************************************/

volatile uint8 I2C_state;  /* Current state of I2C FSM */


    void I2C_I2CInit(void)
    {

        /* Adjust AF and DF filter settings. Ticket ID#176179 */
        I2C_I2C_CFG_ANALOG_FITER_ENABLE_ADJ;

        /* Configure I2C interface */
        I2C_CTRL_REG     = I2C_I2C_DEFAULT_CTRL;
        I2C_I2C_CTRL_REG = I2C_I2C_DEFAULT_I2C_CTRL;

        /* Configure RX direction */
        I2C_RX_CTRL_REG      = I2C_I2C_DEFAULT_RX_CTRL;
        I2C_RX_FIFO_CTRL_REG = I2C_I2C_DEFAULT_RX_FIFO_CTRL;

        /* Set default address and mask */
        I2C_RX_MATCH_REG     = I2C_I2C_DEFAULT_RX_MATCH;

        /* Configure TX direction */
        I2C_TX_CTRL_REG      = I2C_I2C_DEFAULT_TX_CTRL;
        I2C_TX_FIFO_CTRL_REG = I2C_I2C_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt with I2C handler but do not enable it */
        CyIntDisable    (I2C_ISR_NUMBER);
        CyIntSetPriority(I2C_ISR_NUMBER, I2C_ISR_PRIORITY);
    #if(!I2C_I2C_EXTERN_INTR_HANDLER)
        (void) CyIntSetVector(I2C_ISR_NUMBER, &I2C_I2C_ISR);
    #endif /* (I2C_I2C_EXTERN_INTR_HANDLER) */

        /* Configure interrupt sources */
    #if(!I2C_CY_SCBIP_V1)
        I2C_INTR_SPI_EC_MASK_REG = I2C_I2C_DEFAULT_INTR_SPI_EC_MASK;
    #endif /* (!I2C_CY_SCBIP_V1) */

        I2C_INTR_I2C_EC_MASK_REG = I2C_I2C_DEFAULT_INTR_I2C_EC_MASK;
        I2C_INTR_SLAVE_MASK_REG  = I2C_I2C_DEFAULT_INTR_SLAVE_MASK;
        I2C_INTR_MASTER_MASK_REG = I2C_I2C_DEFAULT_INTR_MASTER_MASK;
        I2C_INTR_RX_MASK_REG     = I2C_I2C_DEFAULT_INTR_RX_MASK;
        I2C_INTR_TX_MASK_REG     = I2C_I2C_DEFAULT_INTR_TX_MASK;

        /* Configure global variables */
        I2C_state = I2C_I2C_FSM_IDLE;

    #if(I2C_I2C_MASTER)
    /* Internal master variable */
        I2C_mstrStatus     = 0u;
        I2C_mstrRdBufIndex = 0u;
        I2C_mstrWrBufIndex = 0u;
    #endif /* (I2C_I2C_MASTER) */
    }


/*******************************************************************************
* Function Name: I2C_I2CStop
****************************************************************************//**
*
*  Resets the I2C FSM into the default state.
*
*******************************************************************************/
void I2C_I2CStop(void)
{
    /* Clear command registers because they keep assigned value after IP block was disabled */
    I2C_I2C_MASTER_CMD_REG = 0u;
    I2C_I2C_SLAVE_CMD_REG  = 0u;
    
    I2C_state = I2C_I2C_FSM_IDLE;
}


/*******************************************************************************
* Function Name: I2C_I2CFwBlockReset
****************************************************************************//**
*
* Resets the scb IP block and I2C into the known state.
*
*******************************************************************************/
void I2C_I2CFwBlockReset(void)
{
    /* Disable scb IP: stop respond to I2C traffic */
    I2C_CTRL_REG &= (uint32) ~I2C_CTRL_ENABLED;

    /* Clear command registers they are not cleared after scb IP is disabled */
    I2C_I2C_MASTER_CMD_REG = 0u;
    I2C_I2C_SLAVE_CMD_REG  = 0u;

    I2C_DISABLE_AUTO_DATA;

    I2C_SetTxInterruptMode(I2C_NO_INTR_SOURCES);
    I2C_SetRxInterruptMode(I2C_NO_INTR_SOURCES);
    

    I2C_state = I2C_I2C_FSM_IDLE;

    /* Enable scb IP: start respond to I2C traffic */
    I2C_CTRL_REG |= (uint32) I2C_CTRL_ENABLED;
}


/* [] END OF FILE */
