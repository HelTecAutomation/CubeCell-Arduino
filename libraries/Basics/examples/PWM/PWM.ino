#include "Arduino.h"


void setup() {
  // put your setup code here, to run once:
	pinMode(PWM1,OUTPUT);
	pinMode(PWM2,OUTPUT);
}

uint8_t a=0;
uint8_t b=0;

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(PWM1,a);//GPIO2
  analogWrite(PWM2,b);//GPIO3
  delay(10);
  a++;
  b--;
}
