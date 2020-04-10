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
  
  gps_status_t gpsStatus = GPS.status();
  
  Serial.printf("Date : %02d-%02d-%02d Time : %02d:%02d:%02d\r\n",
      gpsStatus.time.year,gpsStatus.time.month,gpsStatus.time.day,
      gpsStatus.time.hour,gpsStatus.time.minute,gpsStatus.time.second);
      
  // printf double latitude
  Serial.printf("latitude:%d.%d",getIntPart(gpsStatus.latitude),getFracPart(gpsStatus.latitude,5));
  
  // printf double longitude
  Serial.printf(",longitude:%d.%d",getIntPart(gpsStatus.longitude),getFracPart(gpsStatus.longitude,5));

  Serial.print(",altitude:");
  Serial.print(gpsStatus.altitude);
  Serial.print(", hdop:");
  Serial.print(gpsStatus.hdop);
  Serial.print(", speed:");
  Serial.print(gpsStatus.speed);
  Serial.println();

  if(gpsStatus.validation == 0)
  {
    Serial.println("GPS is invalid, the location info is the last valid info.");
  }

}
