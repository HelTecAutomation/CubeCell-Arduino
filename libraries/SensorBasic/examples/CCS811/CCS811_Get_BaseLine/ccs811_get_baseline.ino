/*
  ccs811basic.ino - Demo sketch printing results of the CCS811 digital gas sensor for monitoring indoor air quality from ams.
  Created by Maarten Pennings 2017 Dec 11
*/

#include "Arduino.h"
#include <Wire.h>    // I2C library
#include <CCS811.h>

#define ROW 0
#define ROW_OFFSET 0
#define addr CY_SFLASH_USERBASE+CY_FLASH_SIZEOF_ROW*ROW + ROW_OFFSET

CCS811 ccs;
uint16_t eco2, etvoc, baseline, baselinetemp;

uint8_t baselineflash[2];

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

  //read flash at addr to data2
  FLASH_read_at(addr, baselineflash, sizeof(baselineflash));
  Serial.print("FLASH READ BaseLine=");
  Serial.print(baselineflash[0]);
  Serial.print(",");
  Serial.print(baselineflash[1]);
  Serial.print("=");
  baselinetemp = (baselineflash[0] << 8) | baselineflash[1];
  Serial.print(baselinetemp);
  Serial.println();
  if (baselinetemp > 0) {
    ccs.setBaseline(baselinetemp);
  }
}


void loop() {
  while (!ccs.available());
  ccs.readData();
  eco2 = ccs.geteCO2();
  etvoc = ccs.getTVOC();
  baseline = ccs.getBaseline();


  Serial.print(millis() / 1000);
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
  if (millis() / 1000 > 1200) {
    baselineflash[0] = (uint8_t)(baseline >> 8);
    baselineflash[1] = (uint8_t)baseline;
    //write data1 to flash at addr
    FLASH_update(addr, baselineflash, sizeof(baselineflash));
    Serial.print("FLASH WRITE BaseLine=");
    Serial.print(baselineflash[0]);
    Serial.print(",");
    Serial.print(baselineflash[1]);
    Serial.print("=");
    baselinetemp = (baselineflash[0] << 8) | baselineflash[1];
    Serial.print(baselinetemp);
    Serial.println();
    while(1);
  }
}
