function bytesToFloat(by) {
    var bits = by[3]<<24 | by[2]<<16 | by[1]<<8 | by[0];
    var sign = (bits>>>31 === 0) ? 1.0 : -1.0;
    var e = bits>>>23 & 0xff;
    var m = (e === 0) ? (bits & 0x7fffff)<<1 : (bits & 0x7fffff) | 0x800000;
    var f = sign * m * Math.pow(2, e - 150);
    return f;
  } 
  
function Decoder(bytes, port) {
  
  // Decode an uplink message from a buffer
  // (array) of bytes to an object of fields.
  var decoded = {};

  if (port === 2) {
    var i = 1;
    sensor = bytes[0].toFixed(0);
    decoded.sensortype = sensor;
    if (sensor === "0") { // MJMCU-8128
      decoded.temperature = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100; 
      decoded.humidity = ((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10;
      decoded.pressure = ((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10;
      decoded.lux = ((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10;
      decoded.co2 =  ((bytes[i++] << 8) | bytes[i++]).toFixed(0);
      decoded.tvoc =  ((bytes[i++] << 8) | bytes[i++]).toFixed(0);
      decoded.battery = ((bytes[i++] << 8) | bytes[i++]).toFixed(0);
    }
    if (sensor === "1") { // BME680
      decoded.temperature = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100; 
      decoded.humidity = ((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10;
      decoded.pressure = ((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10;
      decoded.gas = ((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10;
      decoded.battery = ((bytes[i++] << 8) | bytes[i++]).toFixed(0);
    }
    if (sensor === "2") { // BME280
      decoded.temperature = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100; 
      decoded.humidity = ((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10;
      decoded.pressure = ((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10;
      decoded.battery = ((bytes[i++] << 8) | bytes[i++]).toFixed(0);
    }
    if (sensor === "3") { // CCS811
      decoded.co2 =  ((bytes[i++] << 8) | bytes[i++]).toFixed(0);
      decoded.tvoc =  ((bytes[i++] << 8) | bytes[i++]).toFixed(0);
      decoded.battery = ((bytes[i++] << 8) | bytes[i++]).toFixed(0);
    }
    if (sensor === "4") { // HDC1080
      decoded.temperature = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100; 
      decoded.humidity = ((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10;
      decoded.battery = ((bytes[i++] << 8) | bytes[i++]).toFixed(0);
    }
    if (sensor === "5") { // BMP180
      decoded.temperature = (((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10) - 100; 
      decoded.pressure = ((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10;
      decoded.battery = ((bytes[i++] << 8) | bytes[i++]).toFixed(0);
    }
    if (sensor === "6") { // BH1750
      decoded.lux = ((bytes[i++] << 8) | bytes[i++]).toFixed(0) / 10;
      decoded.battery = ((bytes[i++] << 8) | bytes[i++]).toFixed(0);
    }
  }

  return decoded;
}