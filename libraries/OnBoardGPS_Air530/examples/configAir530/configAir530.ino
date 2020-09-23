//the GPS module used is Air530.
#include "Arduino.h"
#include "GPS_Air530.h"

void setup() {
  Serial.begin(115200);
  Air530.begin();
  
  /*three modes supported: 
  * GPS        :    MODE_GPS
  * GPS+BEIDOU :    MODE_GPS_BEIDOU
  * GSP+GLONASS:    MODE_GPS_GLONASS
  * default mode is GPS+BEIDOU.
  */
  //Air530.setmode(MODE_GPS_BEIDOU);
  
  /*supported nmea sentence :
  * GLL, RMC, VTG, GGA, GSA, GSV
  */
//  Air530.setNMEA(NMEA_GGA|NMEA_GSA|NMEA_RMC|NMEA_VTG);
  
  /* set PPS mode and pulse width.
  *  void setPPS(uint8_t mode, uint16_t pulse_width = 500);
  *  mode : 0, pps off;
  *         1, fisrt fixed;
  *         2, 3D fixed;
  *         3, 2D/3D fixed;
  *         4, always on;
  *  width : max value is 998 ms, default value is 500ms;
  */
//  Air530.setPPS(3,200);
}

void loop()
{
  /*get nmea sentence
  * GPS.getNMEA() to get an any kind of nmea sentence;
  * GPS.getRMC() to get RMC sentence;
  * GPS.getGGA() to get GGA sentence;
  * GPS.getGSA() to get GSA sentence;
  * GPS.getGSV() to get GSV sentence;
  * GPS.getGLL() to get GLL sentence;
  * GPS.getVTG() to get VTG sentence;
  */
  String NMEA = Air530.getNMEA(); 
  if(NMEA != "0")
  {
    Serial.println(NMEA);
  }
}