static void prepareTxFrame(uint8_t port)
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(500);
  pinMode(GPIO0, OUTPUT);
  digitalWrite(GPIO0, LOW);

  appDataSize = 0;
  int pnr = 0;
  int modnr = 0;

#if (ModularNode == 1 || TCA9548 == 1)
  for (pnr = minpnr; pnr < (maxpnr+1); pnr++)
  {
    #if (ModularNode == 1)
      if (pnr == 4) {
        pnr = 7;
      }
    #endif
    if (ModularNode == 1 && (pnr == 0 || pnr == 7)) {
      Serial.print("Modular Port ");
    } else {
      Serial.print("Sensor Port ");
    }
    #if (TCA9548 == 1) 
      Serial.print("Sensor Port ");
    #endif
    if (ModularNode == 1 && pnr == 7) {
      Serial.println("1");
    } else {
      Serial.println(pnr);
    }
#endif

    /*
        BME680
    */

    if (BME_680_e[pnr]) // 1
    {
      Wire.begin();

#if (ModularNode == 1 || TCA9548 == 1)
      tcaselect(pnr);
      delay(100);
#endif

      bme680.begin(I2C_STANDARD_MODE);
      delay(1000);
      bme680.setOversampling(TemperatureSensor, Oversample16);
      bme680.setOversampling(HumiditySensor, Oversample16);
      bme680.setOversampling(PressureSensor, Oversample16);
      bme680.setIIRFilter(IIR4);
      bme680.setGas(320, 150); // 320C for 150 milliseconds

      static int32_t temperature, humidity, pressure, gas;
      bme680.getSensorData(temperature, humidity, pressure, gas);
      delay(500);
      bme680.getSensorData(temperature, humidity, pressure, gas);
      while ((temperature > 6000 || humidity == 100 || humidity == 0) && count < maxtry)
      {
        bme680.getSensorData(temperature, humidity, pressure, gas);
        delay(250);
      }

      Temperature = temperature / 100.0;
      Humidity = humidity / 1000.0;
      Pressure = pressure / 100.0;
      co2 = gas / 100.0;
      tvoc = CalculateIAQ();

      Wire.end();

      modnr = pnr;
#if (ModularNode == 1)
      if (modnr == 0 || modnr == 7) {
        modnr += 200;
      }
#endif
      appData[appDataSize++] = modnr;
      appData[appDataSize++] = 1;

      appData[appDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0) >> 8);
      appData[appDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0));

      appData[appDataSize++] = (uint8_t)((int)(Humidity * 10.0) >> 8);
      appData[appDataSize++] = (uint8_t)((int)(Humidity * 10.0));

      appData[appDataSize++] = (uint8_t)((int)(Pressure * 10.0) >> 8);
      appData[appDataSize++] = (uint8_t)((int)(Pressure * 10.0));

      appData[appDataSize++] = (uint8_t)((int)co2 >> 8);
      appData[appDataSize++] = (uint8_t)((int)co2);

      appData[appDataSize++] = (uint8_t)((int)tvoc >> 8);
      appData[appDataSize++] = (uint8_t)((int)tvoc);

      Serial.print("  BME680: T = ");
      Serial.print(Temperature);
      Serial.print("C, RH = ");
      Serial.print(Humidity);
      Serial.print(" %, Pressure = ");
      Serial.print(Pressure);
      Serial.print(" hPA, GAS = ");
      Serial.print(co2);
      Serial.print("mOhm, IAQ = ");
      Serial.println(tvoc);
    }

    /*
      BME280
    */

    if (BME_280_e[pnr]) // 2
    {
      Wire.begin();

#if (ModularNode == 1 || TCA9548 == 1)
      tcaselect(pnr);
      delay(100);
#endif
      if (!bme280.init())
      {
        Serial.println("  BME280 error!");
      }
      delay(250);
      Temperature = bme280.getTemperature();
      Temperature = bme280.getTemperature();
      Pressure = bme280.getPressure() / 100.0;
      Humidity = bme280.getHumidity();

      Wire.end();

      modnr = pnr;
#if (ModularNode == 1)
      if (modnr == 0 || modnr == 7) {
        modnr += 200;
      }
#endif
      appData[appDataSize++] = modnr;
      appData[appDataSize++] = 2;

      appData[appDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0) >> 8);
      appData[appDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0));

      appData[appDataSize++] = (uint8_t)((int)(Humidity * 10.0) >> 8);
      appData[appDataSize++] = (uint8_t)((int)(Humidity * 10.0));

      appData[appDataSize++] = (uint8_t)((int)(Pressure * 10.0) >> 8);
      ;
      appData[appDataSize++] = (uint8_t)((int)(Pressure * 10.0));

      Serial.print("  BME280: T = ");
      Serial.print(Temperature);
      Serial.print("C, RH = ");
      Serial.print(Humidity);
      Serial.print(" %, Pressure = ");
      Serial.print(Pressure);
      Serial.println(" hPA");
    }

    /*
        HDC1080
    */

    if (HDC_1080_e[pnr]) // 4
    {
      Wire.begin();

#if (ModularNode == 1 || TCA9548 == 1)
      tcaselect(pnr);
      delay(100);
#endif

      count = 0;
      hdc1080.begin(0x40);
      delay(500);
      Temperature = (float)(hdc1080.readTemperature());
      Humidity = (float)(hdc1080.readHumidity());
      Wire.end();
      while (Temperature > 120.0 && count < maxtry)
      {
        hdc1080.begin(0x40);
        delay(500);
        Temperature = (float)(hdc1080.readTemperature());
        Humidity = (float)(hdc1080.readHumidity());
        Wire.end();
        count++;
        delay(500);
      }
      if (Temperature > 120.0)
      {
        Temperature = 0.0;
        Humidity = 0.0;
        Serial.println("  HDC ERROR");
      }
      hdc1080.end();

      modnr = pnr;
#if (ModularNode == 1)
      if (modnr == 0 || modnr == 7) {
        modnr += 200;
      }
#endif
      appData[appDataSize++] = modnr;
      appData[appDataSize++] = 4;

      appData[appDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0) >> 8);
      appData[appDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0));

      appData[appDataSize++] = ((int)(Humidity * 10.0)) >> 8;
      appData[appDataSize++] = (int)(Humidity * 10.0);

      Serial.print("  HDC1080: T = ");
      Serial.print(Temperature);
      Serial.print("C, RH = ");
      Serial.print(Humidity);
      Serial.println(" % ");
    }

    /*
      CCS811
    */

    if (CCS_811_e[pnr]) // 3
    {
      Wire.begin();

#if (ModularNode == 1 || TCA9548 == 1)
      tcaselect(pnr);
      delay(100);
#endif

      count = 0;
      ccs.begin();
      delay(1000);

      FLASH_read_at(addr, baselineflash, sizeof(baselineflash));
      baselinetemp = (baselineflash[0] << 8) | baselineflash[1];
      if (baselinetemp > 0)
      {
        baseline = baselinetemp;
        Serial.print("  CCS811 Read BaseLine: ");
        Serial.println(baseline);
        ccs.setBaseline(baseline);
      }
      delay(5000);

      while (!ccs.available())
        ;
      ccs.readData();
      Temperature = ccs.calculateTemperature();
      co2 = ccs.geteCO2();
      tvoc = ccs.getTVOC();

      baseline = ccs.getBaseline();
      baselineflash[0] = (uint8_t)(baseline >> 8);
      baselineflash[1] = (uint8_t)baseline;
      FLASH_update(addr, baselineflash, sizeof(baselineflash));
      Serial.print("  CCS811 Write BaseLine: ");
      Serial.println(baseline);
      Wire.end();
      while (co2 > 65500.0 && count < maxtry)
      {
        ccs.begin();
        delay(1000);
        while (!ccs.available())
          ;
        ccs.readData();
        co2 = ccs.geteCO2();
        tvoc = ccs.getTVOC();
        Wire.end();
        count++;
      }
      if (co2 > 65500.0)
      {
        co2 = 0.0;
        tvoc = 0.0;
        Serial.println("  CCS ERROR");
      }

      modnr = pnr;
#if (ModularNode == 1)
      if (modnr == 0 || modnr == 7) {
        modnr += 200;
      }
#endif
      appData[appDataSize++] = modnr;
      appData[appDataSize++] = 3;

      appData[appDataSize++] = (uint8_t)((int)co2 >> 8);
      appData[appDataSize++] = (uint8_t)((int)co2);

      appData[appDataSize++] = (uint8_t)((int)tvoc >> 8);
      appData[appDataSize++] = (uint8_t)((int)tvoc);

      Serial.print("  CCS811: CO2 = ");
      Serial.print(co2);
      Serial.print(" ppm, TVOC = ");
      Serial.print(tvoc);
      Serial.print(" ppb, Baseline: ");
      Serial.println(baseline);
    }

    /*
      BMP180
    */

    if (BMP_180_e[pnr]) // 5
    {
      Wire.begin();

#if (ModularNode == 1 || TCA9548 == 1)
      tcaselect(pnr);
      delay(100);
#endif

      count = 0;
      bmp180.begin();
      delay(500);
      Temperature = (float)(bmp180.readTemperature());
      Pressure = (float)(bmp180.readPressure()) / 100.0;
      Wire.end();
      while (Temperature > 120.0 && count < maxtry)
      {
        bmp180.begin();
        delay(500);
        Temperature = (float)(bmp180.readTemperature());
        Pressure = (float)(bmp180.readPressure()) / 100.0;
        Wire.end();
        count++;
        delay(500);
      }
      if (Temperature > 120.0)
      {
        Temperature = 0.0;
        Humidity = 0.0;
        Serial.println("  BMP ERROR");
      }

      modnr = pnr;
#if (ModularNode == 1)
      if (modnr == 0 || modnr == 7) {
        modnr += 200;
      }
#endif
      appData[appDataSize++] = modnr;
      appData[appDataSize++] = 5;

      appData[appDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0) >> 8);
      appData[appDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0));

      appData[appDataSize++] = (uint8_t)((int)(Pressure * 10.0) >> 8);
      ;
      appData[appDataSize++] = (uint8_t)((int)(Pressure * 10.0));

      Serial.print("  BMP180: T = ");
      Serial.print(Temperature);
      Serial.print("C, P = ");
      Serial.print(Pressure);
      Serial.println("hPa");
    }

    /*
      BH1750
    */

    if (BH_1750_e[pnr]) // 6
    {

      pinMode(GPIO0, OUTPUT);
      digitalWrite(GPIO0, HIGH);

      Wire.begin();

#if (ModularNode == 1 || TCA9548 == 1)
      tcaselect(pnr);
      delay(100);
#endif

      count = 0;
      lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE_2);
      delay(100);
      lux = lightMeter.readLightLevel();
      delay(100);
      lux = lightMeter.readLightLevel();
      lightMeter.end();
      Wire.end();

      modnr = pnr;
