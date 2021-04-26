#include "Arduino.h"
#include "LoRa_APP.h"

#define INT_GPIO USER_KEY

/* Note for ASR6601
 * gpio 0 - 55 can be used for wake up.
 * every 4 gpio is a group, in one group, only one gpio can be used for wake up in the same time
 * group 0:  0,1,2,3 
 * group 1:  4,5,12,13 
 * group 2:  8,9,10,11 
 * group 3:  6,7,14,15 
 * group 4:  16,17,18,19 
 * group 5:  20,21,22,23 
 * group 6:  24,25,26,27 
 * group 7:  28,29,30,31 
 * group 8:  32,33,34,35 
 * group 9:  36,37,38,39
 * group 10: 40,41,42,43 
 * group 11: 44,45,46,47 
 * group 12: 48,49,50,51
 * group 13: 52,13,54,55   
 */

#define timetillsleep 5000
static TimerEvent_t sleep;
uint8_t lowpower=1;

void onSleep()
{
  Serial.printf("Going into lowpower mode. Press user key to wake up\r\n");
  delay(5);
  lowpower=1;
}
void onWakeUp()
{
  delay(10);
  if(digitalRead(INT_GPIO) == 0)
  {
	  Serial.printf("Woke up by GPIO, %d ms later into lowpower mode.\r\n",timetillsleep);
	  lowpower=0;
	  //timetillsleep ms later into lowpower mode;
	  TimerSetValue( &sleep, timetillsleep );
	  TimerStart( &sleep );
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(INT_GPIO,INPUT);
  attachInterrupt(INT_GPIO,onWakeUp,FALLING);
  TimerInit( &sleep, onSleep );
  Serial.printf("Going into lowpower mode. Press user key to wake up\r\n");
  delay(5);
}

void loop() {
  if(lowpower){
    lowPowerHandler();
  }
  // put your main code here, to run repeatedly:
}