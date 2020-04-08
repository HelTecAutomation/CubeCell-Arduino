#include "Arduino.h"
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(cubecell_random(20));
  delay(1000);
}