#if (ModularNode == 1)
      if (modnr == 0 || modnr == 7) {
        modnr += 200;
      }
#endif
      appData[appDataSize++] = modnr;
      appData[appDataSize++] = 6;

      appData[appDataSize++] = (uint8_t)((int)(lux * 10.0) >> 8);
      appData[appDataSize++] = (uint8_t)((int)(lux * 10.0));

      Serial.print("  BH1750: Light = ");
      Serial.print(lux);
      Serial.println("lx");

      digitalWrite(GPIO0, LOW);
	    pinMode(GPIO0, ANALOG);
    }

    /*
        BMP_280
    */

    if (BMP_280_e[pnr]) // 7
    {
      Wire.begin();

#if (ModularNode == 1 || TCA9548 == 1)
      tcaselect(pnr);
      delay(100);
#endif

      count = 0;
      bmp280.begin();
      delay(500);
      bmp280.setSampling(BMP280::MODE_NORMAL,     /* Operating Mode. */
                         BMP280::SAMPLING_X2,     /* Temp. oversampling */
                         BMP280::SAMPLING_X16,    /* Pressure oversampling */
                         BMP280::FILTER_X16,      /* Filtering. */
                         BMP280::STANDBY_MS_500); /* Standby time. */
      Temperature = bmp280.readTemperature();
      Pressure = (float)bmp280.readPressure() / 100.0;
      Wire.end();
      while (Pressure > 1190.0 && count < maxtry)
      {
        bmp280.begin();
        delay(500);
        bmp280.setSampling(BMP280::MODE_NORMAL,     /* Operating Mode. */
                           BMP280::SAMPLING_X2,     /* Temp. oversampling */
                           BMP280::SAMPLING_X16,    /* Pressure oversampling */
                           BMP280::FILTER_X16,      /* Filtering. */
                           BMP280::STANDBY_MS_500); /* Standby time. */
        Pressure = (float)bmp280.readPressure() / 100.0;
        Wire.end();
        count++;
        delay(500);
      }
      if (Pressure > 1190.0)
      {
        Pressure = 0;
        Serial.println("  BMP ERROR");
      }

      modnr = pnr;
#if (ModularNode == 1)
      if (modnr == 0 || modnr == 7) {
        modnr += 200;
      }
#endif
      appData[appDataSize++] = modnr;

      appData[appDataSize++] = 7;

      appData[appDataSize++] = (uint8_t)(((int)((Temperature + 100.0) * 10.0)) >> 8);
      appData[appDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0));

      appData[appDataSize++] = (uint8_t)(((int)(Pressure * 10.0)) >> 8);
      appData[appDataSize++] = (uint8_t)((int)(Pressure * 10.0));

      Serial.print("  BMP280: T=");
      Serial.print(Temperature);
      Serial.print("C, Pressure=");
      Serial.print(Pressure);
      Serial.println(" hPA");
    }

    /*
        SHT_2X
    */

    if (SHT_2X_e[pnr])
    {
      Wire.begin();

#if (ModularNode == 1 || TCA9548 == 1)
      tcaselect(pnr);
      delay(100);
#endif

      delay(500);
      Temperature = SHT2x.GetTemperature();
      Humidity = SHT2x.GetHumidity();
      /*
      count = 0;
      while (Humidity == 0.0 && count < maxtry)
      {
        Temperature = SHT2x.GetTemperature();
        Humidity = SHT2x.GetHumidity();
        count++;
        delay(100);
      }
      if (Humidity == 0.0)
      {
        //Temperature = 0.0;
        //Humidity = 0.0;
        Serial.println("  SHT2X ERROR");
      }
      */
      Wire.end();

      modnr = pnr;
#if (ModularNode == 1)
      if (modnr == 0 || modnr == 7) {
        modnr += 200;
      }
#endif
      appData[appDataSize++] = modnr;
      appData[appDataSize++] = 8;

      appData[appDataSize++] = (uint8_t)(((int)((Temperature + 100.0) * 10.0)) >> 8);
      appData[appDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0));

      appData[appDataSize++] = ((int)(Humidity * 10.0)) >> 8;
      appData[appDataSize++] = (int)(Humidity * 10.0);

      Serial.print("  SHT2X: T=");
      Serial.print(Temperature);
      Serial.print("C, Humidity=");
      Serial.print(Humidity);
      Serial.println(" %");
    }

    /*
        ADS_1015
    */

    if (ADS_1015_e[pnr])
    {
      Wire.begin();

#if (ModularNode == 1 || TCA9548 == 1)
      tcaselect(pnr);
      delay(100);
#endif

      ads1015.begin();
      int16_t adc0, adc1, adc2, adc3;

      adc0 = ads1015.readADC_SingleEnded(0);
      adc1 = ads1015.readADC_SingleEnded(1);
      adc2 = ads1015.readADC_SingleEnded(2);
      adc3 = ads1015.readADC_SingleEnded(3);

      modnr = pnr;
#if (ModularNode == 1)
      if (modnr == 0 || modnr == 7) {
        modnr += 200;
      }
#endif
      appData[appDataSize++] = modnr;
      appData[appDataSize++] = 9;

      appData[appDataSize++] = ((int)adc0) >> 8;
      appData[appDataSize++] = (int)adc0;
      appData[appDataSize++] = ((int)adc1) >> 8;
      appData[appDataSize++] = (int)adc1;
      appData[appDataSize++] = ((int)adc2) >> 8;
      appData[appDataSize++] = (int)adc2;
      appData[appDataSize++] = ((int)adc3) >> 8;
      appData[appDataSize++] = (int)adc3;

      Serial.print("  ADS1015: ADC0=");
      Serial.print(adc0);
      Serial.print(", ADC1=");
      Serial.print(adc1);
      Serial.print(", ADC2=");
      Serial.print(adc2);
      Serial.print(", ADC3=");
      Serial.print(adc3);
      Serial.println();

      Wire.end();
    }

    /*
      MPU9250
    */

    if (MPU_9250_e[pnr]) // 10
    {
      Wire.begin();

#if (ModularNode == 1 || TCA9548 == 1)
      tcaselect(pnr);
      delay(100);
#endif

      delay(500);
      mpu9250.setWire(&Wire);
      delay(100);
      mpu9250.beginAccel();
      mpu9250.beginGyro();
      mpu9250.beginMag();
      delay(200);
      uint8_t sensorId;

      if (mpu9250.readId(&sensorId) == 0)
      {
        Serial.println("  MPU9250 sensorId: " + String(sensorId));
      }
      else
      {
        Serial.println("  Cannot read sensorId");
      }

      float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
      if (mpu9250.accelUpdate() == 0)
      {
        aX = mpu9250.accelX();
        aY = mpu9250.accelY();
        aZ = mpu9250.accelZ();
        aSqrt = mpu9250.accelSqrt();
        Serial.print("  accelX: " + String(aX));
        Serial.print(", accelY: " + String(aY));
        Serial.print(", accelZ: " + String(aZ));
        Serial.println(", accelSqrt: " + String(aSqrt));
      }
      else
      {
        Serial.println("  Cannod read accel values");
      }

      if (mpu9250.gyroUpdate() == 0)
      {
        gX = mpu9250.gyroX();
        gY = mpu9250.gyroY();
        gZ = mpu9250.gyroZ();
        Serial.print("  gyroX: " + String(gX));
        Serial.print(", gyroY: " + String(gY));
        Serial.println(", gyroZ: " + String(gZ));
      }
      else
      {
        Serial.println("  Cannot read gyro values");
      }

      if (mpu9250.magUpdate() == 0)
      {
        mX = mpu9250.magX();
        mY = mpu9250.magY();
        mZ = mpu9250.magZ();
        mDirection = mpu9250.magHorizDirection();
        Serial.print("  magX: " + String(mX));
        Serial.print(", maxY: " + String(mY));
        Serial.print(", magZ: " + String(mZ));
        Serial.println(", horizontal direction: " + String(mDirection));
      }
      else
      {
        Serial.println("  Cannot read mag values");
      }

      Wire.end();

      modnr = pnr;
#if (ModularNode == 1)
      if (modnr == 0 || modnr == 7) {
        modnr += 200;
      }
#endif
      appData[appDataSize++] = modnr;
      appData[appDataSize++] = 10;

      unsigned char *puc;
      puc = (unsigned char *)(&aX);
      appData[appDataSize++] = puc[0];
      appData[appDataSize++] = puc[1];
      appData[appDataSize++] = puc[2];
      appData[appDataSize++] = puc[3];
      puc = (unsigned char *)(&aY);
      appData[appDataSize++] = puc[0];
      appData[appDataSize++] = puc[1];
      appData[appDataSize++] = puc[2];
      appData[appDataSize++] = puc[3];
      puc = (unsigned char *)(&aZ);
      appData[appDataSize++] = puc[0];
      appData[appDataSize++] = puc[1];
      appData[appDataSize++] = puc[2];
      appData[appDataSize++] = puc[3];
      puc = (unsigned char *)(&gX);
      appData[appDataSize++] = puc[0];
      appData[appDataSize++] = puc[1];
      appData[appDataSize++] = puc[2];
      appData[appDataSize++] = puc[3];
      puc = (unsigned char *)(&gY);
      appData[appDataSize++] = puc[0];
      appData[appDataSize++] = puc[1];
      appData[appDataSize++] = puc[2];
      appData[appDataSize++] = puc[3];
      puc = (unsigned char *)(&gZ);
      appData[appDataSize++] = puc[0];
      appData[appDataSize++] = puc[1];
      appData[appDataSize++] = puc[2];
      appData[appDataSize++] = puc[3];
      puc = (unsigned char *)(&mX);
      appData[appDataSize++] = puc[0];
      appData[appDataSize++] = puc[1];
      appData[appDataSize++] = puc[2];
      appData[appDataSize++] = puc[3];
      puc = (unsigned char *)(&mY);
      appData[appDataSize++] = puc[0];
      appData[appDataSize++] = puc[1];
      appData[appDataSize++] = puc[2];
      appData[appDataSize++] = puc[3];
      puc = (unsigned char *)(&mZ);
      appData[appDataSize++] = puc[0];
      appData[appDataSize++] = puc[1];
      appData[appDataSize++] = puc[2];
      appData[appDataSize++] = puc[3];
      puc = (unsigned char *)(&aSqrt);
      appData[appDataSize++] = puc[0];
      appData[appDataSize++] = puc[1];
      appData[appDataSize++] = puc[2];
      appData[appDataSize++] = puc[3];
      puc = (unsigned char *)(&mDirection);
      appData[appDataSize++] = puc[0];
      appData[appDataSize++] = puc[1];
      appData[appDataSize++] = puc[2];
      appData[appDataSize++] = puc[3];
    }

    /*
        VL53L1X
    */

    if (LR_VL53L1X_e[pnr]) // 11
    {
      Wire.begin();

#if (ModularNode == 1 || TCA9548 == 1)
      tcaselect(pnr);
      delay(100);
#endif

      Serial.print("  VL53L1X: ");

      if (!LRSVL53L1X.begin() == 0) { //Begin returns 0 on a good init
        Serial.println("Sensor failure!");
      }
      LRSVL53L1X.setDistanceModeShort();
      //distanceSensor.setDistanceModeLong();
      LRSVL53L1X.startRanging(); //Write configuration block of 135 bytes to setup a measurement
      int distance = LRSVL53L1X.getDistance(); //Get the result of the measurement from the sensor
      LRSVL53L1X.stopRanging();
      Serial.print("range: ");
      Serial.print(distance);
      Serial.println("mm");

      Wire.end();

      modnr = pnr;
#if (ModularNode == 1)
      if (modnr == 0 || modnr == 7) {
        modnr += 200;
      }
#endif
      appData[appDataSize++] = modnr;
      appData[appDataSize++] = 11;

      unsigned char *puc;
      puc = (unsigned char *)(&distance);
      appData[appDataSize++] = puc[0];
      appData[appDataSize++] = puc[1];
      appData[appDataSize++] = puc[2];
      appData[appDataSize++] = puc[3];
    }

    /*
        HMC5883L
    */

    if (HMC_5883L_e[pnr]) //. 12
    {
      Wire.begin();

#if (ModularNode == 1 || TCA9548 == 1)
      tcaselect(pnr);
      delay(100);
#endif

      Serial.print("  HMC5883L: ");\

      int error = 0;

      error = hmc5883.setScale(1.3); // Set the scale of the compass.
      error = hmc5883.setMeasurementMode(MEASUREMENT_CONTINUOUS); // Set the measurement mode to Continuous
      MagnetometerRaw raw = hmc5883.readRawAxis();

      Serial.print("X: ");
      Serial.print(raw.XAxis);
      Serial.print(", Y: ");   
      Serial.print(raw.YAxis);
      Serial.print(", Z: ");   
      Serial.println(raw.ZAxis);

      Wire.end();

      modnr = pnr;
#if (ModularNode == 1)
      if (modnr == 0 || modnr == 7) {
        modnr += 200;
      }
#endif
      appData[appDataSize++] = modnr;
      appData[appDataSize++] = 12;

      unsigned char *puc;
      puc = (unsigned char *)(&raw.XAxis);
      appData[appDataSize++] = puc[0];
      appData[appDataSize++] = puc[1];
      appData[appDataSize++] = puc[2];
      appData[appDataSize++] = puc[3];
      puc = (unsigned char *)(&raw.YAxis);
      appData[appDataSize++] = puc[0];
      appData[appDataSize++] = puc[1];
      appData[appDataSize++] = puc[2];
      appData[appDataSize++] = puc[3];
      puc = (unsigned char *)(&raw.ZAxis);
      appData[appDataSize++] = puc[0];
      appData[appDataSize++] = puc[1];
      appData[appDataSize++] = puc[2];
      appData[appDataSize++] = puc[3];
    }

