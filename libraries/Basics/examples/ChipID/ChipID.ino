#include "Arduino.h"
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  uint64_t chipID=getID();
  Serial.printf("ChipID:%04X%08X\r\n",(uint32_t)(chipID>>32),(uint32_t)chipID);
}

void loop() {
  // put your main code here, to run repeatedly:

}
