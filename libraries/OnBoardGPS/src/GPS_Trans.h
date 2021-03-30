#ifndef GPS_TRANS_H
#define GPS_TRANS_H


#include "Arduino.h"
#include <CubeCell_TinyGPS++.h>

typedef enum
{
	MODE_GPS_BEIDOU=0,
	MODE_GPS_GLONASS,
	MODE_GPS_GALILEO_BEIDOU,
	MODE_GPS_GALILEO_GLONASS,
	MODE_GPS_BEIDOU_GLONASS,
	MODE_GPS,
	MODE_BEIDOU,
	MODE_GLONASS,
}GPSMODE;

#define NMEA_GGA 0x01
#define NMEA_GLL 0x02
#define NMEA_GSA 0x04
#define NMEA_GSV 0x08
#define NMEA_RMC 0x10
#define NMEA_VTG 0x20
#define NMEA_GRS 0x40
#define NMEA_GST 0x80

extern double gps_x_pi;
extern double gps_pi;
extern double gps_a;
extern double gps_ee;
extern uint32_t bauds[];
extern uint32_t bauds_array;

int str_chop(char *s, int buff_size, char separator, uint8_t *idx_ary, int max_idx);
double transformLon(double x, double y);
double transformLat(double x, double y);
double outOfChina(double lat, double lon);
extern HardwareSerial GPSSerial;

#endif

