#include "Arduino.h"
#ifdef __asr650x__
#include "innerWdt.h"
#endif


/*
 * For asr650x, the max feed time is 2.8 seconds.
 * For asr6601, the max feed time is 24 seconds.
 */
#ifdef __asr650x__
#define MAX_FEEDTIME 2800// ms
#else
#define MAX_FEEDTIME 24000// ms
#endif

bool autoFeed = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  Serial.println("Start");

  /* Enable the WDT. 
  * autoFeed = false: do not auto feed wdt.
  * autoFeed = true : it auto feed the wdt in every watchdog interrupt.
  */
  innerWdtEnable(autoFeed);
}

int feedCnt = 0;

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("running");
  delay(MAX_FEEDTIME - 100);
  
  if(autoFeed == false)
  {
    //feed the wdt
    if(feedCnt < 3)
    {
      Serial.println("feed wdt");
      feedInnerWdt();
      feedCnt++;
    }
    else
    {
      Serial.println("stop feed wdt");
    }
  }
}