#include <stdio.h>

#include <string.h>
#include <stdlib.h>

#include "da217.h"


#define FLOAT_TO_INT(x) ((x) >= 0 ? (int)((x) + 0.5) : (int)((x)-0.5))


void DA217::da217_write_single_reg(uint8_t reg,uint8_t data)
{
		da217_write_multiple_reg(reg,&data,1);
}


void DA217::da217_write_multiple_reg(uint8_t reg_begin,uint8_t *data,uint8_t data_size)
{
    uint8_t num = 0;
		Wire.beginTransmission(_address);
		Wire.write(reg_begin);
    while(num < data_size)
    {
		  Wire.write(data[num++]);
    }
		Wire.endTransmission();			
}


uint8_t DA217::da217_read_single_reg(uint8_t reg)
{
    uint8_t data[2];
    da217_read_multiple_reg(reg,data,1);
    return data[0];
}
void DA217::da217_read_multiple_reg(uint8_t reg_begin,uint8_t* data,uint8_t data_size)
{
		uint8_t num = 0;
		Wire.beginTransmission(_address); // start transmission to device 
		Wire.write(reg_begin); // sends register address to read from
		Wire.endTransmission(0); // end transmission

		Wire.requestFrom((uint16_t)_address, (uint8_t)data_size,true);// send data n-bytes read
		while(Wire.available() && (num < data_size))
    {
		  data[num++] = Wire.read(); // receive DATA
    }
}


void DA217::da217_init(int sda,int scl ,uint32_t freq)
{
	uint8_t da217_id = 0;
	_address = I2C_ADDRESS;
	_freq = freq;
	_sda = sda;
	_scl = scl;
	Wire.begin();
  da217_id = da217_read_single_reg(DA217_REG_WHO_AM_I);
  if(da217_id != 0x13)
  {
  	  Serial.printf("da217_init failed! da217_id = %d \r\n",da217_id);
  }
  else
  {
  	 Serial.printf("da217_init successful! da217_id = 0x%x\r\n",da217_id);
  }
  da217_poweroff();
  delay(100);
  da217_poweron();
  da217_enable_watchdog();

}


void DA217::da217_poweron(void)
{
	uint8_t temp;
  temp = DA217_PWR_ON |DA217_BANDWIDTH_500HZ |DA217_AUTOSLEEP_EN;
  da217_write_single_reg(DA217_REG_MODE_BW,temp);
}

void DA217::da217_poweroff(void)
{
	uint8_t temp;
  temp = ~DA217_PWR_ON  & (~DA217_AUTOSLEEP_EN);
  da217_write_single_reg(DA217_REG_MODE_BW,temp);
}

void DA217::da217_start_up_step_detect(bool step_int_en,bool sm_int_en,uint8_t sm_threshold)
{
	uint8_t temp;
	da217_write_single_reg(DA217_REG_STEP_FILTER,DA217_STEP_FILTER_EN);
	da217_write_single_reg(DA217_REG_RESET_STEP,DA217_RESET_STEP);
	delay(100);
	da217_write_single_reg(DA217_REG_RESET_STEP,~DA217_RESET_STEP);

	if(step_int_en)
	{
		temp = 	da217_read_single_reg(DA217_REG_INT_SET0);  
		temp |= DA217_STEP_INT_EN;
		da217_write_single_reg(DA217_REG_INT_SET0,temp);

		temp = 	da217_read_single_reg(DA217_REG_INTERRUPT_MAPPING3);  
		temp |= DA217_INT2_STEP;
		da217_write_single_reg(DA217_REG_INTERRUPT_MAPPING3,temp);
		
    temp = 	da217_read_single_reg(DA217_REG_INTERRUPT_MAPPING1);  
		temp &= ~DA217_INT1_STEP;
		da217_write_single_reg(DA217_REG_INTERRUPT_MAPPING1,temp);

	}
		temp = 	da217_read_single_reg(DA217_REG_INTERRUPT_MAPPING3);  
	  Serial.printf("DA217_REG_INTERRUPT_MAPPING3 = %d \r\n",temp);
	if(sm_int_en)
	{
		temp = 	da217_read_single_reg(DA217_REG_INT_SET0);  
		temp |= DA217_SM_INT_EN;
		da217_write_single_reg(DA217_REG_INT_SET0,temp);
		da217_write_single_reg(DA217_REG_SM_THRESHOLD, sm_threshold);



    temp = 	da217_read_single_reg(DA217_REG_INTERRUPT_MAPPING1);  
		temp |= DA217_INT1_SM;
		da217_write_single_reg(DA217_REG_INTERRUPT_MAPPING1,temp);

		temp = 	da217_read_single_reg(DA217_REG_INTERRUPT_MAPPING3);  
		temp &= ~DA217_INT2_SM;
    	  Serial.printf("DA217_INT2_SM = %u \r\n",temp);

		da217_write_single_reg(DA217_REG_INTERRUPT_MAPPING3,temp);
  }
  da217_write_single_reg(DA217_REG_INT_LATCH,DA217_INT_LATCH_VALUE);


		temp = 	da217_read_single_reg(DA217_REG_INTERRUPT_MAPPING3);  
	  Serial.printf("DA217_REG_INTERRUPT_MAPPING3 = %d \r\n",temp);
    		temp = 	da217_read_single_reg(DA217_REG_INTERRUPT_MAPPING1);  
	  Serial.printf("DA217_REG_INTERRUPT_MAPPING1 = %d \r\n",temp);
    		temp = 	da217_read_single_reg(DA217_REG_INT_SET0);  
	  Serial.printf("DA217_REG_INT_SET0 = %d \r\n",temp);
}


