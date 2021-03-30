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
 
#include "HDC1080.h"

HDC1080 hdc1080;

void printTandRH(HDC1080_MeasurementResolution humidity, HDC1080_MeasurementResolution temperature);
void printRegister(HDC1080_Registers reg);

void setup()
{
	//vext on
	pinMode(Vext,OUTPUT);
	digitalWrite(Vext,LOW);

	Serial.begin(115200);
	Serial.println("HDC1080 Measurement Resolutions Arduino Test");

	hdc1080.begin(0x40);

	Serial.print("Manufacturer ID=0x");
	Serial.println(hdc1080.readManufacturerId(), HEX); // 0x5449 ID of Texas Instruments
	Serial.print("Device ID=0x");
	Serial.println(hdc1080.readDeviceId(), HEX); // 0x1050 ID of the device
	
	printTandRH(HDC1080_RESOLUTION_8BIT, HDC1080_RESOLUTION_11BIT);
	printTandRH(HDC1080_RESOLUTION_11BIT, HDC1080_RESOLUTION_11BIT);
	printTandRH(HDC1080_RESOLUTION_14BIT, HDC1080_RESOLUTION_11BIT);
	printTandRH(HDC1080_RESOLUTION_8BIT, HDC1080_RESOLUTION_14BIT);
	printTandRH(HDC1080_RESOLUTION_11BIT, HDC1080_RESOLUTION_14BIT);
	printTandRH(HDC1080_RESOLUTION_14BIT, HDC1080_RESOLUTION_14BIT);

}

void loop()
{	
}

void printTandRH(HDC1080_MeasurementResolution humidity, HDC1080_MeasurementResolution temperature) {
	hdc1080.setResolution(humidity, temperature);

	HDC1080_Registers reg = hdc1080.readRegister();
	printRegister(reg);

	Serial.print("T=");
	Serial.print(hdc1080.readTemperature());
	Serial.print("C, RH=");
	Serial.print(hdc1080.readHumidity());
	Serial.println("%");
}

void printRegister(HDC1080_Registers reg) {
	Serial.print("Measurement Resolution: T=");
	Serial.print(reg.TemperatureMeasurementResolution, BIN);
	Serial.print(" (0=14 bit, 1=11 bit)");

	Serial.print(" RH=");
	Serial.print(reg.HumidityMeasurementResolution, BIN);
	Serial.println(" (00=14 bit, 01=11 bit, 10=8 bit)");
}

