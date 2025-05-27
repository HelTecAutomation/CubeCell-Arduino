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
#define MOSI1 P4_0
#define MISO P4_1
#define MISO1 P4_1
#define SCK P4_2
#define SCK1 P4_2
#define UART_RX P3_0
#define UART_RX2 P3_0
#define UART_TX P3_1
#define UART_TX2 P3_1
#define USER_KEY P3_3 //gpio7
#define GPIO0 P0_2
#define GPIO1 P6_1
#define GPIO2 P6_2 //can be used in PWM mode
#define GPIO3 P6_4 //can be used in PWM mode
#define GPIO4 P0_7
#define GPIO5 P0_6
#define GPIO6 P3_2 
#define GPIO7 P3_3 
#define SDA P0_1
#define SDA1 P0_1
#define SCL P0_0
#define SCL1 P0_0
#define ADC P2_1
#define PWM1 P6_2 //gpio2
#define PWM2 P6_4 //gpio3

#endif /* Pins_Arduino_h */
