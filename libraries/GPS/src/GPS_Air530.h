#ifndef GPS_Air530_H
#define GPS_Air530_H

#include "Arduino.h"

typedef enum
{
	MODE_GPS = 0,
	MODE_GPS_BEIDOU,
	MODE_GPS_GLONASS,
}GPSMODE;

typedef struct gpsTime_s
{
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}gpsTime_t;

typedef struct gps_status_s
{
	int validation;
	double latitude;
	double longitude;
	int altitude;
	float hdop;
	float speed;
	gpsTime_t time;
}gps_status_t;


#define NMEA_GLL 0x01
#define NMEA_RMC 0x02
#define NMEA_VTG 0x04
#define NMEA_GGA 0x08
#define NMEA_GSA 0x10
#define NMEA_GSV 0x20
#define NMEA_GRS 0x40
#define NMEA_GST 0x80

class Air530Class{
public:
	Air530Class(int8_t powerCtl);
	void begin();
	void reset();
	void setmode(GPSMODE mode);
	void setPPS(uint8_t mode, uint16_t pulse_width = 500);
	void setNMEA(uint8_t nmeamode);
	void clear();
	String getNMEA();
	String getRMC();
	String getGGA();
	String getVTG();
	gps_status_t status();
	gps_status_t WGSToGCJ(gps_status_t status);
	gps_status_t GCJToBD(gps_status_t status);
	gps_status_t WGSToBD(gps_status_t status);
	void sendcmd(String cmd);
	void end();

private:
	uint8_t _powerCtl;
	gps_status_t gps_status;
};

extern Air530Class GPS;

#endif

