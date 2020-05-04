#include "Arduino.h"


#define timetosleep 5000
#define timetowake 5000
static TimerEvent_t sleep;
static TimerEvent_t wakeUp;
uint8_t lowpower=1;

void onSleep()
{
  Serial.printf("into lowpower mode, %d ms later wake up.\r\n",timetowake);
  lowpower=1;
  //timetosleep ms later wake up;
  TimerSetValue( &wakeUp, timetowake );
  TimerStart( &wakeUp );
}
void onWakeUp()
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
  boardInitMcu();
  Radio.Sleep( );
  TimerInit( &sleep, onSleep );
  TimerInit( &wakeUp, onWakeUp );
  onSleep();
}

void loop() {
  if(lowpower){
    //note that lowPowerHandler() run six times the mcu into lowpower mode;
    lowPowerHandler();
  }
  // put your main code here, to run repeatedly:
}
