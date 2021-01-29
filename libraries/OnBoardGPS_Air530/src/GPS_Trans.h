#ifndef GPS_TRANS_H
#define GPS_TRANS_H


#include "Arduino.h"
#include <CubeCell_TinyGPS++.h>


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

