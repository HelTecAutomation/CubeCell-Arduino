#ifndef GPS_Air530_H
#define GPS_Air530_H

#include "Arduino.h"
#include <CubeCell_TinyGPS++.h>
#include "GPS_Trans.h"


class Air530Class:public TinyGPSPlus{
public:
	Air530Class(uint8_t powerCtl=GPIO14);
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
	String getAll();
/*	gps_status_t WGSToGCJ(gps_status_t status);
	gps_status_t GCJToBD(gps_status_t status);
	gps_status_t WGSToBD(gps_status_t status);*/
	void sendcmd(String cmd);
	void end();

private:
	uint8_t _powerCtl;
	uint32_t _baud;
};

//extern Air530Class Air530;

#endif

