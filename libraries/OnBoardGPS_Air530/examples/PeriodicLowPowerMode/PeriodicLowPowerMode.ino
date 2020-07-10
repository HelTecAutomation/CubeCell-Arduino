#include "Arduino.h"
#include "GPS_Air530.h"
#include "cubecell_SSD1306Wire.h"

 SSD1306Wire  display(0x3c, 500000, I2C_NUM_0,GEOMETRY_128_64,GPIO10 ); // addr , freq , i2c group , ratio , rst

typedef enum
{
  STATUS_LPM = 0,
  STATUS_UPDATE_GPS
}dev_status;

//fisrt fix timeout
#define GPS_INIT_TIMEOUT 60000

//sleep time until next GPS update 
#define GPS_SLEEPTIME 60000

//when gps waked, if in GPS_UPDATE_TIMEOUT, gps not fixed then into low power mode
#define GPS_UPDATE_TIMEOUT 10000

//once fixed, GPS_CONTINUE_TIME later into low power mode
#define GPS_CONTINUE_TIME 5000

static TimerEvent_t autoGPS;
dev_status  mcu_status;
uint32_t starttime;

int32_t fracPart(double val, int n)
{
  return (int32_t)((val - (int32_t)(val))*pow(10,n));
}

void VextON(void)
{
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, LOW);
}

void VextOFF(void) //Vext default OFF
{
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, HIGH);
}
void displayGPSInof()
{
  char str[30];
  display.clear();
  display.setFont(ArialMT_Plain_10);
  int index = sprintf(str,"%02d-%02d-%02d",Air530.date.year(),Air530.date.day(),Air530.date.month());
  str[index] = 0;
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, str);
  
  index = sprintf(str,"%02d:%02d:%02d",Air530.time.hour(),Air530.time.minute(),Air530.time.second(),Air530.time.centisecond());
  str[index] = 0;
  display.drawString(60, 0, str);

  if( Air530.location.age() < 1000 )
  {
    display.drawString(120, 0, "A");
  }
  else
  {
    display.drawString(120, 0, "V");
  }
  
  index = sprintf(str,"alt: %d.%d",(int)Air530.altitude.meters(),fracPart(Air530.altitude.meters(),2));
  str[index] = 0;
  display.drawString(0, 16, str);
   
  index = sprintf(str,"hdop: %d.%d",(int)Air530.hdop.hdop(),fracPart(Air530.hdop.hdop(),2));
  str[index] = 0;
  display.drawString(0, 32, str); 
 
  index = sprintf(str,"lat :  %d.%d",(int)Air530.location.lat(),fracPart(Air530.location.lat(),4));
  str[index] = 0;
  display.drawString(60, 16, str);   
  
  index = sprintf(str,"lon:%d.%d",(int)Air530.location.lng(),fracPart(Air530.location.lng(),4));
  str[index] = 0;
  display.drawString(60, 32, str);

  index = sprintf(str,"speed: %d.%d km/h",(int)Air530.speed.kmph(),fracPart(Air530.speed.kmph(),3));
  str[index] = 0;
  display.drawString(0, 48, str);
  display.display();
}

void printGPSInof()
{
  Serial.print("Date/Time: ");
  if (Air530.date.isValid())
  {
    Serial.printf("%d/%02d/%02d",Air530.date.year(),Air530.date.day(),Air530.date.month());
  }
  else
  {
    Serial.print("INVALID");
  }

  if (Air530.time.isValid())
  {
    Serial.printf(" %02d:%02d:%02d.%02d",Air530.time.hour(),Air530.time.minute(),Air530.time.second(),Air530.time.centisecond());
  }
  else
  {
    Serial.print(" INVALID");
  }
  Serial.println();
  
  Serial.print("LAT: ");
  Serial.print(Air530.location.lat(),6);
  Serial.print(", LON: ");
  Serial.print(Air530.location.lng(),6);
  Serial.print(", ALT: ");
  Serial.print(Air530.altitude.meters());

  Serial.println(); 
  
  Serial.print("SATS: ");
  Serial.print(Air530.satellites.value());
  Serial.print(", HDOP: ");
  Serial.print(Air530.hdop.hdop());
  Serial.print(", AGE: ");
  Serial.print(Air530.location.age());
  Serial.print(", COURSE: ");
  Serial.print(Air530.course.deg());
  Serial.print(", SPEED: ");
  Serial.println(Air530.speed.kmph());
  Serial.println();
}

void onAutoGPS()
{
  TimerStop(&autoGPS);
  mcu_status = STATUS_UPDATE_GPS;
}

void gpsUpdate(uint32_t timeout, uint32_t continuetime)
{
  VextON();// oled power on;
  delay(10); 
  display.init();
  display.clear();
  display.display();
      
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 32-16/2, "GPS Searching...");
  Serial.println("GPS Searching...");
  display.display();
      
  Air530.begin();
  starttime = millis();
  while( (millis()-starttime) < timeout )
  {
    while (Air530.available() > 0)
    {
      Air530.encode(Air530.read());
    }

   // gps fixed in a second
    if( Air530.location.age() < 1000 )
    {
      break;
    }
  }

  //if gps fixed update gps and print gps info every 1 second
  if(Air530.location.age() < 1000)
  {
    starttime = millis();
    uint32_t printinfo = 0;
    while( (millis()-starttime) < continuetime )
    {
      while (Air530.available() > 0)
      {
        Air530.encode(Air530.read());
      }

      if( (millis()-starttime) > printinfo )
      {
        printinfo += 1000;
        printGPSInof();
        displayGPSInof();
      }
    }
  }
  else //if gps no fixed waited for 2s in to lowpowermode
  {
    display.clear(); 
    display.drawString(64, 32-16/2, "GPS TIMEOUT");
    display.display();
    Serial.println("GPS search timeout.");
    delay(2000);  
  }
  Air530.end();  
  display.clear(); 
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 32-16/2, "into lowpower mode...");
  display.display();
  Serial.println("2 seconds later into lowpower mode....");
  delay(2000);
  display.clear();
  display.display();
  display.stop();
  VextOFF(); //oled power off
 
  TimerSetValue( &autoGPS, GPS_SLEEPTIME );
  TimerStart( &autoGPS );   
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  boardInitMcu();

  TimerInit( &autoGPS, onAutoGPS );

  gpsUpdate(GPS_INIT_TIMEOUT,GPS_CONTINUE_TIME);
  mcu_status = STATUS_LPM;
}

void loop() {
  switch(mcu_status)
  {  
    case STATUS_LPM:
      lowPowerHandler();
      break;
    case STATUS_UPDATE_GPS:
      gpsUpdate(GPS_UPDATE_TIMEOUT,GPS_CONTINUE_TIME);
      mcu_status = STATUS_LPM;
      break;
    default:
      mcu_status = STATUS_LPM;
      break;
  }
}
