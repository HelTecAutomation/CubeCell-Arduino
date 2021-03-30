#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

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

#define SS P4_3
#define MOSI P4_0
#define MISO P4_1
#define SCK P4_2
#define UART_RX P3_0
#define UART_TX P3_1
#define UART_RX1 UART_RX
#define UART_TX1 UART_TX
#define UART_RX2 P4_4
#define UART_TX2 P4_5
#define Vext P3_2 
#define VBAT_ADC_CTL P3_3 
#define USER_KEY P3_3
#define SDA P0_1
#define SCL P0_0
#define RGB P0_6  //GPIO13
#define PWM1 P6_2 
#define PWM2 P6_4
#define ADC1 P2_0
#define ADC ADC1
#define ADC2 P2_1
#define ADC3 P2_2
#define GPIO1 P6_0
#define GPIO2 P6_1
#define GPIO3 P6_2
#define GPIO4 P6_4
#define GPIO5 P3_4
#define GPIO6 P3_6
#define GPIO7 P3_7
#define GPIO8 P7_0
#define GPIO9 P7_1
#define GPIO10 P7_2
#define GPIO11 P0_2
#define GPIO12 P0_3
#define GPIO13 P0_6 //RGB
#define GPIO14 P0_7
#define MOSI1  GPIO1
#define MISO1  GPIO2
#define SCK1   GPIO3
#define SDA1   GPIO9
#define SCL1   GPIO8
#endif /* Pins_Arduino_h */
