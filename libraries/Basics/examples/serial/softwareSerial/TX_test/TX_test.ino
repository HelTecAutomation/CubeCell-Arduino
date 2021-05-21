/*
 * HelTec Automation(TM) CubeCell software serial example
 *
 * Function summary:
 * 
 * - date print example via software serial port;
 * - baudrate defined in softwareSerial.begin(9600);
 * |-- Supported baudrate
 * |-- 57600
 * |-- 38400
 * |-- 19200
 * |-- 14400
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

// any free GPIOs can be TX or RX
softSerial softwareSerial(GPIO1 /*TX pin*/, GPIO2 /*RX pin*/);

void setup()
{
	Serial.begin(115200);
	Serial.println("Hardware Serial init");
	// software serial port init
	softwareSerial.begin(9600);
}

void loop()
{
	char  a[] = "12345";
  	uint16_t b  = 45678;

	while(1)
	{
		Serial.println("send...");
		softwareSerial.println("Aaron is a very handsome guy! hahahaha");
		softwareSerial.printf("a = %s\r\n", a);
		softwareSerial.printf("b = %d\r\n", b);
		softwareSerial.printf("b in hex format = %X\r\n", b);
		delay(3000);
	}
}