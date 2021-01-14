#if !defined(__ASR_Arduino__) 
#define __ASR_Arduino__

#include "cytypes.h"
#include "pins_arduino.h"
#include "stdbool.h"
#ifdef __cplusplus 
		extern "C" {
#endif
 
#define COLOR_SEND 0x500000   //color red, light 0x10
#define COLOR_JOINED 0x500050 //color Violet, light 0x10
#define COLOR_RXWINDOW1 0x000050 //color blue, light 0x10
#define COLOR_RXWINDOW2 0x505000 //color yellow, light 0x10
#define COLOR_RECEIVED 0x005000 //color green, light 0x10

#define UART_RX_LEVEL 1 //the of  external uartchip tx PIN when it is powered

#define PORT_REG_SHFIT 0x100
#define PIN_NUMBER_IN_PORT 8

typedef enum
{
  ANALOG = 0,              /**< \brief High Impedance Analog   */
  INPUT,                   /**< \brief High Impedance Digital  */
  OUTPUT_PULLUP ,          /**< \brief Resistive Pull Up       */
  OUTPUT_PULLDOWN ,        /**< \brief Resistive Pull Down     */
  OD_LO ,                  /**< \brief Open Drain, Drives Low  */
  OD_HI ,                  /**< \brief Open Drain, Drives High */
  OUTPUT ,                 /**< \brief Strong Drive            */
  OUTPUT_PULLUP_PULLDOWN , /**< \brief Resistive Pull Up/Down  */
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
    PWM_CLK_FREQ_48M = 1,
    PWM_CLK_FREQ_24M = 2,
    PWM_CLK_FREQ_16M = 3,
    PWM_CLK_FREQ_12M = 4,
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
int16 analogRead(uint8_t pin);//the value returned is in mV units, max value can be read is 2400 mV.
void analogWrite(uint8_t pin, uint16_t value);
void delay(uint32_t milliseconds);
void delayMicroseconds(uint16 microseconds);
uint32_t millis(void);
uint32_t micros(void);
uint64_t getID(void);
int cubecell_random(int r);
void setPWM_Frequency(clock_pwm_t freq);
void setPWM_ComparePeriod(uint16_t period);


#ifdef __cplusplus 
}
#endif

#endif 

/* [] END OF FILE */

