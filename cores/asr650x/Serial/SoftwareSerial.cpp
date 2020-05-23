#include "SoftwareSerial.h"

uint8_t SoftwareSerial::s_currentRx = ~0;
uint8_t SoftwareSerial::s_timeDelay = 0;
RingBuffer<uint8_t, 64> SoftwareSerial::s_rxDataBuffer = RingBuffer<uint8_t, 64>();
bool SoftwareSerial::s_overflow = false;

SoftwareSerial::SoftwareSerial(uint8_t rx_GPIO, uint8_t tx_GPIO)
: _rxpin(rx_GPIO)
, _txpin(tx_GPIO)
{
}

//IO analog serial port initialization
void SoftwareSerial::begin(uint16_t Baudrate)
{
	switch(Baudrate)
	{
		case 14400:
		s_timeDelay = 69 ;
		break;
		case 9600:
		s_timeDelay = 104 ;
		break;
		case 4800:
		s_timeDelay = 208 ;
		break;
		case 2400:
		s_timeDelay = 417 ;
		break;
		default :  
		s_timeDelay = 833 ;  //1200
	}

	pinMode(_rxpin,INPUT);
    digitalWrite(_rxpin,HIGH);

	pinMode(_txpin, OUTPUT);
	digitalWrite(_txpin,HIGH); // TX 0 is to start sending, so it needs to be pulled high

	listen(); // start listening
}

int SoftwareSerial::available()
{
	return s_rxDataBuffer.size();
}

void SoftwareSerial::receiveByte(void)
{
	uint8_t bitCount(0);
	uint8_t tempByte(0);

	delayMicroseconds(s_timeDelay); 

	while(bitCount < 8){
		if(digitalRead(s_currentRx)){
			tempByte |= (1 << bitCount);
		}
		bitCount++;
		delayMicroseconds(s_timeDelay);
	}

	if(s_rxDataBuffer.full() == false){
		s_rxDataBuffer.write(tempByte);
	} else {
		s_overflow = true;
	}
}

void SoftwareSerial::receiver(void)
{
	ClearPinInterrupt(s_currentRx);
	receiveByte();
	digitalWrite(s_currentRx,HIGH);
	attachInterrupt(s_currentRx, receiver, FALLING);
}

bool SoftwareSerial::overflow(){
	return s_overflow;
}

void SoftwareSerial::listen(){
	stop();
	s_currentRx = _rxpin;
	digitalWrite(s_currentRx,HIGH);
	attachInterrupt(s_currentRx, receiver, FALLING);
}

void SoftwareSerial::stop(){
	if(s_currentRx != uint8_t(~0)){
		detachInterrupt(s_currentRx);
		s_currentRx = ~0;
		flush();
	}
}

bool SoftwareSerial::isListening(){
    return (_rxpin == s_currentRx);
}

int SoftwareSerial::peek(){
	if(s_rxDataBuffer.empty() == false)
	{
		return s_rxDataBuffer.peek();
	}
	return (-1);
}

int SoftwareSerial::read(){
	if(s_rxDataBuffer.empty() == false)
	{
		s_overflow = false;
		return s_rxDataBuffer.read();
	}
	return (-1);
}

void SoftwareSerial::flush(){
	s_rxDataBuffer.reset();
	s_overflow = false;
}

size_t SoftwareSerial::write(uint8_t value){
	digitalWrite(_txpin, LOW);
    delayMicroseconds(s_timeDelay); // The baud rate is set according to the delay 
    
    for(uint16_t i = 0; i < 8; i++)
    {
        if(value & 0x01)
        {
            digitalWrite(_txpin, HIGH);
        }
        else
        {
            digitalWrite(_txpin, LOW);
        }
		value >>= 1;
        delayMicroseconds(s_timeDelay);
    }
    
    digitalWrite(_txpin, HIGH);
    delayMicroseconds(s_timeDelay);

	return sizeof(value);
}

size_t SoftwareSerial::write(const uint8_t *buffer, size_t size){
    int i=0;
    while ((size--)!=0)
    {  
        write(buffer[i]);
        i++;
    }

	return i;
}

void SoftwareSerial::softwarePrintf(char *p_fmt, ...){
	uint8_t str[100u];
	uint16_t len;
	va_list vArgs;

	va_start(vArgs, p_fmt);

	vsprintf((char *)str, (char const *)p_fmt, vArgs);

	va_end(vArgs);

	len = strlen((char *)str);

	write((uint8_t *)str, len);
}
