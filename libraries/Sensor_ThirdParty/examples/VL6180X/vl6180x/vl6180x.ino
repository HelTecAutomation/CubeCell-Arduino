/*
  Adapted to CubeCell HTCC-AB01 by Mike Cochrane
  December 31, 2019

  Hardware Connections:

   CubeCell Pin  VL6180X Board      Function

   Vext          VIN                Power
   GND           GND                Ground
   SDA           SDA                I2C Data
   SCL           SCL                I2C Clock

 */
 
#include <Wire.h>
#include "Adafruit_VL6180X.h"

Adafruit_VL6180X vl = Adafruit_VL6180X();

void setup() {
  pinMode(Vext, OUTPUT);  
  Serial.begin(115200);
  Serial.println("Adafruit VL6180x test!");
}

void loop() {
  // Vext ON
  digitalWrite(Vext, LOW);
  delay(50);
  
  if (! vl.begin()) {
    Serial.println("Failed to find sensor");
    digitalWrite(Vext, HIGH);
    delay(1000);
    return;
  }

  float lux = vl.readLux(VL6180X_ALS_GAIN_40);

  Serial.print("Lux: "); Serial.println(lux);
  
  uint8_t range = vl.readRange();
  uint8_t status = vl.readRangeStatus();
  Wire.end();
  
  // Vext OFF
  digitalWrite(Vext, HIGH);

  if (status == VL6180X_ERROR_NONE) {
    Serial.print("Range: "); Serial.println(range);
  }

  // Some error occurred, print it out!
  
  if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
    Serial.println("System error");
  }
  else if (status == VL6180X_ERROR_ECEFAIL) {
    Serial.println("ECE failure");
  }
  else if (status == VL6180X_ERROR_NOCONVERGE) {
    Serial.println("No convergence");
  }
  else if (status == VL6180X_ERROR_RANGEIGNORE) {
    Serial.println("Ignoring range");
  }
  else if (status == VL6180X_ERROR_SNR) {
    Serial.println("Signal/Noise error");
  }
  else if (status == VL6180X_ERROR_RAWUFLOW) {
    Serial.println("Raw reading underflow");
  }
  else if (status == VL6180X_ERROR_RAWOFLOW) {
    Serial.println("Raw reading overflow");
  }
  else if (status == VL6180X_ERROR_RANGEUFLOW) {
    Serial.println("Range reading underflow");
  }
  else if (status == VL6180X_ERROR_RANGEOFLOW) {
    Serial.println("Range reading overflow");
  }
  
  delay(1000);
}
