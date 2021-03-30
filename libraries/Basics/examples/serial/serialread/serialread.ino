//Becuse of the Rx buffer size is only 8, you should read as soon to avoid Rx buffer overflow.

#define TIMEOUT 10//time in ms

uint8_t serialBuffer[256];
int size;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  size = Serial.read(serialBuffer,TIMEOUT);
  if(size)
  {
    Serial.printf("rev data size %d : ",size);
    Serial.write(serialBuffer,size);
  }
}