#ifndef AT24C32_h
#define AT24C32_h

// The AT24C32 is an EEPROM, usually added to the DS1307 breakout board
//  - Endurance: 1 Million Write Cycles
// ** note that the DS1307 already has 56 bytes NVRAM, so not sure what to write here in 4kb!

class EEPROM_AT24C32N
  {
  public:
    void begin(int sda, int scl);
    bool WriteBytes(uint16_t StartAddress,uint16_t SizeOfVar,void* ValToWrite);
    bool WriteBytes(uint16_t StartAddress,char* ValToWrite);
    bool RetrieveBytes(char* buf, uint16_t StorageVarSize, uint16_t StartAddress, bool ZeroTerminated);
  private:
    void SetMemLoc(uint16_t address);
  };
#endif