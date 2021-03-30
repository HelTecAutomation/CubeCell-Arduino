#ifndef GPS_M8M_H
#define GPS_M8M_H

#include "Arduino.h"
#include <CubeCell_TinyGPS++.h>
#include "GPS_Trans.h"


#define NMEA_GGA 0x01
#define NMEA_GLL 0x02
#define NMEA_GSA 0x04
#define NMEA_GSV 0x08
#define NMEA_RMC 0x10
#define NMEA_VTG 0x20


class M8MClass:public TinyGPSPlus{
public:
	M8MClass(int8_t powerCtl);
	void begin(uint32_t baud = 9600);
	void reset();
	void setmode(GPSMODE mode);
	void setPPS(uint8_t mode, uint16_t pulse_width = 500);
	void setNMEA(uint8_t nmeamode);
	void setVersion();
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
	String getAll();
/*	gps_status_t WGSToGCJ(gps_status_t status);
	gps_status_t GCJToBD(gps_status_t status);
	gps_status_t WGSToBD(gps_status_t status);*/
	void sendcmd(uint8_t * cmd,int size);
	void end();

private:
	uint8_t _powerCtl;
	uint32_t _baud;
};

extern M8MClass M8M;

#endif

