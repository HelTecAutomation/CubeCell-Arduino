void accelWakeup()
{
  delay(10);
  if(digitalRead(INT_PIN)==HIGH)
  {
    accelWoke = true;
  }
}



float CalculateIAQ()
{
  float hum_weighting = 0.25; // so hum effect is 25% of the total air quality score
  float gas_weighting = 0.75; // so gas effect is 75% of the total air quality score

  float hum_score, gas_score;
  float gas_reference = co2;
  float hum_reference = 40;
  int getgasreference_count = 0;

  //Calculate humidity contribution to IAQ index
  if (Humidity >= 38 && Humidity <= 42)
    hum_score = 0.25 * 100; // Humidity +/-5% around optimum
  else
  { //sub-optimal
    if (Humidity < 38)
      hum_score = 0.25 / hum_reference * Humidity * 100;
    else
    {
      hum_score = ((-0.25 / (100 - hum_reference) * Humidity) + 0.416666) * 100;
    }
  }

  //Calculate gas contribution to IAQ index
  float gas_lower_limit = 5000;  // Bad air quality limit
  float gas_upper_limit = 50000; // Good air quality limit
  if (gas_reference > gas_upper_limit)
    gas_reference = gas_upper_limit;
  if (gas_reference < gas_lower_limit)
    gas_reference = gas_lower_limit;
  gas_score = (0.75 / (gas_upper_limit - gas_lower_limit) * gas_reference - (gas_lower_limit * (0.75 / (gas_upper_limit - gas_lower_limit)))) * 100;

  //Combine results for the final IAQ index value (0-100% where 100% is good quality air)
  float air_quality_score = hum_score + gas_score;

  return air_quality_score;
}



void tcaselect(uint8_t i)
{
  if (i > 7)
    return;

  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}



void setSubBand2() 
{
  Serial.println("setSubBand2");
  // Enabling only 2nd block of 8 channels (8-15) + channel 65
  MibRequestConfirm_t mibReq;

  uint16_t channelMask[] = { 0xFF00, 0x0000, 0x0000, 0x0000, 0x0002, 0x0000};
  mibReq.Type = MIB_CHANNELS_MASK;
  mibReq.Param.ChannelsMask = channelMask;
  LoRaMacMibSetRequestConfirm( &mibReq );
}

bool checkcopyprotect()
{
  return true;
}