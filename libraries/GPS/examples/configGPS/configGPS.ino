//the GPS module used is Air530.
#include "Arduino.h"
#include "GPS_Air530.h"

void setup() {
  Serial.begin(115200);
  GPS.begin();
  
  /*three modes supported: 
  * GPS        :    MODE_GPS
  * GPS+BEIDOU :    MODE_GPS_BEIDOU
  * GSP+GLONASS:    MODE_GPS_GLONASS
  * default mode is GPS+BEIDOU.
  */
  //GPS.setmode(MODE_GPS_BEIDOU);
  
  /*supported nmea sentence :
  * GLL, RMC, VTG, GGA, GSA, GST
  */
  GPS.setNMEA(NMEA_GGA|NMEA_GSA|NMEA_RMC|NMEA_VTG);
  
  /* set PPS mode and pulse width.
  *  void setPPS(uint8_t mode, uint16_t pulse_width = 500);
  *  mode : 0, pps off;
  *         1, fisrt fixed;
  *         2, 3D fixed;
  *         3, 2D/3D fixed;
  *         4, always on;
  *  width : max value is 998 ms, default value is 500ms;
  */
  GPS.setPPS(3,200);
}

void loop()
{
  //get all nmea sentence
  String NMEA = GPS.getNMEA();
  if(NMEA != "0")
  {
    Serial.println(NMEA);
  }
  
  //only get VTG sentence
  String VTG = GPS.getVTG();
  if(VTG != "0")
  {
    Serial.println(VTG);
  }  
  
  //only get RMC sentence
  String RMC = GPS.getRMC();
  if(RMC != "0")
  {
    Serial.println(RMC);
  }
  
  //only get GGA sentence
  String GGA = GPS.getGGA();
  if(GGA != "0")
  {
    Serial.println(GGA);
  } 
}