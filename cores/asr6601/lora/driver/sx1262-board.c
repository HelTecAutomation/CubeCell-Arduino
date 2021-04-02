/*!
 * \file      sx1262dvk1cas-board.c
 *
 * \brief     Target board SX1262DVK1CAS shield driver implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */
#include <stdlib.h>
#include "tremo_gpio.h"
#include "tremo_regs.h"
#include "tremo_delay.h"
#include "lora_config.h"
#include "sx126x-board.h"
#include "sx126x.h"

#define BOARD_TCXO_WAKEUP_TIME 2
uint8_t gPaOptSetting = 0;

int boardIrqIsDisabled = false;
bool BoardDisableIrq( void )
{
   if(boardIrqIsDisabled==false)
   {
       __disable_irq();
       boardIrqIsDisabled = true;
       return true;
   }
   else
      return false;
}

void BoardEnableIrq( bool disabledhere)
{
    if(disabledhere)
    {
       __enable_irq();
       boardIrqIsDisabled = false;
    }
}

uint16_t SpiInOut( uint16_t outData )
{
    uint8_t read_data = 0;
    
    LORAC->SSP_DR = outData;
	
	while(1) {
		uint32_t status = LORAC->SSP_SR;
		if(((status & 0x01) == 0x01) && ((status & 0x10)==0)) break;
	}
	
	read_data = LORAC->SSP_DR & 0xFF;

    return( read_data );
}


void SX126xLoracInit()
{
	LORAC->CR0 = 0x00000200;

    LORAC->SSP_CR0 = 0x07;
    LORAC->SSP_CPSR = 0x02;

    //wakeup lora 
    //avoid always waiting busy after main reset or soft reset
    if(LORAC->CR1 != 0x80)
    {
        delayMicroseconds(20);
        LORAC->NSS_CR = 0;
        delayMicroseconds(20);
        LORAC->NSS_CR = 1;
    }

    LORAC->SSP_CR1 = 0x02;
    
    NVIC_EnableIRQ(LORA_IRQn);

    //enable LORA_IRQHandler wakeup
    TREMO_REG_WR(0x40001804,TREMO_REG_RD(0x40001804)|0x80);
    //NVIC_SetPriority(LORAC_IRQn, 2);
    pinMode(CONFIG_LORA_RFSW_VDD_PIN,OUTPUT);
    digitalWrite(CONFIG_LORA_RFSW_VDD_PIN,0);
}


uint32_t SX126xGetBoardTcxoWakeupTime( void )
{
    return BOARD_TCXO_WAKEUP_TIME;
}

void SX126xReset( void )
{
    LORAC->CR1 |= 1<<5;  //nreset
    LORAC->CR1 &= ~(1<<7); //por
    LORAC->CR0 |= 1<<5; //irq0
    LORAC->CR1 |= 0x1;  //tcxo
    
    while((LORAC->SR & 0x100));  
}

void SX126xWaitOnBusy( void )
{
    delayMicroseconds(10);
    int n = 0;
    while( LORAC->SR & 0x100 )
    {
        delayMicroseconds(1);
        n++;
        if(n>=10000)
        {
           printf("spi busy\r\n");
           break;
        }
    }
}

void SX126xWakeup( void )
{
    bool tempIrq = BoardDisableIrq( );

    LORAC->NSS_CR = 0;
    delayMicroseconds(10);

    SpiInOut( RADIO_GET_STATUS );
    SpiInOut( 0x00 );

    LORAC->NSS_CR = 1;

    // Wait for chip to be ready.
    SX126xWaitOnBusy( );

    BoardEnableIrq(tempIrq);
}

void SX126xWriteCommand( RadioCommands_t command, uint8_t *buffer, uint16_t size )
{
    bool tempIrq = BoardDisableIrq( );

    SX126xCheckDeviceReady( );

    LORAC->NSS_CR = 0;

    SpiInOut( ( uint8_t )command );

    for( uint16_t i = 0; i < size; i++ )
    {
        SpiInOut( buffer[i] );
    }

    LORAC->NSS_CR = 1;

    if( command != RADIO_SET_SLEEP )
    {
        SX126xWaitOnBusy( );
    }

    BoardEnableIrq(tempIrq);
}

