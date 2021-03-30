/*! @file BME680.cpp
    @section Zanshin_BME680_intro_section Description
    Arduino Library for the Bosch BME680 sensor\n\n
    See the main library header file for all details
*/

#include "BME680.h" // Include the library header definition

BME680_Class::BME680_Class()
{
  /*!
  * @brief   Class constructor
  * @details Currently empty and unused
  */
} // of class constructor
BME680_Class::~BME680_Class() 
{
  /*!
  * @brief   Class destructor
  * @details Currently empty and unused
  */
} // of class destructor
bool BME680_Class::begin() 
{
  /*!
  * @brief   starts communications with device (overloaded)
  * @details When called with no parameters the I2C protocol with I2C_STANDARD_MODE speed is selected. The I2C network
  *          is searched for the first BME680 device.
  * return "true" if successful otherwise false
  */  
  return begin(I2C_STANDARD_MODE); // Initialize I2C with standard speed
} // of method begin()
bool BME680_Class::begin(const uint32_t i2cSpeed)
{
  /*!
  * @brief   starts communications with device (overloaded)
  * @details When called with a 32-bit parameter it is assumed that the I2C protocol is to be used and the speed
  *          setting is determined by the parameter.
  * @param[in] i2cSpeed I2C Speed value
  * return "true" if successful otherwise false
  */
  Wire.begin();                                          // Start I2C as master device
  Wire.setClock(i2cSpeed);                               // Set I2C bus speed
  for (_I2CAddress=0x76;_I2CAddress<=0x77;_I2CAddress++) // loop all possible addresses
  {
    Wire.beginTransmission(_I2CAddress);                 // Check current address for BME680
    if (Wire.endTransmission()==0)                       // If no error we have a device
    {
       return commonInitialization();                    // Perform common initialization
    } // of if-then we have found a device
  } // of for-next each I2C address loop
  _I2CAddress = 0; // Set to 0 to denote no I2C found
  return false;    // return failure if we get here 
} // of method begin()
bool BME680_Class::commonInitialization() 
{
  /*!
  * @brief   Common BME680 initialization function
  * @details Called from all of the overloaded "begin()" functions once protocol has been selected
  * return "true" if successful otherwise false
  */
  if (_I2CAddress==0 && readByte(BME680_SPI_REGISTER)!=0) 
  {
    putData(BME680_SPI_REGISTER,(uint8_t)0); // We are in the wrong mode for SPI so return to correct SPI page
  } // of if-then we are in SPI mode
  if (readByte(BME680_CHIPID_REGISTER)==BME680_CHIPID) // check for correct chip id
  {
    getCalibration();                                       // get the calibration values
    if (_I2CAddress==0) 
    {
      putData(BME680_SPI_REGISTER|0x80,(uint8_t)B00010000); // Switch to correct register bank
    } // of if-then SPI mode
    return true;
  } // of if-then device is really a BME680
  else return false;
} // of method commonInitialization
uint8_t BME680_Class::readByte(const uint8_t addr) 
{
  /*!
  * @brief   Read a single byte from the give address
  * @details interlude function to the "getData()" function
  * param[in] addr Address of device
  * return    single byte read
  */
  uint8_t returnValue;       // Storage for returned value
  getData(addr,returnValue); // Read just one byte
  return (returnValue);      // Return byte just read
} // of method readByte()
void BME680_Class::reset()
{
  /*!
  * @brief   Performs a device reset
  */
  putData(BME680_SOFTRESET_REGISTER,BME680_RESET_CODE); // write reset code to device
  if (_I2CAddress) begin();                             // Start device again if I2C
} // of method reset()
void BME680_Class::getCalibration()
{
  /*!
  * @brief    reads the calibration register data into local variables for use in converting readings
  * @details  The calibration registers are read into a temporary array and then parsed into the appropriate 
  *           calibration variables, this was taken from the example BOSCH software and minimizes register reads, 
  *           but makes it rather difficult to read. This will be redone for legibility at some point in the future
  * param[in] addr Address of device
  * return    single byte read
  */
  const uint8_t BME680_COEFF_SIZE1              =    25; // First array with coefficients
  const uint8_t BME680_COEFF_SIZE2              =    16; // Second array with coefficients
  const uint8_t BME680_COEFF_START_ADDRESS1     =  0x89; // start address for array 1
  const uint8_t BME680_COEFF_START_ADDRESS2     =  0xE1; // start address for array 2
  const uint8_t BME680_HUM_REG_SHIFT_VAL        =     4; // Ambient humidity shift value
  const uint8_t BME680_BIT_H1_DATA_MSK          =  0x0F;
  const uint8_t BME680_T2_LSB_REG               =     1;
  const uint8_t BME680_T2_MSB_REG               =     2;
  const uint8_t BME680_T3_REG		                =     3;
  const uint8_t BME680_P1_LSB_REG   	          =     5;
  const uint8_t BME680_P1_MSB_REG	              =     6;
  const uint8_t BME680_P2_LSB_REG	              =     7;
  const uint8_t BME680_P2_MSB_REG	              =     8;
  const uint8_t BME680_P3_REG		                =     9;
  const uint8_t BME680_P4_LSB_REG         	    =    11;
  const uint8_t BME680_P4_MSB_REG        	      =    12;
  const uint8_t BME680_P5_LSB_REG           	  =    13;
  const uint8_t BME680_P5_MSB_REG         	    =    14;
  const uint8_t BME680_P7_REG  	                =    15;
  const uint8_t BME680_P6_REG	                  =    16;
  const uint8_t BME680_P8_LSB_REG    	          =    19;
  const uint8_t BME680_P8_MSB_REG	              =    20;
  const uint8_t BME680_P9_LSB_REG    	          =    21;
  const uint8_t BME680_P9_MSB_REG	              =    22;
  const uint8_t BME680_P10_REG		              =    23;
  const uint8_t BME680_H2_MSB_REG	              =     0;
  const uint8_t BME680_H2_LSB_REG	              =     1;
  const uint8_t BME680_H1_LSB_REG    	          =     1;
  const uint8_t BME680_H1_MSB_REG	              =     2;
  const uint8_t BME680_H3_REG	                  =     3;
  const uint8_t BME680_H4_REG    	              =     4;
  const uint8_t BME680_H5_REG	                  =     5;
  const uint8_t BME680_H6_REG                   =     6;
  const uint8_t BME680_H7_REG                   =     7;
  const uint8_t BME680_T1_LSB_REG	              =     8;
  const uint8_t BME680_T1_MSB_REG    	          =     9;
  const uint8_t BME680_GH2_LSB_REG              =    10;
  const uint8_t BME680_GH2_MSB_REG              =    11;
  const uint8_t BME680_GH1_REG	                =    12;
  const uint8_t BME680_GH3_REG	                =    13;
  const uint8_t BME680_ADDR_RES_HEAT_RANGE_ADDR =  0x02;
  const uint8_t BME680_RHRANGE_MSK              =  0x30;
  const uint8_t BME680_ADDR_RES_HEAT_VAL_ADDR   =  0x00;
  const uint8_t BME680_ADDR_RANGE_SW_ERR_ADDR   =  0x04;
  const uint8_t BME680_RSERROR_MSK	            =  0xF0;
                       /*************************************
                       ** Temperature related coefficients **
                       *************************************/
	uint8_t coeff_array1[BME680_COEFF_SIZE1]  = { 0 }; // Define temporary array 1
	uint8_t coeff_array2[BME680_COEFF_SIZE2]  = { 0 }; // Define temporary array 2
  getData(BME680_COEFF_START_ADDRESS1,coeff_array1); // Split reading registers into 2
  getData(BME680_COEFF_START_ADDRESS2,coeff_array2); // one 25 bytes and the other 16
  _T1  = (uint16_t) (CONCAT_BYTES(coeff_array2[BME680_T1_MSB_REG], coeff_array2[BME680_T1_LSB_REG]));
  _T2  = (int16_t)  (CONCAT_BYTES(coeff_array1[BME680_T2_MSB_REG], coeff_array1[BME680_T2_LSB_REG]));
  _T3  = (int8_t)   (coeff_array1[BME680_T3_REG]);
                       /*************************************
                       ** Pressure related coefficients    **
                       *************************************/
  _P1  = (uint16_t) (CONCAT_BYTES(coeff_array1[BME680_P1_MSB_REG], coeff_array1[BME680_P1_LSB_REG]));
  _P2  = (int16_t)  (CONCAT_BYTES(coeff_array1[BME680_P2_MSB_REG], coeff_array1[BME680_P2_LSB_REG]));
  _P3  = (int8_t)    coeff_array1[BME680_P3_REG];
  _P4  = (int16_t)  (CONCAT_BYTES(coeff_array1[BME680_P4_MSB_REG], coeff_array1[BME680_P4_LSB_REG]));
  _P5  = (int16_t)  (CONCAT_BYTES(coeff_array1[BME680_P5_MSB_REG], coeff_array1[BME680_P5_LSB_REG]));
  _P6  = (int8_t)   (coeff_array1[BME680_P6_REG]);
  _P7  = (int8_t)   (coeff_array1[BME680_P7_REG]);
  _P8  = (int16_t)  (CONCAT_BYTES(coeff_array1[BME680_P8_MSB_REG], coeff_array1[BME680_P8_LSB_REG]));
  _P9  = (int16_t)  (CONCAT_BYTES(coeff_array1[BME680_P9_MSB_REG], coeff_array1[BME680_P9_LSB_REG]));
  _P10 = (uint8_t)  (coeff_array1[BME680_P10_REG]);
                       /**********************************
                       ** Humidity related coefficients **
                       **********************************/
  _H1  = (uint16_t) (((uint16_t) coeff_array2[BME680_H1_MSB_REG] << BME680_HUM_REG_SHIFT_VAL)	|
         (coeff_array2[BME680_H1_LSB_REG]&BME680_BIT_H1_DATA_MSK));
  _H2  = (uint16_t) (((uint16_t) coeff_array2[BME680_H2_MSB_REG] << BME680_HUM_REG_SHIFT_VAL) |
         ((coeff_array2[BME680_H2_LSB_REG])>>BME680_HUM_REG_SHIFT_VAL));
  _H3  = (int8_t)   coeff_array2[BME680_H3_REG];
  _H4  = (int8_t)   coeff_array2[BME680_H4_REG];
  _H5  = (int8_t)   coeff_array2[BME680_H5_REG];
  _H6  = (uint8_t)  coeff_array2[BME680_H6_REG];
  _H7  = (int8_t)   coeff_array2[BME680_H7_REG];
                       /************************************
                       ** Gas heater related coefficients **
                       ************************************/
  _G1  = (int8_t)   coeff_array2[BME680_GH1_REG];
  _G2  = (int16_t)  (CONCAT_BYTES(coeff_array2[BME680_GH2_MSB_REG], coeff_array2[BME680_GH2_LSB_REG]));
  _G3  = (int8_t)   coeff_array2[BME680_GH3_REG];
  uint8_t temp_var = 0;
  getData(BME680_ADDR_RES_HEAT_RANGE_ADDR,temp_var);
  _res_heat_range = ((temp_var & BME680_RHRANGE_MSK) / 16);
  getData(BME680_ADDR_RES_HEAT_VAL_ADDR,temp_var);
  _res_heat_val = (int8_t) temp_var;
  getData(BME680_ADDR_RANGE_SW_ERR_ADDR,temp_var);
  _range_sw_error = ((int8_t) temp_var & (int8_t) BME680_RSERROR_MSK) / 16;
} // of method getCalibration()
bool BME680_Class::setOversampling(const uint8_t sensor, const uint8_t sampling) 
{
  /*!
  * @brief   sets the oversampling mode for the sensor
  * @details See enumerated "sensorTypes" for a list of values. Set to a valid oversampling rate as defined in the 
  *          enumerated type oversamplingTypes. If either value is out of range or another error occurs then the 
  *          return value is false.
  * param[in] sensor Enumerated sensor type
  * param[in] sampling Sampling rate
  * return "true" if successful otherwise false
  */
  if (sensor>=UnknownSensor || sampling>=UnknownOversample) return(false); // return error if out of range
  uint8_t tempRegister;
  switch (sensor) 
  {
    case HumiditySensor :
    {
      tempRegister = readByte(BME680_CONTROL_HUMIDITY_REGISTER) & B11111000;  // Get contents, mask bits 3-7
      putData(BME680_CONTROL_HUMIDITY_REGISTER,(uint8_t)(tempRegister|sampling));// Update humidity bits 0:2
      break;
    } // of HumiditySensor
    case PressureSensor : 
    {
      tempRegister = readByte(BME680_CONTROL_MEASURE_REGISTER) & B11100011; // Get contents, mask unused bits
      putData(BME680_CONTROL_MEASURE_REGISTER,(uint8_t)(tempRegister|(sampling<<2))); // Update pressure bits
      break;
    } // of PressureSensor
    case TemperatureSensor : 
    {
      tempRegister = readByte(BME680_CONTROL_MEASURE_REGISTER) & B00011111; // Get contents, mask bits 0-4
      putData(BME680_CONTROL_MEASURE_REGISTER,(uint8_t)(tempRegister|(sampling<<5)));// Update humidity bits 5:7
      break;
    } // of TemperatureSensor
    default: return(false); // Return an error if no match
  } // of switch the sensor type
  return(true);
} // of method setOversampling()
uint8_t BME680_Class::setIIRFilter(const uint8_t iirFilterSetting ) 
{
  /*!
  * @brief   Set or return the current IIR filter setting
  * @details When called with no parameters returns the current IIR Filter setting, otherwise when called with one 
  *          parameter will set the IIR filter value and return the new setting
  * param[in] iirFilterSetting New setting
  * return   IIR Filter setting
  */
  uint8_t returnValue = readByte(BME680_CONFIG_REGISTER);              // Get control register byte
  if (iirFilterSetting==UINT8_MAX) return((returnValue>>2)&B00000111); // return the current setting
  returnValue = returnValue&B11100011;                                 // Get control reg, mask IIR bits
  returnValue |= (iirFilterSetting&B00000111)<<2;                      // use 3 bits of iirFilterSetting
  putData(BME680_CONFIG_REGISTER,returnValue);                         // Write new control register value
  returnValue = (returnValue>>2)&B00000111;                            // Extract IIR filter setting
  return(returnValue);                                                 // Return IIR Filter setting
} // of method setIIRFilter()
void BME680_Class::getSensorData(int32_t &temp, int32_t &hum, int32_t &press, int32_t &gas, const bool waitSwitch )
{
  /*!
  * @brief   Returns the most recent temperature, humidity and pressure readings
  * param[out] temp  Temperature reading
  * param[out] hum   Humidity reading
  * param[out] press Pressure reading
  * param[out] gas   Gas reading
  * param[in] waitSwitch Optional switch that, when set to "true" will not return until reading is finished
  */
  readSensors(waitSwitch); // Get compensated data from BME680
  temp  = _Temperature;    // Copy global variables to parameters
  hum   = _Humidity;       // Copy global variables to parameters
  press = _Pressure;       // Copy global variables to parameters
  gas   = _Gas;            // Copy global variables to parameters
} // of method getSensorData()
void BME680_Class::readSensors(const bool waitSwitch) 
{
  /*!
  * @brief   reads all 4 sensor values from the registers in one operation and then proceeds to convert the raw 
  *          temperature, pressure and humidity readings into standard metric units
  * @details The formula is written in the BME680's documentation but the math used below was taken from Adafruit's 
  *          Adafruit_BME680_Library at https://github.com/adafruit/Adafruit_BME680. I think it can be refactored 
  *          into more efficient code at some point in the future, but it does work correctly.
  * param[in] waitSwitch Optional switch that, when set to "true" will not return until reading is finished
  */
  /*! Lookup table for the possible gas range values */
const  uint32_t lookupTable1[16] = {
  UINT32_C(2147483647), UINT32_C(2147483647), UINT32_C(2147483647), UINT32_C(2147483647), UINT32_C(2147483647), 
  UINT32_C(2126008810), UINT32_C(2147483647), UINT32_C(2130303777), UINT32_C(2147483647), UINT32_C(2147483647),
  UINT32_C(2143188679), UINT32_C(2136746228), UINT32_C(2147483647), UINT32_C(2126008810), UINT32_C(2147483647), 
  UINT32_C(2147483647) 
};
const uint32_t lookupTable2[16]  = {
  UINT32_C(4096000000), UINT32_C(2048000000), UINT32_C(1024000000), UINT32_C(512000000), UINT32_C(255744255),  
  UINT32_C(127110228) , UINT32_C(64000000),   UINT32_C(32258064),   UINT32_C(16016016),  UINT32_C(8000000),
  UINT32_C(4000000),    UINT32_C(2000000),    UINT32_C(1000000),    UINT32_C(500000),    UINT32_C(250000),
  UINT32_C(125000) 
};
  uint8_t buff[15],gas_range,status=0;                     // declare array for registers
  int64_t var1, var2, var3, var4, var5, var6, temp_scaled; // Work variables
  uint32_t adc_temp, adc_pres;                             // Raw ADC temperature and pressure
  uint16_t adc_hum, adc_gas_res;                           // Raw ADC humidity and gas
  if (waitSwitch) waitForReadings();                       // Don't return until readings done
  getData(BME680_STATUS_REGISTER,buff);                    // read all 15 bytes in one go
  adc_pres    = (uint32_t)(((uint32_t) buff[2]*4096)|((uint32_t)buff[3]*16)| ((uint32_t)buff[4]/16));// put the 3 bytes of Pressure
  adc_temp    = (uint32_t)(((uint32_t) buff[5]*4096)|((uint32_t)buff[6]*16)| ((uint32_t)buff[7]/16));// put the 3 bytes of Temperature
  adc_hum     = (uint16_t)(((uint32_t) buff[8]*256)|(uint32_t)buff[9]);                              // put the 2 bytes of Humidity
  adc_gas_res = (uint16_t)((uint32_t) buff[13]*4|(((uint32_t)buff[14])/64));                         // put the 2 bytes of Gas
  gas_range   = buff[14] & 0X0F; // Retrieve the range
  status |= buff[14] & 0X20;     // See if the gas range is valid
  status |= buff[14] & 0X10;     // and the measurement is valid
                    //*******************************//
                    // First compute the temperature //
                    //*******************************//
  var1         = ((int32_t)adc_temp>>3)-((int32_t)_T1<<1); // Perform calibration/adjustment
  var2         = (var1*(int32_t)_T2)>>11;                  // of Temperature values according
  var3         = ((var1>>1)*(var1>>1))>>12;                // to formula defined by Bosch
  var3         = ((var3)*((int32_t)_T3<<4))>>14;
  _tfine       = (int32_t)(var2+var3);
  _Temperature = (int16_t)(((_tfine*5)+128)>>8);
                    //*******************************//
                    // Now compute the pressure      //
                    //*******************************//
	var1      = (((int32_t)_tfine) >> 1) - 64000;
	var2      = ((((var1 >> 2)*(var1 >> 2)) >> 11)*(int32_t)_P6) >> 2;
	var2      = var2 + ((var1 * (int32_t)_P5) << 1);
	var2      = (var2 >> 2) + ((int32_t)_P4 << 16);
	var1      = (((((var1>>2)*(var1>>2))>>13)*((int32_t)_P3<<5))>>3)+(((int32_t)_P2*var1)>>1);
	var1      = var1 >> 18;
	var1      = ((32768 + var1) * (int32_t)_P1) >> 15;
	_Pressure = 1048576 - adc_pres;
	_Pressure = (int32_t)((_Pressure - (var2 >> 12)) * ((uint32_t)3125));
	var4      = ((int32_t)1 << 31);
	if (_Pressure >= var4)
    _Pressure = ((_Pressure / (uint32_t)var1) << 1);
	else
    _Pressure = ((_Pressure << 1) / (uint32_t)var1);
	var1 = ((int32_t)_P9*(int32_t)(((_Pressure>>3)*(_Pressure>>3))>>13))>>12;
	var2 = ((int32_t)(_Pressure >> 2) * (int32_t)_P8) >> 13;
	var3 = ((int32_t)(_Pressure >> 8) * (int32_t)(_Pressure >> 8) *	(int32_t)(_Pressure >> 8) * (int32_t)_P10) >> 17;
	_Pressure = (int32_t)(_Pressure)+((var1+var2+var3+((int32_t)_P7<<7))>>4);
                    //**********************//
                    // Compute the humidity //
                    //**********************//
	temp_scaled = (((int32_t) _tfine * 5) + 128) >> 8;
	var1        = (int32_t)(adc_hum-((int32_t)((int32_t)_H1*16)))
                -(((temp_scaled*(int32_t)_H3)/((int32_t)100))>>1);
	var2        = ((int32_t)_H2*(((temp_scaled*(int32_t)_H4)/
                ((int32_t) 100))+(((temp_scaled*((temp_scaled*
                (int32_t)_H5)/((int32_t)100)))>>6)/((int32_t)100))+
                (int32_t) (1 << 14))) >> 10;
	var3        = var1 * var2;
	var4        = (int32_t) _H6 << 7;
	var4        = ((var4)+((temp_scaled*(int32_t)_H7)/((int32_t)100)))>>4;
	var5        = ((var3 >> 14) * (var3 >> 14)) >> 10;
	var6        = (var4 * var5) >> 1;
	_Humidity   = (((var3 + var6) >> 10) * ((int32_t) 1000)) >> 12;
	if (_Humidity > 100000) /* Cap at 100%rH */
	  _Humidity = 100000;
	else if (_Humidity < 0)
	  _Humidity = 0;
                    //**********************//
                    // Compute the Gas      //
                    //**********************//
	uint64_t uvar2;
	var1 = (int64_t)((1340+(5*(int64_t)_range_sw_error))*
	((int64_t) lookupTable1[gas_range])) >> 16;
	uvar2 = (((int64_t)((int64_t)adc_gas_res<<15)-(int64_t)(16777216))+var1);
	var3 = (((int64_t) lookupTable2[gas_range] * (int64_t) var1) >> 9);
	_Gas = (uint32_t) ((var3 + ((int64_t) uvar2 >> 1)) / (int64_t) uvar2);
  uint8_t workRegister = readByte(BME680_CONTROL_MEASURE_REGISTER);   // Read the control measure
  putData(BME680_CONTROL_MEASURE_REGISTER,(uint8_t)(workRegister|1)); // Trigger start of next measurement
} // of method readSensors()
void BME680_Class::waitForReadings() 
{
  /*!
  * @brief   Only returns once a measurement on the BME680 has completed
  */
  while ((readByte(BME680_STATUS_REGISTER)&B00100000)!=0); // Loop until readings bit is cleared by BME680
} // of method waitForReadings
bool BME680_Class::setGas(uint16_t GasTemp,  uint16_t GasMillis) 
{
  /*!
  * @brief    sets the gas measurement target temperature and heating time
  * param[in] GasTemp  Target temperature in Celsius
  * param[in] GasMillis Milliseconds to turn on heater
  * return Always returns "true"
  */
  uint8_t gasRegister = readByte(BME680_CONTROL_GAS_REGISTER2); // Read current register values
  if ( GasTemp==0 || GasMillis==0 ) 
  {
    // If either input variable is zero //
    putData(BME680_CONTROL_GAS_REGISTER1,(uint8_t)B00001000);               // Turn off gas heater
    putData(BME680_CONTROL_GAS_REGISTER2,(uint8_t)(gasRegister&B11101111)); // Turn off gas measurements
  }
  else 
  {
    putData(BME680_CONTROL_GAS_REGISTER1,(uint8_t)0); // Turn off heater bit to turn on
  	uint8_t heatr_res;
	  int32_t var1,var2,var3,var4,var5,heatr_res_x100;
	  if (GasTemp < 200) GasTemp = 200;	else if (GasTemp > 400) GasTemp = 400; // Clamp temperature to min/max
    var1 = (((int32_t)(_Temperature/100)*_H3)/1000)*256;
    var2 = (_H1+784)*(((((_H2+154009)*GasTemp*5)/100)+3276800)/10);
    var3 = var1 + (var2 / 2);
    var4 = (var3 / (_res_heat_range+4));
    var5 = (131 * _res_heat_val) + 65536;
    heatr_res_x100 = (int32_t) (((var4 / var5) - 250) * 34);
    heatr_res = (uint8_t) ((heatr_res_x100 + 50) / 100);
    putData(BME680_GAS_HEATER_REGISTER0,heatr_res);
    uint8_t factor = 0;
    uint8_t durval;
    if (GasMillis >= 0xfc0) durval = 0xff; // Max duration
  	else 
    {
  	  while (GasMillis > 0x3F) 
      {
    	  GasMillis = GasMillis / 4;
    	  factor += 1;
    	} // of while loop
    	durval = (uint8_t) (GasMillis + (factor * 64));
  	} // of if-then-else duration exceeds max
    putData(BME680_CONTROL_GAS_REGISTER1,(uint8_t)0); // then turn off gas heater
    putData(BME680_GAS_DURATION_REGISTER0,durval);
    putData(BME680_CONTROL_GAS_REGISTER2,(uint8_t)(gasRegister|B00010000));
  } // of if-then-else turn gas measurements on or off
  return true;
} // of method setGas()