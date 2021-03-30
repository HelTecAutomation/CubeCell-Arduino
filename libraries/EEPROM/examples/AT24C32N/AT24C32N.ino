#include <Wire.h>
#include <AT24C32N.h>


EEPROM_AT24C32N rtcEeprom;
#define AT24C32N_ADDRESS  0x50

uint16_t StartP;

void setup ()
{
	pinMode(Vext,OUTPUT);
	digitalWrite(Vext,LOW); //SET POWER
	Serial.begin(115200);
	Wire.begin(); // SDA, SCL

	Serial.println("");
	Serial.println("");
	Serial.println("Started");
	delay(1000);
	StartP = 5;
}



void loop ()
{
	uint16_t VarLen;
	char VarToRead[70];
	char ToWrite[] = "abcdefghijklmonpqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	uint8_t lp;

	VarLen = strlen(ToWrite);

	Serial.printf("About to write [%s] \r\nwhich is [%d] bytes long at memory address [%d]\r\n",ToWrite,VarLen,StartP);

	rtcEeprom.WriteBytes(StartP,VarLen,ToWrite);
	memset(VarToRead,0,70);
	rtcEeprom.RetrieveBytes(VarToRead, VarLen, StartP, true);
	Serial.printf("Wrote    : [%s]\r\nReceived : [%s]",ToWrite,VarToRead);

	if(strcmp(ToWrite,VarToRead)!=0)
	  {
	  Serial.print("FAIL FAIL ");
	  for (lp=0;lp<VarLen;lp++)
	    {
	    if (ToWrite[lp]!=VarToRead[lp]) break;
	    }
	  Serial.printf("at position %d, lp/32=%d, lp+address=%d\n",lp,(lp % 32), ((lp + StartP) % 32));
	  }
	else Serial.println("\n");

	Serial.println("\n");
	StartP+=22;if (StartP>4000) StartP=4;
	Serial.printf("\n");
	delay(10000);
}

 
