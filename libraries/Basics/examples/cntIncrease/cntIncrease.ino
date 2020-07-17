#include "Arduino.h"

uint32_t cnt = 0;

void cntIncrease()
{
  cnt++;
  Serial.println(cnt);
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  PINMODE_INPUT_PULLDOWN(GPIO3);
  attachInterrupt(GPIO3,cntIncrease,RISING);
}

void loop() {
  // put your main code here, to run repeatedly:

}
