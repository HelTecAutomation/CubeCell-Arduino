#if !defined(__ASR_Arduino__) 
#define __ASR_Arduino__

#include "cytypes.h"
#include "pins_arduino.h"
#include "stdbool.h"
#include "CyFlash.h"
#include <math.h>

#ifdef __cplusplus 
		extern "C" {
#endif

#define FLASH_EEPROM_BASE             (CY_FLASH_NUMBER_ROWS-4)*CY_FLASH_SIZEOF_ROW
#define FLASH_EEPROM_END              (CY_FLASH_NUMBER_ROWS-2)*CY_FLASH_SIZEOF_ROW-1

#define COLOR_SEND 0x500000   //color red, light 0x10
#define COLOR_JOINED 0x500050 //color Violet, light 0x10
#define COLOR_RXWINDOW1 0x000050 //color blue, light 0x10
#define COLOR_RXWINDOW2 0x505000 //color yellow, light 0x10
#define COLOR_RECEIVED 0x005000 //color green, light 0x10
#define COLOR_RXWINDOW3 0x005050 //

#define UART_RX_LEVEL 1 //the of  external uartchip tx PIN when it is powered

#define PORT_REG_SHFIT 0x100
#define PIN_NUMBER_IN_PORT 8

typedef enum
{
  ANALOG = 0,              /**< \brief High Impedance Analog   */
  INPUT,                   /**< \brief High Impedance Digital  */
  OUTPUT_PULLUP,          /**< \brief output with resistive Pull Up       */
  OUTPUT_PULLDOWN ,        /**< \brief output with resistive Pull Down     */
  OD_LO ,                  /**< \brief Open Drain, Drives Low  */
  OD_HI ,                  /**< \brief Open Drain, Drives High */
  OUTPUT ,                 /**< \brief Strong Drive            */
  OUTPUT_PULLUP_PULLDOWN,  /**< \brief Resistive Pull Up/Down  */
  INPUT_PULLUP,            /**< \brief input with resistive Pull Up       */
  INPUT_PULLDOWN,          /**< \brief input with resistive Pull Down       */
}PINMODE;


typedef enum
{
  LOW = 0,              
  HIGH  ,                
}PINLEVEL;

typedef enum
{
    NONE=0,
    RISING=1,
    FALLING=2,
    BOTH=3,
    CHANGE=3
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


#define DRIVE_MODE_BITS        (3)
#define DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - DRIVE_MODE_BITS))


typedef void( *GpioIrqHandler )( void );
extern GpioIrqHandler GpioIrqHandlerCallback[P7_7];
void globalGpioIsrEntry (void); 
void pinMode(uint8_t pin_name,PINMODE mode);
void digitalWrite(uint8_t pin_name,uint8_t level);
uint8_t digitalRead(uint8_t pin_name);
uint8_t digitalReadOutPut(uint8_t pin_name);
void pinToggle(uint8_t pin_name);
void ClearPinInterrupt(uint8_t pin_name);
void attachInterrupt(uint8_t pin_name, GpioIrqHandler GpioIrqHandlerCallback, IrqModes interrupt_mode);
void detachInterrupt(uint8_t pin_name);
int analogRead(uint8_t pin);
float analogReadmV(uint8_t pin_name);
void analogWrite(uint8_t pin, uint16_t value);
void delay(uint32_t milliseconds);
void delayMicroseconds(uint16 microseconds);
uint32_t millis(void);
uint32_t micros(void);
uint64_t getID(void);
int cubecell_random(int r);
void setPWM_Frequency(clock_pwm_t freq);
void setPWM_ComparePeriod(uint16_t period);

#define assert_param(x) ((void)0)

#ifdef __cplusplus 
}
#endif

#endif 

/* [] END OF FILE */

