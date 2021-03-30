//---------------------------------------------------------------------------------------------
//
// Library for the LM75A temperature sensor by NXP and Texas Instruments.
//
// Copyright 2016-2017, M2M Solutions AB
// Written by Jonny Bergdahl, 2016-11-18
//
// Licensed under the MIT license, see the LICENSE.txt file.
//
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef M2M_LM75_h
#define M2M_LM75_h
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define LM75A_DEFAULT_ADDRESS		0x48		// Address is configured with pins A0-A2
#define LM75A_REGISTER_TEMP			0			// Temperature register (read-only)
#define LM75A_REGISTER_CONFIG		1			// Configuration register
#define LM75A_REGISTER_THYST		2			// Hysterisis register
#define LM75A_REGISTER_TOS			3			// OS register
#define LM75A_REGISTER_PRODID		7			// Product ID register - Only valid for Texas Instruments

#define LM75_CONF_OS_COMP_INT		1			// OS operation mode selection
#define LM75_CONF_OS_POL			2			// OS polarity selection
#define LM75_CONF_OS_F_QUE			3			// OS fault queue programming

#define LM75A_INVALID_TEMPERATURE	-1000.0f	// Just an arbritary value outside of the sensor limits

enum FaultQueueValue : uint8_t
{
	NUMBER_OF_FAULTS_1 = 0,
	NUMBER_OF_FAULTS_2 = 0b01000,
	NUMBER_OF_FAULTS_4 = 0b10000,
	NUMBER_OF_FAULTS_6 = 0b11000
};

enum OsPolarity : uint8_t
{
	OS_POLARITY_ACTIVELOW = 0,
	OS_POLARITY_ACTIVEHIGH = 0b100
};

enum DeviceMode : uint8_t
{
	DEVICE_MODE_COMPARATOR = 0,
	DEVICE_MODE_INTERRUPT = 0b10
};

class M2M_LM75A 
{
	// Private variables
	uint8_t _i2cAddress;

	// Private functions
	uint8_t read8bitRegister(const uint8_t reg);
	bool read16bitRegister(uint8_t register, uint16_t& response);
	bool write16bitRegister(const uint8_t reg, const uint16_t value);
	bool write8bitRegister(const uint8_t reg, const uint8_t value);

public:
	// Constructors
    M2M_LM75A();
    M2M_LM75A(uint8_t address);

	// Startup/Teardown
	void begin();
	void end();
	
	// Power management
	void shutdown();
	void wakeup();
	bool isShutdown();

	// Temperature functions
    float getTemperature();
	float getTemperatureInFarenheit();

	// Configuration functions
	float getHysterisisTemperature();	
	FaultQueueValue getFaultQueueValue();
	float getOSTripTemperature();
	OsPolarity getOsPolarity();
	DeviceMode getDeviceMode();
	void setHysterisisTemperature(const float temperature);
	void setOsTripTemperature(const float temperature);
	void setFaultQueueValue(const FaultQueueValue value);
	void setOsPolarity(const OsPolarity polarity);
	void setDeviceMode(const DeviceMode deviceMode);	

	// Other
	bool isConnected();
	uint8_t getConfig();
	float getProdId();
};

#endif
