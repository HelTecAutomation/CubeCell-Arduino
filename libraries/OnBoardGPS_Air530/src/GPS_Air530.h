#ifndef GPS_Air530_H
#define GPS_Air530_H

#include "Arduino.h"
#include <CubeCell_TinyGPS++.h>

typedef enum
{
	MODE_GPS = 0,
	MODE_GPS_BEIDOU,
	MODE_GPS_GLONASS,
}GPSMODE;

#define NMEA_GLL 0x01
#define NMEA_RMC 0x02
#define NMEA_VTG 0x04
#define NMEA_GGA 0x08
#define NMEA_GSA 0x10
#define NMEA_GSV 0x20
#define NMEA_GRS 0x40
#define NMEA_GST 0x80

class Air530Class:public TinyGPSPlus{
public:
	Air530Class(int8_t powerCtl);
	void begin(uint32_t baud = 9600);
	void reset();
	void setmode(GPSMODE mode);
	void setPPS(uint8_t mode, uint16_t pulse_width = 500);
	void setNMEA(uint8_t nmeamode);
	void clear();
	int available(void);
	int read(void);
	String getNMEA();
	String getRMC();
	String getGGA();
	String getVTG();
	String getGSV();
	String getGSA();
	String getGLL();
/*	gps_status_t WGSToGCJ(gps_status_t status);
	gps_status_t GCJToBD(gps_status_t status);
	gps_status_t WGSToBD(gps_status_t status);*/
	void sendcmd(String cmd);
	void end();

private:
	uint8_t _powerCtl;
	uint32_t _baud;
};

extern Air530Class Air530;

#endif

