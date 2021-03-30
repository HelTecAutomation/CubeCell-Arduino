/*this code is the second UART for cubecell AB02, AB02S, AB02A*/
#define TIMEOUT 10//time in ms

uint8_t serialBuffer[256];
int size;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  size = Serial1.read(serialBuffer,TIMEOUT);
  if(size)
  {
    Serial1.printf("rev data size %d : ",size);
    Serial1.write(serialBuffer,size);
  }
}
