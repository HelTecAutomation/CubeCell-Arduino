#include <GPS_Air530.h>

/*
   This sample sketch demonstrates the normal use of a TinyAir530++ (TinyAir530Plus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   96000-baud serial Air530(Air530) device hooked up.
*/

void setup()
{
  Serial.begin(115200);
  Air530.begin();

  Serial.println(F("A simple demonstration of Air530 module"));
  Serial.println();

  displayInfo();
}

void loop(){
  uint32_t starttime = millis();
  while( (millis()-starttime) < 1000 )
  {
    while (Air530.available() > 0)
    {
      Air530.encode(Air530.read());
    }
  }
  displayInfo();
  if (millis() > 5000 && Air530.charsProcessed() < 10)
  {
    Serial.println("No GPS detected: check wiring.");
    while(true);
  }
}

void displayInfo()
{
  Serial.print("Date/Time: ");
  if (Air530.date.isValid())
  {
    Serial.printf("%d/%02d/%02d",Air530.date.year(),Air530.date.day(),Air530.date.month());
  }
  else
  {
    Serial.print("INVALID");
  }

  if (Air530.time.isValid())
  {
    Serial.printf(" %02d:%02d:%02d.%02d",Air530.time.hour(),Air530.time.minute(),Air530.time.second(),Air530.time.centisecond());
  }
  else
  {
    Serial.print(" INVALID");
  }
  Serial.println();
  
  Serial.print("LAT: ");
  Serial.print(Air530.location.lat(),6);
  Serial.print(", LON: ");
  Serial.print(Air530.location.lng(),6);
  Serial.print(", ALT: ");
  Serial.print(Air530.altitude.meters());

  Serial.println(); 
  
  Serial.print("SATS: ");
  Serial.print(Air530.satellites.value());
  Serial.print(", HDOP: ");
  Serial.print(Air530.hdop.hdop());
  Serial.print(", AGE: ");
  Serial.print(Air530.location.age());
  Serial.print(", COURSE: ");
  Serial.print(Air530.course.deg());
  Serial.print(", SPEED: ");
  Serial.println(Air530.speed.kmph());
  Serial.println();
}
