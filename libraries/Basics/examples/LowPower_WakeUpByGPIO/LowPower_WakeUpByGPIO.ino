#include "Arduino.h"


#define timetosleep 5000
static TimerEvent_t sleep;
uint8_t lowpower=1;

void OnSleep()
{
  Serial.println("into lowpower mode. press user key to wake up");
  lowpower=1;
}
void wakeup()
{
  delay(10);
  if(digitalRead(GPIO7)==0)
  {
    Serial.printf("wake up, %d ms later into lowpower mode.\r\n",timetosleep);
    lowpower=0;
    //timetosleep ms later into lowpower mode;
    TimerSetValue( &sleep, timetosleep );
    TimerStart( &sleep );
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  BoardInitMcu();
  Radio.Sleep( );
  pinMode(GPIO7,INPUT);
  attachInterrupt(GPIO7,wakeup,FALLING);
  TimerInit( &sleep, OnSleep );
  Serial.println("into lowpower mode. press user key to wake up.");
}

void loop() {
  if(lowpower){
    //note that LowPower_Handler() run six times the mcu into lowpower mode;
    LowPower_Handler();
  }
  // put your main code here, to run repeatedly:
}
