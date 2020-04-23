#include "Arduino.h"
#include "GPS_Air530.h"

HardwareSerial GPSSerial(UART_NUM_1);

double gps_x_pi = 3.14159265358979324 * 3000.0 / 180.0;
double gps_pi = 3.14159265358979324;
double gps_a = 6378245.0;
double gps_ee = 0.00669342162296594323;

int str_chop(char *s, int buff_size, char separator, uint8_t *idx_ary, int max_idx);
double transformLon(double x, double y);
double transformLat(double x, double y);
double outOfChina(double lat, double lon);


double transformLon(double x, double y) {
	double ret = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1 * sqrt(abs(x));
	ret += (20.0 * sin(6.0 * x * gps_pi) + 20.0 * sin(2.0 * x * gps_pi)) * 2.0 / 3.0;
	ret += (20.0 * sin(x * gps_pi) + 40.0 * sin(x / 3.0 * gps_pi)) * 2.0 / 3.0;
	ret += (150.0 * sin(x / 12.0 * gps_pi) + 300.0 * sin(x / 30.0 * gps_pi)) * 2.0 / 3.0;
	return ret;
};
double transformLat(double x, double y) {
	double ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y + 0.2 * sqrt(abs(x));
	ret += (20.0 * sin(6.0 * x * gps_pi) + 20.0 * sin(2.0 * x * gps_pi)) * 2.0 / 3.0;
	ret += (20.0 * sin(y * gps_pi) + 40.0 * sin(y / 3.0 * gps_pi)) * 2.0 / 3.0;
	ret += (160.0 * sin(y / 12.0 * gps_pi) + 320 * sin(y * gps_pi / 30.0)) * 2.0 / 3.0;
	return ret;
}

double outOfChina(double lat, double lon) {
	if (lon < 72.004 || lon > 137.8347)
		return true;
	if (lat < 0.8293 || lat > 55.8271)
		return true;
	return false;
}

String calchecksum(String cmd)
{
	uint8_t checksum=cmd[1];
	char temp[5];
	for(int i=2;i<cmd.length();i++)
	{
		checksum^=cmd[i];
	}
	memset(temp,0,5);
	sprintf(temp, "*%02X\r\n", checksum);
	cmd += temp;
	return cmd;
}

Air530Class::Air530Class(int8_t powerCtl) 
	:_powerCtl(powerCtl) 
	{}

void Air530Class::begin()
{
	pinMode(_powerCtl,OUTPUT);
	digitalWrite(_powerCtl, LOW);
	GPSSerial.begin(9600);
	delay(1000);
}


void Air530Class::setmode(GPSMODE mode)
{
	String cmd="";
	switch(mode)
	{
		case MODE_GPS:
			cmd += "$PGKC115,1,0,0,0*2B\r\n";
			break;
		case MODE_GPS_BEIDOU:
			cmd += "$PGKC115,1,0,1,0*2A\r\n";
			break;
		case MODE_GPS_GLONASS:
			cmd += "$PGKC115,1,1,0,0*2A\r\n";
			break;
		default:
			break;
	}
	sendcmd(cmd);
}

void Air530Class::setNMEA(uint8_t nmeamode)
{
	String cmd = "$PGKC242,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
	if(nmeamode & NMEA_GLL)
	{
		cmd[9] = '1';
	}
	if(nmeamode & NMEA_RMC)
	{
		cmd[11] = '1';
	}
	if(nmeamode & NMEA_VTG)
	{
		cmd[13] = '1';
	}
	if(nmeamode & NMEA_GGA)
	{
		cmd[15] = '1';
	}
	if(nmeamode & NMEA_GSA)
	{
		cmd[17] = '1';
	}
	if(nmeamode & NMEA_GSV)
	{
		cmd[19] = '1';
	}
	if(nmeamode & NMEA_GRS)
	{
		cmd[21] = '1';
	}
	if(nmeamode & NMEA_GST)
	{
		cmd[23] = '1';
	}

	cmd = calchecksum(cmd);
	
	sendcmd(cmd);
}

