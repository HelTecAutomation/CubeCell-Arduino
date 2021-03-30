String wasnflash  = "Board"; //Board, Capsule, TCA9548A, CapsuleGPS (IndoorNode, ModularNode)
String wasnver    = "3.0.7";

#define ModularNode 0
#define One_Wire    0    // OneWire on GPIO1
#define GPS_SER     0    // serial GPS on GPIO3 TX, GPIO5 RX
#define TriggerInt  1    // Interrupt on GPIO2, high for trigger
#define TCA9548     0    // enable TCA9548 IIC 8port switch

#define AUTO_SCAN   1

#define BME_680     0
#define BME_280     0
#define CCS_811     0
#define BMP_280     0
#define BMP_180     0
#define HDC_1080    0
#define BH_1750     0
#define SHT_2X      0
#define ADS_1015    0
#define MPU_9250    0
#define LR_VL53L1X  0
#define HMC_5883L   0
//#define MLX_90614 0
#define SHT_31      0

#define minpnr      0    // TCA9548A first I2C Port
#define maxpnr      7    // TCA9548A last I2C Port

/* OTAA para*/
uint8_t devEui[]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appEui[]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appKey[]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/* ABP para*/
uint8_t nwkSKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appSKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint32_t devAddr =  ( uint32_t )0x00000000;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 90000;