void DA217::da217_stop_step_detect(void)
{
	uint8_t temp;
	da217_write_single_reg(DA217_REG_STEP_FILTER,~DA217_STEP_FILTER_EN);
	da217_write_single_reg(DA217_REG_RESET_STEP,DA217_RESET_STEP);

	temp = 	da217_read_single_reg(DA217_REG_INT_SET0);  
	temp &= ~DA217_STEP_INT_EN;
	temp &= ~DA217_SM_INT_EN;
	da217_write_single_reg(DA217_REG_INT_SET0,temp);
}

uint16_t DA217::da217_read_steps(void)
{
	uint16_t step_num = 0;
  step_num = da217_read_single_reg(DA217_REG_STEPS_MSB) << 8;	  //Step Counter
  step_num += da217_read_single_reg(DA217_REG_STEPS_LSB);	    //Step Counter
  return step_num;
}

da217_step_status_t DA217::da217_read_step_status(void)
{
	uint8_t step_status,temp;
  temp = da217_read_single_reg(DA217_ORIENT_STATUS) ;	  //Step Counter
  step_status =  temp & DA217_STEP_STATUS;
  return (da217_step_status_t)step_status;
}



void DA217::da217_enable_watchdog(void)
{
	uint8_t temp;
  temp = 	da217_read_single_reg(DA217_REG_RESOLUTION_RANGE);  
	temp |= WATCHDOG_EN;
	temp |= WATCHDOG_TIME_50MS;
	da217_write_single_reg(DA217_REG_RESOLUTION_RANGE,temp);
  temp = 	da217_read_single_reg(DA217_REG_RESOLUTION_RANGE);  
}

void DA217::da217_disable_watchdog(void)
{
	uint8_t temp;
  temp = 	da217_read_single_reg(DA217_REG_RESOLUTION_RANGE);  
	temp &= ~WATCHDOG_EN;
	da217_write_single_reg(DA217_REG_RESOLUTION_RANGE,temp);
}



void DA217::da217_start_xyz_axis(bool x_en,bool y_en,bool z_en)
{
  uint8_t temp;
  temp = 	da217_read_single_reg(DA217_REG_ODR_AXIS);  
  if(x_en)
  {
    temp &= ~DA217_X_AXIS_DIS;
  }
  else
  {
    temp |= DA217_X_AXIS_DIS;
  }

  if(y_en)
  {
    temp &= ~DA217_Y_AXIS_DIS;
  }
  else
  {
    temp |= DA217_Y_AXIS_DIS;
  }

  if(z_en)
  {
    temp &= ~DA217_Z_AXIS_DIS;
  }
  else
  {
    temp |= DA217_Z_AXIS_DIS;
  }
	da217_write_single_reg(DA217_REG_ODR_AXIS,temp);

}

void DA217::da217_set_odr_rate(da217_odr_rate_t odr_rate)
{
	uint8_t temp;
  temp = 	da217_read_single_reg(DA217_REG_ODR_AXIS)&0xF0;  
	temp |= odr_rate;
	da217_write_single_reg(DA217_REG_ODR_AXIS,temp);
}

void DA217::da217_set_fifo_mode(da217_fifo_mode_t mode)
{
	uint8_t temp;
  temp = 	da217_read_single_reg(DA217_REG_FIFO_CTRL) &0x3F;  
  temp |= mode;
	da217_write_single_reg(DA217_REG_FIFO_CTRL,temp);
}

void DA217::da217_set_full_scale(da217_full_scale_t fs)
{
	uint8_t temp;
  temp = 	da217_read_single_reg(DA217_REG_RESOLUTION_RANGE) &0xFC;  
  temp |= fs;
	da217_write_single_reg(DA217_REG_RESOLUTION_RANGE,temp);
}

void DA217::da217_set_resolution(da217_resolution_t resolution)
{
	uint8_t temp;
  temp = 	da217_read_single_reg(DA217_REG_RESOLUTION_RANGE) &0xF3;  
  temp |= resolution;
	da217_write_single_reg(DA217_REG_RESOLUTION_RANGE,temp);
}

void DA217::da217_read_xyz_data(uint16_t *x_data,uint16_t *y_data,uint16_t *z_data)
{
  uint8_t data[6];
  if(x_data ==NULL || y_data ==NULL || z_data ==NULL)
  {
    return;
  }
  da217_read_multiple_reg(DA217_REG_ACC_X_LSB,data,6);
  *x_data = data[0]+ (data[1]<< 8);
  *y_data = data[2]+ (data[3]<< 8);
  *z_data = data[4]+ (data[5]<< 8);
}