void Air530Class::reset()
{
	String cmd = "$PGKC030,3,1*2E\r\n";
	sendcmd(cmd);
}

int Air530Class::available(void)
{
	return GPSSerial.available();
}

int Air530Class::read(void)
{
	return GPSSerial.read();
}

void Air530Class::clear()
{
	String cmd = "$PGKC040*2B\r\n";
	sendcmd(cmd);
}

void Air530Class::setPPS(uint8_t mode, uint16_t pulse_width)
{

	String cmd = "$PGKC161,";
	char temp[15];

	if(pulse_width>=999)
	{
		pulse_width = 998;
	}
	memset(temp,0,15);
	sprintf(temp, "%d,%d,%d", mode,pulse_width,1000);
	cmd += temp;

	cmd = calchecksum(cmd);
	sendcmd(cmd);
}

void Air530Class::end()
{
	digitalWrite(_powerCtl, HIGH);
	GPSSerial.end();
}

void Air530Class::sendcmd(String cmd)
{

	while(GPSSerial.available())//wait for gps serial idel
	{
		GPSSerial.readStringUntil('\n');
	}
	GPSSerial.print(cmd);
}

String Air530Class::getNMEA()
{
	uint32_t starttime = millis();
	String nmea = "";
	while(millis() - starttime <1000)
	{
		if(GPSSerial.available())
		{
			char c = GPSSerial.read();
			if(c=='$')
			{
				nmea += c;
				nmea += GPSSerial.readStringUntil('\n');
				return nmea;
			}
		}
	}
	return "0";
}

String Air530Class::getRMC()
{
	String nmea = "";
	uint32_t starttime = millis();
	while(millis() - starttime <1000)
	{
		if ( GPSSerial.available())
		{
			char c = GPSSerial.read();
			if(c=='$')
			{
				nmea = GPSSerial.readStringUntil('\n');
				if(nmea[2] == 'R' && nmea[3] == 'M' && nmea[4] == 'C')
				{
					nmea = c + nmea;
					return nmea;
				}
			}
		}
	}
	return "0";
}


String Air530Class::getGGA()
{
	String nmea = "";
	uint32_t starttime = millis();
	while(millis() - starttime <1000)
	{
		if ( GPSSerial.available())
		{
			char c = GPSSerial.read();
			if(c=='$')
			{
				nmea = GPSSerial.readStringUntil('\n');
				if(nmea[2] == 'G' && nmea[3] == 'G' && nmea[4] == 'A')
				{
					nmea = c + nmea;
					return nmea;
				}
			}
		}
	}
	return "0";
}

String Air530Class::getVTG()
{
	String nmea = "";
	uint32_t starttime = millis();
	while(millis() - starttime <1000)
	{
		if ( GPSSerial.available())
		{
			char c = GPSSerial.read();
			if(c=='$')
			{
				nmea = GPSSerial.readStringUntil('\n');
				if(nmea[2] == 'V' && nmea[3] == 'T' && nmea[4] == 'G')
				{
					nmea = c + nmea;
					return nmea;
				}
			}
		}
	}
	return "0";
}

String Air530Class::getGSA()
{
	String nmea = "";
	uint32_t starttime = millis();
	while(millis() - starttime <1000)
	{
		if ( GPSSerial.available())
		{
			char c = GPSSerial.read();
			if(c=='$')
			{
				nmea = GPSSerial.readStringUntil('\n');
				if(nmea[2] == 'G' && nmea[3] == 'S' && nmea[4] == 'A')
				{
					nmea = c + nmea;
					return nmea;
				}
			}
		}
	}
	return "0";
}


