// The AT24C32N is an EEPROM, usually added to the DS1307 breakout board
//  - Endurance: 1 Million Write Cycles
//  - Data Retention: 100 Years

#include <Wire.h>
#include <AT24C32N.h>

#define AT24C32N_ADDRESS  0x50
#define MaxMemSize        4096 // 32,768 bits


 #include <Arduino.h> // capital A so it is error prone on case-sensitive filesystems
 // Macro to deal with the difference in I2C write functions from old and new Arduino versions.
 #define _I2C_WRITE write
 #define _I2C_READ  read

//--------------------------------------------------------------------------------
void EEPROM_AT24C32N::begin(int sda, int scl)
{
Wire.begin(sda,scl);
// there is also write protection and a cycle memory reset sequence that can be called here
}
//--------------------------------------------------------------------------------
void EEPROM_AT24C32N::SetMemLoc(uint16_t address)
{
byte HByte,LByte;

HByte = address >> 8;           Wire._I2C_WRITE(HByte);
LByte = address - (HByte << 8); Wire._I2C_WRITE(LByte);
delay(10);
}
//--------------------------------------------------------------------------------
bool EEPROM_AT24C32N::WriteBytes(uint16_t StartAddress,uint16_t SizeOfVar,void* ValToWrite)
{
uint8_t lp,xval;

if ((SizeOfVar+StartAddress)>MaxMemSize) {Serial.println("Error - Fatal : Size problem");return(false);}
//--
for (lp = 0; lp<SizeOfVar; lp++)
  {
  xval=((char*)ValToWrite)[lp];
  if ((((StartAddress+lp) % 32)==0) || (((StartAddress+lp) % 32)==30) || (lp==0)) // internally organized as 256 pages of 32 bytes each
    {
    if (lp>0) {Wire.endTransmission();delay(10);}
    Wire.beginTransmission(AT24C32N_ADDRESS);delay(10);
    SetMemLoc(StartAddress+lp);
    }
  Wire._I2C_WRITE(xval);
  delay(10);
  }
Wire.endTransmission();
delay(10);
return(true);
}
//--------------------------------------------------------------------------------
bool EEPROM_AT24C32N::WriteBytes(uint16_t StartAddress,char* ValToWrite)
{
uint16_t vlen;

vlen = strlen(ValToWrite)+1; // +1 to allow for terminating zero
return(WriteBytes(StartAddress,vlen,(void*)ValToWrite));
}
//--------------------------------------------------------------------------------
bool EEPROM_AT24C32N::RetrieveBytes(char* buf, uint16_t StorageVarSize, uint16_t StartAddress, bool ZeroTerminated)
{
uint16_t addrByte,lp;
char ReadByte;

addrByte = StartAddress+StorageVarSize;
if (addrByte>MaxMemSize) {Serial.println("Error - Fatal : Size problem");return(false);}
for (lp = 0; lp < StorageVarSize; ++lp)
  {
  if ((((StartAddress+lp) % 32)==0) || (lp==0)) // internally organized as 256 pages of 32 bytes each
    {
    Wire.beginTransmission(AT24C32N_ADDRESS);delay(10);
    SetMemLoc(StartAddress+lp);
    Wire.endTransmission();delay(10);
    Wire.requestFrom(AT24C32N_ADDRESS, (StorageVarSize-lp));delay(10);
    }
  ReadByte = Wire._I2C_READ();
  buf[lp] = ReadByte;
  if (ZeroTerminated && (!ReadByte)) break; // if zero terminated and a zero byte was just read then get out.
  delay(10);
  }
return(true);
}
//-----------------------------------------------------------------------------