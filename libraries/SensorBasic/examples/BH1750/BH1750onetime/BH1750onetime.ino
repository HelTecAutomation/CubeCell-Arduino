/*Heltec Automation BH1750 Sensors test example
 *
 * Example of BH1750 library usage.
 * 
 * Function:
 * Lighting intensity test
 * 
 * Description:
 * This example initialises the BH1750 object using the high resolution
 * one-time mode and then makes a light level reading every second.
 *
 * The BH1750 component starts up in default mode when it next powers up.
 * The BH1750 library automatically reconfigures the one-time mode in
 * preparation for the next measurement.
 *
 * HelTec AutoMation, Chengdu, China
 * www.heltec.org
 *
 * this project also realess in GitHub:
 * https://github.com/HelTecAutomation/ASR650x-Arduino
*/

#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

void setup(){

  Serial.begin(115200);

  //Vext ON
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext,LOW);


  lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE);

  Serial.println(F("BH1750 One-Time Test"));

}


void loop() {

  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);

}
