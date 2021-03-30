extern "C" {
#include <stdlib.h>
#include <ASR_Arduino.h>
}

void randomSeed(unsigned long seed)
{
    if(seed != 0) {
        srand(seed);
    }
}

long random(long howbig)
{
    uint32_t x = (uint32_t)cubecell_random(0xFFFF)|(uint32_t)(cubecell_random(0xFFFF)<<16);
    uint64_t m = uint64_t(x) * uint64_t(howbig);
    uint32_t l = uint32_t(m);
    if (l < howbig) {
        uint32_t t = -howbig;
        if (t >= howbig) {
            t -= howbig;
            if (t >= howbig) 
                t %= howbig;
        }
        while (l < t) {
            x = (uint32_t)cubecell_random(0xFFFF)|(uint32_t)(cubecell_random(0xFFFF)<<16);
            m = uint64_t(x) * uint64_t(howbig);
            l = uint32_t(m);
        }
    }
    return m >> 32;
}

long random(long howsmall, long howbig)
{
    if(howsmall >= howbig) {
        return howsmall;
    }
    long diff = howbig - howsmall;
    return random(diff) + howsmall;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    long divisor = (in_max - in_min);
    if(divisor == 0){
        return -1; //AVR returns -1, SAM returns 0
    }
    return (x - in_min) * (out_max - out_min) / divisor + out_min;
}

unsigned int makeWord(unsigned int w)
{
    return w;
}

unsigned int makeWord(unsigned char h, unsigned char l)
{
    return (h << 8) | l;
}


uint32_t pulseIn(uint8_t pin_name,uint8_t level,uint32_t timeout)
{
  if(level)
  {
    pinMode(pin_name,OUTPUT);
    digitalWrite(pin_name,LOW);
  }
  else
  {
    pinMode(pin_name,OUTPUT);
    digitalWrite(pin_name,HIGH);
  }

  uint32_t time = micros();
 
  while(digitalRead(pin_name)!=level)
  {
    if(micros()-time>timeout)
    {
      return 0;
    }
  }
  uint32_t timestart = micros();
  while(digitalRead(pin_name)==level){}
  uint32_t timestop = micros();
  if(timestop < timestart)
  {
    return  0xFFFFFFFF - timestart + timestop;
  }
  else
  {
    return timestop - timestart;
  }
}