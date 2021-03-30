/* Heltec Automation MPU9520 Sensors test example
 *
 * Function:
 * 9-axis sensor
 *
 * Description:
 *  1.The MPU9250 integrates a 3-axis gyroscope, a 3-axis accelerometer, 
 * and a 3-axis magnetometer. The output is 16-bit digital.
 *
 *  2. Data can be exchanged via the Integrated Circuit Bus (IIC) interface 
 * and the microcontroller with a transfer rate of up to 400 kHz / s.
 *
 *  3. The accelerometer's measurement range is up to ��16g (g is gravitational
 * acceleration), and the static measurement accuracy is high.
 *
 *  4. The magnetometer uses a high-intensity Hall-type sensor for data acquisition. 
 * The magnetic induction measurement range is ��4800��T, which can be used for auxiliary
 * measurement of yaw angle.
 *
 *  5.The MPU9250's built-in digital motion processor (DMP: Hardware Motion Processor)
 * hardware acceleration engine integrates nine-axis sensor  data and outputs 
 * complete 9-axis fusion calculation data to the application.
 *
 * HelTec AutoMation, Chengdu, China
 * www.heltec.org
 *
 * this project also realess in GitHub:
 * https://github.com/HelTecAutomation/ASR650x-Arduino
 * 
 */

#include <MPU9250.h>

MPU9250 mySensor;
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;

void setup() {
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext,LOW);
  Serial.begin(115200);
  //while(!Serial);
  Serial.println("started");

//#ifdef _ESP32_HAL_I2C_H_ // For ESP32
  Wire.begin();
  mySensor.setWire(&Wire);
//#endif

  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();

  // You can set your own offset for mag values
  // mySensor.magXOffset = -50;
  // mySensor.magYOffset = -55;
  // mySensor.magZOffset = -10;
}

void loop() {
  uint8_t sensorId;
  if (mySensor.readId(&sensorId) == 0) {
    Serial.println("sensorId: " + String(sensorId));
  } else {
    Serial.println("Cannot read sensorId");
  }

  if (mySensor.accelUpdate() == 0) {
    aX = mySensor.accelX();
    aY = mySensor.accelY();
    aZ = mySensor.accelZ();
    aSqrt = mySensor.accelSqrt();
    Serial.println("accelX: " + String(aX));
    Serial.println("accelY: " + String(aY));
    Serial.println("accelZ: " + String(aZ));
    Serial.println("accelSqrt: " + String(aSqrt));
  } else {
    Serial.println("Cannod read accel values");
  }

  if (mySensor.gyroUpdate() == 0) {
    gX = mySensor.gyroX();
    gY = mySensor.gyroY();
    gZ = mySensor.gyroZ();
    Serial.println("gyroX: " + String(gX));
    Serial.println("gyroY: " + String(gY));
    Serial.println("gyroZ: " + String(gZ));
  } else {
    Serial.println("Cannot read gyro values");
  }

  if (mySensor.magUpdate() == 0) {
    mX = mySensor.magX();
    mY = mySensor.magY();
    mZ = mySensor.magZ();
    mDirection = mySensor.magHorizDirection();
    Serial.println("magX: " + String(mX));
    Serial.println("maxY: " + String(mY));
    Serial.println("magZ: " + String(mZ));
    Serial.println("horizontal direction: " + String(mDirection));
  } else {
    Serial.println("Cannot read mag values");
  }

  mySensor.computeEulerAngles();
  Serial.println("pitch: " + String(mySensor.pitch()));
  Serial.println("roll: " + String(mySensor.roll()));
  Serial.println("yaw: " + String(mySensor.yaw()));

  Serial.println("at " + String(millis()) + "ms");
  Serial.println(""); // Add an empty line
  delay(500);
}
