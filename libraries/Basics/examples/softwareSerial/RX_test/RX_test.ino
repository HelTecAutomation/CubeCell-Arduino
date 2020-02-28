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
#include <stdlib.h>

softSerial softwareSerial(GPIO1 /*TX pin*/, GPIO2 /*TX pin*/);

extern uint8_t IRREC_RX_BUF[64];    //用于存放接收到的数据，所有数据都放到这个里面
extern uint16_t IRREC_RX_CNT;       //接收到的字符个数
extern uint8_t print_Sign;
extern uint8_t rebit;

softSerial softwareSerial;

void setup()
{
	Serial.begin(115200);
	Serial.println("Normal serial init");
	//software serial init
	softwareSerial.begin(9600);
}

void loop(){
	delay(3000);
	if(print_Sign)
	{
		uint8_t num;
		for(num = 0; num < IRREC_RX_CNT; num++)
		{
			Serial.write(IRREC_RX_BUF[num]); 
		}
		memset(IRREC_RX_BUF, 0, 64*sizeof(uint8_t));
		IRREC_RX_CNT = 0;
		print_Sign = 0;  //clear flag
	}
}