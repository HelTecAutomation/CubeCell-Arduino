/*Heltec Automation BH1750 Sensors test example
 *
 * Example of BH1750 library usage.
 *
 * This example initialises the BH1750 object using the default high resolution
 *  continuous mode and then makes a light level reading every second.
 * 
 *  Function:
 *  Lighting intensity test
 *
 * Connection:
 *
 *   VCC -> 3V3 or 5V
 *   GND -> GND
 *   SCL -> SCL
 *   SDA -> SDA
 *   ADD -> (not connected) or GND
 * Description:
 * ADD pin is used to set sensor I2C address. If it has voltage greater or equal to
 * 0.7VCC voltage (e.g. you've connected it to VCC) the sensor address will be
 * 0x5C. In other case (if ADD voltage less than 0.7 * VCC) the sensor address will
 * be 0x23 (by default).
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

  lightMeter.begin();

  Serial.println(F("BH1750 Test begin"));

}


void loop() {

  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);

}
