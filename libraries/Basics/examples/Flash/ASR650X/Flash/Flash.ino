#include "Arduino.h"

/*note that flash row 0-33 is bootloader, must not be edited;
* the size of flash row is 256; 
* flash row 34-509 can be edited;
* flash row 510-511 used for AT mode, must not be edited;
* if you want to use flash to save data, use the flash from back to front, or program areas may be edited
*/

#define ROW 500
#define ROW_OFFSET 100
//CY_FLASH_SIZEOF_ROW is 256
#define addr CY_FLASH_BASE+CY_FLASH_SIZEOF_ROW*ROW + ROW_OFFSET

uint8_t data1[512];
uint8_t data2[512];
uint8_t data3;
uint8_t data4;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  for(int i=0;i<512;i++){
    data1[i]=(uint8_t)i;
  }

  //write data1 to flash at addr
  FLASH_update(addr,data1,sizeof(data1));

  //read flash at addr to data2
  FLASH_read_at(addr,data2,sizeof(data2));

  uint16_t error=0;
  for(int i=0;i<512;i++){
    if(data1[i]!=data2[i])
    {
      Serial.printf("error:data1[%d] %d , data2[%d] %d\r\n",i,data1[i],i,data2[i]);
      error++;
    }
  }
  
  Serial.printf("error:%d\r\n",error);

  //read a byte at addr to data4
  FLASH_read_at(addr,&data4,1);
  Serial.printf("data4:%d\r\n",data4);
  data3=100;
  //write a byte at addr
  FLASH_update(addr,&data3,1);
  //read a byte at addr to data4
  FLASH_read_at(addr,&data4,1);  
  Serial.printf("data4:%d\r\n",data4);
}

void loop() {
  // put your main code here, to run repeatedly:

}
