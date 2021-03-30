bool checkUserAt(char *cmd, char *content)
{
  if (strcmp(cmd, "VER") == 0) 
  {
    if (content[0] == '?')
    {
      Serial.print("+VER=");
      Serial.println(wasnver);
      return true;
    }
  }
  if (strcmp(cmd, "FLASH") == 0) 
  {
    if (content[0] == '?')
    {
      Serial.print("+FLASH=");
      Serial.println(wasnflash);
      return true;
    }
  }
  if (strcmp(cmd, "BATTERY") == 0)
  {
    if (content[0] == '?')
    {
      uint16_t BatteryVoltage = getBatteryVoltage();
      Serial.print("+BATTERY=");
      Serial.print(BatteryVoltage);
      Serial.println();
      return true;
    }
  }
  if (strcmp(cmd, "LED") == 0)
  {
    if (content[0] == '?')
    {
      Serial.print("+LED=");
      Serial.print(LoraWan_RGB);
      Serial.println();
      return true;
    }
  }
  
  return false;
}