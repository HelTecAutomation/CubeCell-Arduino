#include "Arduino.h"
#include "LoRaWan_APP.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t voltage = getBatteryVoltage();
  Serial.println(voltage);
  delay(1000);
}