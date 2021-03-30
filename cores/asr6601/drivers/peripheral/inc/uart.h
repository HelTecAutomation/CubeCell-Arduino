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

#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ASR_Arduino.h"

typedef void( *uartIrqHandler )( void );

#define SERIAL_5N1 UART_DATA_WIDTH_5 << 16 | UART_PARITY_NO << 8 | UART_STOP_BITS_1
#define SERIAL_6N1 UART_DATA_WIDTH_6 << 16 | UART_PARITY_NO << 8 | UART_STOP_BITS_1
#define SERIAL_7N1 UART_DATA_WIDTH_7 << 16 | UART_PARITY_NO << 8 | UART_STOP_BITS_1
#define SERIAL_8N1 UART_DATA_WIDTH_8 << 16 | UART_PARITY_NO << 8 | UART_STOP_BITS_1
#define SERIAL_5N2 UART_DATA_WIDTH_5 << 16 | UART_PARITY_NO << 8 | UART_STOP_BITS_2
#define SERIAL_6N2 UART_DATA_WIDTH_6 << 16 | UART_PARITY_NO << 8 | UART_STOP_BITS_2
#define SERIAL_7N2 UART_DATA_WIDTH_7 << 16 | UART_PARITY_NO << 8 | UART_STOP_BITS_2
#define SERIAL_8N2 UART_DATA_WIDTH_8 << 16 | UART_PARITY_NO << 8 | UART_STOP_BITS_2
#define SERIAL_5E1 UART_DATA_WIDTH_5 << 16 | UART_PARITY_EVEN << 8 | UART_STOP_BITS_1
#define SERIAL_6E1 UART_DATA_WIDTH_6 << 16 | UART_PARITY_EVEN << 8 | UART_STOP_BITS_1
#define SERIAL_7E1 UART_DATA_WIDTH_7 << 16 | UART_PARITY_EVEN << 8 | UART_STOP_BITS_1
#define SERIAL_8E1 UART_DATA_WIDTH_8 << 16 | UART_PARITY_EVEN << 8 | UART_STOP_BITS_1
#define SERIAL_5E2 UART_DATA_WIDTH_5 << 16 | UART_PARITY_EVEN << 8 | UART_STOP_BITS_2
#define SERIAL_6E2 UART_DATA_WIDTH_6 << 16 | UART_PARITY_EVEN << 8 | UART_STOP_BITS_2
#define SERIAL_7E2 UART_DATA_WIDTH_7 << 16 | UART_PARITY_EVEN << 8 | UART_STOP_BITS_2
#define SERIAL_8E2 UART_DATA_WIDTH_8 << 16 | UART_PARITY_EVEN << 8 | UART_STOP_BITS_2
#define SERIAL_5O1 UART_DATA_WIDTH_5 << 16 | UART_PARITY_ODD << 8 | UART_STOP_BITS_1
#define SERIAL_6O1 UART_DATA_WIDTH_6 << 16 | UART_PARITY_ODD << 8 | UART_STOP_BITS_1
#define SERIAL_7O1 UART_DATA_WIDTH_7 << 16 | UART_PARITY_ODD << 8 | UART_STOP_BITS_1
#define SERIAL_8O1 UART_DATA_WIDTH_8 << 16 | UART_PARITY_ODD << 8 | UART_STOP_BITS_1
#define SERIAL_5O2 UART_DATA_WIDTH_5 << 16 | UART_PARITY_ODD << 8 | UART_STOP_BITS_2
#define SERIAL_6O2 UART_DATA_WIDTH_6 << 16 | UART_PARITY_ODD << 8 | UART_STOP_BITS_2
#define SERIAL_7O2 UART_DATA_WIDTH_7 << 16 | UART_PARITY_ODD << 8 | UART_STOP_BITS_2
#define SERIAL_8O2 UART_DATA_WIDTH_8 << 16 | UART_PARITY_ODD << 8 | UART_STOP_BITS_2

int uartStart(uint32_t baud,uint32_t conf,int rx, int tx);
void uartEnd(int uart,int rx, int tx);
int uart_tx_queue_push(uint8_t c,int uart);
void uartFlush(int uart);
void uartFlushTxOnly(int uart,bool txOnly);
int uartAvailableForWrite(int uart);
size_t uartWrite(int uart,const uint8_t *buffer, size_t size);
int uartAvailable(int uart);
uint8_t uartRead(int uart);
uint8_t uartPeek(int uart);
uint32_t uartGetBaudRate(int uart);
bool uartBusy(int uart);
extern uart_t* serialArray[];
extern bool uart0started;

#ifdef __cplusplus
}
#endif
#endif //__TREMO_UART_H

/**
 * @} 
 * @}
 */
