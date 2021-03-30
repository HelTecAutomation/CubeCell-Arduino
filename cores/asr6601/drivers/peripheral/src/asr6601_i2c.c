#include "asr6601_i2c.h"

i2c_t * i2cInit(int8_t i2c_num, int8_t sda, int8_t scl, uint32_t frequency)
{
	i2c_num = -1;
	uint8_t sda_temp = sda;
	uint8_t scl_temp = scl;
	
	if( sda>47 || scl>47 || scl==2 || sda<0 || scl<0 )
		return NULL;
		
	while(sda_temp>11)
		sda_temp-=12;
	while(scl_temp>11)
		scl_temp-=12;
	
	if( scl_temp == 2 && sda_temp == 3)
	{
		i2c_num = 0;
	}
	else if( scl_temp == 6 && sda_temp == 7)
	{
		i2c_num = 1;
	}
	else if( scl_temp == 10 && sda_temp == 11)
	{
		i2c_num = 2;
	}
	if(i2c_num<0)
		return NULL;

	pinMode((uint8_t)sda,ANALOG); 
	pinMode((uint8_t)scl,ANALOG);
	iomux((uint8_t)sda,3);
	iomux((uint8_t)scl,3);

	i2c_t* i2cArray[] = {I2C0,I2C1,I2C2};
	i2c_t * i2c = i2cArray[i2c_num];
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_I2C0-i2c_num, true);
	
	i2c_config_t config;
	i2c_config_init(&config);
	config.freq = frequency;
	i2c_init(i2c, &config);
	i2c_cmd(i2c, true);
	
	return i2c;
}
void i2cEnd(i2c_t * i2c,int8_t sda, int8_t scl)
{
	uint8_t i2c_num;
	if(i2c==I2C0) 
		i2c_num=0;
	else if(i2c==I2C1)
		i2c_num=1;
	else if(i2c==I2C2)
		i2c_num=2;
	else
		return;
	pinMode((uint8_t)sda,ANALOG);
	pinMode((uint8_t)scl,ANALOG);

	uint32_t peripheral = RCC_PERIPHERAL_I2C0 - i2c_num;
	rcc_enable_peripheral_clk(peripheral, false);
	rcc_rst_peripheral(peripheral, true);
	rcc_rst_peripheral(peripheral, false);
}

i2c_err_t i2cSetFrequency(i2c_t * i2c, uint32_t clk_speed)
{
	uint32_t i2c_clk_freq = (i2c == I2C0) ? rcc_get_clk_freq(RCC_PCLK0) : rcc_get_clk_freq(RCC_PCLK1);
	uint32_t flv = ( i2c_clk_freq / clk_speed )/2 - 19 - round(clk_speed/100000);
	i2c->LCR = flv << 9; // set divider
	//i2cx->LCR = slv | flv << 9; // set divider
	i2c->WCR = flv / 3;        // set wait count value to adjust clock for
	return I2C_ERROR_OK;
}

uint32_t i2cGetFrequency(i2c_t * i2c)
{
	uint32_t flv = i2c->LCR >> 9;
	uint32_t i2c_clk_freq = (i2c == I2C0) ? rcc_get_clk_freq(RCC_PCLK0) : rcc_get_clk_freq(RCC_PCLK1);
	int temp =  i2c_clk_freq/((flv + 22)*2);
	return i2c_clk_freq/((flv + 19 + round(temp/100000))*2);
}

i2c_err_t i2cWrite(i2c_t * i2c, uint16_t address, uint8_t* buff, uint16_t size, bool sendStop, uint16_t timeOutMillis)
{
	if((i2c==NULL)||((size>0)&&(buff==NULL))) { // need to have location to store requested data
		return I2C_ERROR_DEV;
	}

	uint32_t time;
	i2c_master_send_start(i2c, address, I2C_WRITE);

	i2c_clear_flag_status(i2c, I2C_FLAG_TRANS_EMPTY);

	time = millis();

	while (i2c_get_flag_status(i2c, I2C_FLAG_TRANS_EMPTY) != SET)
	{
		if( (millis() - time) >= timeOutMillis )
		{
			return I2C_ERROR_TIMEOUT;
		}
	}

	for(uint16_t i = 0;i<size;i++)
	{
		i2c_send_data(i2c, buff[i]);
		
		i2c_clear_flag_status(i2c, I2C_FLAG_TRANS_EMPTY);
		time = millis();
		while (i2c_get_flag_status(i2c, I2C_FLAG_TRANS_EMPTY) != SET)
		{
			if( (millis() - time) >= timeOutMillis )
			{
				return I2C_ERROR_TIMEOUT;
			}
		}
	}

	if(sendStop)
	{
		i2c_master_send_stop(i2c);
	}
	return I2C_ERROR_OK;
}

i2c_err_t i2cRead(i2c_t * i2c, uint16_t address, uint8_t* buff, uint16_t size, bool sendStop, uint16_t timeOutMillis, uint32_t *readCount)
{
	if((i2c==NULL)||((size>0)&&(buff==NULL))) { // need to have location to store requested data
		return I2C_ERROR_DEV;
	}

	uint32_t time;

	for(uint16_t i = 0;i<size;i++)
	{
		i2c_master_send_start(i2c, address, I2C_READ);
		i2c_clear_flag_status(i2c, I2C_FLAG_TRANS_EMPTY);
		time = millis();
		while (i2c_get_flag_status(i2c, I2C_FLAG_TRANS_EMPTY) != SET)
		{
			if( (millis() - time) >= timeOutMillis )
				return I2C_ERROR_TIMEOUT;
		}

		i2c_set_receive_mode(i2c, I2C_NAK);
		time = millis();
		while (i2c_get_flag_status(i2c, I2C_FLAG_RECV_FULL) != SET)
		{
			if( (millis() - time) >= timeOutMillis )
				return I2C_ERROR_TIMEOUT;
		}
		i2c_clear_flag_status(i2c, I2C_FLAG_RECV_FULL);
		buff[i] = i2c_receive_data(i2c);
		(* readCount)++;
	}
	if(sendStop)
	{
		i2c_master_send_stop(i2c);
	}
	return I2C_ERROR_OK;
}

i2c_err_t i2cFlush(i2c_t * i2c)
{
	return I2C_ERROR_OK;
}

uint32_t i2cGetStatus(i2c_t * i2c)
{
	if(i2c==NULL)
		return I2C_ERROR_DEV;
	else
		return I2C_ERROR_OK;
}

