/***************************************************************************//**
* \file I2C_1_I2C_MASTER.c
* \version 4.0
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  I2C Master mode.
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

#include "I2C_1_PVT.h"
#include "I2C_1_I2C_PVT.h"

#if(I2C_1_I2C_MASTER_CONST)

/***************************************
*      I2C Master Private Vars
***************************************/

/* Master variables */
volatile uint16 I2C_1_mstrStatus;      /* Master Status byte  */
volatile uint8  I2C_1_mstrControl;     /* Master Control byte */

/* Receive buffer variables */
volatile uint8 * I2C_1_mstrRdBufPtr;   /* Pointer to Master Read buffer */
volatile uint32  I2C_1_mstrRdBufSize;  /* Master Read buffer size       */
volatile uint32  I2C_1_mstrRdBufIndex; /* Master Read buffer Index      */

/* Transmit buffer variables */
volatile uint8 * I2C_1_mstrWrBufPtr;   /* Pointer to Master Write buffer */
volatile uint32  I2C_1_mstrWrBufSize;  /* Master Write buffer size       */
volatile uint32  I2C_1_mstrWrBufIndex; /* Master Write buffer Index      */
volatile uint32  I2C_1_mstrWrBufIndexTmp; /* Master Write buffer Index Tmp */

#if (!I2C_1_CY_SCBIP_V0 && \
    I2C_1_I2C_MULTI_MASTER_SLAVE_CONST && I2C_1_I2C_WAKE_ENABLE_CONST)
    static void I2C_1_I2CMasterDisableEcAm(void);
#else
    #define I2C_1_I2CMasterDisableEcAm()     do{ /* Empty */ }while(0)
#endif /* (!I2C_1_CY_SCBIP_V0) */

static uint32 I2C_1_I2CMasterHandleStatus(uint32 status);
static uint32 I2C_1_I2CMasterWaitOneUnit(uint32 *timeout);


/*******************************************************************************
* Function Name: I2C_1_I2CMasterWriteBuf
****************************************************************************//**
*
*  Automatically writes an entire buffer of data to a slave device.
*  Once the data transfer is initiated by this function, further data transfer
*  is handled by the included ISR.
*  Enables the I2C interrupt and clears I2C_1_I2C_MSTAT_WR_CMPLT
*  status.
*
*  \param slaveAddr: 7-bit slave address.
*  \param xferData: Pointer to buffer of data to be sent.
*  \param cnt: Size of buffer to send.
*  \param mode: Transfer mode defines:
*  (1) Whether a start or restart condition is generated at the beginning
*  of the transfer, and
*  (2) Whether the transfer is completed or halted before the stop
*  condition is generated on the bus.Transfer mode, mode constants
*  may be ORed together.
*  - I2C_1_I2C_MODE_COMPLETE_XFER - Perform complete transfer
*    from Start to Stop.
*  - I2C_1_I2C_MODE_REPEAT_START - Send Repeat Start instead
*    of Start. A Stop is generated after transfer is completed unless
*    NO_STOP is specified.
*  - I2C_1_I2C_MODE_NO_STOP Execute transfer without a Stop.
*    The following transfer expected to perform ReStart.
*
* \return
*  Error status.
*  - I2C_1_I2C_MSTR_NO_ERROR - Function complete without error.
*    The master started the transfer.
*  - I2C_1_I2C_MSTR_BUS_BUSY - Bus is busy. Nothing was sent on
*    the bus. The attempt has to be retried.
*  - I2C_1_I2C_MSTR_NOT_READY - Master is not ready for to start
*    transfer. A master still has not completed previous transaction or a
*    slave operation is in progress (in multi-master-slave configuration).
*    Nothing was sent on the bus. The attempt has to be retried.
*
* \globalvars
*  I2C_1_mstrStatus  - used to store current status of I2C Master.
*  I2C_1_state       - used to store current state of software FSM.
*  I2C_1_mstrControl - used to control master end of transaction with
*  or without the Stop generation.
*  I2C_1_mstrWrBufPtr - used to store pointer to master write buffer.
*  I2C_1_mstrWrBufIndex - used to current index within master write
*  buffer.
*  I2C_1_mstrWrBufSize - used to store master write buffer size.
*
*******************************************************************************/
uint32 I2C_1_I2CMasterWriteBuf(uint32 slaveAddress, uint8 * wrData, uint32 cnt, uint32 mode)
{
    uint32 errStatus;

    errStatus = I2C_1_I2C_MSTR_NOT_READY;

    if(NULL != wrData)  /* Check buffer pointer */
    {
        /* Check FSM state and bus before generating Start/ReStart condition */
        if(I2C_1_CHECK_I2C_FSM_IDLE)
        {
            I2C_1_DisableInt();  /* Lock from interruption */

            /* Check bus state */
            errStatus = I2C_1_CHECK_I2C_STATUS(I2C_1_I2C_STATUS_BUS_BUSY) ?
                            I2C_1_I2C_MSTR_BUS_BUSY : I2C_1_I2C_MSTR_NO_ERROR;
        }
        else if(I2C_1_CHECK_I2C_FSM_HALT)
        {
            I2C_1_mstrStatus &= (uint16) ~I2C_1_I2C_MSTAT_XFER_HALT;
                              errStatus  = I2C_1_I2C_MSTR_NO_ERROR;
        }
        else
        {
            /* Unexpected FSM state: exit */
        }
    }

    /* Check if master is ready to start  */
    if(I2C_1_I2C_MSTR_NO_ERROR == errStatus) /* No error proceed */
    {
        /* Non-empty for master-slave mode when wakeup enabled */
        I2C_1_I2CMasterDisableEcAm();

        /* Set up write transaction */
        I2C_1_state = I2C_1_I2C_FSM_MSTR_WR_ADDR;
        I2C_1_mstrWrBufIndexTmp = 0u;
        I2C_1_mstrWrBufIndex    = 0u;
        I2C_1_mstrWrBufSize     = cnt;
        I2C_1_mstrWrBufPtr      = (volatile uint8 *) wrData;
        I2C_1_mstrControl       = (uint8) mode;

        slaveAddress = I2C_1_GET_I2C_8BIT_ADDRESS(slaveAddress);

        I2C_1_mstrStatus &= (uint16) ~I2C_1_I2C_MSTAT_WR_CMPLT;

        I2C_1_ClearTxInterruptSource    (I2C_1_INTR_TX_UNDERFLOW);
        I2C_1_ClearMasterInterruptSource(I2C_1_INTR_MASTER_ALL);
        I2C_1_SetMasterInterruptMode    (I2C_1_I2C_INTR_MASTER_MASK);

        /* The TX and RX FIFO have to be EMPTY */

        /* Enable interrupt source to catch when address is sent */
        I2C_1_SetTxInterruptMode(I2C_1_INTR_TX_UNDERFLOW);

        /* Generate Start or ReStart */
        if(I2C_1_CHECK_I2C_MODE_RESTART(mode))
        {
            I2C_1_I2C_MASTER_GENERATE_RESTART;
            I2C_1_TX_FIFO_WR_REG = slaveAddress;
        }
        else
        {
            I2C_1_TX_FIFO_WR_REG = slaveAddress;
            I2C_1_I2C_MASTER_GENERATE_START;
        }
    }

    I2C_1_EnableInt();   /* Release lock */

    return(errStatus);
}


