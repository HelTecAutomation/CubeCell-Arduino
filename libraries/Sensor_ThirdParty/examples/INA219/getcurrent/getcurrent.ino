/**
 * Adapted to CubeCell HTCC-AB01 by Mike Cochrane
 * December 28, 2019
 *
 * Hardware Connections:
 *
 *  CubeCell Pin  INA219 Board     Function
 *
 *  Vext          Vcc              Power
 *  GND           Gnd              Ground
 *  SDA           Sda              I2C Data
 *  SCL           Scl              I2C Clock
 *  
 */
 
#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;


void setup(void) 
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(500);
  
  Serial.begin(115200);

  uint32_t currentFrequency;
    
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  ina219.begin();
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  ina219.setCalibration_16V_400mA();

  Serial.println("Measuring voltage and current with INA219 ...");
}

void loop(void) 
{
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");

  delay(2000);
}
