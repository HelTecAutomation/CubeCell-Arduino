/* millis() use the external 32.768KHz RTC timer. This is a very accurate timer
 *  
 * millis can work during the deep sleep period
 * 
 * The printed `b - a` value is not accuracy 1000, because of the delay function use
 * internal system ticker, it's not accuracy, indeed delay(1000) is about 1020 ms.
 * 
 * http://community.heltec.cn/t/cubecell-micros-function/985
 */

#include "Arduino.h"

void setup() {
  Serial.begin(115200);
}

void loop() {
  uint32_t a, b;
  //Serial.println(millis());
  a = millis();
  delay(1000);
  b = millis();
  Serial.println(b - a);
}