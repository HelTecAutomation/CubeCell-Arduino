/*
  ccs811basic.ino - Demo sketch printing results of the CCS811 digital gas sensor for monitoring indoor air quality from ams.
  Created by Maarten Pennings 2017 Dec 11
*/


#include <Wire.h>    // I2C library
#include <CCS811.h>

CCS811 ccs;
uint16_t eco2, etvoc, baseline;


void setup() {
  // Enable serial
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Starting CCS811 baseline BurnIn");

  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(500);

  // Enable I2C
  Wire.begin();

  // Enable CCS811
  ccs.begin();
}


void loop() {
  while (!ccs.available());
  ccs.readData();
  eco2 = ccs.geteCO2();
  etvoc = ccs.getTVOC();
  baseline = ccs.getBaseline();


  Serial.print(millis()/1000);
  Serial.print(" Sec. since start; ");
  Serial.print("CCS811: ");
  Serial.print("eco2=");  Serial.print(eco2);
  Serial.print(" ppm  ");
  Serial.print("etvoc="); Serial.print(etvoc);
  Serial.print(" ppb  ");
  Serial.print("baseline="); Serial.print(baseline);
  Serial.println();


  // Wait
  delay(1000);
}
