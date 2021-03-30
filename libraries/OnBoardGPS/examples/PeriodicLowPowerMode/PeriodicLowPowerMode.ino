#include "Arduino.h"
#include "GPS_Air530.h"
#include "GPS_Air530Z.h"
#include "HT_SSD1306Wire.h"

SSD1306Wire  display(0x3c, 500000, I2C_NUM_0,GEOMETRY_128_64,GPIO10 ); // addr , freq , i2c group , ratio , rst

//if GPS module is Air530, use this
//Air530Class GPS;

//if GPS module is Air530Z, use this
Air530ZClass GPS;

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
  int index = sprintf(str,"%02d-%02d-%02d",GPS.date.year(),GPS.date.day(),GPS.date.month());
  str[index] = 0;
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, str);
  
  index = sprintf(str,"%02d:%02d:%02d",GPS.time.hour(),GPS.time.minute(),GPS.time.second(),GPS.time.centisecond());
  str[index] = 0;
  display.drawString(60, 0, str);

  if( GPS.location.age() < 1000 )
  {
    display.drawString(120, 0, "A");
  }
  else
  {
    display.drawString(120, 0, "V");
  }
  
  index = sprintf(str,"alt: %d.%d",(int)GPS.altitude.meters(),fracPart(GPS.altitude.meters(),2));
  str[index] = 0;
  display.drawString(0, 16, str);
   
  index = sprintf(str,"hdop: %d.%d",(int)GPS.hdop.hdop(),fracPart(GPS.hdop.hdop(),2));
  str[index] = 0;
  display.drawString(0, 32, str); 
 
  index = sprintf(str,"lat :  %d.%d",(int)GPS.location.lat(),fracPart(GPS.location.lat(),4));
  str[index] = 0;
  display.drawString(60, 16, str);   
  
  index = sprintf(str,"lon:%d.%d",(int)GPS.location.lng(),fracPart(GPS.location.lng(),4));
  str[index] = 0;
  display.drawString(60, 32, str);

  index = sprintf(str,"speed: %d.%d km/h",(int)GPS.speed.kmph(),fracPart(GPS.speed.kmph(),3));
  str[index] = 0;
  display.drawString(0, 48, str);
  display.display();
}

void printGPSInof()
{
  Serial.print("Date/Time: ");
  if (GPS.date.isValid())
  {
    Serial.printf("%d/%02d/%02d",GPS.date.year(),GPS.date.day(),GPS.date.month());
  }
  else
  {
    Serial.print("INVALID");
  }

  if (GPS.time.isValid())
  {
    Serial.printf(" %02d:%02d:%02d.%02d",GPS.time.hour(),GPS.time.minute(),GPS.time.second(),GPS.time.centisecond());
  }
  else
  {
    Serial.print(" INVALID");
  }
  Serial.println();
  
  Serial.print("LAT: ");
  Serial.print(GPS.location.lat(),6);
  Serial.print(", LON: ");
  Serial.print(GPS.location.lng(),6);
  Serial.print(", ALT: ");
  Serial.print(GPS.altitude.meters());

  Serial.println(); 
  
  Serial.print("SATS: ");
  Serial.print(GPS.satellites.value());
  Serial.print(", HDOP: ");
  Serial.print(GPS.hdop.hdop());
  Serial.print(", AGE: ");
  Serial.print(GPS.location.age());
  Serial.print(", COURSE: ");
  Serial.print(GPS.course.deg());
  Serial.print(", SPEED: ");
  Serial.println(GPS.speed.kmph());
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
      
  GPS.begin();
  starttime = millis();
  while( (millis()-starttime) < timeout )
  {
    while (GPS.available() > 0)
    {
      GPS.encode(GPS.read());
    }

   // gps fixed in a second
    if( GPS.location.age() < 1000 )
    {
      break;
    }
  }

  //if gps fixed update gps and print gps info every 1 second
  if(GPS.location.age() < 1000)
  {
    starttime = millis();
    uint32_t printinfo = 0;
    while( (millis()-starttime) < continuetime )
    {
      while (GPS.available() > 0)
      {
        GPS.encode(GPS.read());
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
  GPS.end();  
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
