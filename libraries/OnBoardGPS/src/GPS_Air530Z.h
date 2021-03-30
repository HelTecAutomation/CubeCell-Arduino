#ifndef GPS_Air530Z_H
#define GPS_Air530Z_H

#include "Arduino.h"
#include <CubeCell_TinyGPS++.h>
#include "GPS_Trans.h"

class Air530ZClass:public TinyGPSPlus{
public:
	Air530ZClass(uint8_t powerCtl=GPIO14,uint8_t modePin=GPIO11);
	void begin(uint32_t baud = 9600);
	void setmode(GPSMODE mode);
	void setNMEA(uint8_t nmeamode);
	int available(void);
	void setBaud(uint32_t baud);
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
	uint8_t _modePin;
};

//extern Air530ZClass Air530;

#endif

