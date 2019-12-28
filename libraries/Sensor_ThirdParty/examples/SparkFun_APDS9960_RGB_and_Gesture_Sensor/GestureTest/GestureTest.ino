/****************************************************************
GestureTest.ino
APDS-9960 RGB and Gesture Sensor
Shawn Hymel @ SparkFun Electronics
May 30, 2014
https://github.com/sparkfun/APDS-9960_RGB_and_Gesture_Sensor

Adapted to CubeCell HTCC-AB01 by Mike Cochrane
December 28, 2019

Tests the gesture sensing abilities of the APDS-9960. Configures
APDS-9960 over I2C and waits for gesture events. Calculates the
direction of the swipe (up, down, left, right) and displays it
on a serial console.

To perform a NEAR gesture, hold your hand
far above the sensor and move it close to the sensor (within 2
inches). Hold your hand there for at least 1 second and move it
away.

To perform a FAR gesture, hold your hand within 2 inches of the
sensor for at least 1 second and then move it above (out of
range) of the sensor.

Hardware Connections:

 CubeCell Pin  APDS-9960 Board  Function

 Vext          VCC              Power
 GND           GND              Ground
 SDA           SDA              I2C Data
 SCL           SCL              I2C Clock
 GPIO1         INT              Interrupt

Resources:
Include Wire.h and SparkFun_APDS-9960.h

Development environment specifics:
Written in Arduino 1.8.10
Tested with CubeCell HTCC-AB01

This code is beerware; if you see me (or any other SparkFun
employee) at the local, and you've found our code helpful, please
buy us a round!

Distributed as-is; no warranty is given.
****************************************************************/

#include <Wire.h>
#include <SparkFun_APDS9960.h>

// Pins
#define APDS9960_INT GPIO1 // Needs to be an interrupt pin

// Constants

// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
volatile int isr_flag = 0;

void setup()
{
  // Vext ON
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(10);

  // Initialize Serial port
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - GestureTest"));
  Serial.println(F("--------------------------------"));

  // Initialize interrupt service routine
  pinMode(APDS9960_INT, INPUT);
  attachInterrupt(APDS9960_INT, interruptRoutine, FALLING);

  // Initialize APDS-9960 (configure I2C and initial values)
  if (apds.init())
  {
    Serial.println(F("APDS-9960 initialization complete"));
  }
  else
  {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }

  // Start running the APDS-9960 gesture sensor engine
  if (apds.enableGestureSensor(true))
  {
    Serial.println(F("Gesture sensor is now running"));
  }
  else
  {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }
}

void loop()
{
  if (isr_flag == 1)
  {
    detachInterrupt(APDS9960_INT);
    handleGesture();
    isr_flag = 0;
    attachInterrupt(APDS9960_INT, interruptRoutine, FALLING);
  }
}

void interruptRoutine()
{
  isr_flag = 1;
}

void handleGesture()
{
  if (apds.isGestureAvailable())
  {
    switch (apds.readGesture())
    {
    case DIR_UP:
      Serial.println("UP");
      break;
    case DIR_DOWN:
      Serial.println("DOWN");
      break;
    case DIR_LEFT:
      Serial.println("LEFT");
      break;
    case DIR_RIGHT:
      Serial.println("RIGHT");
      break;
    case DIR_NEAR:
      Serial.println("NEAR");
      break;
    case DIR_FAR:
      Serial.println("FAR");
      break;
    default:
      Serial.println("NONE");
    }
  }
}
