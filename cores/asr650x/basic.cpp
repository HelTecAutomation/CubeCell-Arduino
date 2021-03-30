extern "C" {
#include <stdlib.h>
#include <ASR_Arduino.h>
}

#define BAT_LEVEL_EMPTY  0x01
#define BAT_LEVEL_FULL   0xFE

/*  get the BatteryVoltage in mV. */
uint16_t getBatteryVoltage(void)
{
	float temp = 0;
	uint16_t volt;
	uint8_t pin;
	pin = ADC;

#if defined(CubeCell_Board)||defined(CubeCell_Capsule)||defined(CubeCell_BoardPlus)||defined(CubeCell_BoardPRO)||defined(CubeCell_GPS)||defined(CubeCell_HalfAA)
	/*
	* have external 10K VDD pullup resistor
	* connected to VBAT_ADC_CTL pin
	*/
	
	pinMode(VBAT_ADC_CTL,OUTPUT);
	digitalWrite(VBAT_ADC_CTL,LOW);
#endif
	for(int i=0;i<50;i++)//read 50 times and get average
		temp+=analogReadmV(pin);
	volt = temp / 50;
	
#if defined(CubeCell_Board)||defined(CubeCell_Capsule)||defined(CubeCell_BoardPlus)||defined(CubeCell_BoardPRO)||defined(CubeCell_GPS)||defined(CubeCell_HalfAA)
	pinMode(VBAT_ADC_CTL, INPUT);
#endif

	volt = volt*2;
	return volt;
}

uint8_t BoardGetBatteryLevel(void)
{
	// 5.5 End-Device Status (DevStatusReq, DevStatusAns)
	// 0      The end-device is connected to an external power source.
	// 1..254 The battery level, 1 being at minimum and 254 being at maximum
	// 255    The end-device was not able to measure the battery level.
	const double maxBattery = 4.212;
	const double minBattery = 3.7;
	const double batVoltage = fmax(minBattery, fmin(maxBattery, getBatteryVoltage() / 1000.0));
	const uint8_t batlevel = BAT_LEVEL_EMPTY + ((batVoltage - minBattery) / (maxBattery - minBattery)) * (BAT_LEVEL_FULL - BAT_LEVEL_EMPTY);
	//printf("battery level (1-254): %u\n", batlevel);
	return batlevel;
}


