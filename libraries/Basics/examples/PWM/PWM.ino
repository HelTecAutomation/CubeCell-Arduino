#include "Arduino.h"


void setup() {
  // put your setup code here, to run once:
	pinMode(PWM1,OUTPUT);
	pinMode(PWM2,OUTPUT);
}

uint16_t a=0;
uint16_t b=0;

void loop() {
  // PWM frequency is 65535
  analogWrite(PWM1,a);
  analogWrite(PWM2,b);
  delay(10);
  a+=255;
  b-=255;
}