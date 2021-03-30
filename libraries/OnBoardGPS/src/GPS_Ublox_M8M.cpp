#include "Arduino.h"
#include "GPS_Ublox_M8M.h"

static void calchecksum(uint8_t * cmd,int size)
{
	uint8_t checksumA=0;
	uint8_t checksumB=0;

	for(int i=0;i<(size-2);i++)
	{
		checksumA = checksumA + cmd[i];
		checksumB = checksumB + checksumA;
	}
	cmd[size-2]=checksumA;
	cmd[size-1]=checksumB;
}

static bool checkcmdback(uint8_t * cmd)
{
	uint8_t checksumA=0;
	uint8_t checksumB=0;

	for(int i=0;i<(8-2);i++)
	{
		checksumA = checksumA + cmd[i];
		checksumB = checksumB + checksumA;
	}
	if(checksumA==cmd[8-2] && checksumB==cmd[8-1])
		return true;
	return false;
}

M8MClass::M8MClass(int8_t powerCtl) 
	:_powerCtl(powerCtl) 
	{}

void M8MClass::begin(uint32_t baud)
{
	pinMode(_powerCtl,OUTPUT);
	digitalWrite(_powerCtl, LOW);

	int i = 0;
	//String cmd="$PGKC147,"+(String)baud;
	
	GPSSerial.begin(bauds[i]);

	Serial.println("Current GPS baudrate detecting...");
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
	}
	Serial.print("GPS baudrate detected:");
	Serial.println(bauds[i]);
	setVersion();
	setmode(MODE_GPS_GALILEO_BEIDOU);
	//update GPS serial baudrate;
	uint8_t cmd[] = {0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x80, \
					0x25, 0x00, 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x0a};

	*(uint32_t *)&cmd[12] = baud;
	calchecksum(cmd,sizeof(cmd));

	sendcmd(cmd,sizeof(cmd));
	GPSSerial.updateBaudRate(baud);
	GPSSerial.flush();

	Serial.println("GPS baudrate updating... ");
	
	while(getNMEA() =="0")
	{
		GPSSerial.updateBaudRate(bauds[i]);
		delay(50);
		sendcmd(cmd,sizeof(cmd));
		delay(50);
		GPSSerial.updateBaudRate(baud);
		delay(50);
		GPSSerial.flush();
	}

	Serial.print("GPS baudrate updated to ");
	Serial.println(baud);

	_baud = baud;
	
}

bool CmdBackOK()
{
	uint8_t cmdback[8];
	int size = 0;
	uint32_t starttime = millis();
	while(millis() - starttime <1000)
	{
		if ( GPSSerial.available())
		{
			uint8_t c = GPSSerial.read();
			if(c==0x05)
			{
				cmdback[size++]=c;
				while(size<8)
				{
					if ( GPSSerial.available())
						cmdback[size++]=GPSSerial.read();
				}
/*
				Serial.printf("cmd back size : %d ",size);
				for(int i = 0;i<size;i++)
				{
					Serial.printf("%02X ",cmdback[i]);
				}
				Serial.println();
*/
				if(checkcmdback(cmdback)&&cmdback[1]==0x01)
				{
					return true;
				}
			}
		}
	}
	return false;
}
void M8MClass::setVersion()
{
	uint8_t cmd[]={0x06,0x17,0x14,0x00,0x00,0x41,0x00,0x02,0x00,0x00,0x00,0x00,0x00,\
					0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x75,0x57};
	sendcmd(cmd,sizeof(cmd));
	while(CmdBackOK()==false)
	{
		sendcmd(cmd,sizeof(cmd));
	}
}
void M8MClass::setmode(GPSMODE mode)
{
	uint8_t cmd[]={0x06, 0x3E, 0x3C, 0x00, 0x00, 0x00, 0x20, 0x07, 0x00, 0x08, 0x10, 0x00, 0x01,0x00, 0x01, 0x01, 0x01, 0x01,\
					0x03, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02, 0x04, 0x08, 0x00, 0x01, 0x00, 0x01, 0x01, 0x03, 0x08, 0x10, 0x00,\
					0x00, 0x00, 0x01, 0x01, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x01, 0x05, 0x00, 0x03, 0x00, 0x00, 0x00,\
					0x01, 0x01, 0x06, 0x08, 0x0E, 0x00, 0x01, 0x00, 0x01, 0x01, 0x2E, 0x75};
	switch(mode)
	{
		case MODE_GPS_GALILEO_BEIDOU:
			cmd[36]=1;
			cmd[60]=0;
			break;
		case MODE_GPS_GALILEO_GLONASS:
			cmd[36]=0;
			cmd[60]=1;
			break;
		default:
			break;
	}
	calchecksum(cmd,sizeof(cmd));
	sendcmd(cmd,sizeof(cmd));
	while(CmdBackOK()==false)
	{
		sendcmd(cmd,sizeof(cmd));
	}
}

void M8MClass::setNMEA(uint8_t nmeamode)
{
	uint8_t cmd[] = {0x06, 0x01, 0x08, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x00};
	for(int i = 0;i < 6;i++)
	{
		if(nmeamode & (0x01<<i))
		{
			cmd[5] = i;
			cmd[7] = 0x01;
		}
		else
		{
			cmd[5] = i;
			cmd[7] = 0x00;
		}
		
		calchecksum(cmd,sizeof(cmd));
		sendcmd(cmd,sizeof(cmd));
		while(CmdBackOK()==false)
		{
			sendcmd(cmd,sizeof(cmd));
		}
	}
}

void M8MClass::reset()
{
	//String cmd = "$PGKC030,3,1*2E\r\n";
	//sendcmd(cmd);
}

int M8MClass::available(void)
{
	return GPSSerial.available();
}

int M8MClass::read(void)
{
	return GPSSerial.read();
}

void M8MClass::clear()
{
	String cmd = "$PGKC040*2B\r\n";
	//sendcmd(cmd);
}

void M8MClass::setPPS(uint8_t mode, uint16_t pulse_width)
{

}

void M8MClass::end()
{
	digitalWrite(_powerCtl, HIGH);
	GPSSerial.end();
}

void M8MClass::sendcmd(uint8_t * cmd,int size)
{

	while(GPSSerial.available())//wait for gps serial idel
	{
		GPSSerial.readStringUntil('\r');
	}
	GPSSerial.write(0xB5);
	GPSSerial.write(0x62);
	GPSSerial.write(cmd,size);
}

String M8MClass::getNMEA()
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

String M8MClass::getRMC()
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


String M8MClass::getGGA()
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

String M8MClass::getVTG()
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

String M8MClass::getGSA()
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


String M8MClass::getGSV()
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

String M8MClass::getGLL()
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

String M8MClass::getAll()
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
/*    
 * WGS-84: international standard GPS coordinate£¨Google Earth¡¢ GPS module¡¢Tian Map£©
 * GCJ-02: China coordinate migration standard, Google Map¡¢Gaode Map¡¢Tencent map
 * BD-09:  Baidu coordinate offset standard, Baidu Map
*/

//WGS-84 to GCJ-02
/*
gps_status_t M8MClass::WGSToGCJ(gps_status_t status) {
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

gps_status_t M8MClass::GCJToBD(gps_status_t status){
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

gps_status_t M8MClass::WGSToBD(gps_status_t status)
{
	status = WGSToGCJ(status);
	status = GCJToBD(status);
	return status;
}
*/


//M8MClass GPS(GPIO14);
M8MClass M8M(GPIO14);


