// Copyright (c) Heltec Automation. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "heltec.h"


Heltec_CubeCell::Heltec_CubeCell(){

     display = new SSD1306Wire(0x3c, SDA, SCL,  GEOMETRY_128_64);
}

Heltec_CubeCell::~Heltec_CubeCell(){
	delete display;
}

void Heltec_CubeCell::begin(bool DisplayEnable, bool SerialEnable) {


	VextON();

	// UART
	if (SerialEnable) {
		Serial.begin(115200);
		Serial.flush();
		delay(50);
		Serial.print("Serial initial done\r\n");
	}

	// OLED
	if (DisplayEnable)
	{
		display->init();
		display->flipScreenVertically();
		display->setFont(ArialMT_Plain_10);
		display->drawString(0, 0, "OLED initial done!");
		display->display();
		delay(300);
		if (SerialEnable){
			Serial.print("you can see OLED printed OLED initial done!\r\n");
		}

	}
}

void Heltec_CubeCell::VextON(void)
{
	pinMode(Vext,OUTPUT);
	digitalWrite(Vext, LOW);
}

void Heltec_CubeCell::VextOFF(void) //Vext default OFF
{
	pinMode(Vext,OUTPUT);
	digitalWrite(Vext, HIGH);
}

void Heltec_CubeCell::end()
{
	Serial.end();
	Wire.end();
}
Heltec_CubeCell Heltec;