/* MLX90614

    if (MLX_90614_e[pnr]) // 13
    {
      Wire.begin();

#if (ModularNode == 1 || TCA9548 == 1)
      tcaselect(pnr);
      delay(100);
#endif

      Serial.print("  MLX90614: ");

      if (mlx.begin() == 0) { //Begin returns 0 on a good init
        Serial.println("Sensor failure!");
      } else {
        //double ambienttemp = mlx.readAmbientTempC();
        //double objecttemp = mlx.readObjectTempC();
        double ambienttemp = mlx.readTemp(MLX90614::MLX90614_SRCA, MLX90614::MLX90614_TK);
        double objecttemp = mlx.readTemp(MLX90614::MLX90614_SRC01, MLX90614::MLX90614_TK);

        Serial.print("Ambienttemp: ");
        Serial.print(ambienttemp);
        Serial.print("C, ObjectTemp: ");
        Serial.print(objecttemp);
        Serial.println("C");

        Wire.end();

        modnr = pnr;
#if (ModularNode == 1)
        if (modnr == 0 || modnr == 7) {
          modnr += 200;
        }
#endif
        appData[appDataSize++] = modnr;
        appData[appDataSize++] = 13;

        unsigned char *puc;
        puc = (unsigned char *)(&ambienttemp);
        appData[appDataSize++] = puc[0];
        appData[appDataSize++] = puc[1];
        appData[appDataSize++] = puc[2];
        appData[appDataSize++] = puc[3];

        puc = (unsigned char *)(&objecttemp);
        appData[appDataSize++] = puc[0];
        appData[appDataSize++] = puc[1];
        appData[appDataSize++] = puc[2];
        appData[appDataSize++] = puc[3];
      }
    }
*/

    /*
        SHT31
    */

    if (SHT_31_e[pnr]) // 13
    {
      Wire.begin();

#if (ModularNode == 1 || TCA9548 == 1)
      tcaselect(pnr);
      delay(100);
#endif

      sht31.begin(0x44);
      delay(250);
      Temperature = sht31.readTemperature();
      Humidity = sht31.readHumidity();
        
      Wire.end();

      modnr = pnr;
#if (ModularNode == 1)
      if (modnr == 0 || modnr == 7) {
        modnr += 200;
      }
#endif
      appData[appDataSize++] = modnr;
      appData[appDataSize++] = 14;

      appData[appDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0) >> 8);
      appData[appDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0));

      appData[appDataSize++] = (uint8_t)((int)(Humidity * 10.0) >> 8);
      appData[appDataSize++] = (uint8_t)((int)(Humidity * 10.0));
      
      Serial.print("  SHT31: T=");
      Serial.print(Temperature);
      Serial.print("C, Humidity=");
      Serial.print(Humidity);
      Serial.println("%");
  }

