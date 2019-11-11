/*

Arduino Library for Bosch Sensortec BME680 Environment Server
Written by AA for ClosedCube
---

The MIT License (MIT)

Copyright (c) 2017 ClosedCube Limited

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include "BME680.h"



BME680::BME680() {
}

void BME680::init(uint8_t address) {
	_address = address;
	_chipID = readByte(BME680_REG_CHIPID);

	loadCalData();
}

uint8_t BME680::reset() {
	Wire.beginTransmission(_address);
	Wire.write(BME680_REG_RESET);
	Wire.write(BME680_SOFT_RESET_CMD);
	return Wire.endTransmission();
}

uint8_t BME680::getChipID() {
	return _chipID;
}

uint8_t BME680::setSleepMode() {
	return changeMode(0x00);
}

uint8_t BME680::setForcedMode() {
	return changeMode(0x01);
}

uint8_t BME680::changeMode(uint8_t mode) {
	BME680_Ctrl_TP_Register ctrl_meas;

	ctrl_meas.rawData = readByte(BME680_REG_CTRL_MEAS);
	ctrl_meas.mode = mode;

	Wire.beginTransmission(_address);
	Wire.write(BME680_REG_CTRL_MEAS);
	Wire.write(ctrl_meas.rawData);
	return Wire.endTransmission();
}

BME680_Status BME680::readStatus() {
	BME680_Status status;
	status.rawData = readByte(BME680_REG_MEAS_STATUS);
	return status;
}


uint8_t BME680::setGasOn(uint16_t heaterTemperature, uint16_t heaterDuration) {
	uint8_t result;

	Wire.beginTransmission(_address);
	Wire.write(0x5A);
	Wire.write(calculateHeaterTemperature(heaterTemperature));
	Wire.write(0x64);
	Wire.write(calculateHeaterDuration(heaterDuration));
	result = Wire.endTransmission();

	BME680_Heater_Profile profile;
	profile.nb_conv = 0;
	profile.run_gas = 1;

	Wire.beginTransmission(_address);
	Wire.write(BME680_REG_CTRL_GAS);
	Wire.write(profile.rawData);
	return result & Wire.endTransmission();

}

uint8_t BME680::setGasOff() {
	BME680_Heater_Profile profile;
	profile.nb_conv = 0;
	profile.run_gas = 0;

	Wire.beginTransmission(_address);
	Wire.write(BME680_REG_CTRL_GAS);
	Wire.write(profile.rawData);
	return Wire.endTransmission();
}


uint32_t BME680::readGasResistance() {
	typedef union {
		uint8_t raw;
		union {
			uint8_t range : 2;
			uint8_t lsb : 6;
		};
	} gas_lsb_t;

	gas_lsb_t gas_lsb;

	uint8_t gas_msb = readByte(0x2A);
	gas_lsb.raw = readByte(0x2B);

	uint16_t gas_raw = gas_msb << 8 | gas_lsb.lsb;

	int64_t var1, var2, var3;

	var1 = (int64_t)((1340 + (5 * (int64_t)_calib_dev.range_sw_err)) * ((int64_t)lookupTable1[gas_lsb.range])) / 65536;
	var2 = (((int64_t)((int64_t)gas_raw * 32768) - (int64_t)(16777216)) + var1);
	var3 = (((int64_t)lookupTable2[gas_lsb.range] * (int64_t)var1) / 512);

	return (uint32_t)((var3 + ((int64_t)var2 / 2)) / (int64_t)var2);
}

double BME680::readHumidity() {
	uint8_t hum_msb = readByte(0x25);
	uint8_t hum_lsb = readByte(0x26);
	uint16_t hum_raw = hum_msb << 8 | hum_lsb;

	int32_t var1, var2, var3, var4, var5, var6, temp, calc_hum;

	temp = (((int32_t)_calib_dev.tfine * 5) + 128) / 256;
	var1 = (int32_t)(hum_raw - ((int32_t)((int32_t)_calib_hum.h1 * 16)))
		- (((temp * (int32_t)_calib_hum.h3) / ((int32_t)100)) / 2);
	var2 = ((int32_t)_calib_hum.h2
		* (((temp * (int32_t)_calib_hum.h4) / ((int32_t)100))
			+ (((temp * ((temp * (int32_t)_calib_hum.h5) / ((int32_t)100))) / 64)
				/ ((int32_t)100)) + (int32_t)(1 * 16384))) / 1024;
	var3 = var1 * var2;
	var4 = (int32_t)_calib_hum.h6 * 128;
	var4 = ((var4)+((temp * (int32_t)_calib_hum.h7) / ((int32_t)100))) / 16;
	var5 = ((var3 / 16384) * (var3 / 16384)) / 1024;
	var6 = (var4 * var5) / 2;
	calc_hum = (((var3 + var6) / 1024) * ((int32_t)1000)) / 4096 / 1000.0;

	if (calc_hum > 100)
		calc_hum = 100;
	else if (calc_hum < 0)
		calc_hum = 0;
	
	return calc_hum;
}

double BME680::readPressure() {
	uint8_t pres_msb = readByte(0x1F);
	uint8_t pres_lsb = readByte(0x20);
	uint8_t pres_xlsb = readByte(0x21);

	uint32_t pres_raw = ((uint32_t)pres_msb << 12) | ((uint32_t)pres_lsb << 4) | ((uint32_t)pres_xlsb >> 4);

	int32_t var1, var2, var3, calc_pres;

	var1 = (((int32_t)_calib_dev.tfine) / 2) - 64000;
	var2 = ((var1 / 4) * (var1 / 4)) / 2048;
	var2 = ((var2) * (int32_t)_calib_pres.p6) / 4;
	var2 = var2 + ((var1 * (int32_t)_calib_pres.p5) * 2);
	var2 = (var2 / 4) + ((int32_t)_calib_pres.p4 * 65536);
	var1 = ((var1 / 4) * (var1 / 4)) / 8192;
	var1 = (((var1) * ((int32_t)_calib_pres.p3 * 32)) / 8) + (((int32_t)_calib_pres.p2 * var1) / 2);
	var1 = var1 / 262144;
	var1 = ((32768 + var1) * (int32_t)_calib_pres.p1) / 32768;
	calc_pres = (int32_t)(1048576 - pres_raw);
	calc_pres = (int32_t)((calc_pres - (var2 / 4096)) * (3125));
	calc_pres = ((calc_pres / var1) * 2);
	var1 = ((int32_t)_calib_pres.p9 * (int32_t)(((calc_pres / 8) * (calc_pres / 8)) / 8192)) / 4096;
	var2 = ((int32_t)(calc_pres / 4) * (int32_t)_calib_pres.p8) / 8192;
	var3 = ((int32_t)(calc_pres / 256) * (int32_t)(calc_pres / 256) * (int32_t)(calc_pres / 256)
		* (int32_t)_calib_pres.p10) / 131072;
	calc_pres = (int32_t)(calc_pres)+((var1 + var2 + var3 + ((int32_t)_calib_pres.p7 * 128)) / 16);

	return calc_pres / 100.0;
}

double BME680::readTemperature() {
	uint8_t temp_msb = readByte(0x22);
	uint8_t temp_lsb = readByte(0x23);
	uint8_t temp_xlsb = readByte(0x24);

	uint32_t temp_raw = ((uint32_t)temp_msb << 12) | ((uint32_t)temp_lsb << 4) | ((uint32_t)temp_xlsb >> 4);

	uint32_t var1, var2, var3, calc_temp;

	var1 = ((int32_t)temp_raw / 8) - ((int32_t)_calib_temp.t1 * 2);
	var2 = (var1 * (int32_t)_calib_temp.t2) / 2048;
	var3 = ((var1 / 2) * (var1 / 2)) / 4096;
	var3 = ((var3) * ((int32_t)_calib_temp.t3 * 16)) / 16384;
	_calib_dev.tfine = (int32_t)(var2 + var3);
	calc_temp =(int16_t)(((_calib_dev.tfine * 5) + 128) / 256);

	return calc_temp / 100.0;
}

uint8_t BME680::setOversampling(BME680_Oversampling humidity, BME680_Oversampling temperature, BME680_Oversampling pressure) {	
	BME680_Ctrl_TP_Register ctrl_meas;
	ctrl_meas.osrs_t = temperature;
	ctrl_meas.osrs_p = pressure;
	ctrl_meas.mode = 0x0;

	Wire.beginTransmission(_address);
	Wire.write(BME680_REG_CTRL_HUM);
	Wire.write(humidity);
	Wire.write(BME680_REG_CTRL_MEAS);
	Wire.write(ctrl_meas.rawData);
	return Wire.endTransmission();
}

uint8_t BME680::setIIRFilter(BME680_IIRFilter filter) {
	BME680_Config_Register config;
	config.filter = filter;

	Wire.beginTransmission(_address);
	Wire.write(BME680_REG_CONFIG);
	Wire.write(config.rawData);
	return Wire.endTransmission();
}

uint8_t BME680::calculateHeaterTemperature(uint16_t heaterTemperature) {
	int32_t var1, var2, var3, var4, var5;
	int32_t heatr_res_x100;

	if (heaterTemperature < 200)
		heaterTemperature = 200;
	else if (heaterTemperature > 400)
		heaterTemperature = 400;

	var1 = (((int32_t)_calib_dev.amb_temp * _calib_gas.gh3) / 1000) * 256;
	var2 = (_calib_gas.gh1 + 784) * (((((_calib_gas.gh2 + 154009) * heaterTemperature * 5) / 100) + 3276800) / 10);
	var3 = var1 + (var2 / 2);
	var4 = (var3 / (_calib_dev.res_heat_range + 4));
	var5 = (131 * _calib_dev.res_heat_val) + 65536;
	heatr_res_x100 = (int32_t)(((var4 / var5) - 250) * 34);
	return (uint8_t)((heatr_res_x100 + 50) / 100);
}

uint8_t BME680::calculateHeaterDuration(uint16_t heaterDuration)
{
	uint8_t factor = 0;
	uint8_t durval;

	if (heaterDuration >= 0xfc0) {
		durval = 0xff;
	} else {
		while (heaterDuration > 0x3F) {
			heaterDuration = heaterDuration / 4;
			factor += 1;
		}
		durval = (uint8_t)(heaterDuration + (factor * 64));
	}

	return durval;
}

uint8_t BME680::readByte(uint8_t cmd) {
	Wire.beginTransmission(_address);
	Wire.write(cmd);
	Wire.endTransmission();
    delay(9);
	Wire.requestFrom(_address, (uint8_t)1);
	return Wire.read();
}


uint8_t BME680::loadCalData() {
	uint8_t cal1[25];
	uint8_t cal2[16];

	Wire.beginTransmission(_address);
	Wire.write(0x89);
	Wire.endTransmission();

	Wire.requestFrom(_address, (uint8_t)25);
	Wire.readBytes(cal1, 25);

	Wire.beginTransmission(_address);
	Wire.write(0xE1);
	Wire.endTransmission();

	Wire.requestFrom(_address, (uint8_t)16);
	Wire.readBytes(cal2, 16);

	_calib_temp.t1 = cal2[9] << 8 | cal2[8];
	_calib_temp.t2 = cal1[2] << 8 | cal1[1];
	_calib_temp.t3 = cal1[3];
	
	_calib_hum.h1 = cal2[2] << 4 | (cal2[1] & 0x0F);
	_calib_hum.h2 = cal2[0] << 4 | cal2[1];
	_calib_hum.h3 = cal2[3];
	_calib_hum.h4 = cal2[4];
	_calib_hum.h5 = cal2[5];
	_calib_hum.h6 = cal2[6];
	_calib_hum.h7 = cal2[7];

	_calib_pres.p1 = cal1[6] << 8 | cal1[5];
	_calib_pres.p2 = cal1[8] << 8 | cal1[7];
	_calib_pres.p3 = cal1[9];
	_calib_pres.p4 = cal1[12] << 8 | cal1[11];
	_calib_pres.p5 = cal1[14] << 8 | cal1[13];
	_calib_pres.p6 = cal1[16];
	_calib_pres.p7 = cal1[15];
	_calib_pres.p8 = cal1[20] << 8 | cal1[19];
	_calib_pres.p9 = cal1[22] << 8 | cal1[21];
	_calib_pres.p10 = cal1[23];

	_calib_gas.gh1 = cal2[14];
	_calib_gas.gh2 = cal2[12] << 8 | cal2[13];
	_calib_gas.gh3 = cal2[15];

	_calib_dev.res_heat_range = readByte(0x02);
	_calib_dev.res_heat_val = readByte(0x00);
	_calib_dev.range_sw_err = readByte(0x04);

	return 0;
}
