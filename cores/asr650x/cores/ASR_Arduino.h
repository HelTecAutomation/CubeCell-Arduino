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
    NONE,
    RISING,
    FALLING,
    BOTH,
}IrqModes;

#define DRIVE_MODE_BITS        (3)
#define DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - DRIVE_MODE_BITS))


typedef void( *GpioIrqHandler )( void );
extern GpioIrqHandler GpioIrqHandlerCallback[P7_7];
extern bool lowPowerEnabled;
void globalGpioIsrEntry (void); 
void pinMode(uint8_t pin_name,PINMODE mode);
void digitalWrite(uint8_t pin_name,PINLEVEL level);
uint8 digitalRead(uint8_t pin_name);
uint8 digitalReadOutPut(uint8_t pin_name);
void pinToggle(uint8_t pin_name);
void ClearPinInterrupt(uint8_t pin_name);
void attachInterrupt(uint8_t pin_name, GpioIrqHandler GpioIrqHandlerCallback, IrqModes interrupt_mode);
void detachInterrupt(uint8_t pin_name);
int16 analogRead (uint8_t pin);//the value returned is in mV units, max value can be read is 3300 mV.
void analogWrite (uint8_t pin, uint8_t value) ;
void delay(uint32_t milliseconds);
void delayMicroseconds(uint16 microseconds);
uint32_t millis(void);
uint32_t micros(void);

#ifdef __cplusplus 
}
#endif

#endif 

/* [] END OF FILE */

