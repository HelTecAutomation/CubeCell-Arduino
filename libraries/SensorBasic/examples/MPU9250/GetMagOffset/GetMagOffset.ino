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
 *  3. The accelerometer's measurement range is up to ??16g (g is gravitational
 * acceleration), and the static measurement accuracy is high.
 *
 *  4. The magnetometer uses a high-intensity Hall-type sensor for data acquisition. 
 * The magnetic induction measurement range is ??4800??T, which can be used for auxiliary
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
 * MPU9250 Get Mag Offset original project available here: https://github.com/asukiaaa/MPU9250_asukiaaa
 */

#include <MPU9250.h>

#define CALIB_SEC 20

MPU9250 mySensor;

uint8_t sensorId;
float mDirection, mX, mY, mZ;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("started");

  mySensor.setWire(&Wire);
  while (mySensor.readId(&sensorId) != 0) {
    Serial.println("Cannot find device to read sensorId");
    delay(2000);
  }
  mySensor.beginMag();

  float magXMin, magXMax, magYMin, magYMax, magZ, magZMin, magZMax;

  Serial.println("Start scanning values of magnetometer to get offset values.");
  Serial.println("Rotate your device for " + String(CALIB_SEC) + " seconds.");
  setMagMinMaxAndSetOffset(&mySensor, CALIB_SEC);
  Serial.println("Finished setting offset values.");
}

void setMagMinMaxAndSetOffset(MPU9250* sensor, int seconds) {
  unsigned long calibStartAt = millis();
  float magX, magXMin, magXMax, magY, magYMin, magYMax, magZ, magZMin, magZMax;

  sensor->magUpdate();
  magXMin = magXMax = sensor->magX();
  magYMin = magYMax = sensor->magY();
  magZMin = magZMax = sensor->magZ();

  while(millis() - calibStartAt < (unsigned long) seconds * 1000) {
    delay(100);
    sensor->magUpdate();
    magX = sensor->magX();
    magY = sensor->magY();
    magZ = sensor->magZ();
    if (magX > magXMax) magXMax = magX;
    if (magY > magYMax) magYMax = magY;
    if (magZ > magZMax) magZMax = magZ;
    if (magX < magXMin) magXMin = magX;
    if (magY < magYMin) magYMin = magY;
    if (magZ < magZMin) magZMin = magZ;
  }

  sensor->magXOffset = - (magXMax - magXMin) / 2;
  sensor->magYOffset = - (magYMax - magYMin) / 2;
  sensor->magZOffset = - (magZMax - magZMin) / 2;
}

void loop() {
  Serial.println("sensorId: " + String(sensorId));

  mySensor.magUpdate();
  mX = mySensor.magX();
  mY = mySensor.magY();
  mZ = mySensor.magZ();
  mDirection = mySensor.magHorizDirection();
  Serial.println("mySensor.magXOffset = " + String(mySensor.magXOffset) + ";");
  Serial.println("mySensor.maxYOffset = " + String(mySensor.magYOffset) + ";");
  Serial.println("mySensor.magZOffset = " + String(mySensor.magZOffset) + ";");
  Serial.println("magX: " + String(mX));
  Serial.println("maxY: " + String(mY));
  Serial.println("magZ: " + String(mZ));
  Serial.println("horizontal direction: " + String(mDirection));

  Serial.println("at " + String(millis()) + "ms");
  Serial.println(""); // Add an empty line
  delay(500);
}