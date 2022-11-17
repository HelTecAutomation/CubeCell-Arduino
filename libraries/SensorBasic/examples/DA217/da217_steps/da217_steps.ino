

#include "Arduino.h"
#include "da217.h"

#define INT1_PIN GPIO0
#define INT2_PIN GPIO1
bool da217_int1_flag,da217_int2_flag;

DA217 da217;
uint16_t step_num;
da217_step_status_t step_status;


void da217_int1_isr(void) 
{
	da217_int1_flag =true;
}
void da217_int2_isr(void) 
{
	da217_int2_flag =true;
}

void setup()
{
	Serial.begin(115200);
	PINMODE_INPUT_PULLDOWN(INT1_PIN);
	PINMODE_INPUT_PULLDOWN(INT2_PIN);
	attachInterrupt(INT1_PIN, da217_int1_isr, RISING);
	attachInterrupt(INT2_PIN, da217_int2_isr, RISING);
	pinMode(Vext,OUTPUT);
  	digitalWrite(Vext,LOW);//set vext to high
	da217.da217_init();
	da217.da217_start_up_step_detect(true,true,5);
}


void loop()
{ 
	step_num = da217.da217_read_steps();
	step_status = da217.da217_read_step_status();
	Serial.printf("step_num = %d  step_status =%d\r\n",step_num,step_status);
	if(	da217_int1_flag )
	{
		Serial.println("da217_int1_flag");
		da217_int1_flag = false;
	}
	if(da217_int2_flag)
	{
		Serial.println("da217_int2_flag");
		da217_int2_flag = false;
	}
	delay(3000);
}
