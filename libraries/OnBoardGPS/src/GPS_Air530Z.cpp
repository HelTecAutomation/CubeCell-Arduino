#include "Arduino.h"
#include "GPS_Air530Z.h"

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

Air530ZClass::Air530ZClass(uint8_t powerCtl,uint8_t modePin) 
	:_powerCtl(powerCtl),_modePin(modePin)
	{}

void Air530ZClass::begin(uint32_t baud)
{
	pinMode(_powerCtl,OUTPUT);
	digitalWrite(_powerCtl, LOW);

	int i = 0;
	GPSSerial.begin(bauds[i]);
	String temp = "";

	Serial.println("GPS Current baudrate detecting...");
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
	Serial.print("GPS Current baudrate detected:");
	Serial.println(bauds[i]);

	setBaud(baud);
	GPSSerial.updateBaudRate(baud);
	delay(50);
	GPSSerial.flush();

	Serial.printf("GPS baudrate updating to %d\r\n",baud);
	uint32_t timeTemp = millis();

	while(getNMEA() =="0")
	{
		if((millis()-timeTemp)<5000)
		{
			GPSSerial.updateBaudRate(bauds[i]);
			delay(50);
			setBaud(baud);
			delay(50);
			GPSSerial.updateBaudRate(baud);
			delay(50);
			GPSSerial.flush();
		}
		else
		{
			break;
		}
	}

	if( (millis()-timeTemp)>5000)
	{
		GPSSerial.updateBaudRate(bauds[i]);
		_baud = bauds[i];
		Serial.printf("GPS baudrate updated failed, use GPS baudrate %d\r\n",_baud);
	}
	else
	{
		_baud = baud;
		Serial.print("GPS baudrate updated to ");
		Serial.println(baud);
	}
	
	delay(10);
	setmode(MODE_GPS_BEIDOU_GLONASS);
	delay(10);
	setNMEA(NMEA_GSV|NMEA_GGA|NMEA_GSA|NMEA_RMC|NMEA_VTG|NMEA_GLL);
	delay(10);
}


void Air530ZClass::setmode(GPSMODE mode)
{
	String cmd="$PCAS04,7";
	switch(mode)
	{
		case MODE_GPS_BEIDOU:
			cmd[8] = '3';
			break;
		case MODE_GPS_GLONASS:
			cmd[8] = '5';
			break;
		case MODE_GPS_BEIDOU_GLONASS:
			cmd[8] = '7';
			break;
		case MODE_GPS:
			cmd[8] = '1';
			break;
		case MODE_BEIDOU:
			cmd[8] = '2';
			break;
		case MODE_GLONASS:
			cmd[8] = '4';
			break;
		default:
			cmd[8] = '7';
			break;
	}
	cmd = calchecksum(cmd);
	sendcmd(cmd);
}

void Air530ZClass::setNMEA(uint8_t nmeamode)
{
	String cmd = "$PCAS03,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
	if(nmeamode & NMEA_GGA)
	{
		cmd[8] = '1';
	}
	if(nmeamode & NMEA_GLL)
	{
		cmd[10] = '1';
	}
	if(nmeamode & NMEA_GSA)
	{
		cmd[12] = '1';
	}
	if(nmeamode & NMEA_GSV)
	{
		cmd[14] = '1';
	}
	if(nmeamode & NMEA_RMC)
	{
		cmd[16] = '1';
	}
	if(nmeamode & NMEA_VTG)
	{
		cmd[18] = '1';
	}
	if(nmeamode & NMEA_GST)
	{
		cmd[36] = '1';
	}

	cmd = calchecksum(cmd);
	
	sendcmd(cmd);
}

void Air530ZClass::setBaud(uint32_t baud)
{
	String cmd = "$PCAS01,1";
	switch(baud)
	{
		case 9600:
			cmd[8] = '1';
			break;
		case 19200:
			cmd[8] = '2';
			break;
		case 38400:
			cmd[8] = '3';
			break;
		case 57600:
			cmd[8] = '4';
			break;
		case 115200:
			cmd[8] = '5';
			break;
		default:
			cmd[8] = '1';
			break;
	}
	cmd = calchecksum(cmd);
	sendcmd(cmd);
}

int Air530ZClass::available(void)
{
	return GPSSerial.available();
}

int Air530ZClass::read(void)
{
	return GPSSerial.read();
}


void Air530ZClass::end()
{
	digitalWrite(_powerCtl, HIGH);
	GPSSerial.end();
}

void Air530ZClass::sendcmd(String cmd)
{

	while(GPSSerial.available())//wait for gps serial idel
	{
		GPSSerial.readStringUntil('\r');
	}
	GPSSerial.print(cmd);
}

String Air530ZClass::getAll()
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

String Air530ZClass::getNMEA()
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

String Air530ZClass::getRMC()
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


String Air530ZClass::getGGA()
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

String Air530ZClass::getVTG()
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

String Air530ZClass::getGSA()
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


String Air530ZClass::getGSV()
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

String Air530ZClass::getGLL()
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