/*******************************************************************************
* Function Name: I2C_1_I2CMasterReadBuf
****************************************************************************//**
*
*  Automatically reads an entire buffer of data from a slave device.
*  Once the data transfer is initiated by this function, further data transfer
*  is handled by the included ISR.
*  Enables the I2C interrupt and clears I2C_1_I2C_MSTAT_RD_CMPLT
*  status.
*
*  \param slaveAddr: 7-bit slave address.
*  \param xferData: Pointer to buffer of data to be sent.
*  \param cnt: Size of buffer to send.
*  \param mode: Transfer mode defines:
*  (1) Whether a start or restart condition is generated at the beginning
*  of the transfer, and
*  (2) Whether the transfer is completed or halted before the stop
*  condition is generated on the bus.Transfer mode, mode constants may
*  be ORed together. See I2C_1_I2CMasterWriteBuf()
*  function for constants.
*
* \return
*  Error status.See I2C_1_I2CMasterWriteBuf()
*  function for constants.
*
* \globalvars
*  I2C_1_mstrStatus  - used to store current status of I2C Master.
*  I2C_1_state       - used to store current state of software FSM.
*  I2C_1_mstrControl - used to control master end of transaction with
*  or without the Stop generation.
*  I2C_1_mstrRdBufPtr - used to store pointer to master read buffer.
*  I2C_1_mstrRdBufIndex - used to current index within master read
*  buffer.
*  I2C_1_mstrRdBufSize - used to store master read buffer size.
*
*******************************************************************************/
uint32 I2C_1_I2CMasterReadBuf(uint32 slaveAddress, uint8 * rdData, uint32 cnt, uint32 mode)
{
    uint32 errStatus;

    errStatus = I2C_1_I2C_MSTR_NOT_READY;

    if(NULL != rdData)
    {
        /* Check FSM state and bus before generating Start/ReStart condition */
        if(I2C_1_CHECK_I2C_FSM_IDLE)
        {
            I2C_1_DisableInt();  /* Lock from interruption */

            /* Check bus state */
            errStatus = I2C_1_CHECK_I2C_STATUS(I2C_1_I2C_STATUS_BUS_BUSY) ?
                            I2C_1_I2C_MSTR_BUS_BUSY : I2C_1_I2C_MSTR_NO_ERROR;
        }
        else if(I2C_1_CHECK_I2C_FSM_HALT)
        {
            I2C_1_mstrStatus &= (uint16) ~I2C_1_I2C_MSTAT_XFER_HALT;
                              errStatus  =  I2C_1_I2C_MSTR_NO_ERROR;
        }
        else
        {
            /* Unexpected FSM state: exit */
        }
    }

    /* Check master ready to proceed */
    if(I2C_1_I2C_MSTR_NO_ERROR == errStatus) /* No error proceed */
    {
        /* Non-empty for master-slave mode when wakeup enabled */
        I2C_1_I2CMasterDisableEcAm();

        /* Set up read transaction */
        I2C_1_state = I2C_1_I2C_FSM_MSTR_RD_ADDR;
        I2C_1_mstrRdBufIndex = 0u;
        I2C_1_mstrRdBufSize  = cnt;
        I2C_1_mstrRdBufPtr   = (volatile uint8 *) rdData;
        I2C_1_mstrControl    = (uint8) mode;

        slaveAddress = (I2C_1_GET_I2C_8BIT_ADDRESS(slaveAddress) | I2C_1_I2C_READ_FLAG);

        I2C_1_mstrStatus &= (uint16) ~I2C_1_I2C_MSTAT_RD_CMPLT;

        I2C_1_ClearMasterInterruptSource(I2C_1_INTR_MASTER_ALL);
        I2C_1_SetMasterInterruptMode    (I2C_1_I2C_INTR_MASTER_MASK);

        /* TX and RX FIFO have to be EMPTY */

        /* Prepare reading */
        if(I2C_1_mstrRdBufSize < I2C_1_I2C_FIFO_SIZE)
        {
            /* Reading byte-by-byte */
            I2C_1_SetRxInterruptMode(I2C_1_INTR_RX_NOT_EMPTY);
        }
        else
        {
            /* Receive RX FIFO chunks */
            I2C_1_ENABLE_MASTER_AUTO_DATA_ACK;
            I2C_1_SetRxInterruptMode(I2C_1_INTR_RX_FULL);
        }

        /* Generate Start or ReStart */
        if(I2C_1_CHECK_I2C_MODE_RESTART(mode))
        {
            I2C_1_I2C_MASTER_GENERATE_RESTART;
            I2C_1_TX_FIFO_WR_REG = slaveAddress;
        }
        else
        {
            I2C_1_TX_FIFO_WR_REG = slaveAddress;
            I2C_1_I2C_MASTER_GENERATE_START;
        }
    }

    I2C_1_EnableInt();   /* Release lock */

    return(errStatus);
}


