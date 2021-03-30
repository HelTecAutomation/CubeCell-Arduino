#include "Arduino.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

float voltage;
uint16_t level;

void loop() {
  // put your main code here, to run repeatedly:
  
  //return adc level 0-4095;
  level=analogRead(ADC); 

  /*return the voltage in mV
  * for asr650x, max value can be read is 2400mV
  * for asr6601,value can be read is 100mV-1100mV
  */
  voltage=analogReadmV(ADC);

  Serial.print("voltage: ");
  Serial.print(voltage);
  Serial.printf(",  level: ");
  Serial.println(level);
  delay(1000);
}