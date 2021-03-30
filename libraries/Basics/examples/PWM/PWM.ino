#include "Arduino.h"

/*  for example:
 *
 *	setPWM_Frequency(a);
 * 	setPWM_ComparePeriod(b);
 *	analogWrite(PWM1,c);
 *
 *	the PWM frequency is a/b. 
 *  c used for set dutycycle, it's value can be 0-b;
 */
void setup() {
  // put your setup code here, to run once:

  /*PWM CLK frequency default is 12M;
   *PWM CLK frequency value:
   *PWM_CLK_FREQ_48M
   *PWM_CLK_FREQ_24M
   *PWM_CLK_FREQ_16M
   *PWM_CLK_FREQ_12M
   *PWM_CLK_FREQ_8M
   *PWM_CLK_FREQ_6M
   *PWM_CLK_FREQ_4M
   *PWM_CLK_FREQ_3M
   *PWM_CLK_FREQ_2M
   *PWM_CLK_FREQ_1M
   */
  setPWM_Frequency(PWM_CLK_FREQ_12M);

  //pwm period can be 0xFF~0xFFFF, default is 0xFFFF
  setPWM_ComparePeriod(0xFFFF);
  
  pinMode(PWM1,OUTPUT);
  pinMode(PWM2,OUTPUT);
}

uint16_t a=0;
uint16_t b=0;

void loop() {
  analogWrite(PWM1,a);
  analogWrite(PWM2,b);
  delay(10);
  a+=255;
  b-=255;
}