/*******************************************************************************
* Function Name: I2C_1_I2CMasterSendStart
****************************************************************************//**
*
* Generates Start condition and sends slave address with read/write bit.
* Disables the I2C interrupt.
* This function is blocking and does not return until start condition and
* address byte are sent and ACK/NACK response is received or errors occurred.
*
* \param slaveAddress
* Right justified 7-bit Slave address (valid range 8 to 120).
*
* \param bitRnW
* Direction of the following transfer. It is defined by read/write bit within
* address byte.
*  - I2C_1_I2C_WRITE_XFER_MODE - Set write direction for the
*    following transfer.
*  - I2C_1_I2C_READ_XFER_MODE - Set read direction for the
*    following transfer.
*
* \param timeoutMs
* Defines in milliseconds the time that this function can block for.
* If that time expires, the function returns. If a zero is passed,
* the function waits forever for the action to complete. If a timeout occurs,
* the SCB block is reset. Note The maximum value is (maximum uint32)/1000.
*
* \return
* Error status.
*  - I2C_1_I2C_MSTR_NO_ERROR - Function complete without error.
*  - I2C_1_I2C_MSTR_BUS_BUSY - Bus is busy.
*    Nothing was sent on the bus. The attempt has to be retried.
*  - I2C_1_I2C_MSTR_NOT_READY - Master is not ready for to
*    start transfer.
*    A master still has not completed previous transaction or a slave
*    operation is in progress (in multi-master-slave configuration).
*    Nothing was sent on the bus. The attempt has to be retried.
*  - I2C_1_I2C_MSTR_ERR_LB_NAK - Error condition: Last byte was
*    NAKed.
*  - I2C_1_I2C_MSTR_ERR_ARB_LOST - Error condition: Master lost
*    arbitration.
*  - I2C_1_I2C_MSTR_ERR_BUS_ERR - Error condition: Master
*    encountered a bus error. Bus error is misplaced start or stop detection.
*  - I2C_1_I2C_MSTR_ERR_ABORT_START - Error condition: The start
*    condition generation was aborted due to beginning of Slave operation.
*    This error condition is only applicable for Multi-Master-Slave mode.
*
* \globalvars
* I2C_1_state - used to store current state of software FSM.
*
*******************************************************************************/
uint32 I2C_1_I2CMasterSendStart(uint32 slaveAddress, uint32 bitRnW, uint32 timeoutMs)
{
    uint32 errStatus = I2C_1_I2C_MSTR_NOT_READY;

    if(I2C_1_CHECK_I2C_FSM_IDLE)
    {
        I2C_1_DisableInt();  /* Lock from interruption */

        if(I2C_1_CHECK_I2C_STATUS(I2C_1_I2C_STATUS_BUS_BUSY))
        {
            errStatus = I2C_1_I2C_MSTR_BUS_BUSY;
        }
        else
        {
            uint32 locStatus;
            uint32 timeout = I2C_1_I2C_CONVERT_TIMEOUT_TO_US(timeoutMs);

            /* Non-empty for master-slave mode when wakeup enabled */
            I2C_1_I2CMasterDisableEcAm();

            /* Clean up the hardware before a transaction. */
            I2C_1_SetMasterInterruptMode    (I2C_1_NO_INTR_SOURCES);
            I2C_1_ClearMasterInterruptSource(I2C_1_INTR_MASTER_ALL);
            I2C_1_ClearRxInterruptSource    (I2C_1_INTR_RX_ALL);

            slaveAddress = I2C_1_GET_I2C_8BIT_ADDRESS(slaveAddress);

            if(0u == bitRnW) /* Write direction */
            {
                I2C_1_state = I2C_1_I2C_FSM_MSTR_WR_DATA;
            }
            else             /* Read direction */
            {
                I2C_1_state = I2C_1_I2C_FSM_MSTR_RD_DATA;
                         slaveAddress |= I2C_1_I2C_READ_FLAG;
            }

            /* TX and RX FIFO have to be empty at here */

             /* Generate Start and send address */
            I2C_1_TX_FIFO_WR_REG = slaveAddress;
            I2C_1_I2C_MASTER_GENERATE_START;

            /* Wait until address has been transferred. Note that for master reads at least one byte. */
            do
            {
                locStatus  = I2C_1_GetMasterInterruptSource() & I2C_1_INTR_MASTER_SEND_BYTE;
                locStatus |= I2C_1_GetSlaveInterruptSource() & I2C_1_SLAVE_INTR_I2C_ADDR;
                locStatus |= I2C_1_I2CMasterWaitOneUnit(&timeout);
            }
            while (0u == locStatus);

            /* Handle completion event and convert to status */
            errStatus = I2C_1_I2CMasterHandleStatus(locStatus);
        }

        I2C_1_EnableInt();  /* Release lock */
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: I2C_1_I2CMasterSendRestart
****************************************************************************//**
*
* Generates Restart condition and sends slave address with read/write bit.
* This function is blocking and does not return until start condition and
* address are sent and ACK/NACK response is received or errors occurred.
*
* \param slaveAddress
* Right justified 7-bit Slave address (valid range 8 to 120).
*
* \param bitRnW
* Direction of the following transfer. It is defined by read/write bit within
* address byte.See I2C_1_I2CMasterSendStart() function for constants.
*
* \param timeoutMs
* Defines in milliseconds the time that this function can block for.
* If that time expires, the function returns. If a zero is passed,
* the function waits forever for the action to complete. If a timeout occurs,
* the SCB block is reset. Note The maximum value is (maximum uint32)/1000.
*
* \return
* Error status.
* See I2C_1_I2CMasterSendStart() function for constants.
*
* \sideeffect
* A valid Start or ReStart condition must be generated before calling
* this function. This function does nothing if Start or ReStart conditions
* failed before this function was called.
* For read transaction, at least one byte has to be read before ReStart
* generation.
*
* \globalvars
* I2C_1_state - used to store current state of software FSM.
*
*******************************************************************************/
uint32 I2C_1_I2CMasterSendRestart(uint32 slaveAddress, uint32 bitRnW, uint32 timeoutMs)
{
    uint32 errStatus = I2C_1_I2C_MSTR_NOT_READY;

    /* Check FSM state before generating ReStart condition */
    if(I2C_1_CHECK_I2C_MASTER_ACTIVE)
    {
        uint32 locStatus;
        uint32 timeout = I2C_1_I2C_CONVERT_TIMEOUT_TO_US(timeoutMs);

        slaveAddress = I2C_1_GET_I2C_8BIT_ADDRESS(slaveAddress);

        if(0u == bitRnW) /* Write direction */
        {
            I2C_1_state = I2C_1_I2C_FSM_MSTR_WR_DATA;
        }
        else             /* Read direction */
        {
            I2C_1_state  = I2C_1_I2C_FSM_MSTR_RD_DATA;
                      slaveAddress |= I2C_1_I2C_READ_FLAG;
        }

        /* TX and RX FIFO have to be empty at here */

        /* A proper ReStart sequence is: set command to generate ReStart, then put an address byte in the TX FIFO.
        * Put address into the TX FIFO the 1st makes scb IP think that this data byte for current write transaction.
        */
        I2C_1_I2C_MASTER_GENERATE_RESTART;
        I2C_1_TX_FIFO_WR_REG = slaveAddress;

        /* Wait until address has been transferred. Note that for master reads at least one byte. */
        do
        {
            locStatus  = I2C_1_GetMasterInterruptSource() & I2C_1_INTR_MASTER_SEND_BYTE;
            locStatus |= I2C_1_I2CMasterWaitOneUnit(&timeout);
        }
        while (0u == locStatus);


        /* Handle completion event and convert to status */
        errStatus = I2C_1_I2CMasterHandleStatus(locStatus);
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: I2C_1_I2CMasterSendStop
****************************************************************************//**
*
* Generates Stop condition on the bus.
* The NAK is generated before Stop in case of a read transaction.
* At least one byte has to be read if a Start or ReStart condition with read
* direction was generated before.
* This function is blocking and does not return until a Stop condition is
* generated or error occurred.
*
* \param timeoutMs
* Defines in milliseconds the time that this function can block for.
* If that time expires, the function returns. If a zero is passed,
* the function waits forever for the action to complete. If a timeout occurs,
* the SCB block is reset. Note The maximum value is (maximum uint32)/1000.
*
* \return
* Error status.
* See I2C_1_I2CMasterSendStart() function for constants.
*
* \sideeffect
* A valid Start or ReStart condition must be generated before calling
* this function. This function does nothing if Start or ReStart conditions
* failed before this function was called.
* For read transaction, at least one byte has to be read before ReStart
* generation.
*
* \globalvars
* I2C_1_state - used to store current state of software FSM.
*
*******************************************************************************/
uint32 I2C_1_I2CMasterSendStop(uint32 timeoutMs)
{
    uint32 errStatus = I2C_1_I2C_MSTR_NOT_READY;

    /* Check FSM state before generating Stop condition */
    if(I2C_1_CHECK_I2C_MASTER_ACTIVE)
    {
        uint32 locStatus;
        uint32 timeout = I2C_1_I2C_CONVERT_TIMEOUT_TO_US(timeoutMs);

        /* Write direction: generates Stop; Read  direction: generates NACK and Stop; */
        I2C_1_I2C_MASTER_GENERATE_STOP;

        /* Wait for a completion event from the master */
        do
        {
            locStatus  = I2C_1_GetMasterInterruptSource() & I2C_1_INTR_MASTER_SEND_STOP;
            locStatus |= I2C_1_I2CMasterWaitOneUnit(&timeout);
        }
        while (0u == locStatus);

        /* Handle completion event and convert to status */
        errStatus = I2C_1_I2CMasterHandleStatus(locStatus);
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: I2C_1_I2CMasterWriteByte
****************************************************************************//**
*
* Sends one byte to a slave.
* This function is blocking and does not return until byte is transmitted
* or error occurred.
*
* \param wrByte
* The data byte to send to the slave.
*
* \param timeoutMs
* Defines in milliseconds the time that this function can block for.
* If that time expires, the function returns. If a zero is passed,
* the function waits forever for the action to complete. If a timeout occurs,
* the SCB block is reset. Note The maximum value is (maximum uint32)/1000.
*
* \return
* Error status.
* See I2C_1_I2CMasterSendStart() function for constants.
*
* \sideeffect
* A valid Start or ReStart condition must be generated before calling
* this function. This function does nothing if Start or ReStart condition
* failed before this function was called.
*
* \globalvars
* I2C_1_state - used to store current state of software FSM.
*
*******************************************************************************/
uint32 I2C_1_I2CMasterWriteByte(uint32 wrByte, uint32 timeoutMs)
{
    uint32 errStatus = I2C_1_I2C_MSTR_NOT_READY;

    /* Check FSM state before write byte */
    if(I2C_1_CHECK_I2C_MASTER_ACTIVE)
    {
        uint32 locStatus;
        uint32 timeout = I2C_1_I2C_CONVERT_TIMEOUT_TO_US(timeoutMs);

        I2C_1_TX_FIFO_WR_REG = wrByte;

        /* Wait for a completion event from the master */
        do
        {
            locStatus  = I2C_1_GetMasterInterruptSource() & I2C_1_INTR_MASTER_SEND_BYTE;
            locStatus |= I2C_1_I2CMasterWaitOneUnit(&timeout);
        }
        while (0u == locStatus);

        /* Handle completion event and convert to status */
        errStatus = I2C_1_I2CMasterHandleStatus(locStatus);
    }

    return(errStatus);
}



/*******************************************************************************
* Function Name: I2C_1_I2CMasterReadByte
****************************************************************************//**
*
* Reads one byte from a slave and generates ACK or prepares to generate NAK.
* The NAK will be generated before Stop or ReStart condition by
* I2C_1_I2CMasterSendStop() or
* I2C_1_I2CMasterSendRestart() function appropriately.
* This function is blocking. It does not return until a byte is received or
* an error occurs.
*
* \param ackNack
* Response to received byte.
*  - I2C_1_I2C_ACK_DATA - Generates ACK.
*     The master notifies slave that transfer continues.
*  - I2C_1_I2C_NAK_DATA - Prepares to generate NAK.
*     The master will notify slave that transfer is completed.
*
* \param rdByte
* The pointer to the location to store the data byte that was read from
* the slave.
* Note that the byte should be ignored if error status is returned.
*
* \param timeoutMs
* Defines in milliseconds the time that this function can block for.
* If that time expires, the function returns. If a zero is passed,
* the function waits forever for the action to complete. If a timeout occurs,
* the SCB block is reset. Note The maximum value is (maximum uint32)/1000.
*
* \return
* Error status.
* See I2C_1_I2CMasterSendStart() function for constants.
*
* \sideeffect
* A valid Start or ReStart condition must be generated before calling
* this function. This function does nothing if Start or ReStart condition
* failed before this function was called.
*
* \globalvars
* I2C_1_state - used to store current state of software FSM.
*
*******************************************************************************/
uint32 I2C_1_I2CMasterReadByte(uint32 ackNack, uint8 *rdByte, uint32 timeoutMs)
{
    uint32 errStatus = I2C_1_I2C_MSTR_NOT_READY;

    /* Check FSM state before read byte */
    if(I2C_1_CHECK_I2C_MASTER_ACTIVE)
    {
        uint32 locStatus;
        uint32 rxNotEmpty;
        uint32 timeout = I2C_1_I2C_CONVERT_TIMEOUT_TO_US(timeoutMs);

        /* Wait for a completion event from the master */
        do
        {
            locStatus  = I2C_1_GetMasterInterruptSource() & I2C_1_INTR_MASTER_RECEIVE_BYTE;
            rxNotEmpty = I2C_1_GetRxInterruptSource() & I2C_1_INTR_RX_NOT_EMPTY;
            locStatus |= I2C_1_I2CMasterWaitOneUnit(&timeout);
        }
        while ((0u == locStatus) && (0u == rxNotEmpty));

        /* Get byte from RX FIFO */
        if (0u != rxNotEmpty)
        {
            *rdByte = (uint8)I2C_1_RX_FIFO_RD_REG;
            I2C_1_ClearRxInterruptSource(I2C_1_INTR_RX_ALL);
        }

        /* Handle completion event and convert to status */
        errStatus = I2C_1_I2CMasterHandleStatus(locStatus);

        if (I2C_1_I2C_MSTR_NO_ERROR == errStatus)
        {
            if(I2C_1_I2C_ACK_DATA == ackNack)
            {
                I2C_1_I2C_MASTER_GENERATE_ACK;
            }
            else
            {
                /* NACK is generated by Stop or ReStart command */
            }
        }
    }

    return(errStatus);
}


/*******************************************************************************
* Function Name: I2C_1_I2CMasterGetReadBufSize
****************************************************************************//**
*
*  Returns the number of bytes that has been transferred with an
*  I2C_1_I2CMasterReadBuf() function.
*
* \return
*  Byte count of transfer. If the transfer is not yet complete, it returns
*  the byte count transferred so far.
*
* \sideeffect
*  This function returns not valid value if
*  I2C_1_I2C_MSTAT_ERR_ARB_LOST or
*  I2C_1_I2C_MSTAT_ERR_BUS_ERROR occurred while read transfer.
*
* \globalvars
*  I2C_1_mstrRdBufIndex - used to current index within master read
*  buffer.
*
*******************************************************************************/
uint32 I2C_1_I2CMasterGetReadBufSize(void)
{
    return(I2C_1_mstrRdBufIndex);
}


/*******************************************************************************
* Function Name: I2C_1_I2CMasterGetWriteBufSize
****************************************************************************//**
*
*  Returns the number of bytes that have been transferred with an
*  SCB_I2CMasterWriteBuf() function.
*
* \return
*   Byte count of transfer. If the transfer is not yet complete, it returns
*   zero unit transfer completion.
*
* \sideeffect
*   This function returns not valid value if
*   I2C_1_I2C_MSTAT_ERR_ARB_LOST or
*   I2C_1_I2C_MSTAT_ERR_BUS_ERROR occurred while read transfer.
*
* \globalvars
*  I2C_1_mstrWrBufIndex - used to current index within master write
*  buffer.
*
*******************************************************************************/
uint32 I2C_1_I2CMasterGetWriteBufSize(void)
{
    return(I2C_1_mstrWrBufIndex);
}


/*******************************************************************************
* Function Name: I2C_1_I2CMasterClearReadBuf
****************************************************************************//**
*
*  Resets the read buffer pointer back to the first byte in the buffer.
*
* \globalvars
*  I2C_1_mstrRdBufIndex - used to current index within master read
*   buffer.
*  I2C_1_mstrStatus - used to store current status of I2C Master.
*
*******************************************************************************/
void I2C_1_I2CMasterClearReadBuf(void)
{
    I2C_1_DisableInt();  /* Lock from interruption */

    I2C_1_mstrRdBufIndex = 0u;
    I2C_1_mstrStatus    &= (uint16) ~I2C_1_I2C_MSTAT_RD_CMPLT;

    I2C_1_EnableInt();   /* Release lock */
}


/*******************************************************************************
* Function Name: I2C_1_I2CMasterClearWriteBuf
****************************************************************************//**
*
*  Resets the write buffer pointer back to the first byte in the buffer.
*
* \globalvars
*  I2C_1_mstrRdBufIndex - used to current index within master read
*   buffer.
*  I2C_1_mstrStatus - used to store current status of I2C Master.
*
*******************************************************************************/
void I2C_1_I2CMasterClearWriteBuf(void)
{
    I2C_1_DisableInt();  /* Lock from interruption */

    I2C_1_mstrWrBufIndex = 0u;
    I2C_1_mstrStatus    &= (uint16) ~I2C_1_I2C_MSTAT_WR_CMPLT;

    I2C_1_EnableInt();   /* Release lock */
}


/*******************************************************************************
* Function Name: I2C_1_I2CMasterStatus
****************************************************************************//**
*
*  Returns the master's communication status.
*
* \return
*  Current status of I2C master. This status incorporates status constants.
*  Each constant is a bit field value. The value returned may have multiple
*  bits set to indicate the status of the read or write transfer.
*  - I2C_1_I2C_MSTAT_RD_CMPLT - Read transfer complete.
*    The error condition status bits must be checked to ensure that
*    read transfer was completed successfully.
*  - I2C_1_I2C_MSTAT_WR_CMPLT - Write transfer complete.
*    The error condition status bits must be checked to ensure that write
*    transfer was completed successfully.
*  - I2C_1_I2C_MSTAT_XFER_INP - Transfer in progress.
*  - I2C_1_I2C_MSTAT_XFER_HALT - Transfer has been halted.
*    The I2C bus is waiting for ReStart or Stop condition generation.
*  - I2C_1_I2C_MSTAT_ERR_SHORT_XFER - Error condition: Write
*    transfer completed before all bytes were transferred. The slave NAKed
*    the byte which was expected to be ACKed.
*  - I2C_1_I2C_MSTAT_ERR_ADDR_NAK - Error condition: Slave did
*    not acknowledge address.
*  - I2C_1_I2C_MSTAT_ERR_ARB_LOST - Error condition: Master lost
*    arbitration during communications with slave.
*  - I2C_1_I2C_MSTAT_ERR_BUS_ERROR - Error condition: bus error
*    occurred during master transfer due to misplaced Start or Stop
*    condition on the bus.
*  - I2C_1_I2C_MSTAT_ERR_ABORT_XFER - Error condition: Slave was
*    addressed by another master while master performed the start condition
*    generation. As a result, master has automatically switched to slave
*    mode and is responding. The master transaction has not taken place
*    This error condition only applicable for Multi-Master-Slave mode.
*  - I2C_1_I2C_MSTAT_ERR_XFER - Error condition: This is the
*    ORed value of all error conditions provided above.
*
* \globalvars
*  I2C_1_mstrStatus - used to store current status of I2C Master.
*
*******************************************************************************/
uint32 I2C_1_I2CMasterStatus(void)
{
    uint32 status;

    I2C_1_DisableInt();  /* Lock from interruption */

    status = (uint32) I2C_1_mstrStatus;

    if (I2C_1_CHECK_I2C_MASTER_ACTIVE)
    {
        /* Add status of master pending transaction: MSTAT_XFER_INP */
        status |= (uint32) I2C_1_I2C_MSTAT_XFER_INP;
    }

    I2C_1_EnableInt();   /* Release lock */

    return(status);
}


/*******************************************************************************
* Function Name: I2C_1_I2CMasterClearStatus
****************************************************************************//**
*
*  Clears all status flags and returns the master status.
*
* \return
*  Current status of master. See the I2C_1_I2CMasterStatus()
*  function for constants.
*
* \globalvars
*  I2C_1_mstrStatus - used to store current status of I2C Master.
*
*******************************************************************************/
uint32 I2C_1_I2CMasterClearStatus(void)
{
    uint32 status;

    I2C_1_DisableInt();  /* Lock from interruption */

    /* Read and clear master status */
    status = (uint32) I2C_1_mstrStatus;
    I2C_1_mstrStatus = I2C_1_I2C_MSTAT_CLEAR;

    I2C_1_EnableInt();   /* Release lock */

    return(status);
}


/*******************************************************************************
* Function Name: I2C_1_I2CReStartGeneration
****************************************************************************//**
*
*  Generates a ReStart condition:
*  - SCB IP V1 and later: Generates ReStart using the scb IP functionality
*    Sets the I2C_MASTER_CMD_M_START and I2C_MASTER_CMD_M_NACK (if the previous
*    transaction was read) bits in the SCB.I2C_MASTER_CMD register.
*    This combination forces the master to generate ReStart.
*
*  - SCB IP V0: Generates Restart using the GPIO and scb IP functionality.
*    After the master completes write or read, the SCL is stretched.
*    The master waits until SDA line is released by the slave. Then the GPIO
*    function is enabled and the scb IP disabled as it already does not drive
*    the bus. In case of the previous transfer was read, the NACK is generated
*    by the GPIO. The delay of tLOW is added to manage the hold time.
*    Set I2C_M_CMD.START and enable the scb IP. The ReStart generation
*    is started after the I2C function is enabled for the SCL.
*    Note1: the scb IP due re-enable generates Start but on the I2C bus it
*           appears as ReStart.
*    Note2: the I2C_M_CMD.START is queued if scb IP is disabled.
*    Note3: the I2C_STATUS_M_READ is cleared is address was NACKed before.
*
* \sideeffect
*  SCB IP V0: The NACK generation by the GPIO may cause a greater SCL period
*             than expected for the selected master data rate.
*
*******************************************************************************/
void I2C_1_I2CReStartGeneration(void)
{
#if(I2C_1_CY_SCBIP_V0)
    /* Generates Restart use GPIO and scb IP functionality. Ticket ID#143715,
    * ID#145238 and ID#173656.
    * In case of timeout the I2C_1_I2CMasterSendRestart handles this
    * case using own timeout and execute appropriate recovery.
    */

    const uint16 oneUnit = 1u;
    uint32 timeout = I2C_1_I2C_PHASE_GEN_TIMEOUT;

    /* Store direction of transfer */
    uint32 readDirection = I2C_1_I2C_STATUS_REG & I2C_1_I2C_STATUS_M_READ;

    /* Wait until slave releases SDA line: SCL tHIGH is complete */
    while ((I2C_1_WAIT_SDA_SET_HIGH) && (timeout > 0u))
    {
        CyDelayUs(oneUnit);
        --timeout;
    }

    if (timeout > 0u)
    {
        /* Refill timeout */
        timeout = I2C_1_I2C_PHASE_GEN_TIMEOUT;

        /* Drive SCL line use GPIO */
        I2C_1_SET_I2C_SCL_DR       (I2C_1_I2C_SCL_LOW);
        I2C_1_SET_I2C_SCL_HSIOM_SEL(I2C_1_HSIOM_GPIO_SEL);

        /* Disable SCB block */
        I2C_1_CTRL_REG &= (uint32) ~I2C_1_CTRL_ENABLED;

        if (0u != readDirection)
        {
            /* Generate NACK for read transactions use GPIO functionality */

            /* Drive tLOW: SCL low phase */
            I2C_1_SET_I2C_SCL_DR(I2C_1_I2C_SCL_LOW);
            CyDelayUs(I2C_1_I2C_TLOW_TIME);

            /* Drive tHIGH: SCL high phase */
            I2C_1_SET_I2C_SCL_DR(I2C_1_I2C_SCL_HIGH);
            CyDelayUs(I2C_1_I2C_THIGH_TIME);

            /* Wait until slave releases SDA line: SCL tHIGH is complete */
            while ((I2C_1_WAIT_SCL_SET_HIGH) && (timeout > 0u))
            {
                CyDelayUs(oneUnit);
                --timeout;
            }
        }

        if (timeout > 0u)
        {
            /* Drive tLOW: hold time for write and read */
            I2C_1_SET_I2C_SCL_DR(I2C_1_I2C_SCL_LOW);
            CyDelayUs(I2C_1_I2C_TLOW_TIME);

            /* Set Start command, it will appear as ReStart on the bus */
            I2C_1_I2C_MASTER_CMD_REG  = I2C_1_I2C_MASTER_CMD_M_START;
        }

        /* Enable SCB block: ReStart will appear on the bus as soon as SCL
        * configuration is restored and bus busy cleared.
        */
        I2C_1_CTRL_REG |= I2C_1_CTRL_ENABLED;

        /* Restore SCL configuration */
        I2C_1_SET_I2C_SCL_HSIOM_SEL(I2C_1_HSIOM_I2C_SEL);
        I2C_1_SET_I2C_SCL_DR       (I2C_1_I2C_SCL_HIGH);
    }
#else
    uint32 cmd;

    /* Generates ReStart use scb IP functionality */
    cmd  = I2C_1_I2C_MASTER_CMD_M_START;
    cmd |= I2C_1_CHECK_I2C_STATUS(I2C_1_I2C_STATUS_M_READ) ?
                (I2C_1_I2C_MASTER_CMD_M_NACK) : (0u);

    I2C_1_I2C_MASTER_CMD_REG = cmd;
#endif /* (I2C_1_CY_SCBIP_V1) */
}

#endif /* (I2C_1_I2C_MASTER_CONST) */


#if (!I2C_1_CY_SCBIP_V0 && \
    I2C_1_I2C_MULTI_MASTER_SLAVE_CONST && I2C_1_I2C_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: I2C_1_I2CMasterDisableEcAm
    ****************************************************************************//**
    *
    *  Disables externally clocked address match logic to enable master operation
    *  in active mode.
    *
    *******************************************************************************/
    static void I2C_1_I2CMasterDisableEcAm(void)
    {
        /* Disables externally clocked address match logic to enable master operation
        * in active mode. This applicable only for Multi-Master-Slave when wakeup is
        * enabled. Ticket ID#192742.
        */
        if (0u != (I2C_1_CTRL_REG & I2C_1_CTRL_EC_AM_MODE))
        {
            /* Enable external address match logic */
            I2C_1_Stop();
            I2C_1_CTRL_REG &= (uint32) ~I2C_1_CTRL_EC_AM_MODE;
            I2C_1_Enable();
        }
    }
#endif /* (!I2C_1_CY_SCBIP_V0) */


/*******************************************************************************
* Function Name: I2C_1_I2CMasterHandleStatus
****************************************************************************//**
*
* Converts hardware status to firmware status and executes required processing.
*
* \param status
* Hardware I2C master status to handle. It also includes
* I2C_1_I2C_MASTER_TIMEOUT and I2C_1_SLAVE_INTR_I2C_ADDR.
*
* \return
* Error status.
* See I2C_1_I2CMasterSendStart() function for constants.
*
*******************************************************************************/
static uint32 I2C_1_I2CMasterHandleStatus(uint32 status)
{
    uint32 retStatus;
    uint32 resetBlock = 0u;

    if (0u != (status & I2C_1_I2C_MASTER_TIMEOUT))
    {
        retStatus  = I2C_1_I2C_MASTER_TIMEOUT;
        resetBlock = I2C_1_I2C_RESET_ERROR;
    }
    else if (0u != (I2C_1_SLAVE_INTR_I2C_ADDR & status))
    {
        /* Abort the master operation, the slave was addressed first */
        retStatus = I2C_1_I2C_MSTR_ERR_ABORT_START;

        I2C_1_I2C_MASTER_CMD_REG = 0u;

        I2C_1_state = I2C_1_I2C_FSM_IDLE;
    }
    else if (0u != (status & I2C_1_INTR_MASTER_I2C_ARB_LOST))
    {
        retStatus  = I2C_1_I2C_MSTR_ERR_ARB_LOST;

        if (I2C_1_CHECK_I2C_FSM_ADDR)
        {
            /* If slave is not enabled reset IP, otherwise give it a chance to match address */
            if (0u == (I2C_1_I2C_CTRL_REG & I2C_1_I2C_CTRL_SLAVE_MODE))
            {
                resetBlock = I2C_1_I2C_RESET_ERROR;
            }

            I2C_1_state = I2C_1_I2C_FSM_IDLE;
        }
        else
        {
            /* Reset IP block when on address stage */
            resetBlock = I2C_1_I2C_RESET_ERROR;
        }
    }
    else if (0u != (status & I2C_1_INTR_MASTER_I2C_BUS_ERROR))
    {
        retStatus  = I2C_1_I2C_MSTR_ERR_BUS_ERR;
        resetBlock = I2C_1_I2C_RESET_ERROR;
    }
    else if (0u != (status & I2C_1_INTR_MASTER_I2C_NACK))
    {
        retStatus = I2C_1_I2C_MSTR_ERR_LB_NAK;
    }
    else /* Includes: INTR_MASTER_I2C_STOP and INTR_MASTER_I2C_ACK */
    {
        retStatus = I2C_1_I2C_MSTR_NO_ERROR;

        if (0u != (status & I2C_1_INTR_MASTER_I2C_STOP))
        {
            /* Stop: end of transaction, go to idle state */
            I2C_1_state = I2C_1_I2C_FSM_IDLE;
        }
        else
        {
            /* ACK: continue transaction */
            if (I2C_1_CHECK_I2C_FSM_ADDR)
            {
                /* Move from address state to data */
                I2C_1_state = (I2C_1_CHECK_I2C_FSM_RD) ?
                                          I2C_1_I2C_FSM_MSTR_RD_DATA : I2C_1_I2C_FSM_MSTR_WR_DATA;
            }
        }
    }

    if (I2C_1_I2C_RESET_ERROR == resetBlock)
    {
        I2C_1_I2CFwBlockReset();
    }
    else
    {
        I2C_1_ClearMasterInterruptSource(I2C_1_INTR_MASTER_ALL);
    }

    return (retStatus);
}


/******************************************************************************
* Function Name: I2CMasterWaitOneUnit
****************************************************************************//**
*
* Waits for one unit before unblock code execution.
* Note If a timeout value is 0, this function does nothing and returns 0.
*
* \param timeout
* The pointer to a timeout value.
*
* \return
* Returns 0 if a timeout does not expire or the timeout mask.
*
*******************************************************************************/
static uint32 I2C_1_I2CMasterWaitOneUnit(uint32 *timeout)
{
    const uint16 oneUnit = 1u;
    uint32 status = 0u;

    /* If the timeout equal to 0. Ignore the timeout */
    if (*timeout > 0u)
    {
        CyDelayUs(oneUnit);
        --(*timeout);

        if (0u == *timeout)
        {
            status = I2C_1_I2C_MASTER_TIMEOUT;
        }
    }

    return (status);
}




/* [] END OF FILE */
