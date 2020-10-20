
//Becuse of the Rx buffer size is only 8, so if use serial.read(). you should read as soon to avoid Rx buffer overflow.

//in TIMEOUT the serial has no data again, we think the data is over. it should longer than one symbol time.
#define TIMEOUT 1000

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.flush();
}

uint8_t serialBuffer[256] = {0} ;
int serialBuffer_index=0;

void loop() {
  // put your main code here, to run repeatedly:
    if(Serial.available())
    {
      serialBuffer_index=0;
      while(Serial.available())
      {
        serialBuffer[serialBuffer_index++]=Serial.read();
        int cnt = 0;
        //in TIMEOUT the serial has no data again, we think the data is over. it should longer than one symbol time.
        while(Serial.available()==0)
        {
          cnt++;
          delayMicroseconds(1);//delay 1 us
          if(cnt>=TIMEOUT)
            break;
        }
      }
      Serial.write(serialBuffer,serialBuffer_index);
    }
}
