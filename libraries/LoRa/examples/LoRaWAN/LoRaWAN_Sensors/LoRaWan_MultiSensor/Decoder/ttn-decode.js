function bytesToInt(by) {
  f = by[0] | by[1]<<8 | by[2]<<16 | by[3]<<24;
  return f;
} 

function bytesToFloat(by) {
  var bits = by[3]<<24 | by[2]<<16 | by[1]<<8 | by[0];
  var sign = (bits>>>31 === 0) ? 1.0 : -1.0;
  var e = bits>>>23 & 0xff;
  var m = (e === 0) ? (bits & 0x7fffff)<<1 : (bits & 0x7fffff) | 0x800000;
  var f = sign * m * Math.pow(2, e - 150);
  return f;
} 

function Decoder(bytes, port) {

var decoded = {};
i = 0;
//decoded.AppDataSize = bytes.length;

if (port === 2) {
  while (i < bytes.length-3) {
    iicport = bytes[i++].toFixed(0);
    sensor = bytes[i++].toFixed(0);
    if (sensor === "1") { // BME680
      if (iicport === "0") {
        decoded.P0_BME680_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P0_BME680_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P0_BME680_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P0_BME680_gas = ((bytes[i++] << 8) | bytes[i++]);
        decoded.P0_BME680_IAQ = ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "1") {
        decoded.P1_BME680_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P1_BME680_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P1_BME680_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P1_BME680_gas = ((bytes[i++] << 8) | bytes[i++]);
        decoded.P1_BME680_IAQ = ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "2") {
        decoded.P2_BME680_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P2_BME680_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P2_BME680_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P2_BME680_gas = ((bytes[i++] << 8) | bytes[i++]);
        decoded.P2_BME680_IAQ = ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "3") {
        decoded.P3_BME680_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P3_BME680_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P3_BME680_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P3_BME680_gas = ((bytes[i++] << 8) | bytes[i++]);
        decoded.P3_BME680_IAQ = ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "4") {
        decoded.P4_BME680_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P4_BME680_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P4_BME680_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P4_BME680_gas = ((bytes[i++] << 8) | bytes[i++]);
        decoded.P4_BME680_IAQ = ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "5") {
        decoded.P5_BME680_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P5_BME680_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P5_BME680_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P5_BME680_gas = ((bytes[i++] << 8) | bytes[i++]);
        decoded.P5_BME680_IAQ = ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "6") {
        decoded.P6_BME680_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P6_BME680_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P6_BME680_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P6_BME680_gas = ((bytes[i++] << 8) | bytes[i++]);
        decoded.P6_BME680_IAQ = ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "7") {
        decoded.P7_BME680_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P7_BME680_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P7_BME680_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P7_BME680_gas = ((bytes[i++] << 8) | bytes[i++]);
        decoded.P7_BME680_IAQ = ((bytes[i++] << 8) | bytes[i++]);
      }
    }
    if (sensor === "2") { // BME280
      if (iicport === "0") {
        decoded.P0_BME280_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);
        decoded.P0_BME280_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P0_BME280_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "1") {
        decoded.P1_BME280_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);
        decoded.P1_BME280_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P1_BME280_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "2") {
        decoded.P2_BME280_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);
        decoded.P2_BME280_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P2_BME280_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "3") {
        decoded.P3_BME280_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);
        decoded.P3_BME280_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P3_BME280_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "4") {
        decoded.P4_BME280_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);
        decoded.P4_BME280_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P4_BME280_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "5") {
        decoded.P5_BME280_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);
        decoded.P5_BME280_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P5_BME280_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "6") {
        decoded.P6_BME280_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);
        decoded.P6_BME280_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P6_BME280_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "7") {
        decoded.P7_BME280_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);
        decoded.P7_BME280_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
        decoded.P7_BME280_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
    }
    if (sensor === "3") { // CCS811
      if (iicport === "0") {
        decoded.P0_CCS811_co2 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P0_CCS811_tvoc =  ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "1") {
        decoded.P1_CCS811_co2 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P1_CCS811_tvoc =  ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "2") {
        decoded.P2_CCS811_co2 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P2_CCS811_tvoc =  ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "3") {
        decoded.P3_CCS811_co2 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P3_CCS811_tvoc =  ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "4") {
        decoded.P4_CCS811_co2 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P4_CCS811_tvoc =  ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "5") {
        decoded.P5_CCS811_co2 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P5_CCS811_tvoc =  ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "6") {
        decoded.P6_CCS811_co2 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P6_CCS811_tvoc =  ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "7") {
        decoded.P7_CCS811_co2 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P7_CCS811_tvoc =  ((bytes[i++] << 8) | bytes[i++]);
      }
    }
    if (sensor === "4") { // HDC1080
      if (iicport === "0") {
        decoded.P0_HDC1080_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P0_HDC1080_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "1") {
        decoded.P1_HDC1080_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P1_HDC1080_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "2") {
        decoded.P2_HDC1080_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P2_HDC1080_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "3") {
        decoded.P3_HDC1080_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P3_HDC1080_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "4") {
        decoded.P4_HDC1080_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P4_HDC1080_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "5") {
        decoded.P5_HDC1080_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P5_HDC1080_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "6") {
        decoded.P6_HDC1080_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P6_HDC1080_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "7") {
        decoded.P7_HDC1080_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P7_HDC1080_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
    }
    if (sensor === "5") { // BMP180
      if (iicport === "0") {
        decoded.P0_BMP180_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P0_BMP180_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "1") {
        decoded.P1_BMP180_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P1_BMP180_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "2") {
        decoded.P2_BMP180_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P2_BMP180_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "3") {
        decoded.P3_BMP180_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P3_BMP180_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "4") {
        decoded.P4_BMP180_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P4_BMP180_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "5") {
        decoded.P5_BMP180_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P5_BMP180_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "6") {
        decoded.P6_BMP180_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P6_BMP180_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "7") {
        decoded.P7_BMP180_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P7_BMP180_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
    }
    if (sensor === "6") { // BH1750
      if (iicport === "0") {  
        decoded.P0_BH1750_lux = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "1") {  
        decoded.P1_BH1750_lux = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "2") {  
        decoded.P2_BH1750_lux = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "3") {  
        decoded.P3_BH1750_lux = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "4") {  
        decoded.P4_BH1750_lux = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "5") {  
        decoded.P5_BH1750_lux = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "6") {  
        decoded.P6_BH1750_lux = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "7") {  
        decoded.P7_BH1750_lux = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
    }
    if (sensor === "7") { // BMP280
      if (iicport === "0") {  
        decoded.P0_BMP280_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P0_BMP280_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "1") {  
        decoded.P1_BMP280_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P1_BMP280_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "2") {  
        decoded.P2_BMP280_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P2_BMP280_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "3") {  
        decoded.P3_BMP280_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P3_BMP280_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "4") {  
        decoded.P4_BMP280_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P4_BMP280_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "5") {  
        decoded.P5_BMP280_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P5_BMP280_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "6") {  
        decoded.P6_BMP280_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P6_BMP280_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "7") {  
        decoded.P7_BMP280_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P7_BMP280_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
    }
    else if (sensor === "8") { // SHT2X
      if (iicport === "0") {
        decoded.P0_SHT2X_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P0_SHT2X_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "1") {
        decoded.P1_SHT2X_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P1_SHT2X_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "2") {
        decoded.P2_SHT2X_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P2_SHT2X_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "3") {
        decoded.P3_SHT2X_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P3_SHT2X_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "4") {
        decoded.P4_SHT2X_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P4_SHT2X_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "5") {
        decoded.P5_SHT2X_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P5_SHT2X_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "6") {
        decoded.P6_SHT2X_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P6_SHT2X_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "7") {
        decoded.P7_SHT2X_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100); 
        decoded.P7_SHT2X_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
    }
    else if (sensor === "9") { // ADS1015/ADS1115
      if (iicport === "0") {
        decoded.P0_ADS1015_ADC0 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P0_ADS1015_ADC1 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P0_ADS1015_ADC2 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P0_ADS1015_ADC3 =  ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "1") {
        decoded.P1_ADS1015_ADC0 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P1_ADS1015_ADC1 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P1_ADS1015_ADC2 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P1_ADS1015_ADC3 =  ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "2") {
        decoded.P2_ADS1015_ADC0 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P2_ADS1015_ADC1 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P2_ADS1015_ADC2 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P2_ADS1015_ADC3 =  ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "3") {
        decoded.P3_ADS1015_ADC0 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P3_ADS1015_ADC1 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P3_ADS1015_ADC2 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P3_ADS1015_ADC3 =  ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "4") {
        decoded.P4_ADS1015_ADC0 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P4_ADS1015_ADC1 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P4_ADS1015_ADC2 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P4_ADS1015_ADC3 =  ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "5") {
        decoded.P5_ADS1015_ADC0 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P5_ADS1015_ADC1 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P5_ADS1015_ADC2 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P5_ADS1015_ADC3 =  ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "6") {
        decoded.P6_ADS1015_ADC0 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P6_ADS1015_ADC1 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P6_ADS1015_ADC2 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P6_ADS1015_ADC3 =  ((bytes[i++] << 8) | bytes[i++]);
      }
      if (iicport === "7") {
        decoded.P7_ADS1015_ADC0 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P7_ADS1015_ADC1 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P7_ADS1015_ADC2 =  ((bytes[i++] << 8) | bytes[i++]);
        decoded.P7_ADS1015_ADC3 =  ((bytes[i++] << 8) | bytes[i++]);
      }
    }
    else if (sensor === "10") { // MPU9250
      if (iicport === "0") {
        decoded.P0_MPU9250_ax = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P0_MPU9250_aY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P0_MPU9250_aZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P0_MPU9250_gX = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P0_MPU9250_gY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P0_MPU9250_gZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P0_MPU9250_mX = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P0_MPU9250_mY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P0_MPU9250_mZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P0_MPU9250_aSqrt = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P0_MPU9250_mDirection = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "1") {
        decoded.P1_MPU9250_ax = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P1_MPU9250_aY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P1_MPU9250_aZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P1_MPU9250_gX = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P1_MPU9250_gY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P1_MPU9250_gZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P1_MPU9250_mX = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P1_MPU9250_mY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P1_MPU9250_mZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P1_MPU9250_aSqrt = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P1_MPU9250_mDirection = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "2") {
        decoded.P2_MPU9250_ax = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P2_MPU9250_aY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P2_MPU9250_aZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P2_MPU9250_gX = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P2_MPU9250_gY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P2_MPU9250_gZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P2_MPU9250_mX = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P2_MPU9250_mY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P2_MPU9250_mZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P2_MPU9250_aSqrt = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P2_MPU9250_mDirection = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "3") {
        decoded.P3_MPU9250_ax = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P3_MPU9250_aY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P3_MPU9250_aZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P3_MPU9250_gX = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P3_MPU9250_gY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P3_MPU9250_gZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P3_MPU9250_mX = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P3_MPU9250_mY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P3_MPU9250_mZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P3_MPU9250_aSqrt = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P3_MPU9250_mDirection = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "4") {
        decoded.P4_MPU9250_ax = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P4_MPU9250_aY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P4_MPU9250_aZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P4_MPU9250_gX = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P4_MPU9250_gY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P4_MPU9250_gZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P4_MPU9250_mX = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P4_MPU9250_mY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P4_MPU9250_mZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P4_MPU9250_aSqrt = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P4_MPU9250_mDirection = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "5") {
        decoded.P5_MPU9250_ax = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P5_MPU9250_aY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P5_MPU9250_aZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P5_MPU9250_gX = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P5_MPU9250_gY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P5_MPU9250_gZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P5_MPU9250_mX = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P5_MPU9250_mY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P5_MPU9250_mZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P5_MPU9250_aSqrt = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P5_MPU9250_mDirection = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "6") {
        decoded.P6_MPU9250_ax = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P6_MPU9250_aY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P6_MPU9250_aZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P6_MPU9250_gX = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P6_MPU9250_gY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P6_MPU9250_gZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P6_MPU9250_mX = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P6_MPU9250_mY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P6_MPU9250_mZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P6_MPU9250_aSqrt = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P6_MPU9250_mDirection = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "7") {
        decoded.P7_MPU9250_ax = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P7_MPU9250_aY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P7_MPU9250_aZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P7_MPU9250_gX = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P7_MPU9250_gY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P7_MPU9250_gZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P7_MPU9250_mX = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P7_MPU9250_mY = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P7_MPU9250_mZ = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P7_MPU9250_aSqrt = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P7_MPU9250_mDirection = bytesToFloat(bytes.slice(i,i+=4));
      }
    }
    else if (sensor === "11") { // VL53L1X
      if (iicport === "0") {
        decoded.P0_VL53L1X_distance = bytesToInt(bytes.slice(i,i+=4));
      }
      if (iicport === "1") {
        decoded.P1_VL53L1X_distance = bytesToInt(bytes.slice(i,i+=4));
      }
      if (iicport === "2") {
        decoded.P2_VL53L1X_distance = bytesToInt(bytes.slice(i,i+=4));
      }
      if (iicport === "3") {
        decoded.P3_VL53L1X_distance = bytesToInt(bytes.slice(i,i+=4));
      }
      if (iicport === "4") {
        decoded.P4_VL53L1X_distance = bytesToInt(bytes.slice(i,i+=4));
      }
      if (iicport === "5") {
        decoded.P5_VL53L1X_distance = bytesToInt(bytes.slice(i,i+=4));
      }
      if (iicport === "6") {
        decoded.P6_VL53L1X_distance = bytesToInt(bytes.slice(i,i+=4));
      }
      if (iicport === "7") {
        decoded.P7_VL53L1X_distance = bytesToInt(bytes.slice(i,i+=4));
      }
    }
    else if (sensor === "12") { // HMC5883L
      if (iicport === "0") {
        decoded.P0_HMC5883L_X = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P0_HMC5883L_Y = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P0_HMC5883L_Z = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "1") {
        decoded.P1_HMC5883L_X = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P1_HMC5883L_Y = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P1_HMC5883L_Z = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "2") {
        decoded.P2_HMC5883L_X = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P2_HMC5883L_Y = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P2_HMC5883L_Z = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "3") {
        decoded.P3_HMC5883L_X = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P3_HMC5883L_Y = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P3_HMC5883L_Z = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "4") {
        decoded.P4_HMC5883L_X = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P4_HMC5883L_Y = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P4_HMC5883L_Z = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "5") {
        decoded.P5_HMC5883L_X = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P5_HMC5883L_Y = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P5_HMC5883L_Z = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "6") {
        decoded.P6_HMC5883L_X = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P6_HMC5883L_Y = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P6_HMC5883L_Z = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "7") {
        decoded.P7_HMC5883L_X = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P7_HMC5883L_Y = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P7_HMC5883L_Z = bytesToFloat(bytes.slice(i,i+=4));
      }
    }
    else if (sensor === "13") { // MLX90614
      if (iicport === "0") {
        decoded.P0_VMLX90614_ambienttemp = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P0_VMLX90614_objecttemp = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "1") {
        decoded.P1_VMLX90614_ambienttemp = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P1_VMLX90614_objecttemp = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "2") {
        decoded.P2_VMLX90614_ambienttemp = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P2_VMLX90614_objecttemp = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "3") {
        decoded.P3_VMLX90614_ambienttemp = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P3_VMLX90614_objecttemp = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "4") {
        decoded.P4_VMLX90614_ambienttemp = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P4_VMLX90614_objecttemp = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "5") {
        decoded.P5_VMLX90614_ambienttemp = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P5_VMLX90614_objecttemp = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "6") {
        decoded.P6_VMLX90614_ambienttemp = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P6_VMLX90614_objecttemp = bytesToFloat(bytes.slice(i,i+=4));
      }
      if (iicport === "7") {
        decoded.P7_VMLX90614_ambienttemp = bytesToFloat(bytes.slice(i,i+=4));
        decoded.P7_VMLX90614_objecttemp = bytesToFloat(bytes.slice(i,i+=4));
      }
    }
    else if (sensor === "14") { // SHT31
      if (iicport === "0") {
        decoded.P0_SHT31_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);
        decoded.P0_SHT31_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "1") {
        decoded.P1_SHT31_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);
        decoded.P1_SHT31_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "2") {
        decoded.P2_SHT31_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);
        decoded.P2_SHT31_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "3") {
        decoded.P3_SHT31_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);
        decoded.P3_SHT31_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "4") {
        decoded.P4_SHT31_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);
        decoded.P4_SHT31_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "5") {
        decoded.P5_SHT31_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);
        decoded.P5_SHT31_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "6") {
        decoded.P6_SHT31_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);
        decoded.P6_SHT31_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
      if (iicport === "7") {
        decoded.P7_SHT31_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);
        decoded.P7_SHT31_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10);
      }
    }
    else if (sensor === "100") { // OneWire
      decoded.OW_01_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);   
    }
    else if (sensor === "101") { // OneWire
      decoded.OW_02_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);   
    }
    else if (sensor === "102") { // OneWire
      decoded.OW_03_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);   
    }
    else if (sensor === "103") { // OneWire
      decoded.OW_04_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100);   
    }
    else if (sensor == "200") { // GPS_SER
      decoded.latitude = bytesToFloat(bytes.slice(i,i+=4));
      decoded.longitude = bytesToFloat(bytes.slice(i,i+=4));
      decoded.altitude = bytesToFloat(bytes.slice(i,i+=4));
      decoded.course = bytesToFloat(bytes.slice(i,i+=4));
      decoded.speed = bytesToFloat(bytes.slice(i,i+=4));
      decoded.hdop = bytesToFloat(bytes.slice(i,i+=4));
    }
  } 
  decoded.battery = ((bytes[i++] << 8) | bytes[i++]);
  decoded.trigger = bytes[i++];
}

return decoded;
}