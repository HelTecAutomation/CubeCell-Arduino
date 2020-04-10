//the GPS module used is Air530.
#include "Arduino.h"
#include "GPS_Air530.h"
#include <Wire.h>  
#include "cubecell_SSD1306Wire.h"

 SSD1306Wire  display(0x3c, 500000, I2C_NUM_0,GEOMETRY_128_64,GPIO10 ); // addr , freq , i2c group , ratio , rst

int32_t getIntPart(double val)
{
  return (int32_t)val;
}

int32_t getFracPart(double val, int n)
{
  return (int32_t)((val - (int32_t)(val))*pow(10,n));
}

void setup() {
  Serial.begin(115200);
  GPS.begin();

  display.init();
  display.clear();
  display.display();
}

void loop()
{
  /*
  * typedef struct gps_status_s
  * {
  *   int validation;    //gps validation
  *   double latitude;   
  *   double longitude;
  *   int altitude;
  *   float hdop;        //Horizontal accuracy
  *   float speed;
  *   gpsTime_t time;
  * }gps_status_t;
  * 
  */
  
  gps_status_t gpsStatus = GPS.status();
  
  char str[30];
  display.clear();
  display.setFont(ArialMT_Plain_10);
  int index = sprintf(str,"%02d-%02d-%02d",gpsStatus.time.year,gpsStatus.time.month,gpsStatus.time.day);
  str[index] = 0;
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, str);
  
  index = sprintf(str,"%02d:%02d:%02d",gpsStatus.time.hour,gpsStatus.time.minute,gpsStatus.time.second);
  str[index] = 0;
  display.drawString(60, 0, str);

  if(gpsStatus.validation)
  {
    display.drawString(120, 0, "A");
  }
  else
  {
    display.drawString(120, 0, "V");
  }
  
  index = sprintf(str,"alt : %d",gpsStatus.altitude);
  str[index] = 0;
  display.drawString(0, 16, str);
   
  index = sprintf(str,"hdop: %d.%d",getIntPart(gpsStatus.hdop),getFracPart((double)gpsStatus.hdop,2));
  str[index] = 0;
  display.drawString(0, 32, str); 
 
  index = sprintf(str,"lat :  %d.%d",getIntPart(gpsStatus.latitude),getFracPart(gpsStatus.latitude,4));
  str[index] = 0;
  display.drawString(60, 16, str);   
  
  index = sprintf(str,"lon:%d.%d",getIntPart(gpsStatus.longitude),getFracPart(gpsStatus.longitude,4));
  str[index] = 0;
  display.drawString(60, 32, str);

  index = sprintf(str,"speed: %d.%d km/h",getIntPart(gpsStatus.speed),getFracPart((double)gpsStatus.speed,3));
  str[index] = 0;
  display.drawString(0, 48, str);
  display.display();
}