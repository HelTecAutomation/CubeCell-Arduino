

#include "Arduino.h"
#include "da217.h"

DA217 da217;
uint16_t step_num;
da217_step_status_t step_status;

void setup()
{
	Serial.begin(115200);
	pinMode(Vext,OUTPUT);
  	digitalWrite(Vext,LOW);//set vext to high
	da217.da217_init();
	da217.da217_start_up_step_detect();
}

void loop()
{ 
	step_num = da217.da217_read_steps();
	step_status = da217.da217_read_step_status();
	Serial.printf("step_num = %d  step_status =%d\r\n",step_num,step_status);
	delay(3000);
}
