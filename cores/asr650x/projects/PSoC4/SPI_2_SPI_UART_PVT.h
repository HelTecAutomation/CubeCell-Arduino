/***************************************************************************//**
* \file SPI_2_SPI_UART_PVT.h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component in SPI and UART modes.
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

#if !defined(CY_SCB_SPI_UART_PVT_SPI_2_H)
#define CY_SCB_SPI_UART_PVT_SPI_2_H

#include "SPI_2_SPI_UART.h"


/***************************************
*     Internal Global Vars
***************************************/

#if (SPI_2_INTERNAL_RX_SW_BUFFER_CONST)
    extern volatile uint32  SPI_2_rxBufferHead;
    extern volatile uint32  SPI_2_rxBufferTail;
    
    /**
    * \addtogroup group_globals
    * @{
    */
    
    /** Sets when internal software receive buffer overflow
     *  was occurred.
    */  
    extern volatile uint8   SPI_2_rxBufferOverflow;
    /** @} globals */
#endif /* (SPI_2_INTERNAL_RX_SW_BUFFER_CONST) */

#if (SPI_2_INTERNAL_TX_SW_BUFFER_CONST)
    extern volatile uint32  SPI_2_txBufferHead;
    extern volatile uint32  SPI_2_txBufferTail;
#endif /* (SPI_2_INTERNAL_TX_SW_BUFFER_CONST) */

#if (SPI_2_INTERNAL_RX_SW_BUFFER)
    extern volatile uint8 SPI_2_rxBufferInternal[SPI_2_INTERNAL_RX_BUFFER_SIZE];
#endif /* (SPI_2_INTERNAL_RX_SW_BUFFER) */

#if (SPI_2_INTERNAL_TX_SW_BUFFER)
    extern volatile uint8 SPI_2_txBufferInternal[SPI_2_TX_BUFFER_SIZE];
#endif /* (SPI_2_INTERNAL_TX_SW_BUFFER) */


/***************************************
*     Private Function Prototypes
***************************************/

void SPI_2_SpiPostEnable(void);
void SPI_2_SpiStop(void);

#if (SPI_2_SCB_MODE_SPI_CONST_CFG)
    void SPI_2_SpiInit(void);
#endif /* (SPI_2_SCB_MODE_SPI_CONST_CFG) */

#if (SPI_2_SPI_WAKE_ENABLE_CONST)
    void SPI_2_SpiSaveConfig(void);
    void SPI_2_SpiRestoreConfig(void);
#endif /* (SPI_2_SPI_WAKE_ENABLE_CONST) */

void SPI_2_UartPostEnable(void);
void SPI_2_UartStop(void);

#if (SPI_2_SCB_MODE_UART_CONST_CFG)
    void SPI_2_UartInit(void);
#endif /* (SPI_2_SCB_MODE_UART_CONST_CFG) */

#if (SPI_2_UART_WAKE_ENABLE_CONST)
    void SPI_2_UartSaveConfig(void);
    void SPI_2_UartRestoreConfig(void);
#endif /* (SPI_2_UART_WAKE_ENABLE_CONST) */


/***************************************
*         UART API Constants
***************************************/

/* UART RX and TX position to be used in SPI_2_SetPins() */
#define SPI_2_UART_RX_PIN_ENABLE    (SPI_2_UART_RX)
#define SPI_2_UART_TX_PIN_ENABLE    (SPI_2_UART_TX)

/* UART RTS and CTS position to be used in  SPI_2_SetPins() */
#define SPI_2_UART_RTS_PIN_ENABLE    (0x10u)
#define SPI_2_UART_CTS_PIN_ENABLE    (0x20u)


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Interrupt processing */
#define SPI_2_SpiUartEnableIntRx(intSourceMask)  SPI_2_SetRxInterruptMode(intSourceMask)
#define SPI_2_SpiUartEnableIntTx(intSourceMask)  SPI_2_SetTxInterruptMode(intSourceMask)
uint32  SPI_2_SpiUartDisableIntRx(void);
uint32  SPI_2_SpiUartDisableIntTx(void);


#endif /* (CY_SCB_SPI_UART_PVT_SPI_2_H) */


/* [] END OF FILE */
