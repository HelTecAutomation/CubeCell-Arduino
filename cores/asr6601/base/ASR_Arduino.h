#if !defined(__ASR_Arduino__) 
#define __ASR_Arduino__

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "timer.h"
#include "tremo_adc.h"
#include "tremo_uart.h"
#include "tremo_rcc.h"
#include "tremo_timer.h"
#include "tremo_regs.h"
#include "tremo_gpio.h"
#include "tremo_delay.h"
#include "tremo_pwr.h"
#include "tremo_uart.h"
#include "tremo_flash.h"
#include "rtc-board.h"
#include "tremo_dma.h"
#include "tremo_dac.h"
#include "tremo_iwdg.h"
#include "tremo_bstimer.h"
#include "tremo_system.h"
#include "tremo_dma_handshake.h"
#include "asr6601_spi.h"
#include "utilities.h"
#include "pins_arduino.h"
#include "sx126x-board.h"

#ifdef __cplusplus 
		extern "C" {
#endif
typedef bool boolean;
typedef uint8_t byte;
typedef unsigned int word;

#define PIN_NUMBER_IN_PORT 16
#define PORT_REG_SHFIT 0x400

#define COLOR_SEND 0x500000   //color red, light 0x10
#define COLOR_JOINED 0x500050 //color Violet, light 0x10
#define COLOR_RXWINDOW1 0x000050 //color blue, light 0x10
#define COLOR_RXWINDOW2 0x505000 //color yellow, light 0x10
#define COLOR_RECEIVED 0x005000 //color green, light 0x10
#define COLOR_RXWINDOW3 0x005050 //


typedef enum
{
  INPUT,	 /*!< Input floating */
  INPUT_PULLUP,   /*!< Input pull-up */
  INPUT_PULLDOWN, /*!< Input pull-down */
  OUTPUT, /*!< Output */
  OD_HI,  /*!< Output open-drain hig-impedance */
  OD_LO,  /*!< Output open-drain low level */
  ANALOG,  /*!< Analog */
}PINMODE;

typedef enum
{
  LOW = 0,              
  HIGH  ,               
}PINLEVEL;

typedef enum
{
    NONE=0,   /*!< Disable GPIO interrupt */
    RISING, /*!< GPIO interrupt type: rising edge*/
    FALLING,/*!< GPIO interrupt type: falling edge*/
    BOTH,   /*!< GPIO interrupt type: both rising and falling edge*/
}IrqModes;

typedef enum {
    PWM_CLK_FREQ_48M = 0,
    PWM_CLK_FREQ_24M = 1,
    PWM_CLK_FREQ_16M = 2,
    PWM_CLK_FREQ_12M = 3,
    PWM_CLK_FREQ_8M = 5,
    PWM_CLK_FREQ_6M = 7,
    PWM_CLK_FREQ_4M = 11,
    PWM_CLK_FREQ_3M = 15,
    PWM_CLK_FREQ_2M = 23,
    PWM_CLK_FREQ_1M = 47
} clock_pwm_t;

#define FLASH_START_ADDR              FLASH_BASE
#define FLASH_END_ADDR                FLASH_BASE+0x1000*64

#define FLASH_EEPROM_BASE             FLASH_BASE+0x1000*64-512-1024
#define FLASH_EEPROM_END              FLASH_BASE+0x1000*64-513

typedef void (*timer_callback_func)();

typedef void( *GpioIrqHandler )( void );
uint64_t getID(void);
void pinMode(uint8_t pin_name,PINMODE mode);
void digitalWrite(uint8_t pin_name,uint8_t level);
uint8_t digitalRead(uint8_t pin_name);
void iomux(uint8_t pin_name,uint8_t func_num);
void pinToggle(uint8_t pin_name);
void ClearPinInterrupt(uint8_t pin_name);
void attachInterrupt(uint8_t pin_name, GpioIrqHandler GpioIrqHandlerCallback, IrqModes interrupt_mode);
void detachInterrupt(uint8_t pin_name);
int analogRead(uint8_t pin_name);
float analogReadmV(uint8_t pin_name);
void analogWrite(uint8_t pin_name, uint16_t value);
void setPWM_Frequency(clock_pwm_t freq);
void setPWM_ComparePeriod(uint16_t period);
void enableGpioWakeUp(uint8_t pin_name,uint8_t wakeup_level);




/*dacWrite(uint16_t* buff,uint16_t size,bool repeat);
 *
 * \param [IN] buff    dac buffer pointer
 * \param [IN] size    dac buffer size
 * \param [IN] repeat  true: dac out repeatly; false: dac out once.
 */
void dacWriteBuffer(uint16_t* buff,uint16_t size,bool repeat);
void dacWrite(uint16_t val);


void microTimerStart(uint32_t time,timer_callback_func callback);
int cubecell_random(int r);
uint32_t programSize();


/*****************************************************************/
/**
  * @brief  Read from FLASH memory.
  * @param  In: address     Destination address.
  * @param  In: pData       Data to be programmed: Must be 8 byte aligned.
  * @param  In: len_bytes   Number of bytes to be programmed.
  * @retval  0: Success.
            -1: Failure.
  */
int FLASH_read_at(uint32_t address, uint8_t *pData, uint32_t len_bytes);

/**
  * @brief  Update a chunk of the FLASH memory.
  * @note   The FLASH chunk must no cross a FLASH bank boundary.
  * @note   The source and destination buffers have no specific alignment constraints.
  * @param  In: dst_addr    Destination address in the FLASH memory.
  * @param  In: data        Source address. 
  * @param  In: size        Number of bytes to update.
  * @retval  0:  Success.
  *         <0:  Failure.
  */
int FLASH_update(uint32_t dst_addr, const void *data, uint32_t size);


uint32_t millis(void);
uint32_t micros(void);
uint64_t getID(void);

#ifdef __cplusplus 
}
#endif

#endif 

/* [] END OF FILE */

