#include "Arduino.h"
#include "innerWdt.h"

bool autoFeed = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  Serial.println("Start");

  /* Enable the WDT. 
  * The wdt about every 1.4 seconds generates an interruption, 
  * Two unserviced interrupts lead to a system reset(i.e. at the third match). 
  * The max feed time shoud be 2.8 seconds.
  * autoFeed = false: do not auto feed wdt.
  * autoFeed = true : it auto feed the wdt in every interrupt.
  */
  innerWdtEnable(autoFeed);
}

int feedCnt = 0;

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("running");
  delay(2800);
  
  if(autoFeed == false)
  {
    //feed the wdt
    if(feedCnt < 10)
    {
      feedInnerWdt();
      feedCnt++;
    }
    else
    {
      Serial.println("stop feed wdt");
    }
  }
}