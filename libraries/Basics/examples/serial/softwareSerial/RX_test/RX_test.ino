/*
 * HelTec Automation(TM) CubeCell software serial example
 *
 * Function summary:
 * 
 * - software read data from a UART device and print received data via normal serial prot
 * - baudrate defined in softwareSerial.begin(9600);
 * |-- Supported baudrate
 * |-- 14400 (Not statble)
 * |-- 9600
 * |-- 4800
 * |-- 2400
 * |-- 1200
 *
 * HelTec AutoMation, Chengdu, China.
 * 成都惠利特自动化科技有限公司
 * https://heltec.org
 * support@heltec.cn
 *
 * this project also release in GitHub:
 * 
 */

#include "softSerial.h"
// #include <stdlib.h>

softSerial softwareSerial(GPIO1 /*TX pin*/, GPIO2 /*RX pin*/);

void setup()
{
	Serial.begin(115200);
	Serial.println("Normal serial init");
	//software serial init
	softwareSerial.begin(9600);
}

void loop()
{
	if(softwareSerial.available())
	{
		char serialbuffer[64] = {0};
		int i = 0;
		while (softwareSerial.available() && i<63)
		{
			serialbuffer[i] = (char)softwareSerial.read();
			i++;
		}
		serialbuffer[i] = '\0';
		if(serialbuffer[0])
		{
			Serial.print("Received data from software Serial:");
			Serial.println(serialbuffer);
		}
	}
	 delay(1000);
}