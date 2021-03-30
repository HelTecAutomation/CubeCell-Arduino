/* Heltec Automation HDC1080 Sensors test example
 *
 * Function:
 * Temperature and humidity Measurement
 *
 * HelTec AutoMation, Chengdu, China
 * www.heltec.org
 *
 * this project also realess in GitHub:
 * https://github.com/HelTecAutomation/ASR650x-Arduino
 * 
 */
 
#include <Wire.h>
#include "HDC1080.h"

HDC1080 hdc1080;

void setup()
{
	//vext on
	pinMode(Vext,OUTPUT);
	digitalWrite(Vext,LOW);

	Serial.begin(115200);
	Serial.println("HDC1080 Heater Arduino Test");
	Serial.println();

	// Default settings: 
	//  - Heater off
	//  - 14 bit Temperature and Humidity Measurement Resolutions
	hdc1080.begin(0x40);

	Serial.print("Manufacturer ID=0x");
	Serial.println(hdc1080.readManufacturerId(), HEX); // 0x5449 ID of Texas Instruments
	Serial.print("Device ID=0x");
	Serial.println(hdc1080.readDeviceId(), HEX); // 0x1050 ID of the device

	printSerialNumber();
	
	uint8_t huTime = 10;
	Serial.print("Heating up for approx. ");
	Serial.print(huTime);
	Serial.println(" seconds. Please wait...");

	hdc1080.heatUp(huTime);
	hdc1080.heatUp(10); // approx 10 sec
	printRegister(hdc1080.readRegister());
}

void loop()
{
	Serial.print("T=");
	Serial.print(hdc1080.readTemperature());
	Serial.print("C, RH=");
	Serial.print(hdc1080.readHumidity());
	Serial.println("%");
	delay(300);
}

void printRegister(HDC1080_Registers reg) {
	Serial.println("HDC1080 Configuration Register");
	Serial.println("------------------------------");
	
	Serial.print("Software reset bit: ");
	Serial.print(reg.SoftwareReset, BIN);
	Serial.println(" (0=Normal Operation, 1=Software Reset)");

	Serial.print("Heater: ");
	Serial.print(reg.Heater, BIN);
	Serial.println(" (0=Disabled, 1=Enabled)");

	Serial.print("Mode of Acquisition: ");
	Serial.print(reg.ModeOfAcquisition, BIN);
	Serial.println(" (0=T or RH is acquired, 1=T and RH are acquired in sequence, T first)");

	Serial.print("Battery Status: ");
	Serial.print(reg.BatteryStatus, BIN);
	Serial.println(" (0=Battery voltage > 2.8V, 1=Battery voltage < 2.8V)");

	Serial.print("T Measurement Resolution: ");
	Serial.print(reg.TemperatureMeasurementResolution, BIN);
	Serial.println(" (0=14 bit, 1=11 bit)");

	Serial.print("RH Measurement Resolution: ");
	Serial.print(reg.HumidityMeasurementResolution, BIN);
	Serial.println(" (00=14 bit, 01=11 bit, 10=8 bit)");
}

void printSerialNumber() {
	Serial.print("Device Serial Number=");
	HDC1080_SerialNumber sernum = hdc1080.readSerialNumber();
	char format[12];
	sprintf(format, "%02X-%04X-%04X", sernum.serialFirst, sernum.serialMid, sernum.serialLast);
	Serial.println(format);
}