String Air530Class::getGSV()
{
	String nmea = "";
	uint32_t starttime = millis();
	while(millis() - starttime <1000)
	{
		if ( GPSSerial.available())
		{
			char c = GPSSerial.read();
			if(c=='$')
			{
				nmea = GPSSerial.readStringUntil('\n');
				if(nmea[2] == 'G' && nmea[3] == 'S' && nmea[4] == 'V')
				{
					nmea = c + nmea;
					return nmea;
				}
			}
		}
	}
	return "0";
}

String Air530Class::getGLL()
{
	String nmea = "";
	uint32_t starttime = millis();
	while(millis() - starttime <1000)
	{
		if ( GPSSerial.available())
		{
			char c = GPSSerial.read();
			if(c=='$')
			{
				nmea = GPSSerial.readStringUntil('\n');
				if(nmea[2] == 'G' && nmea[3] == 'L' && nmea[4] == 'L')
				{
					nmea = c + nmea;
					return nmea;
				}
			}
		}
	}
	return "0";
}


/*    
 * WGS-84: international standard GPS coordinate£¨Google Earth¡¢ GPS module¡¢Tian Map£©
 * GCJ-02: China coordinate migration standard, Google Map¡¢Gaode Map¡¢Tencent map
 * BD-09:  Baidu coordinate offset standard, Baidu Map
*/

//WGS-84 to GCJ-02
/*
gps_status_t Air530Class::WGSToGCJ(gps_status_t status) {
	if (outOfChina(status.latitude, status.longitude)) {
		return status;
	}
	double dLat = transformLat(status.longitude - 105.0, status.latitude - 35.0);
	double dLon = transformLon(status.longitude - 105.0, status.latitude - 35.0);
	double radLat = status.latitude / 180.0 * gps_pi;
	double magic = sin(radLat);
	magic = 1 - gps_ee * magic * magic;
	double sqrtMagic = sqrt(magic);
	dLat = (dLat * 180.0) / ((gps_a * (1 - gps_ee)) / (magic * sqrtMagic) * gps_pi);
	dLon = (dLon * 180.0) / (gps_a / sqrtMagic * cos(radLat) * gps_pi);
	double lat = status.latitude + dLat;
	double lon = status.longitude + dLon;
	status.latitude = lat;
	status.longitude = lon;
	return status;
};

gps_status_t Air530Class::GCJToBD(gps_status_t status){
	double x = status.longitude;
	double y = status.latitude;
	double z = sqrt(x * x + y * y) + 0.00002 * sin(y * gps_x_pi);
	double theta = atan2(y, x) + 0.000003 * cos(x * gps_x_pi); 
	double bd_lon = z * cos(theta) + 0.0065;
	double bd_lat = z * sin(theta) + 0.006;
	status.latitude=bd_lat;
	status.longitude=bd_lon;
	return status;
};

gps_status_t Air530Class::WGSToBD(gps_status_t status)
{
	status = WGSToGCJ(status);
	status = GCJToBD(status);
	return status;
}
*/

int str_chop(char *s, int buff_size, char separator, uint8_t *idx_ary, int max_idx) {
    int i = 0; /* index in the string */
    int j = 0; /* index in the result array */

    if ((s == NULL) || (buff_size < 0) || (separator == 0) || (idx_ary == NULL) || (max_idx < 0)) {
        /* unsafe to do anything */
        return -1;
    }
    if ((buff_size == 0) || (max_idx == 0)) {
        /* nothing to do */
        return 0;
    }
    s[buff_size - 1] = 0; /* add string terminator at the end of the buffer, just to be sure */
    idx_ary[j] = 0;
    j += 1;
    /* loop until string terminator is reached */
    while (s[i] != 0) {
        if (s[i] == separator) {
            s[i] = 0; /* replace separator by string terminator */
            if (j >= max_idx) { /* no more room in the index array */
                return j;
            }
            idx_ary[j] = i+1; /* next token start after replaced separator */
            ++j;
        }
        ++i;
    }
    return j;
}


//Air530Class GPS(GPIO14);
Air530Class Air530(GPIO14);


