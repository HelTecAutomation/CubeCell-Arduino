//the GPS module used is Air530.
#include "Arduino.h"
#include "GPS_Air530.h"

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

  //WGS-84:international standard GPS coordinate£¨Google Earth¡¢ GPS module¡¢Tian Map£©
  gps_status_t status_WGS = GPS.status();
  
  // printf double latitude
  Serial.printf("WGS-84: latitude:%d.%d",getIntPart(status_WGS.latitude),getFracPart(status_WGS.latitude,5));  
  // printf double longitude
  Serial.printf(",longitude:%d.%d\r\n",getIntPart(status_WGS.longitude),getFracPart(status_WGS.longitude,5));

  //GCJ-02: China coordinate migration standard, Google Map¡¢Gaode Map¡¢Tencent map
  gps_status_t status_GCJ =GPS.WGSToGCJ(status_WGS);
  // printf double latitude
  Serial.printf("GCJ-02: latitude:%d.%d",getIntPart(status_GCJ.latitude),getFracPart(status_GCJ.latitude,5));  
  // printf double longitude
  Serial.printf(",longitude:%d.%d\r\n",getIntPart(status_GCJ.longitude),getFracPart(status_GCJ.longitude,5));

  //BD-09: Baidu coordinate offset standard, Baidu Map
  gps_status_t status_BD = GPS.WGSToBD(status_WGS);
  // printf double latitude
  Serial.printf("BD-09: latitude:%d.%d",getIntPart(status_BD.latitude),getFracPart(status_BD.latitude,5));  
  // printf double longitude
  Serial.printf(",longitude:%d.%d\r\n",getIntPart(status_BD.longitude),getFracPart(status_BD.longitude,5));
}
