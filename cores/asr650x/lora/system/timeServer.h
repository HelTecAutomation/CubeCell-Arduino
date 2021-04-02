/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: Timer objects and scheduling management

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
/******************************************************************************
  * @file    timeServer.h
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    01-June-2017
  * @brief   is the timer server driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMESERVER_H__
#define __TIMESERVER_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
   
#include "utilities.h"

/* Exported types ------------------------------------------------------------*/

/*!
 * \brief Timer object description
 */
typedef struct TimerEvent_s
{
    uint32_t Timestamp;         //! Expiring timer value in ticks from TimerContext
    uint32_t ReloadValue;       //! Reload Value when Timer is restarted
    bool IsRunning;             //! Is the timer currently running
    void ( *Callback )( void ); //! Timer IRQ callback function
    struct TimerEvent_s *Next;  //! Pointer to the next Timer object.
} TimerEvent_t;


/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
/*!
 * \brief Number of seconds elapsed between Unix and GPS epoch
 */
#define UNIX_GPS_EPOCH_OFFSET                       315964800

/*!
 * \brief Structure holding the system time in seconds and miliseconds.
 */
typedef struct TimerSysTime_s
{
    uint32_t Seconds;
    int16_t SubSeconds;
}TimerSysTime_t;

/*!
 * Adds 2 TimerSysTime_t values
 *
 * \param a Value
 * \param b Value to added
 *
 * \retval result Addition result (TimerSysTime_t value)
 */
inline TimerSysTime_t TimerAddSysTime( TimerSysTime_t a, TimerSysTime_t b )
{
    TimerSysTime_t c = { 0 };

    c.Seconds = a.Seconds + b.Seconds;
    c.SubSeconds = a.SubSeconds + b.SubSeconds;
    if( c.SubSeconds >= 1000 )
    {
        c.Seconds++;
        c.SubSeconds -= 1000;
    }
    return c;
}

/*!
 * Subtracts 2 TimerSysTime_t values
 *
 * \param a Value
 * \param b Value to be subtracted
 *
 * \retval result Subtraction result (TimerSysTime_t value)
 */
inline TimerSysTime_t TimerSubSysTime( TimerSysTime_t a, TimerSysTime_t b )
{
    TimerSysTime_t c = { 0 };

    c.Seconds = a.Seconds - b.Seconds;
    c.SubSeconds = a.SubSeconds - b.SubSeconds;
    if( c.SubSeconds < 0 )
    {
        c.Seconds--;
        c.SubSeconds += 1000;
    }
    return c;
}

/*!
 * \brief Gets current MCU system time
 *
 * \retval sysTime    Current seconds/sub-seconds since Mcu started
 */
TimerSysTime_t SysTimeGetMcuTime( void );

/*!
 * Converts the given SysTime to the equivalent RTC value in milliseconds
 *
 * \param [IN] sysTime System time to be converted
 * 
 * \retval timeMs The RTC converted time value in ms
 */
uint32_t SysTimeToMs( TimerSysTime_t sysTime );

/*!
 * Converts the given RTC value in milliseconds to the equivalent SysTime
 *
 * \param [IN] timeMs The RTC time value in ms to be converted
 * 
 * \retval sysTime Converted system time
 */
TimerSysTime_t SysTimeFromMs( uint32_t timeMs );


/*!
 * \brief Sets the system time with the number of sconds elapsed since epoch
 *
 * \param [IN] sysTime Structure provideing the number of seconds and 
 *                     subseconds elapsed since epoch
  */
void TimerSetSysTime( TimerSysTime_t sysTime );

/*!
 * \brief Gets the current system number of sconds elapsed since epoch
 *
 * \retval sysTime Structure provideing the number of seconds and 
 *                 subseconds elapsed since epoch
  */
TimerSysTime_t TimerGetSysTime( void );

/*!
 * \brief Initializes the timer object
 *
 * \remark TimerSetValue function must be called before starting the timer.
 *         this function initializes timestamp and reload value at 0.
 *
 * \param [IN] obj          Structure containing the timer object parameters
 * \param [IN] callback     Function callback called at the end of the timeout
 */
void TimerInit( TimerEvent_t *obj, void ( *callback )( void ) );

/*!
 * \brief Timer IRQ event handler
 *
 * \note Head Timer Object is automaitcally removed from the List
 *
 * \note e.g. it is snot needded to stop it
 */
void TimerIrqHandler( void );

/*!
 * \brief Starts and adds the timer object to the list of timer events
 *
 * \param [IN] obj Structure containing the timer object parameters
 */
void TimerStart( TimerEvent_t *obj );

/*!
 * \brief Stops and removes the timer object from the list of timer events
 *
 * \param [IN] obj Structure containing the timer object parameters
 */
void TimerStop( TimerEvent_t *obj );

/*!
 * \brief Resets the timer object
 *
 * \param [IN] obj Structure containing the timer object parameters
 */
void TimerReset( TimerEvent_t *obj );

/*!
 * \brief Set timer new timeout value
 *
 * \param [IN] obj   Structure containing the timer object parameters
 * \param [IN] value New timer timeout value
 */
void TimerSetValue( TimerEvent_t *obj, uint32_t value );


/*!
 * \brief Read the current time
 *
 * \retval returns current time in ms
 */
TimerTime_t TimerGetCurrentTime( void );

/*!
 * \brief Read the RTC time
 *
 * \retval returns RTC time in ms
 */
TimerTime_t RtcGetTimerValue( void );

/*!
 * \brief Return the Time elapsed since a fix moment in Time
 *
 * \param [IN] savedTime    fix moment in Time
 * \retval time             returns elapsed time in ms
 */
TimerTime_t TimerGetElapsedTime( TimerTime_t savedTime );

/*!
 * \brief Computes the temperature compensation for a period of time on a
 *        specific temperature.
 *
 * \param [IN] period Time period to compensate
 * \param [IN] temperature Current temperature
 *
 * \retval Compensated time period
 */
TimerTime_t TimerTempCompensation( TimerTime_t period, float temperature );
#ifdef __cplusplus
}
#endif

#endif /* __TIMESERVER_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
