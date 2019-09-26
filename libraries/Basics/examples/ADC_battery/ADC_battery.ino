#include "Arduino.h"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

uint16_t voltage;

void loop() {
  // put your main code here, to run repeatedly:
  pinMode(ADC_CTL,OUTPUT);
  digitalWrite(ADC_CTL,LOW);
  uint16_t voltage=analogRead(ADC)*2;
  digitalWrite(ADC_CTL,HIGH);
  Serial.print(millis());
  Serial.print("  ");
  Serial.println(voltage);
  delay(1000);
}
