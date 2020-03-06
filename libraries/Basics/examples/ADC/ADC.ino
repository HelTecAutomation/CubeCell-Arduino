#include "Arduino.h"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

uint16_t voltage;

void loop() {
  // put your main code here, to run repeatedly:
  
  //ADC == ADC1
  voltage=analogRead(ADC);//return the voltage in mV, max value can be read is 2400mV 
  Serial.print(millis());
  Serial.print("  ");
  Serial.println(voltage);
  delay(1000);
}
