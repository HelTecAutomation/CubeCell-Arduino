

#include "Arduino.h"
#include "da217.h"


DA217 da217;
uint16_t step_num;
uint16_t x_data,y_data,z_data;
void setup()
{
	Serial.begin(115200);
	pinMode(Vext,OUTPUT);
	digitalWrite(Vext,LOW);//set vext to high
	da217.da217_init();
	da217.da217_set_odr_rate(DA217_ODR_500HZ);
	da217.da217_set_fifo_mode(FIFO_MODE_FIFO);
	da217.da217_set_full_scale(FS_FULL_SCALE_2g);
	da217.da217_set_resolution(RESOLUTION_14_BIT);
	da217.da217_start_xyz_axis(true,true,true);
}

void loop()
{ 
	da217.da217_read_xyz_data(&x_data,&y_data,&z_data);
    Serial.printf("x_data=%d  y_data=%d z_data=%d\r\n",x_data,y_data,z_data);
	delay(3000);
}