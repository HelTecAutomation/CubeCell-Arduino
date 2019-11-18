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
  decoded.AppDataSize = bytes.length;
  
  if (port === 2) {
    while (i < bytes.length-2) {
      sensor = bytes[i++].toFixed(0);
      if (sensor === "1") { // BME680
        decoded.BME680_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100).toFixed(1); 
        decoded.BME680_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10).toFixed(1);
        decoded.BME680_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10).toFixed(1);
        decoded.BME680_gas = ((bytes[i++] << 8) | bytes[i++]).toFixed(0);
        decoded.BME680_IAQ = ((bytes[i++] << 8) | bytes[i++]).toFixed(0);
      }
      else if (sensor === "2") { // BME280
        decoded.BME280_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100).toFixed(1); 
        decoded.BME280_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10).toFixed(1);
        decoded.BME280_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10).toFixed(1);
      }
      else if (sensor === "3") { // CCS811
        decoded.CCS811_co2 =  ((bytes[i++] << 8) | bytes[i++]).toFixed(0);
        decoded.CCS811_tvoc =  ((bytes[i++] << 8) | bytes[i++]).toFixed(0);
      }
      else if (sensor === "4") { // HDC1080
        decoded.HDC0180_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100).toFixed(1); 
        decoded.HDC1080_humidity = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10).toFixed(1);
      }
      else if (sensor === "5") { // BMP180
        decoded.BMP180_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100).toFixed(1); 
        decoded.BMP180_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10).toFixed(1);
      }
      else if (sensor === "6") { // BH1750
        decoded.BH1750_lux = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10).toFixed(1);
      }
      else if (sensor === "7") { // BMP280
        decoded.BMP280_temperature = ((((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100).toFixed(1); 
        decoded.BMP280_pressure = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10).toFixed(1);
      }
    } 
    decoded.battery = ((bytes[i++] << 8) | bytes[i++]).toFixed(0);
  }

  return decoded;
}