/* micros() use the internal 48MHz system ticker. Actually this timer is not very accurate.
 *  
 * As tested, micros() have about 1.7% error;
 * 
 * system ticker can't run during the deep sleep period, so if use micros() with LoRaWAN, must
 * disable deep sleep by set "lowPowerEnabed" to false
 * 
 * http://community.heltec.cn/t/cubecell-micros-function/985
 */

#include "Arduino.h"

void setup() {
  Serial.begin(115200);
}

void loop() {
  uint32_t a, b;
  //Serial.println(micros());
  a = micros();
  delay(1000);
  b = micros();
  Serial.println(b - a);
}