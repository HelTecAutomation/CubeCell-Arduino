#if !defined(__ASR_Arduino__) 
#define __ASR_Arduino__

#include "cytypes.h"

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
#define MCU_PINS \
    P0_0 = 0, P0_1, P0_2, P0_3, P0_4, P0_5, P0_6, P0_7,  \
    P1_0 , P1_1, P1_2, P1_3, P1_4, P1_5, P1_6, P1_7,  \
    P2_0 , P2_1, P2_2, P2_3, P2_4, P2_5, P2_6, P2_7,  \
    P3_0 , P3_1, P3_2, P3_3, P3_4, P3_5, P3_6, P3_7,  \
    P4_0 , P4_1, P4_2, P4_3, P4_4, P4_5, P4_6, P4_7,  \
    P5_0 , P5_1, P5_2, P5_3, P5_4, P5_5, P5_6, P5_7,  \
    P6_0 , P6_1, P6_2, P6_3, P6_4, P6_5, P6_6, P6_7,  \
    P7_0 , P7_1, P7_2, P7_3, P7_4, P7_5, P7_6, P7_7  
    
typedef enum
{
    MCU_PINS,
    NC = (int)0xFFFFFFFF    
}PinNames;    

#define UART_RX P3_0
#define UART_TX P3_1
#define Vext P3_2 //gpio6
#define ADC_CTL P3_3 //gpio7
#define GPIO0 P0_2
#define GPIO1 P6_1
#define GPIO2 P6_2 //can be used in PWM mode
#define GPIO3 P6_4 //can be used in PWM mode
#define GPIO4 P0_7
#define GPIO5 P0_6
#define GPIO6 P3_2 
#define GPIO7 P3_3 
#define SDA P0_1
#define SCL P0_0
#define ADC P2_3
#define PWM1 P6_2 //gpio2
#define PWM2 P6_4 //gpio3
#define RGB P0_7 //gpio4

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


#ifdef __cplusplus 
}
#endif

#endif 

/* [] END OF FILE */

