#include "softSerial.h"
#include "OneWire.h"
#include "util/OneWire_direct_gpio.h"

#define SOFTSERIAL_BUFF_SIZE  255
#define SOFTSERIAL_RX_TIMEOUT  3000
static float timedelay = 0;
static uint8_t Recev[8]  ={0};
static uint8_t temp_bin  = 0;

static uint8_t TX_GPIO;
static uint8_t RX_GPIO;
static uint8_t _rxbitmask;
static uint8_t _txbitmask;
static uint32_t *_rxbaseReg;
static uint32_t *_txbaseReg;

static uint8_t print_Sign = 0;
static uint8_t IRREC_RX_BUF[SOFTSERIAL_BUFF_SIZE] = {0};
static uint16_t IRREC_RX_CNT = 0;
static uint8_t rebit = 0;
static uint8_t tikerInUs;

#if defined(__asr650x__)
#define GET_MCU_TIKER   CY_SYS_SYST_CVR_REG
#define GET_MCU_RELOAD  CY_SYS_SYST_RVR_REG
#elif defined(__asr6601__)
#define GET_MCU_TIKER   SysTick->VAL
#define GET_MCU_RELOAD  SysTick->LOAD
#endif
static void delayTiker(uint32_t  ticks)
{
    uint32_t tpre = GET_MCU_TIKER;
    uint32_t tnow, tcnt = 0;
    uint32_t reload = GET_MCU_RELOAD;
    while (1) {
        tnow = GET_MCU_TIKER;
        if (tnow < tpre)
            tcnt += tpre - tnow;
        else
            tcnt += reload - tnow + tpre;

        tpre = tnow;

        if (tcnt >= ticks)
            break;
    };
}

static void delayus(float nus)
{
    uint32_t tpre = GET_MCU_TIKER;
    uint32_t tnow, tcnt = 0;
    uint32_t reload = GET_MCU_RELOAD;
    uint32_t ticks = (uint32_t)(nus * tikerInUs);
    while (1) {
        tnow = GET_MCU_TIKER;
        if (tnow < tpre)
            tcnt += tpre - tnow;
        else
            tcnt += reload - tnow + tpre;

        tpre = tnow;

        if (tcnt >= ticks)
            break;
    };
}

softSerial::softSerial(uint8_t tx_GPIO, uint8_t rx_GPIO):
pbuffer(0)
{
	TX_GPIO = tx_GPIO;
	RX_GPIO = rx_GPIO;
	_rxbitmask = PIN_TO_BITMASK(RX_GPIO);
	_rxbaseReg = PIN_TO_BASEREG(RX_GPIO);
	_txbitmask = PIN_TO_BITMASK(TX_GPIO);
	_txbaseReg = PIN_TO_BASEREG(TX_GPIO);

}


void softSerial::begin(uint16_t Baudrate)
{
	timedelay = 1000000/Baudrate;
	if(Baudrate<=19200)
		tikerInUs=48;
	else
		tikerInUs=45;
	pinMode(RX_GPIO,INPUT);
	digitalWrite(RX_GPIO,HIGH);
	attachInterrupt(RX_GPIO, receiverBegin, FALLING);

	pinMode(TX_GPIO, OUTPUT);
	digitalWrite(TX_GPIO,HIGH);
}

void softSerial::end()
{
    digitalWrite(TX_GPIO, LOW);
    detachInterrupt(RX_GPIO);
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
extern uint32_t intTime;
void softSerial::receiverBegin(void)
{
	
	uint32_t timetemp=GET_MCU_TIKER;
	uint32_t tcnt=0;
	if (timetemp < intTime)
		tcnt += intTime - timetemp;
	else
		tcnt += GET_MCU_RELOAD - timetemp + intTime;

	delayTiker((uint32_t)(timedelay * tikerInUs)-tcnt);

	uint8_t data = 0;
	uint32_t start = millis();
    do {
		for( uint8_t count = 0; count < 8; count++){
			data |= DIRECT_READ(_rxbaseReg, _rxbitmask)<<count;
			delayus(timedelay);
		}

		if(IRREC_RX_CNT < SOFTSERIAL_BUFF_SIZE)
		{
			IRREC_RX_BUF[IRREC_RX_CNT++] = data;
		}
		uint32_t tpre = GET_MCU_TIKER;
		uint32_t tnow, tcnt = 0;
		uint32_t reload = GET_MCU_RELOAD;
		uint32_t ticks = 2*(uint32_t)((float)timedelay * tikerInUs);

		while (1) {
			if(DIRECT_READ(_rxbaseReg, _rxbitmask)==0)
				break;
			tnow = GET_MCU_TIKER;
			if (tnow < tpre)
				tcnt += tpre - tnow;
			else
				tcnt += reload - tnow + tpre;
	
			tpre = tnow;
	
			if (tcnt >= ticks)
				return;
		};
		delayus(timedelay);
		data=0;
	} while (millis() - start < SOFTSERIAL_RX_TIMEOUT);
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
	memset(IRREC_RX_BUF, 0, SOFTSERIAL_BUFF_SIZE);
	IRREC_RX_CNT = 0;
	pbuffer = 0;
}

void softSerial::sendByte(uint8_t val)
{
    noInterrupts();
    DIRECT_WRITE_LOW(_txbaseReg, _txbitmask);
    delayus(timedelay);
    
    for(int i=0;i<8;i++)
    {
        if(val&0x01)
        {
            DIRECT_WRITE_HIGH(_txbaseReg, _txbitmask);
        }
        else
        {
            DIRECT_WRITE_LOW(_txbaseReg, _txbitmask);
        }
		val>>=1;
        delayus(timedelay);
    }
    
    DIRECT_WRITE_HIGH(_txbaseReg, _txbitmask);
    delayus(timedelay);
    interrupts();
}

size_t softSerial::write(uint8_t c)
{
	sendByte(c);
	return 1;
}

int softSerial::peek(void)
{
  return 0;
}

size_t softSerial::write(const uint8_t *buffer, size_t size)
{
	uint32_t bufIndex;
	bufIndex = 0u;
	   
	while(bufIndex < size)
	{
		write(buffer[bufIndex]);
		bufIndex++;
	}
	return size;
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
    char loc_buf[64];
    char * temp = loc_buf;
    va_list arg;
    va_list copy;
    va_start(arg, p_fmt);
    va_copy(copy, arg);
    size_t len = vsnprintf(NULL, 0, p_fmt, arg);
    va_end(copy);
    if(len >= sizeof(loc_buf)){
        temp = new char[len+1];
        if(temp == NULL) {
            return;
        }
    }
    len = vsnprintf(temp, len+1, p_fmt, arg);
    write((uint8_t*)temp, len);
    va_end(arg);
    if(len >= sizeof(loc_buf)){
        delete[] temp;
    }
}
