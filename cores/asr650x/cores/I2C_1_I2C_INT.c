/***************************************************************************//**
* \file I2C_1_I2C_INT.c
* \version 4.0
*
* \brief
*  This file provides the source code to the Interrupt Service Routine for
*  the SCB Component in I2C mode.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "I2C_1_PVT.h"
#include "I2C_1_I2C_PVT.h"
#include "cyapicallbacks.h"


/*******************************************************************************
* Function Name: I2C_1_I2C_ISR
****************************************************************************//**
*
*  Handles the Interrupt Service Routine for the SCB I2C mode.
*
*******************************************************************************/
CY_ISR(I2C_1_I2C_ISR)
{
    uint32 diffCount;
    uint32 endTransfer;

#ifdef I2C_1_I2C_ISR_ENTRY_CALLBACK
    I2C_1_I2C_ISR_EntryCallback();
#endif /* I2C_1_I2C_ISR_ENTRY_CALLBACK */

#if (I2C_1_I2C_CUSTOM_ADDRESS_HANDLER_CONST)
    uint32 response;

    response = I2C_1_I2C_ACK_ADDR;
#endif /* (I2C_1_I2C_CUSTOM_ADDRESS_HANDLER_CONST) */

    endTransfer = 0u; /* Continue active transfer */

    /* Calls customer routine if registered */
    if(NULL != I2C_1_customIntrHandler)
    {
        I2C_1_customIntrHandler();
    }

    if(I2C_1_CHECK_INTR_I2C_EC_MASKED(I2C_1_INTR_I2C_EC_WAKE_UP))
    {
        /* Mask-off after wakeup */
        I2C_1_SetI2CExtClkInterruptMode(I2C_1_NO_INTR_SOURCES);
    }

    /* Master and Slave error tracking:
    * Add the master state check to track only the master errors when the master is active or
    * track slave errors when the slave is active or idle.
    * A special MMS case: in the address phase with misplaced Start: the master sets the LOST_ARB and
    * slave BUS_ERR. The valid event is LOST_ARB comes from the master.
    */
    if(I2C_1_CHECK_I2C_FSM_MASTER)
    {
        #if(I2C_1_I2C_MASTER)
        {
            /* INTR_MASTER_I2C_BUS_ERROR:
            * A misplaced Start or Stop condition occurred on the bus: complete the transaction.
            * The interrupt is cleared in I2C_FSM_EXIT_IDLE.
            */
            if(I2C_1_CHECK_INTR_MASTER_MASKED(I2C_1_INTR_MASTER_I2C_BUS_ERROR))
            {
                I2C_1_mstrStatus |= (uint16) (I2C_1_I2C_MSTAT_ERR_XFER |
                                                         I2C_1_I2C_MSTAT_ERR_BUS_ERROR);

                endTransfer = I2C_1_I2C_CMPLT_ANY_TRANSFER;
            }

            /* INTR_MASTER_I2C_ARB_LOST:
            * The MultiMaster lost arbitrage during transaction.
            * A Misplaced Start or Stop condition is treated as lost arbitration when the master drives the SDA.
            * The interrupt source is cleared in I2C_FSM_EXIT_IDLE.
            */
            if(I2C_1_CHECK_INTR_MASTER_MASKED(I2C_1_INTR_MASTER_I2C_ARB_LOST))
            {
                I2C_1_mstrStatus |= (uint16) (I2C_1_I2C_MSTAT_ERR_XFER |
                                                         I2C_1_I2C_MSTAT_ERR_ARB_LOST);

                endTransfer = I2C_1_I2C_CMPLT_ANY_TRANSFER;
            }

            #if(I2C_1_I2C_MULTI_MASTER_SLAVE)
            {
                /* I2C_MASTER_CMD_M_START_ON_IDLE:
                * MultiMaster-Slave does not generate start, because Slave was addressed.
                * Pass control to slave.
                */
                if(I2C_1_CHECK_I2C_MASTER_CMD(I2C_1_I2C_MASTER_CMD_M_START_ON_IDLE))
                {
                    I2C_1_mstrStatus |= (uint16) (I2C_1_I2C_MSTAT_ERR_XFER |
                                                             I2C_1_I2C_MSTAT_ERR_ABORT_XFER);

                    endTransfer = I2C_1_I2C_CMPLT_ANY_TRANSFER;
                }
            }
            #endif

            /* The error handling common part:
            * Sets a completion flag of the master transaction and passes control to:
            *  - I2C_FSM_EXIT_IDLE - to complete transaction in case of: ARB_LOST or BUS_ERR.
            *  - I2C_FSM_IDLE      - to take chance for the slave to process incoming transaction.
            */
            if(0u != endTransfer)
            {
                /* Set completion flags for master */
                I2C_1_mstrStatus |= (uint16) I2C_1_GET_I2C_MSTAT_CMPLT;

                #if(I2C_1_I2C_MULTI_MASTER_SLAVE)
                {
                    if(I2C_1_CHECK_I2C_FSM_ADDR)
                    {
                        /* Start generation is set after another master starts accessing Slave.
                        * Clean-up master and turn to slave. Set state to IDLE.
                        */
                        if(I2C_1_CHECK_I2C_MASTER_CMD(I2C_1_I2C_MASTER_CMD_M_START_ON_IDLE))
                        {
                            I2C_1_I2C_MASTER_CLEAR_START;

                            endTransfer = I2C_1_I2C_CMPLT_ANY_TRANSFER; /* Pass control to Slave */
                        }
                        /* Valid arbitration lost on the address phase happens only when: master LOST_ARB is set and
                        * slave BUS_ERR is cleared. Only in that case set the state to IDLE without SCB IP re-enable.
                        */
                        else if((!I2C_1_CHECK_INTR_SLAVE_MASKED(I2C_1_INTR_SLAVE_I2C_BUS_ERROR))
                               && I2C_1_CHECK_INTR_MASTER_MASKED(I2C_1_INTR_MASTER_I2C_ARB_LOST))
                        {
                            endTransfer = I2C_1_I2C_CMPLT_ANY_TRANSFER; /* Pass control to Slave */
                        }
                        else
                        {
                            endTransfer = 0u; /* Causes I2C_FSM_EXIT_IDLE to be set below */
                        }

                        if(0u != endTransfer) /* Clean-up master to proceed with slave */
                        {
                            I2C_1_CLEAR_TX_FIFO; /* Shifter keeps address, clear it */

                            I2C_1_DISABLE_MASTER_AUTO_DATA_ACK; /* In case of reading disable autoACK */

                            /* Clean-up master interrupt sources */
                            I2C_1_ClearMasterInterruptSource(I2C_1_INTR_MASTER_ALL);

                            /* Disable data processing interrupts: they have to be cleared before */
                            I2C_1_SetRxInterruptMode(I2C_1_NO_INTR_SOURCES);
                            I2C_1_SetTxInterruptMode(I2C_1_NO_INTR_SOURCES);

                            I2C_1_state = I2C_1_I2C_FSM_IDLE;
                        }
                        else
                        {
                            /* Set I2C_FSM_EXIT_IDLE for BUS_ERR and ARB_LOST (that is really bus error) */
                            I2C_1_state = I2C_1_I2C_FSM_EXIT_IDLE;
                        }
                    }
                    else
                    {
                        /* Set I2C_FSM_EXIT_IDLE if any other state than address */
                        I2C_1_state = I2C_1_I2C_FSM_EXIT_IDLE;
                    }
                }
                #else
                {
                    /* In case of LOST*/
                    I2C_1_state = I2C_1_I2C_FSM_EXIT_IDLE;
                }
                #endif
            }
        }
        #endif
    }
    else /* (I2C_1_CHECK_I2C_FSM_SLAVE) */
    {
        #if(I2C_1_I2C_SLAVE)
        {
            /* INTR_SLAVE_I2C_BUS_ERROR or I2C_1_INTR_SLAVE_I2C_ARB_LOST:
            * A Misplaced Start or Stop condition occurred on the bus: set a flag
            * to notify an error condition.
            */
            if(I2C_1_CHECK_INTR_SLAVE_MASKED(I2C_1_INTR_SLAVE_I2C_BUS_ERROR |
                                                        I2C_1_INTR_SLAVE_I2C_ARB_LOST))
            {
                if(I2C_1_CHECK_I2C_FSM_RD)
                {
                    /* TX direction: master reads from slave */
                    I2C_1_slStatus &= (uint8) ~I2C_1_I2C_SSTAT_RD_BUSY;
                    I2C_1_slStatus |= (uint8) (I2C_1_I2C_SSTAT_RD_ERR |
                                                          I2C_1_I2C_SSTAT_RD_CMPLT);
                }
                else
                {
                    /* RX direction: master writes into slave */
                    I2C_1_slStatus &= (uint8) ~I2C_1_I2C_SSTAT_WR_BUSY;
                    I2C_1_slStatus |= (uint8) (I2C_1_I2C_SSTAT_WR_ERR |
                                                          I2C_1_I2C_SSTAT_WR_CMPLT);
                }

                I2C_1_state = I2C_1_I2C_FSM_EXIT_IDLE;
            }
        }
        #endif
    }

    /* States description:
    * Any Master operation starts from: the ADDR_RD/WR state as the master generates traffic on the bus.
    * Any Slave operation starts from: the IDLE state as the slave always waits for actions from the master.
    */

    /* FSM Master */
    if(I2C_1_CHECK_I2C_FSM_MASTER)
    {
        #if(I2C_1_I2C_MASTER)
        {
            /* INTR_MASTER_I2C_STOP:
            * A Stop condition was generated by the master: the end of the transaction.
            * Set completion flags to notify the API.
            */
            if(I2C_1_CHECK_INTR_MASTER_MASKED(I2C_1_INTR_MASTER_I2C_STOP))
            {
                I2C_1_ClearMasterInterruptSource(I2C_1_INTR_MASTER_I2C_STOP);

                I2C_1_mstrStatus |= (uint16) I2C_1_GET_I2C_MSTAT_CMPLT;
                I2C_1_state       = I2C_1_I2C_FSM_IDLE;
            }
            else
            {
                if(I2C_1_CHECK_I2C_FSM_ADDR) /* Address stage */
                {
                    /* INTR_MASTER_I2C_NACK:
                    * The master sent an address but it was NACKed by the slave. Complete transaction.
                    */
                    if(I2C_1_CHECK_INTR_MASTER_MASKED(I2C_1_INTR_MASTER_I2C_NACK))
                    {
                        I2C_1_ClearMasterInterruptSource(I2C_1_INTR_MASTER_I2C_NACK);

                        I2C_1_mstrStatus |= (uint16) (I2C_1_I2C_MSTAT_ERR_XFER |
                                                                 I2C_1_I2C_MSTAT_ERR_ADDR_NAK);

                        endTransfer = I2C_1_I2C_CMPLT_ANY_TRANSFER;
                    }
                    /* INTR_TX_UNDERFLOW. The master sent an address:
                    *  - TX direction: the clock is stretched after the ACK phase, because the TX FIFO is
                    *    EMPTY. The TX EMPTY cleans all the TX interrupt sources.
                    *  - RX direction: the 1st byte is received, but there is no ACK permission,
                    *    the clock is stretched after 1 byte is received.
                    */
                    else
                    {
                        if(I2C_1_CHECK_I2C_FSM_RD) /* Reading */
                        {
                            I2C_1_state = I2C_1_I2C_FSM_MSTR_RD_DATA;
                        }
                        else /* Writing */
                        {
                            I2C_1_state = I2C_1_I2C_FSM_MSTR_WR_DATA;
                            if(0u != I2C_1_mstrWrBufSize)
                            {
                                /* Enable INTR.TX_EMPTY if there is data to transmit */
                                I2C_1_SetTxInterruptMode(I2C_1_INTR_TX_EMPTY);
                            }
                        }
                    }
                }

                if(I2C_1_CHECK_I2C_FSM_DATA) /* Data phase */
                {
                    if(I2C_1_CHECK_I2C_FSM_RD) /* Reading */
                    {
                        /* INTR_RX_FULL:
                        * RX direction: the master received 8 bytes.
                        * Get data from RX FIFO and decide whether to ACK or  NACK the following bytes.
                        */
                        if(I2C_1_CHECK_INTR_RX_MASKED(I2C_1_INTR_RX_FULL))
                        {
                            /* Calculate difference */
                            diffCount =  I2C_1_mstrRdBufSize -
                                        (I2C_1_mstrRdBufIndex + I2C_1_GET_RX_FIFO_ENTRIES);

                            /* Proceed transaction or end it when RX FIFO becomes FULL again */
                            if(diffCount > I2C_1_I2C_FIFO_SIZE)
                            {
                                diffCount = I2C_1_I2C_FIFO_SIZE;
                            }
                            else
                            {
                                if(0u == diffCount)
                                {
                                    I2C_1_DISABLE_MASTER_AUTO_DATA_ACK;

                                    diffCount   = I2C_1_I2C_FIFO_SIZE;
                                    endTransfer = I2C_1_I2C_CMPLT_ANY_TRANSFER;
                                }
                            }

                            for(; (0u != diffCount); diffCount--)
                            {
                                I2C_1_mstrRdBufPtr[I2C_1_mstrRdBufIndex] = (uint8)
                                                                                        I2C_1_RX_FIFO_RD_REG;
                                I2C_1_mstrRdBufIndex++;
                            }
                        }
                        /* INTR_RX_NOT_EMPTY:
                        * RX direction: the master received one data byte, ACK or NACK it.
                        * The last byte is stored and NACKed by the master. The NACK and Stop is
                        * generated by one command generate Stop.
                        */
                        else if(I2C_1_CHECK_INTR_RX_MASKED(I2C_1_INTR_RX_NOT_EMPTY))
                        {
                            /* Put data in component buffer */
                            I2C_1_mstrRdBufPtr[I2C_1_mstrRdBufIndex] = (uint8) I2C_1_RX_FIFO_RD_REG;
                            I2C_1_mstrRdBufIndex++;

                            if(I2C_1_mstrRdBufIndex < I2C_1_mstrRdBufSize)
                            {
                                I2C_1_I2C_MASTER_GENERATE_ACK;
                            }
                            else
                            {
                               endTransfer = I2C_1_I2C_CMPLT_ANY_TRANSFER;
                            }
                        }
                        else
                        {
                            /* Do nothing */
                        }

                        I2C_1_ClearRxInterruptSource(I2C_1_INTR_RX_ALL);
                    }
                    else /* Writing */
                    {
                        /* INTR_MASTER_I2C_NACK :
                        * The master writes data to the slave and NACK was received: not all the bytes were
                        * written to the slave from the TX FIFO. Revert the index if there is data in
                        * the TX FIFO and pass control to a complete transfer.
                        */
                        if(I2C_1_CHECK_INTR_MASTER_MASKED(I2C_1_INTR_MASTER_I2C_NACK))
                        {
                            I2C_1_ClearMasterInterruptSource(I2C_1_INTR_MASTER_I2C_NACK);

                            /* Rollback write buffer index: NACKed byte remains in shifter */
                            I2C_1_mstrWrBufIndexTmp -= (I2C_1_GET_TX_FIFO_ENTRIES +
                                                                   I2C_1_GET_TX_FIFO_SR_VALID);

                            /* Update number of transferred bytes */
                            I2C_1_mstrWrBufIndex = I2C_1_mstrWrBufIndexTmp;

                            I2C_1_mstrStatus |= (uint16) (I2C_1_I2C_MSTAT_ERR_XFER |
                                                                     I2C_1_I2C_MSTAT_ERR_SHORT_XFER);

                            I2C_1_CLEAR_TX_FIFO;

                            endTransfer = I2C_1_I2C_CMPLT_ANY_TRANSFER;
                        }
                        /* INTR_TX_EMPTY :
                        * TX direction: the TX FIFO is EMPTY, the data from the buffer needs to be put there.
                        * When there is no data in the component buffer, the underflow interrupt is
                        * enabled to catch when all the data has been transferred.
                        */
                        else if(I2C_1_CHECK_INTR_TX_MASKED(I2C_1_INTR_TX_EMPTY))
                        {
                            while(I2C_1_I2C_FIFO_SIZE != I2C_1_GET_TX_FIFO_ENTRIES)
                            {
                                /* The temporary mstrWrBufIndexTmp is used because slave could NACK the byte and index
                                * roll-back required in this case. The mstrWrBufIndex is updated at the end of transfer.
                                */
                                if(I2C_1_mstrWrBufIndexTmp < I2C_1_mstrWrBufSize)
                                {
                                #if(!I2C_1_CY_SCBIP_V0)
                                   /* Clear INTR_TX.UNDERFLOW before putting the last byte into TX FIFO. This ensures
                                    * a proper trigger at the end of transaction when INTR_TX.UNDERFLOW single trigger
                                    * event. Ticket ID# 156735.
                                    */
                                    if(I2C_1_mstrWrBufIndexTmp == (I2C_1_mstrWrBufSize - 1u))
                                    {
                                        I2C_1_ClearTxInterruptSource(I2C_1_INTR_TX_UNDERFLOW);
                                        I2C_1_SetTxInterruptMode(I2C_1_INTR_TX_UNDERFLOW);
                                    }
                                 #endif /* (!I2C_1_CY_SCBIP_V0) */

                                    /* Put data into TX FIFO */
                                    I2C_1_TX_FIFO_WR_REG = (uint32) I2C_1_mstrWrBufPtr[I2C_1_mstrWrBufIndexTmp];
                                    I2C_1_mstrWrBufIndexTmp++;
                                }
                                else
                                {
                                    break; /* No more data to put */
                                }
                            }

                        #if(I2C_1_CY_SCBIP_V0)
                            if(I2C_1_mstrWrBufIndexTmp == I2C_1_mstrWrBufSize)
                            {
                                I2C_1_SetTxInterruptMode(I2C_1_INTR_TX_UNDERFLOW);
                            }

                            I2C_1_ClearTxInterruptSource(I2C_1_INTR_TX_ALL);
                        #else
                            I2C_1_ClearTxInterruptSource(I2C_1_INTR_TX_EMPTY);
                        #endif /* (I2C_1_CY_SCBIP_V0) */
                        }
                        /* INTR_TX_UNDERFLOW:
                        * TX direction: all data from the TX FIFO was transferred to the slave.
                        * The transaction needs to be completed.
                        */
                        else if(I2C_1_CHECK_INTR_TX_MASKED(I2C_1_INTR_TX_UNDERFLOW))
                        {
                            /* Update number of transferred bytes */
                            I2C_1_mstrWrBufIndex = I2C_1_mstrWrBufIndexTmp;

                            endTransfer = I2C_1_I2C_CMPLT_ANY_TRANSFER;
                        }
                        else
                        {
                            /* Do nothing */
                        }
                    }
                }

                if(0u != endTransfer) /* Complete transfer */
                {
                    /* Clean-up master after reading: only in case of NACK */
                    I2C_1_DISABLE_MASTER_AUTO_DATA_ACK;

                    /* Disable data processing interrupts: they have to be cleared before */
                    I2C_1_SetRxInterruptMode(I2C_1_NO_INTR_SOURCES);
                    I2C_1_SetTxInterruptMode(I2C_1_NO_INTR_SOURCES);

                    if(I2C_1_CHECK_I2C_MODE_NO_STOP(I2C_1_mstrControl))
                    {
                        /* On-going transaction is suspended: the ReStart is generated by the API request */
                        I2C_1_mstrStatus |= (uint16) (I2C_1_I2C_MSTAT_XFER_HALT |
                                                                 I2C_1_GET_I2C_MSTAT_CMPLT);

                        I2C_1_state = I2C_1_I2C_FSM_MSTR_HALT;
                    }
                    else
                    {
                        /* Complete transaction: exclude the data processing state and generate Stop.
                        * The completion status will be set after Stop generation.
                        * A special case is read: because NACK and Stop are generated by the command below.
                        * Lost arbitration can occur during NACK generation when
                        * the other master is still reading from the slave.
                        */
                        I2C_1_I2C_MASTER_GENERATE_STOP;
                    }
                }
            }

        } /* (I2C_1_I2C_MASTER) */
        #endif

    } /* (I2C_1_CHECK_I2C_FSM_MASTER) */


    /* FSM Slave */
    else if(I2C_1_CHECK_I2C_FSM_SLAVE)
    {
        #if(I2C_1_I2C_SLAVE)
        {
            /* INTR_SLAVE_NACK:
            * The master completes reading the slave: the appropriate flags have to be set.
            * The TX FIFO is cleared after an overflow condition is set.
            */
            if(I2C_1_CHECK_INTR_SLAVE_MASKED(I2C_1_INTR_SLAVE_I2C_NACK))
            {
                I2C_1_ClearSlaveInterruptSource(I2C_1_INTR_SLAVE_I2C_NACK);

                /* All entries that remain in TX FIFO are: FIFO Size + 1 (SHIFTER) */
                diffCount = (I2C_1_GET_TX_FIFO_ENTRIES + I2C_1_GET_TX_FIFO_SR_VALID);

                if(I2C_1_slOverFlowCount > diffCount) /* Overflow */
                {
                    I2C_1_slStatus |= (uint8) I2C_1_I2C_SSTAT_RD_OVFL;
                }
                else /* No Overflow */
                {
                    /* Roll-back temporary index */
                    I2C_1_slRdBufIndexTmp -= (diffCount - I2C_1_slOverFlowCount);
                }

                /* Update slave of transferred bytes */
                I2C_1_slRdBufIndex = I2C_1_slRdBufIndexTmp;

                /* Clean-up TX FIFO */
                I2C_1_SetTxInterruptMode(I2C_1_NO_INTR_SOURCES);
                I2C_1_slOverFlowCount = 0u;
                I2C_1_CLEAR_TX_FIFO;

                /* Complete master reading */
                I2C_1_slStatus &= (uint8) ~I2C_1_I2C_SSTAT_RD_BUSY;
                I2C_1_slStatus |= (uint8)  I2C_1_I2C_SSTAT_RD_CMPLT;
                I2C_1_state     =  I2C_1_I2C_FSM_IDLE;
                
            #ifdef I2C_1_I2C_SLAVE_CMPLT_CALLBACK
                /* Read complete */
                I2C_1_I2C_SlaveCompleteCallback();
            #endif /* I2C_1_I2C_SLAVE_CMPLT_CALLBACK */
            }


            /* INTR_SLAVE_I2C_WRITE_STOP:
            * The master completes writing to the slave: the appropriate flags have to be set.
            * The RX FIFO contains 1-8 bytes from the previous transaction which needs to be read.
            * There is a possibility that RX FIFO contains an address, it needs to leave it there.
            */
            if(I2C_1_CHECK_INTR_SLAVE_MASKED(I2C_1_INTR_SLAVE_I2C_WRITE_STOP))
            {
                I2C_1_ClearSlaveInterruptSource(I2C_1_INTR_SLAVE_I2C_WRITE_STOP);

                /* Read bytes from RX FIFO when auto data ACK receive logic is enabled. Otherwise all data bytes
                * were already read from the RX FIFO except for address byte which has to stay here to be handled by
                * I2C_ADDR_MATCH.
                */
                if (0u != (I2C_1_I2C_CTRL_REG & I2C_1_I2C_CTRL_S_READY_DATA_ACK))
                {
                    while(0u != I2C_1_GET_RX_FIFO_ENTRIES)
                    {
                        #if(I2C_1_CHECK_I2C_ACCEPT_ADDRESS)
                        {
                            if((1u == I2C_1_GET_RX_FIFO_ENTRIES) &&
                               (I2C_1_CHECK_INTR_SLAVE_MASKED(I2C_1_INTR_SLAVE_I2C_ADDR_MATCH)))
                            {
                                break; /* Leave address in RX FIFO */
                            }
                        }
                        #endif

                        /* Put data in component buffer */
                        I2C_1_slWrBufPtr[I2C_1_slWrBufIndex] = (uint8) I2C_1_RX_FIFO_RD_REG;
                        I2C_1_slWrBufIndex++;
                    }

                    I2C_1_DISABLE_SLAVE_AUTO_DATA;
                }

                if(I2C_1_CHECK_INTR_RX(I2C_1_INTR_RX_OVERFLOW))
                {
                    I2C_1_slStatus |= (uint8) I2C_1_I2C_SSTAT_WR_OVFL;
                }

                /* Clears RX interrupt sources triggered on data receiving */
                I2C_1_SetRxInterruptMode(I2C_1_NO_INTR_SOURCES);
                I2C_1_ClearRxInterruptSource(I2C_1_INTR_RX_ALL);

                /* Complete master writing */
                I2C_1_slStatus &= (uint8) ~I2C_1_I2C_SSTAT_WR_BUSY;
                I2C_1_slStatus |= (uint8)  I2C_1_I2C_SSTAT_WR_CMPLT;
                I2C_1_state     =  I2C_1_I2C_FSM_IDLE;

            #ifdef I2C_1_I2C_SLAVE_CMPLT_CALLBACK
                /* Write complete */
                I2C_1_I2C_SlaveCompleteCallback();
            #endif /* I2C_1_I2C_SLAVE_CMPLT_CALLBACK */
            }


            /* INTR_SLAVE_I2C_ADDR_MATCH or INTR_SLAVE_I2C_GENERAL:
            * The address match or general call address event starts the slave operation:
            * after leaving the TX or RX direction has to be chosen.
            * The wakeup interrupt must be cleared only after an address match is set.
            */
        #if (I2C_1_I2C_CUSTOM_ADDRESS_HANDLER_CONST)
            if (I2C_1_CHECK_INTR_SLAVE_MASKED(I2C_1_INTR_SLAVE_I2C_ADDR_MATCH |
                                                         I2C_1_INTR_SLAVE_I2C_GENERAL))
        #else
            if (I2C_1_CHECK_INTR_SLAVE_MASKED(I2C_1_INTR_SLAVE_I2C_ADDR_MATCH))
        #endif /* (I2C_1_I2C_CUSTOM_ADDRESS_HANDLER_CONST) */
            {
                /* Clear externally clocked address match interrupt source when internally clocked is set */
                I2C_1_ClearI2CExtClkInterruptSource(I2C_1_INTR_I2C_EC_WAKE_UP);

                #if (I2C_1_I2C_CUSTOM_ADDRESS_HANDLER)
                {
                    if (NULL != I2C_1_customAddressHandler)
                    {
                        /* Call custom address handler */
                        response = I2C_1_customAddressHandler();
                    }
                    else
                    {
                        /* Read address from the RX FIFO. If there is no address underflow triggers but
                        * component does not use that source. */
                        (void) I2C_1_RX_FIFO_RD_REG;
                        response = I2C_1_I2C_ACK_ADDR;
                    }

                    /* Clears RX sources after address was received in the RX FIFO */
                    I2C_1_ClearRxInterruptSource(I2C_1_INTR_RX_ALL);
                }
                #endif

            #if (I2C_1_I2C_CUSTOM_ADDRESS_HANDLER_CONST)
                if (response == I2C_1_I2C_NAK_ADDR)
                {
                #if (!I2C_1_CY_SCBIP_V0)
                    /* Disable write stop interrupt source as it triggers after address was NACKed. Ticket ID#156094 */
                    I2C_1_DISABLE_INTR_SLAVE(I2C_1_INTR_SLAVE_I2C_WRITE_STOP);
                #endif /* (!I2C_1_CY_SCBIP_V0) */

                    /* Clear address match and stop history */
                    I2C_1_ClearSlaveInterruptSource(I2C_1_INTR_SLAVE_ALL);

                    /* ACK the address byte */
                    I2C_1_I2C_SLAVE_GENERATE_NACK;
                }
                else
            #endif /* (I2C_1_I2C_CUSTOM_ADDRESS_HANDLER_CONST) */
                {
                    if(I2C_1_CHECK_I2C_STATUS(I2C_1_I2C_STATUS_S_READ))
                    /* TX direction: master reads from slave */
                    {
                        I2C_1_SetTxInterruptMode(I2C_1_INTR_TX_EMPTY);

                        /* Set temporary index to address buffer clear from API */
                        I2C_1_slRdBufIndexTmp = I2C_1_slRdBufIndex;

                        /* Start master reading */
                        I2C_1_slStatus |= (uint8) I2C_1_I2C_SSTAT_RD_BUSY;
                        I2C_1_state     = I2C_1_I2C_FSM_SL_RD;
                    }
                    else
                    /* RX direction: master writes into slave */
                    {
                        /* Calculate available buffer size */
                        diffCount = (I2C_1_slWrBufSize - I2C_1_slWrBufIndex);

                    #if (I2C_1_CY_SCBIP_V0)
                        if(diffCount < I2C_1_I2C_FIFO_SIZE)
                        /* Receive data: byte-by-byte */
                        {
                            I2C_1_SetRxInterruptMode(I2C_1_INTR_RX_NOT_EMPTY);
                        }
                        else
                        /* Receive data: into RX FIFO */
                        {
                            if(diffCount == I2C_1_I2C_FIFO_SIZE)
                            {
                                /* NACK when RX FIFO become FULL */
                                I2C_1_ENABLE_SLAVE_AUTO_DATA;
                            }
                            else
                            {
                                /* Stretch clock when RX FIFO becomes FULL */
                                I2C_1_ENABLE_SLAVE_AUTO_DATA_ACK;
                                I2C_1_SetRxInterruptMode(I2C_1_INTR_RX_FULL);
                            }
                        }

                    #else
                        #if(I2C_1_CHECK_I2C_ACCEPT_ADDRESS)
                        {
                            /* Enable RX.NOT_EMPTY interrupt source to receive byte by byte.
                            * The byte by byte receive is always chosen for the case when an address is accepted
                            * in RX FIFO. Ticket ID#175559.
                            */
                            I2C_1_SetRxInterruptMode(I2C_1_INTR_RX_NOT_EMPTY);
                        }
                        #else
                        {
                            if(diffCount < I2C_1_I2C_FIFO_SIZE)
                            /* Receive data: byte-by-byte */
                            {
                                I2C_1_SetRxInterruptMode(I2C_1_INTR_RX_NOT_EMPTY);
                            }
                            else
                            /* Receive data: into RX FIFO */
                            {
                                if(diffCount == I2C_1_I2C_FIFO_SIZE)
                                {
                                    /* NACK when RX FIFO become FULL */
                                    I2C_1_ENABLE_SLAVE_AUTO_DATA;
                                }
                                else
                                {
                                    /* Stretch clock when RX FIFO becomes FULL */
                                    I2C_1_ENABLE_SLAVE_AUTO_DATA_ACK;
                                    I2C_1_SetRxInterruptMode(I2C_1_INTR_RX_FULL);
                                }
                            }
                        }
                        #endif
                    #endif /* (I2C_1_CY_SCBIP_V0) */

                        /* Start master reading */
                        I2C_1_slStatus |= (uint8) I2C_1_I2C_SSTAT_WR_BUSY;
                        I2C_1_state     = I2C_1_I2C_FSM_SL_WR;
                    }

                    /* Clear address match and stop history */
                    I2C_1_ClearSlaveInterruptSource(I2C_1_INTR_SLAVE_ALL);

                #if (!I2C_1_CY_SCBIP_V0)
                    /* Enable write stop interrupt source as it triggers after address was NACKed. Ticket ID#156094 */
                    I2C_1_ENABLE_INTR_SLAVE(I2C_1_INTR_SLAVE_I2C_WRITE_STOP);
                #endif /* (!I2C_1_CY_SCBIP_V0) */

                    /* ACK the address byte */
                    I2C_1_I2C_SLAVE_GENERATE_ACK;
                }
            }

            /* I2C_1_INTR_RX_FULL:
            * Get data from the RX FIFO and decide whether to ACK or NACK the following bytes
            */
            if(I2C_1_CHECK_INTR_RX_MASKED(I2C_1_INTR_RX_FULL))
            {
                /* Calculate available buffer size to take into account that RX FIFO is FULL */
                diffCount =  I2C_1_slWrBufSize -
                            (I2C_1_slWrBufIndex + I2C_1_I2C_FIFO_SIZE);

                if(diffCount > I2C_1_I2C_FIFO_SIZE) /* Proceed transaction */
                {
                    diffCount   = I2C_1_I2C_FIFO_SIZE;
                    endTransfer = 0u;  /* Continue active transfer */
                }
                else /* End when FIFO becomes FULL again */
                {
                    endTransfer = I2C_1_I2C_CMPLT_ANY_TRANSFER;
                }

                for(; (0u != diffCount); diffCount--)
                {
                    /* Put data in component buffer */
                    I2C_1_slWrBufPtr[I2C_1_slWrBufIndex] = (uint8) I2C_1_RX_FIFO_RD_REG;
                    I2C_1_slWrBufIndex++;
                }

                if(0u != endTransfer) /* End transfer sending NACK */
                {
                    I2C_1_ENABLE_SLAVE_AUTO_DATA_NACK;

                    /* INTR_RX_FULL triggers earlier than INTR_SLAVE_I2C_STOP:
                    * disable all RX interrupt sources.
                    */
                    I2C_1_SetRxInterruptMode(I2C_1_NO_INTR_SOURCES);
                }

                I2C_1_ClearRxInterruptSource(I2C_1_INTR_RX_FULL);
            }
            /* I2C_1_INTR_RX_NOT_EMPTY:
            * The buffer size is less than 8: it requires processing in byte-by-byte mode.
            */
            else if(I2C_1_CHECK_INTR_RX_MASKED(I2C_1_INTR_RX_NOT_EMPTY))
            {
                diffCount = I2C_1_RX_FIFO_RD_REG;

                if(I2C_1_slWrBufIndex < I2C_1_slWrBufSize)
                {
                    I2C_1_I2C_SLAVE_GENERATE_ACK;

                    /* Put data into component buffer */
                    I2C_1_slWrBufPtr[I2C_1_slWrBufIndex] = (uint8) diffCount;
                    I2C_1_slWrBufIndex++;
                }
                else /* Overflow: there is no space in write buffer */
                {
                    I2C_1_I2C_SLAVE_GENERATE_NACK;

                    I2C_1_slStatus |= (uint8) I2C_1_I2C_SSTAT_WR_OVFL;
                }

                I2C_1_ClearRxInterruptSource(I2C_1_INTR_RX_NOT_EMPTY);
            }
            else
            {
                /* Does nothing */
            }


            /* I2C_1_INTR_TX_EMPTY:
            * The master reads the slave: provide data to read or 0xFF in the case of the end of the buffer
            * The overflow condition must be captured, but not set until the end of transaction.
            * There is a possibility of a false overflow due to TX FIFO utilization.
            */
            if(I2C_1_CHECK_INTR_TX_MASKED(I2C_1_INTR_TX_EMPTY))
            {
                while(I2C_1_I2C_FIFO_SIZE != I2C_1_GET_TX_FIFO_ENTRIES)
                {
                    /* Temporary slRdBufIndexTmp is used because the master can NACK the byte and
                    * index roll-back is required in this case. The slRdBufIndex is updated at the end
                    * of the read transfer.
                    */
                    if(I2C_1_slRdBufIndexTmp < I2C_1_slRdBufSize)
                    /* Data from buffer */
                    {
                        I2C_1_TX_FIFO_WR_REG = (uint32) I2C_1_slRdBufPtr[I2C_1_slRdBufIndexTmp];
                        I2C_1_slRdBufIndexTmp++;
                    }
                    else
                    /* Probably Overflow */
                    {
                        I2C_1_TX_FIFO_WR_REG = I2C_1_I2C_SLAVE_OVFL_RETURN;

                        if(I2C_1_slOverFlowCount <= I2C_1_I2C_TX_OVERFLOW_COUNT)
                        {
                            /* Get counter in range of overflow. */
                            I2C_1_slOverFlowCount++;
                        }
                    }
                }

                I2C_1_ClearTxInterruptSource(I2C_1_INTR_TX_EMPTY);
            }

        }  /* (I2C_1_I2C_SLAVE) */
        #endif
    }


    /* FSM EXIT:
    * Slave:  INTR_SLAVE_I2C_BUS_ERROR, INTR_SLAVE_I2C_ARB_LOST
    * Master: INTR_MASTER_I2C_BUS_ERROR, INTR_MASTER_I2C_ARB_LOST.
    */
    else
    {
        I2C_1_I2CFwBlockReset();
        
    #ifdef I2C_1_I2C_SLAVE_CMPLT_CALLBACK
        #if(I2C_1_I2C_SLAVE)
        {
            /* Error condition: read or write complete is set */
            I2C_1_I2C_SlaveCompleteCallback();
        }
        #endif
    #endif /* I2C_1_I2C_SLAVE_CMPLT_CALLBACK */
    }

#ifdef I2C_1_I2C_ISR_EXIT_CALLBACK
    I2C_1_I2C_ISR_ExitCallback();
#endif /* I2C_1_I2C_ISR_EXIT_CALLBACK */

}


/* [] END OF FILE */