void SX126xReadCommand( RadioCommands_t command, uint8_t *buffer, uint16_t size )
{
    bool tempIrq = BoardDisableIrq( );

    SX126xCheckDeviceReady( );

    LORAC->NSS_CR = 0;

    SpiInOut( ( uint8_t )command );
    SpiInOut( 0x00 );
    for( uint16_t i = 0; i < size; i++ )
    {
        buffer[i] = SpiInOut( 0 );
    }

    LORAC->NSS_CR = 1;

    SX126xWaitOnBusy( );

    BoardEnableIrq(tempIrq);
}

void SX126xWriteRegisters( uint16_t address, uint8_t *buffer, uint16_t size )
{
    bool tempIrq = BoardDisableIrq( );

    SX126xCheckDeviceReady( );

    LORAC->NSS_CR = 0;
    
    SpiInOut( RADIO_WRITE_REGISTER );
    SpiInOut( ( address & 0xFF00 ) >> 8 );
    SpiInOut( address & 0x00FF );
    
    for( uint16_t i = 0; i < size; i++ )
    {
        SpiInOut( buffer[i] );
    }

    LORAC->NSS_CR = 1;

    SX126xWaitOnBusy( );

    BoardEnableIrq(tempIrq);
}

void SX126xWriteRegister( uint16_t address, uint8_t value )
{
    SX126xWriteRegisters( address, &value, 1 );
}

void SX126xReadRegisters( uint16_t address, uint8_t *buffer, uint16_t size )
{
    bool tempIrq = BoardDisableIrq( );

    SX126xCheckDeviceReady( );

    LORAC->NSS_CR = 0;

    SpiInOut( RADIO_READ_REGISTER );
    SpiInOut( ( address & 0xFF00 ) >> 8 );
    SpiInOut( address & 0x00FF );
    SpiInOut( 0 );
    for( uint16_t i = 0; i < size; i++ )
    {
        buffer[i] = SpiInOut( 0 );
    }
    LORAC->NSS_CR = 1;

    SX126xWaitOnBusy( );

    BoardEnableIrq(tempIrq);
}

uint8_t SX126xReadRegister( uint16_t address )
{
    uint8_t data;
    SX126xReadRegisters( address, &data, 1 );
    return data;
}

void SX126xWriteBuffer( uint8_t offset, uint8_t *buffer, uint8_t size )
{
    bool tempIrq = BoardDisableIrq( );

    SX126xCheckDeviceReady( );

    LORAC->NSS_CR = 0;

    SpiInOut( RADIO_WRITE_BUFFER );
    SpiInOut( offset );
    for( uint16_t i = 0; i < size; i++ )
    {
        SpiInOut( buffer[i] );
    }
    LORAC->NSS_CR = 1;

    SX126xWaitOnBusy( );

    BoardEnableIrq(tempIrq);
}

void SX126xReadBuffer( uint8_t offset, uint8_t *buffer, uint8_t size )
{
    bool tempIrq = BoardDisableIrq( );

    SX126xCheckDeviceReady( );

    LORAC->NSS_CR = 0;

    SpiInOut( RADIO_READ_BUFFER );
    SpiInOut( offset );
    SpiInOut( 0 );
    for( uint16_t i = 0; i < size; i++ )
    {
        buffer[i] = SpiInOut( 0 );
    }
    LORAC->NSS_CR = 1;
    
    SX126xWaitOnBusy( );

    BoardEnableIrq(tempIrq);
}

void SX126xSetRfTxPower( int8_t power )
{
    SX126xSetTxParams( power, RADIO_RAMP_40_US );
}

uint8_t SX126xGetPaSelect( uint32_t channel )
{
    return SX1262;
}

void SX126xAntSwOn( void )
{
    digitalWrite(CONFIG_LORA_RFSW_VDD_PIN,HIGH);
}

void SX126xAntSwOff( void )
{
    digitalWrite(CONFIG_LORA_RFSW_VDD_PIN,LOW);
}

bool SX126xCheckRfFrequency( uint32_t frequency )
{
    // Implement check. Currently all frequencies are supported
    return true;
}

uint8_t SX126xGetPaOpt( )
{
    return gPaOptSetting;
}

void SX126xSetPaOpt( uint8_t opt )
{
    if(opt>3) return;
    
    gPaOptSetting = opt;
}
