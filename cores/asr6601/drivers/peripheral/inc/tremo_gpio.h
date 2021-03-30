/**
 ******************************************************************************
 * @file    tremo_gpio.h
 * @author  ASR Tremo Team
 * @version v1.1.0
 * @date    2020-10-19
 * @brief   This file contains all the functions prototypes for the CRC firmware
 *          library.
 * @addtogroup Tremo_Drivers
 * @{
 * @defgroup GPIO
 * @{
 */

#ifndef __TREMO_GPIO_H
#define __TREMO_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "tremo_regs.h"

#define GPIO_PIN_0  (0)  /*!< Pin 0 selected */
#define GPIO_PIN_1  (1)  /*!< Pin 1 selected */
#define GPIO_PIN_2  (2)  /*!< Pin 2 selected */
#define GPIO_PIN_3  (3)  /*!< Pin 3 selected */
#define GPIO_PIN_4  (4)  /*!< Pin 4 selected */
#define GPIO_PIN_5  (5)  /*!< Pin 5 selected */
#define GPIO_PIN_6  (6)  /*!< Pin 6 selected */
#define GPIO_PIN_7  (7)  /*!< Pin 7 selected */
#define GPIO_PIN_8  (8)  /*!< Pin 8 selected */
#define GPIO_PIN_9  (9)  /*!< Pin 9 selected */
#define GPIO_PIN_10 (10) /*!< Pin 10 selected */
#define GPIO_PIN_11 (11) /*!< Pin 11 selected */
#define GPIO_PIN_12 (12) /*!< Pin 12 selected */
#define GPIO_PIN_13 (13) /*!< Pin 13 selected */
#define GPIO_PIN_14 (14) /*!< Pin 14 selected */
#define GPIO_PIN_15 (15) /*!< Pin 15 selected */

/**
 * @brief Macro is used to check the gpio pin
 */
#define IS_GPIO_PIN(__GPIO__, __PIN__)                                                          \
    (((__GPIO__ == GPIOA) || (__GPIO__ == GPIOB) || (__GPIO__ == GPIOC) || (__GPIO__ == GPIOD)) \
        && (__PIN__ <= GPIO_PIN_15))

/**
 * @brief Macro is used to check the gpio mode
 */
#define IS_GPIO_MODE(__MODE__)                                                                   \
    (((__MODE__) == GPIO_MODE_INPUT_FLOATING) || ((__MODE__) == GPIO_MODE_INPUT_PULL_UP)         \
        || ((__MODE__) == GPIO_MODE_INPUT_PULL_DOWN) || ((__MODE__) == GPIO_MODE_OUTPUT_PP_HIGH) \
        || ((__MODE__) == GPIO_MODE_OUTPUT_PP_LOW) || ((__MODE__) == GPIO_MODE_OUTPUT_OD_HIZ)    \
        || ((__MODE__) == GPIO_MODE_OUTPUT_OD_LOW) || ((__MODE__) == GPIO_MODE_ANALOG))

/**
 * @brief Macro is used to check the gpio interrupt
 */
#define IS_GPIO_INTR(__INTR__)                                                                                         \
    (((__INTR__) == GPIO_INTR_NONE) || ((__INTR__) == GPIO_INTR_RISING_EDGE) || ((__INTR__) == GPIO_INTR_FAILING_EDGE) \
        || ((__INTR__) == GPIO_INTR_RISING_FAILING_EDGE))

/**
 * @brief The level of the gpio pin
 */
typedef enum {
    GPIO_LEVEL_LOW = 0,  /*!< GPIO high level */
    GPIO_LEVEL_HIGH = 1, /*!< GPIO low level */
} gpio_level_t;

/**
 * @brief The output drive capability of the gpio pin
 */
typedef enum { GPIO_DRIVE_CAPABILITY_4MA = 0, GPIO_DRIVE_CAPABILITY_8MA = 1 } gpio_drive_capability_t;

/**
 * @brief The mode of the gpio pin
 */
typedef enum {
    GPIO_MODE_INPUT_FLOATING,      /*!< Input floating */
    GPIO_MODE_INPUT_PULL_UP,    /*!< Input pull-up */
    GPIO_MODE_INPUT_PULL_DOWN,  /*!< Input pull-down */
    GPIO_MODE_OUTPUT_PP_HIGH,   /*!< Output push-pull high level */
    GPIO_MODE_OUTPUT_PP_LOW,    /*!< Output push-pull low level */
    GPIO_MODE_OUTPUT_OD_HIZ,    /*!< Output open-drain hig-impedance */
    GPIO_MODE_OUTPUT_OD_LOW,    /*!< Output open-drain low level */
    GPIO_MODE_ANALOG,           /*!< Analog */
} gpio_mode_t;

/**
 * @brief The interrupt type of the gpio pin
 */
typedef enum {
    GPIO_INTR_NONE                = 0,  /*!< Disable GPIO interrupt */
    GPIO_INTR_RISING_EDGE         = 1,  /*!< GPIO interrupt type: rising edge*/
    GPIO_INTR_FALLING_EDGE        = 2,  /*!< GPIO interrupt type: falling edge*/
    GPIO_INTR_RISING_FALLING_EDGE = 3,  /*!< GPIO interrupt type: both rising and falling edge*/
} gpio_intr_t;

void gpio_deinit(void);
void gpio_init(gpio_t* gpiox, uint8_t gpio_pin, gpio_mode_t mode);

gpio_level_t gpio_read(gpio_t* gpiox, uint8_t gpio_pin);
void gpio_write(gpio_t* gpiox, uint8_t gpio_pin, gpio_level_t level);
void gpio_toggle(gpio_t* gpiox, uint8_t gpio_pin);

void gpio_config_interrupt(gpio_t* gpiox, uint8_t gpio_pin, uint8_t intr_type);
void gpio_clear_interrupt(gpio_t* gpiox, uint8_t gpio_pin);
it_status_t gpio_get_interrupt_status(gpio_t* gpiox, uint8_t gpio_pin);

void gpio_config_wakeup(gpio_t* gpiox, uint8_t gpio_pin, bool enable, gpio_level_t wakeup_level);
void gpio_config_stop3_wakeup(gpio_t* gpiox, uint8_t gpio_pin, bool enable, gpio_level_t wakeup_level);

void gpio_config_drive_capability(gpio_t* gpiox, uint8_t gpio_pin, gpio_drive_capability_t drive);

void gpio_set_iomux(gpio_t* gpiox, uint8_t gpio_pin, uint8_t func_num);

#ifdef __cplusplus
}
#endif

#endif //__TREMO_GPIO_H

/**
 * @} 
 * @}
 */
