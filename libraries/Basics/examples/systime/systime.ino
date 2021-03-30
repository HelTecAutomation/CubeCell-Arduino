TimerSysTime_t sysTimeCurrent;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  /*typedef struct TimerSysTime_s
  *{
  *  uint32_t Seconds;
  *  int16_t SubSeconds;
  *}TimerSysTime_t;
  */
  sysTimeCurrent = TimerGetSysTime( );
  Serial.printf("sys time:%u.%d\r\n",(unsigned int)sysTimeCurrent.Seconds, sysTimeCurrent.SubSeconds);
  TimerSysTime_t newSysTime ;
  newSysTime.Seconds = 1000;
  newSysTime.SubSeconds = 50;
  TimerSetSysTime( newSysTime );
  sysTimeCurrent = TimerGetSysTime( );
  Serial.printf("sys time:%u.%d\r\n",(unsigned int)sysTimeCurrent.Seconds, sysTimeCurrent.SubSeconds);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  sysTimeCurrent = TimerGetSysTime( );
  Serial.printf("sys time:%u.%d\r\n",(unsigned int)sysTimeCurrent.Seconds, sysTimeCurrent.SubSeconds);
}
