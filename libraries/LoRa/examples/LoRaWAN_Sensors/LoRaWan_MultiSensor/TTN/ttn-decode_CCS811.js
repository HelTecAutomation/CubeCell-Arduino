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
    var i = 0;
    decoded.temperature = bytesToFloat(bytes.slice(i,i+=4)).toFixed(2);
    decoded.co2 =  bytesToFloat(bytes.slice(i,i+=4)).toFixed(0);
    decoded.tvoc =  bytesToFloat(bytes.slice(i,i+=4)).toFixed(0);
    decoded.battery = ((bytes[24] << 8) | bytes[25]).toFixed(0);
  }

  return decoded;
}
