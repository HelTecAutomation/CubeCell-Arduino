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

#ifndef __ASR6601_I2C_H
#define __ASR6601_I2C_H

#ifdef __cplusplus
extern "C" {
#endif 

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tremo_i2c.h"
#include "ASR_Arduino.h"

typedef enum {
	I2C_ERROR_OK=0,
	I2C_ERROR_DEV,
	I2C_ERROR_ACK,
	I2C_ERROR_TIMEOUT,
	I2C_ERROR_BUS,
	I2C_ERROR_BUSY,
	I2C_ERROR_MEMORY,
	I2C_ERROR_CONTINUE,
	I2C_ERROR_NO_BEGIN
} i2c_err_t;


i2c_t * i2cInit(int8_t i2c_num, int8_t sda, int8_t scl, uint32_t frequency);
i2c_err_t i2cSetFrequency(i2c_t * i2c, uint32_t clk_speed);
uint32_t i2cGetFrequency(i2c_t * i2c);
i2c_err_t i2cWrite(i2c_t * i2c, uint16_t address, uint8_t* buff, uint16_t size, bool sendStop, uint16_t timeOutMillis);
i2c_err_t i2cRead(i2c_t * i2c, uint16_t address, uint8_t* buff, uint16_t size, bool sendStop, uint16_t timeOutMillis, uint32_t *readCount);
i2c_err_t i2cFlush(i2c_t * i2c);
uint32_t i2cGetStatus(i2c_t * i2c);
void i2cEnd(i2c_t * i2c,int8_t sda, int8_t scl);

#ifdef __cplusplus
}
#endif
#endif //__TREMO_UART_H

/**
 * @} 
 * @}
 */