#if (ModularNode == 1 || TCA9548 == 1)
  }
#endif

  /*
    One_Wire
  */

  if (One_Wire_e)
  {
    Serial.println("Scan for OneWire Sensors ...");
    sensors.begin();

    if (sensors.getDeviceCount() > 0) {
      sensors.requestTemperatures();

      for (int idxx=0; idxx < sensors.getDeviceCount(); idxx++) {
        Temperature = sensors.getTempCByIndex(idxx);

        appData[appDataSize++] = 100;
        appData[appDataSize++] = 100 + idxx;
        appData[appDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0) >> 8);
        appData[appDataSize++] = (uint8_t)((int)((Temperature + 100.0) * 10.0));

        Serial.print("  OW");
        if (idxx < 10) {
          Serial.print("00");
        } else if (idxx < 100) {
          Serial.print("0");
        }
        Serial.print(idxx);
        Serial.print(": T=");
        Serial.print(Temperature);
        Serial.println("C");
      }
    }
    else
    {
      Serial.println("  No OneWire Sensors found.");
    }
    ds.reset_search();
  }

  /*
    GPS Serial
  */
 
#if (GPS_SER == 1)
  float lat, lon, alt, course, speed, hdop, sats;

  Serial.println("Probing for GPS ...");
 
  bool gpsactive = true;

  while (!gps.location.isValid() && gpsactive) 
  {
    unsigned long start = millis();
    do 
    {
      if (ss.available()) 
      {
        gps.encode(ss.read());
      }
    } while ((millis() - start) < 200); 
    //} while (gps.charsProcessed() < 100)

    if ((millis()) > 5000 && gps.charsProcessed() < 10)
    //if (millis() > 2500 && gps.charsProcessed() < 10) 
    {
      Serial.println("No GPS connected");
      gpsactive = false;
    }
  }

  if (gpsactive)
  {
    lat = gps.location.lat();
    lon = gps.location.lng();
    alt = gps.altitude.meters();
    course = gps.course.deg();
    speed = gps.speed.kmph();
    sats = gps.satellites.value();
    hdop = gps.hdop.hdop();

    appData[appDataSize++] = 200;
    unsigned char *puc;
    puc = (unsigned char *)(&lat);
    appData[appDataSize++] = puc[0];
    appData[appDataSize++] = puc[1];
    appData[appDataSize++] = puc[2];
    appData[appDataSize++] = puc[3];
    puc = (unsigned char *)(&lon);
    appData[appDataSize++] = puc[0];
    appData[appDataSize++] = puc[1];
    appData[appDataSize++] = puc[2];
    appData[appDataSize++] = puc[3];
    puc = (unsigned char *)(&alt);
    appData[appDataSize++] = puc[0];
    appData[appDataSize++] = puc[1];
    appData[appDataSize++] = puc[2];
    appData[appDataSize++] = puc[3];
    puc = (unsigned char *)(&course);
    appData[appDataSize++] = puc[0];
    appData[appDataSize++] = puc[1];
    appData[appDataSize++] = puc[2];
    appData[appDataSize++] = puc[3];
    puc = (unsigned char *)(&speed);
    appData[appDataSize++] = puc[0];
    appData[appDataSize++] = puc[1];
    appData[appDataSize++] = puc[2];
    appData[appDataSize++] = puc[3];
    puc = (unsigned char *)(&hdop);
    appData[appDataSize++] = puc[0];
    appData[appDataSize++] = puc[1];
    appData[appDataSize++] = puc[2];
    appData[appDataSize++] = puc[3];

    Serial.print("SATS: ");
    Serial.print(gps.satellites.value());
    Serial.print(", HDOP: ");
    Serial.print(gps.hdop.hdop());
    Serial.print(", LAT: ");
    Serial.print(gps.location.lat());
    Serial.print(", LON: ");
    Serial.print(gps.location.lng());
    Serial.print(", AGE: ");
    Serial.print(gps.location.age());
    Serial.print(", ALT: ");
    Serial.print(gps.altitude.meters());
    Serial.print(", COURSE: ");
    Serial.print(gps.course.deg());
    Serial.print(", SPEED: ");
    Serial.println(gps.speed.kmph());
  }
#endif

  digitalWrite(Vext, HIGH);

  uint16_t BatteryVoltage = getBatteryVoltage();
  appData[appDataSize++] = (uint8_t)(BatteryVoltage >> 8);
  appData[appDataSize++] = (uint8_t)BatteryVoltage;
  Serial.print("BatteryVoltage: ");
  Serial.println(BatteryVoltage);

  Serial.print("Trigger: ");
  if (accelWoke) 
  {
    Serial.println("Interrupt");
    appData[appDataSize++] = 0;
  } else {
    Serial.println("Timer");
    appData[appDataSize++] = 1;
  }
}