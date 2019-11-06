#include "Arduino.h"


#define timetosleep 5000
#define timetowake 5000
static TimerEvent_t sleep;
static TimerEvent_t wakeup;
uint8_t lowpower=1;

void OnSleep()
{
  Serial.printf("into lowpower mode, %d ms later wake up.\r\n",timetowake);
  lowpower=1;
  //timetosleep ms later wake up;
  TimerSetValue( &wakeup, timetowake );
  TimerStart( &wakeup );
}
void OnWakeup()
{
  Serial.printf("wake up, %d ms later into lowpower mode.\r\n",timetosleep);
  lowpower=0;
  //timetosleep ms later into lowpower mode;
  TimerSetValue( &sleep, timetosleep );
  TimerStart( &sleep );
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  BoardInitMcu();
  Radio.Sleep( );
  TimerInit( &sleep, OnSleep );
  TimerInit( &wakeup, OnWakeup );
  OnSleep();
}

void loop() {
  if(lowpower){
    //note that LowPower_Handler() run six times the mcu into lowpower mode;
    LowPower_Handler();
  }
  // put your main code here, to run repeatedly:
}
