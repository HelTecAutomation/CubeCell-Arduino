//the GPS module used is Air530.
#include "Arduino.h"
#include "GPS_Air530.h"
#include <Wire.h>  
#include "cubecell_SSD1306Wire.h"

 SSD1306Wire  display(0x3c, 500000, I2C_NUM_0, GEOMETRY_128_64, GPIO10 ); // addr , freq , i2c group , resolution , rst

int fracPart(double val, int n)
{
  return (int)((val - (int)(val))*pow(10,n));
}

void VextON(void)
{
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, LOW);
}

void VextOFF(void) //Vext default OFF
{
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, HIGH);
}

void setup() {
  VextON();
  delay(10);
  
  display.init();
  display.clear();
  display.display();
  
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 32-16/2, "GPS initing...");
  display.display();
  
  Serial.begin(115200);
  Air530.begin();
}

void loop()
{
  uint32_t starttime = millis();
  while( (millis()-starttime) < 1000 )
  {
    while (Air530.available() > 0)
    {
      Air530.encode(Air530.read());
    }
  }
  
  char str[30];
  display.clear();
  display.setFont(ArialMT_Plain_10);
  int index = sprintf(str,"%02d-%02d-%02d",Air530.date.year(),Air530.date.day(),Air530.date.month());
  str[index] = 0;
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, str);
  
  index = sprintf(str,"%02d:%02d:%02d",Air530.time.hour(),Air530.time.minute(),Air530.time.second(),Air530.time.centisecond());
  str[index] = 0;
  display.drawString(60, 0, str);

  if( Air530.location.age() < 1000 )
  {
    display.drawString(120, 0, "A");
  }
  else
  {
    display.drawString(120, 0, "V");
  }
  
  index = sprintf(str,"alt: %d.%d",(int)Air530.altitude.meters(),fracPart(Air530.altitude.meters(),2));
  str[index] = 0;
  display.drawString(0, 16, str);
   
  index = sprintf(str,"hdop: %d.%d",(int)Air530.hdop.hdop(),fracPart(Air530.hdop.hdop(),2));
  str[index] = 0;
  display.drawString(0, 32, str); 
 
  index = sprintf(str,"lat :  %d.%d",(int)Air530.location.lat(),fracPart(Air530.location.lat(),4));
  str[index] = 0;
  display.drawString(60, 16, str);   
  
  index = sprintf(str,"lon:%d.%d",(int)Air530.location.lng(),fracPart(Air530.location.lng(),4));
  str[index] = 0;
  display.drawString(60, 32, str);

  index = sprintf(str,"speed: %d.%d km/h",(int)Air530.speed.kmph(),fracPart(Air530.speed.kmph(),3));
  str[index] = 0;
  display.drawString(0, 48, str);
  display.display();
}
