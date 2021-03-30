#include "Arduino.h"
#include "GPS_Air530.h"

static String calchecksum(String cmd)
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

Air530Class::Air530Class(uint8_t powerCtl) 
	:_powerCtl(powerCtl) 
	{}

void Air530Class::begin(uint32_t baud)
{
	pinMode(_powerCtl,OUTPUT);
	digitalWrite(_powerCtl, LOW);

	int i = 0;
	//String cmd="$PGKC147,"+(String)baud;
	
	String cmd="$PGKC149,0,"+(String)baud;

	cmd = calchecksum(cmd);
	
	GPSSerial.begin(bauds[i]);
	String temp = "";

	Serial.println("GPS current baudrate detecting...");
	while(getNMEA() == "0" )
	{
		i++;
		if( i== bauds_array )
		{
			i=0;
		}
		//Serial.println(bauds[i]);
		GPSSerial.updateBaudRate(bauds[i]);
		delay(50);
		GPSSerial.flush();
		temp = getNMEA();
	}
	Serial.print("GPS current baudrate detected:");
	Serial.println(bauds[i]);

	sendcmd(cmd);
	GPSSerial.updateBaudRate(baud);
	delay(50);
	GPSSerial.flush();

	Serial.println("GPS baudrate updating... ");
	
	while(getNMEA() =="0")
	{
		GPSSerial.updateBaudRate(bauds[i]);
		delay(50);
		sendcmd(cmd);
		delay(50);
		GPSSerial.updateBaudRate(baud);
		delay(50);
		GPSSerial.flush();
	}

	Serial.print("GPS baudrate updated to ");
	Serial.println(baud);

	_baud = baud;
}


void Air530Class::setmode(GPSMODE mode)
{
	String cmd="";
	switch(mode)
	{
		case MODE_GPS:
			cmd += "$PGKC115,1,0,0,0";
			break;
		case MODE_GPS_BEIDOU:
			cmd += "$PGKC115,1,0,1,0";
			break;
		case MODE_GPS_GLONASS:
			cmd += "$PGKC115,1,1,0,0";
			break;
		case MODE_BEIDOU:
			cmd += "$PGKC115,0,0,1,0";
			break;
		default:
			break;
	}
	cmd = calchecksum(cmd);
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
		GPSSerial.readStringUntil('\r');
	}
	GPSSerial.print(cmd);
}

String Air530Class::getAll()
{
	String nmea = "";
	uint32_t starttime = millis();
	while(millis() - starttime <1000)
	{
		if ( GPSSerial.available())
		{
			while(GPSSerial.available())
			{
				nmea+=(char)GPSSerial.read();
				int n = 0;
				while(n<=1000)
				{
					if(GPSSerial.available())
						break;
					delayMicroseconds(1);
					n++;
				}
			}
			return nmea;
		}
	}
	return "0";
}

String Air530Class::getNMEA()
{
	uint32_t starttime = millis();
	String nmea = "";
	char buff[128]; 
	int index = 0;
	while(millis() - starttime <1000)
	{
		if(GPSSerial.available())
		{
			char c = GPSSerial.read();
			if(c=='$')
			{
				nmea += c;
				index = GPSSerial.readBytesUntil('\r',buff,127);
				buff[index]=0;
				if(buff[index-3]!='*')
					return "0";

				nmea += (String)buff;
				return nmea;
			}
		}
	}
	return "0";
}

String Air530Class::getRMC()
{
	String nmea = "";
	String res="";
	uint32_t starttime = millis();
	while(millis() - starttime <1000)
	{
		if ( GPSSerial.available())
		{
			char c = GPSSerial.read();
			if(c=='$')
			{
				nmea = GPSSerial.readStringUntil('\r');
				if(nmea[2] == 'R' && nmea[3] == 'M' && nmea[4] == 'C')
				{
					res = res + c + nmea + "\r\n";
				}
			}
		}
	}
	if(res!="")
		return res;
	return "0";
}


String Air530Class::getGGA()
{
	String nmea = "";
	String res="";
	uint32_t starttime = millis();
	while(millis() - starttime <1000)
	{
		if ( GPSSerial.available())
		{
			char c = GPSSerial.read();
			if(c=='$')
			{
				nmea = GPSSerial.readStringUntil('\r');
				if(nmea[2] == 'G' && nmea[3] == 'G' && nmea[4] == 'A')
				{
					res = res + c + nmea + "\r\n";
				}
			}
		}
	}
	if(res!="")
		return res;
	return "0";
}

String Air530Class::getVTG()
{
	String nmea = "";
	String res="";
	uint32_t starttime = millis();
	while(millis() - starttime <1000)
	{
		if ( GPSSerial.available())
		{
			char c = GPSSerial.read();
			if(c=='$')
			{
				nmea = GPSSerial.readStringUntil('\r');
				if(nmea[2] == 'V' && nmea[3] == 'T' && nmea[4] == 'G')
				{
					res = res + c + nmea + "\r\n";
				}
			}
		}
	}
	if(res!="")
		return res;
	return "0";
}

String Air530Class::getGSA()
{
	String nmea = "";
	String res="";
	uint32_t starttime = millis();
	while(millis() - starttime <1000)
	{
		if ( GPSSerial.available())
		{
			char c = GPSSerial.read();
			if(c=='$')
			{
				nmea = GPSSerial.readStringUntil('\r');
				if(nmea[2] == 'G' && nmea[3] == 'S' && nmea[4] == 'A')
				{
					res = res + c + nmea + "\r\n";
				}
			}
		}
	}
	if(res!="")
		return res;
	return "0";
}


String Air530Class::getGSV()
{
	String nmea = "";
	String res="";
	uint32_t starttime = millis();
	while(millis() - starttime <1000)
	{
		if ( GPSSerial.available())
		{
			char c = GPSSerial.read();
			if(c=='$')
			{
				nmea = GPSSerial.readStringUntil('\r');
				if(nmea[2] == 'G' && nmea[3] == 'S' && nmea[4] == 'V')
				{
					res = res + c + nmea + "\r\n";
				}
			}
		}
	}
	if(res!="")
		return res;
	return "0";
}

String Air530Class::getGLL()
{
	String nmea = "";
	String res="";
	uint32_t starttime = millis();
	while(millis() - starttime <1000)
	{
		if ( GPSSerial.available())
		{
			char c = GPSSerial.read();
			if(c=='$')
			{
				nmea = GPSSerial.readStringUntil('\r');
				if(nmea[2] == 'G' && nmea[3] == 'L' && nmea[4] == 'L')
				{
					res = res + c + nmea + "\r\n";
				}
			}
		}
	}
	if(res!="")
		return res;
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

//Air530Class Air530(GPIO14);


