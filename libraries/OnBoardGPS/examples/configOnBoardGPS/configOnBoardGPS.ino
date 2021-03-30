#include "Arduino.h"
#include "GPS_Air530.h"
#include "GPS_Air530Z.h"

//if GPS module is Air530, use this
Air530Class GPS;

//if GPS module is Air530Z, use this
//Air530ZClass GPS;

void setup() {
  Serial.begin(115200);
  
  /*GPS uart baud: 
  * Air530Z support : 9600,19200,38400,57600,115200.
  * Air530  support : 9600,19200,38400,57600,115200,230400,460800
  * default is 9600
  */
  GPS.begin(9600);
  
  /*GPS mode set: 
  * GPS        :        MODE_GPS
  * BEIDOU     :        MODE_BEIDOU
  * GLONASS    :        MODE_GLONASS             //only Air530Z supported.
  * GPS+BEIDOU :        MODE_GPS_BEIDOU
  * GSP+GLONASS:        MODE_GPS_GLONASS
  * GPS+BEIDOU+GLONASS: MODE_GPS_BEIDOU_GLONASS  //only Air530Z supported.
  * For Air530, default mode is GPS+BEIDOU.
  * For Air530Z, default mode is GPS+BEIDOU+GLONASS.
  */
//  GPS.setmode(MODE_GPS_GLONASS);
  
  /*supported nmea sentence :
  * GLL, RMC, VTG, GGA, GSA, GSV
  */
//  GPS.setNMEA(NMEA_GSV);
}

void loop()
{
  /*get nmea sentence
  * GPS.getAll() to get all nmea sentence;
  * GPS.getNMEA() to get an any kind of nmea sentence;
  * GPS.getRMC() to get RMC sentence;
  * GPS.getGGA() to get GGA sentence;
  * GPS.getGSA() to get GSA sentence;
  * GPS.getGSV() to get GSV sentence;
  * GPS.getGLL() to get GLL sentence;
  * GPS.getVTG() to get VTG sentence;
  */
  String NMEA = GPS.getAll(); 
  if(NMEA != "0")
  {
    Serial.println(NMEA);
  }
}
