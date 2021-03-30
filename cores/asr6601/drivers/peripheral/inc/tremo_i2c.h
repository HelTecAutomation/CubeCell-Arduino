/**
 ******************************************************************************
 * @file    tremo_i2c.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   This file contains all the functions prototypes for the I2C firmware
 *          library.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup I2C
 * @{
 */
 
#ifndef __TREMO_I2C_H
#define __TREMO_I2C_H

#ifdef __cplusplus
extern "C" {
#endif
#include <math.h>
#include <stdbool.h>
#include "tremo_regs.h"

/**
 * @brief This macro is used to check the fifo mode
 */
#define IS_I2C_FIFO_MODE(cr) (cr & I2C_CR_FIFO_EN_MASK)

/**
 * @brief I2C mode
 */
typedef enum {
    I2C_MODE_MASTER = 0,  /*!< Master mode */
    I2C_MODE_SLAVE  = 1,  /*!< Slave mode */
} i2c_mode_t;

/**
 * @brief I2C speed
 */
typedef enum {
    I2C_SPEED_STANDARD = I2C_CR_BUS_MODE_STANDARD,  /*!< Standard speed(100K) */
    I2C_SPEED_FAST     = I2C_CR_BUS_MODE_FAST,      /*!< Fast speed(400K) */
} i2c_speed_t;

/**
 * @brief I2C write/read
 */
typedef enum {
    I2C_WRITE = 0, /*!< I2C write data */
    I2C_READ  = 1, /*!< I2C read data */
} i2c_rw_t;

/**
 * @brief I2C ACK flag
 */
typedef enum {
    I2C_NAK = 0,  /*!< I2C NAK */
    I2C_ACK = 1,  /*!< I2C ACK */
} i2c_ack_t;

/**
 * @brief I2C interrupts
 */
typedef enum {
    I2C_INTR_ARBITRATION_LOSS = 18, /*!< Arbitration loss */
    I2C_INTR_TRANS_EMPTY      = 19, /*!< Transmit data empty */
    I2C_INTR_RECV_FULL        = 20, /*!< New data received */
    I2C_INTR_BUS_ERROR_DET    = 22, /*!< Bus error is detected */
    I2C_INTR_SLAVE_ADDR_DET   = 23, /*!< Address is detected in slave mode */
    I2C_INTR_SLAVE_STOP_DET   = 24, /*!< Stop is detected in slave mode */
    I2C_INTR_MASTER_STOP_DET  = 25, /*!< Stop is sent in master mode */
    I2C_INTR_TRANS_DONE       = 27, /*!< Transaction is done */
    I2C_INTR_TFIFO_EMPTY      = 28, /*!< Transmit FIFO is empty */
    I2C_INTR_RFIFO_HFULL      = 29, /*!< Receive FIFO is half-full */
    I2C_INTR_RFIFO_FULL       = 30, /*!< Receive FIFO is full */
    I2C_INTR_RFIFO_OVERRUN    = 31  /*!< Receive FIFO is overrun */
} i2c_interrupt_t;

/**
 * @brief I2C flags
 */
typedef enum {
    I2C_FLAG_RFIFO_EMPTY = 0,  /*!< Receive FIFO is empty */
    I2C_FLAG_UNIT_BUSY   = 15, /*!< I2C unit is busy */
    I2C_FLAG_BUS_BUSY    = 16, /*!< I2C bus is empty */

    I2C_FLAG_ARBITRATION_LOSS = 18, /*!< Arbitration loss */
    I2C_FLAG_TRANS_EMPTY      = 19, /*!< Transmit data empty */
    I2C_FLAG_RECV_FULL        = 20, /*!< New data received */
    I2C_FLAG_BUS_ERROR_DET    = 22, /*!< Bus error is detected */
    I2C_FLAG_SLAVE_ADDR_DET   = 23, /*!< Address is detected in slave mode */
    I2C_FLAG_SLAVE_STOP_DET   = 24, /*!< Stop is detected in slave mode */
    I2C_FLAG_MASTER_STOP_DET  = 26, /*!< Stop is sent in master mode */
    I2C_FLAG_TRANS_DONE       = 27, /*!< Transaction is done */
    I2C_FLAG_TFIFO_EMPTY      = 28, /*!< Transmit FIFO is empty */
    I2C_FLAG_RFIFO_HFULL      = 29, /*!< Receive FIFO is half-full */
    I2C_FLAG_RFIFO_FULL       = 30, /*!< Receive FIFO is full */
    I2C_FLAG_RFIFO_OVERRUN    = 31  /*!< Receive FIFO is overrun */
} i2c_flag_t;

/**
 * @brief I2C configuration
 */
typedef struct {
	i2c_mode_t mode;
	bool fifo_mode_en;
	uint32_t freq;
	union {
		struct {
			uint32_t speed; 
		} master;
		struct {
			uint8_t slave_addr;
		} slave;
	} settings;
} i2c_config_t;

/*
typedef struct {
    i2c_mode_t mode;
    bool fifo_mode_en;
    union {
        struct {
            uint32_t speed; 
        } master;
        struct {
            uint8_t slave_addr;
        } slave;
    } settings;
} i2c_config_t;
*/
void i2c_deinit(i2c_t* i2cx);

void i2c_config_init(i2c_config_t* config);
void i2c_init(i2c_t* i2cx, i2c_config_t* config);
void i2c_cmd(i2c_t* i2cx, bool newstate);
void i2c_dma_cmd(i2c_t* i2cx, bool newstate);

void i2c_config_interrupt(i2c_t* i2cx, i2c_interrupt_t interrupt, bool new_state);
void i2c_clear_interrupt(i2c_t* i2cx, i2c_interrupt_t interrupt);
it_status_t i2c_get_interrupt_status(i2c_t* i2cx, i2c_interrupt_t intr);

flag_status_t i2c_get_flag_status(i2c_t* i2cx, i2c_flag_t flag);
void i2c_clear_flag_status(i2c_t* i2cx, i2c_flag_t flag);

void i2c_master_send_start(i2c_t* i2cx, uint8_t slave_addr, uint8_t bit_rw);
void i2c_master_send_stop(i2c_t* i2cx);
void i2c_master_send_stop_with_data(i2c_t* i2cx, uint8_t data);

void i2c_send_data(i2c_t* i2cx, uint8_t data);
uint8_t i2c_receive_data(i2c_t* i2cx);
void i2c_set_receive_mode(i2c_t* i2cx, i2c_ack_t ack);

#ifdef __cplusplus
}
#endif

#endif /* __TREMO_I2C_H */

/**
 * @} 
 * @}
 */
