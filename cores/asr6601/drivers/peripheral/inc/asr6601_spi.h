/**
 ******************************************************************************
 * @file    tremo_uart.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   This file contains all the functions prototypes for the I2C firmware
 *          library.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup UART
 * @{
 */

#ifndef __ASR6601_SPI_H
#define __ASR6601_SPI_H

#ifdef __cplusplus
extern "C" {
#endif 

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tremo_spi.h"
#include "ASR_Arduino.h"


#define SPI_LSBFIRST 0
#define SPI_MSBFIRST 1
#define MSBFIRST SPI_MSBFIRST
#define LSBFIRST SPI_LSBFIRST

#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

#define SPI_TIMEOUT 50

uint32_t spiFrequencyToClockDiv(uint32_t freq);
spi_t * spiStartBus(int8_t spi_num, uint32_t freq, uint8_t dataMode, uint8_t bitOrder,int8_t sck,int8_t mosi,int8_t miso,int8_t ss);
int spiSSEnable(spi_t * spi,int8_t ss);
void spiSSDisable(spi_t * spi,int8_t ss);
uint32_t spiSetFrequency(spi_t * spi,uint32_t freq);
uint32_t spiGetFrequency(spi_t * spi);
uint8_t spiGetDataMode(spi_t * spi);
void spiSetDataMode(spi_t * spi, uint8_t dataMode);
uint8_t spiGetBitOrder(spi_t * spi);
void spiSetBitOrder(spi_t * spi, uint8_t bitOrder);
void spiStopBus(spi_t * spi,int8_t sck,int8_t mosi,int8_t miso,int8_t ss);
void spiWaitReady(spi_t * spi);
void spiWrite(spi_t * spi, uint32_t *data, uint8_t len);
void spiTransfer(spi_t * spi, uint32_t *data, uint8_t len);
void spiWriteByte(spi_t * spi, uint8_t data);
uint8_t spiTransferByte(spi_t * spi, uint8_t data);
void spiWriteWord(spi_t * spi, uint16_t data);
uint16_t spiTransferWord(spi_t * spi, uint16_t data);
void spiWriteLong(spi_t * spi, uint32_t data);
uint32_t spiTransferLong(spi_t * spi, uint32_t data);
void spiTransferBytes(spi_t * spi, uint8_t * data, uint8_t * out, uint32_t size);
void spiTransferBits(spi_t * spi, uint32_t data, uint32_t * out, uint8_t bits);
void spiTransaction(spi_t * spi, uint32_t freq, uint8_t dataMode, uint8_t bitOrder);
void spiSimpleTransaction(spi_t * spi);
void spiEndTransaction(spi_t * spi);


#ifdef __cplusplus
}
#endif
#endif //__TREMO_UART_H

/**
 * @} 
 * @}
 */
