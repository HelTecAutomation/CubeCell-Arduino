#include "softSerial.h"

uint16_t timedelay = 0;
uint8_t Recev[8]  ={0};
uint8_t temp_bin  = 0;

uint8_t TX_GPIO;
uint8_t RX_GPIO;

uint8_t print_Sign = 0;
uint8_t IRREC_RX_BUF[64] = {0};
uint16_t IRREC_RX_CNT = 0;
uint8_t rebit = 0;

softSerial::softSerial(uint8_t tx_GPIO, uint8_t rx_GPIO):
pbuffer(0)
// print_Sign(0),
// IRREC_RX_BUF{0},
// IRREC_RX_CNT(0),
// rebit(0)
{
	TX_GPIO = tx_GPIO;
	RX_GPIO = rx_GPIO;
}

//IO模拟串口初始�?
void softSerial::begin(uint16_t Baudrate)
{
	switch(Baudrate)
	{
		case 14400:
		timedelay = 69 ;
		break;
		case 9600:
		timedelay = 104 ;
		break;
		case 4800:
		timedelay = 208 ;
		break;
		case 2400:
		timedelay = 417 ;
		break;
		default :  
		timedelay = 833 ;  //1200
	}
	pinMode(RX_GPIO,INPUT);
	digitalWrite(RX_GPIO,HIGH);
	attachInterrupt(RX_GPIO, receiver, FALLING);

	pinMode(TX_GPIO, OUTPUT);
	digitalWrite(TX_GPIO,HIGH);   //TX�?是开始发送，所以需要拉�?
}

int softSerial::available(void)
{
	if(IRREC_RX_CNT){
		return IRREC_RX_CNT;
	}
	else
	{
		return 0;
	}
	
}

void softSerial::receiverBegin(void)
{
	uint8_t count;
	uint8_t data = 0;
	delayMicroseconds(timedelay); 
	for(count = 0; count < 8; count++){
		data |= digitalRead(RX_GPIO)<<count;
		delayMicroseconds(timedelay);
	}

	if(IRREC_RX_CNT < 64)
	{
		IRREC_RX_BUF[IRREC_RX_CNT++] = data;
	}

}

void softSerial::receiver(void)
{
	rebit = 0 ;
	receiverBegin();
}

int softSerial::read(void)
{
	if(available())
	{
		IRREC_RX_CNT--;
		uint8_t temp = IRREC_RX_BUF[pbuffer++];
		if(IRREC_RX_CNT==0)
		{
			pbuffer = 0;
		}
		return temp;
	}
	return (-1);
}

void softSerial::flush()
{
	memset(IRREC_RX_BUF, 0, 64*sizeof(uint8_t));
	IRREC_RX_CNT = 0;
	pbuffer = 0;
}

void softSerial::sendByte(uint8_t val)//发送bit�?
{
    uint16_t i;
	digitalWrite(TX_GPIO, LOW);
    delayMicroseconds(timedelay);//波特率根据延时在设置 
    
    for(i=0;i<8;i++)
    {
        if(val&0x01)
        {
            digitalWrite(TX_GPIO,HIGH);
        }
        else
        {
            digitalWrite(TX_GPIO,LOW);
        }
		val>>=1;
        delayMicroseconds(timedelay);
    }
    
    digitalWrite(TX_GPIO,HIGH);
    delayMicroseconds(timedelay);
}

void softSerial::sendStr(uint8_t *st,uint16_t len)
{
    int i=0;
    while ((len--)!=0)
    {  
        sendByte(st[i]);
        i++;
    }
}

void  softSerial::softwarePrintf(char *p_fmt, ...)
{
	uint8_t  str[100u];
	uint16_t  len;

	va_list     vArgs;


	va_start(vArgs, p_fmt);

	vsprintf((char *)str, (char const *)p_fmt, vArgs);

	va_end(vArgs);

	len = strlen((char *)str);

	sendStr((uint8_t *)str, len);
}
