/*!
 * \file      gpio-board.c
 *
 * \brief     Target board GPIO driver implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */
#include <project.h>
//#include "asr_project.h"
#include "utilities.h"
#include "board-config.h"
#include "rtc-board.h"
#include "gpio-board.h"
#include "debug.h"
#include "ASR_Arduino.h"

void hal_gpio_write(int pin, int value)
{
    digitalWrite(pin,value);
}

int hal_gpio_read(int pin)
{
    return digitalRead(pin);
}

void GpioMcuInit( Gpio_t *obj, PinNames pin, PINMODE mode, PinConfigs config, PinTypes type, uint32_t value )
{
    obj->pin = pin;

    if( pin == NC )
    {
        return;
    }

    obj->pin = pin;
    // Sets initial output value
    pinMode(pin,mode);
    GpioMcuWrite( obj, value );
   
}

void GpioMcuSetInterrupt( Gpio_t *obj, IrqModes irqMode, IrqPriorities irqPriority, GpioIrqHandler irqHandler )
{
    attachInterrupt(obj->pin,irqHandler,irqMode);
}

void GpioMcuRemoveInterrupt( Gpio_t *obj )
{
    detachInterrupt(obj->pin);
}

void GpioMcuWrite( Gpio_t *obj, uint32_t value )
{
    if(obj == NULL)
    {
        DBG_PRINTF("write gpio faild!\r\n");
    }

    // Check if pin is not connected
    if( obj->pin == NC )
    {
        return;
    }

    hal_gpio_write(obj->pin, value);
}

void GpioMcuToggle( Gpio_t *obj )
{
    if(obj == NULL)
    {
        DBG_PRINTF("toggle gpio faild!\r\n");
    }

    // Check if pin is not connected
    if( obj->pin == NC )
    {
        return;
    }
    hal_gpio_write(obj->pin, hal_gpio_read(obj->pin) == 0 ? 1 : 0);
}

uint32_t GpioMcuRead( Gpio_t *obj )
{
    if( obj == NULL )
    {
        DBG_PRINTF("gpio read faild!\r\n");
    }
    // Check if pin is not connected
    if( obj->pin == NC )
    {
        return 0;
    }

    return hal_gpio_read(obj->pin);
}
