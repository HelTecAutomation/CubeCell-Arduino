 /*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: Generic lora driver implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis, Gregory Cristian and Wael Guibene
*/
/******************************************************************************
  * @file    timeserver.c
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    01-June-2017
  * @brief   Time server infrastructure
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

/* Includes ------------------------------------------------------------------*/
#include <time.h>
#include "hw.h"
#include "timeServer.h"
#include "port.h"

/*!
 * safely execute call back
 */
#define exec_cb( _callback_ )   \
do                              \
{                               \
    if( _callback_ == NULL )    \
    {                           \
        while(1);               \
    }                           \
    else                        \
    {                           \
        _callback_( );          \
    }                           \
} while(0);                   

#define ROUND_TO_IDLE_AFTER_RTC   5000
#define ROUND_TO_IDLE_AFTER_UART  50000
/* MCU Wake Up Time */
#define MIN_ALARM_DELAY               3 /* in ticks */
#define MIN_ALARM_DELAY_TIME			(MIN_ALARM_DELAY)  /* in ms*/

#define RTC_MS_PER_SEC 1000/32768
#define RTC_COUNTS_OVERFLOW_VAL 4294967296

static uint32 RTC_last_count = 0;
static uint64_t RTC_counts = 0;
static TimerTime_t g_systime_ref = 0;
static TimerTime_t RtcTimerContext;
static TimerTime_t timerContext;

/*!
 * Timers list head pointer
 */
static TimerEvent_t *TimerListHead = NULL;

/*!
 * \brief Adds or replace the head timer of the list.
 *
 * \remark The list is automatically sorted. The list head always contains the
 *     next timer to expire.
 *
 * \param [IN]  obj Timer object to be become the new head
 * \param [IN]  remainingTime Remaining time of the previous head to be replaced
 */
static void TimerInsertNewHeadTimer( TimerEvent_t *obj );

/*!
 * \brief Adds a timer to the list.
 *
 * \remark The list is automatically sorted. The list head always contains the
 *     next timer to expire.
 *
 * \param [IN]  obj Timer object to be added to the list
 * \param [IN]  remainingTime Remaining time of the running head after which the object may be added
 */
static void TimerInsertTimer( TimerEvent_t *obj );

/*!
 * \brief Sets a timeout with the duration "timestamp"
 * 
 * \param [IN] timestamp Delay duration
 */
static void TimerSetTimeout( TimerEvent_t *obj );

/*!
 * \brief Check if the Object to be added is not already in the list
 * 
 * \param [IN] timestamp Delay duration
 * \retval true (the object is already in the list) or false  
 */
static bool TimerExists( TimerEvent_t *obj );


static void RtcSetTimeout(TimerEvent_t *obj);
static void TimerSetTimeout( TimerEvent_t *obj );
static void RtcStartWakeUpAlarm(uint32_t timeout);
static void RtcStopTimeout();
static TimerTime_t RtcGetTimerContext( void );
static TimerTime_t RtcSetTimerContext( void );
static TimerTime_t RtcGetElapsedTime( void );


static void TimeStampsUpdate()
{    
    TimerTime_t old =  RtcGetTimerContext(); 
    TimerTime_t now =  RtcSetTimerContext(); 
    uint32_t DeltaContext = (uint32_t)(now - old);
    
    TimerEvent_t* cur = TimerListHead;
    while(cur) {
        if (cur->Timestamp > DeltaContext)
            cur->Timestamp -= DeltaContext;
        else
            cur->Timestamp = 0 ;
        cur = cur->Next;
    }
}

void TimerInit( TimerEvent_t *obj, void ( *callback )( void ) )
{
  obj->Timestamp = 0;
  obj->ReloadValue = 0;
  obj->IsRunning = false;
  obj->Callback = callback;
  obj->Next = NULL;
}

void TimerStart( TimerEvent_t *obj )
{
    uint32_t elapsedTime = 0;
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();

    if( ( obj == NULL ) || ( TimerExists( obj ) == true ) )
    {
        RHINO_CPU_INTRPT_ENABLE();;
        return;
    }
    obj->Timestamp = obj->ReloadValue;
    obj->IsRunning = false;

    if( TimerListHead == NULL )
    {
        RtcSetTimerContext();
        TimerInsertNewHeadTimer( obj ); // insert a timeout at now+obj->Timestamp
    }
    else 
    {
        elapsedTime = RtcGetElapsedTime();
        obj->Timestamp += elapsedTime;
        if( obj->Timestamp < TimerListHead->Timestamp )
        {
            TimeStampsUpdate();
            obj->Timestamp -= elapsedTime;
            TimerInsertNewHeadTimer( obj);
        }
        else
        {
            TimerInsertTimer( obj);
        }
    }
    RHINO_CPU_INTRPT_ENABLE();
}

static void TimerInsertTimer( TimerEvent_t *obj)
{
  TimerEvent_t* cur = TimerListHead;
  TimerEvent_t* next = TimerListHead->Next;

  while (cur->Next != NULL )
  {  
    if( obj->Timestamp  > next->Timestamp )
    {
        cur = next;
        next = next->Next;
    }
    else
    {
        cur->Next = obj;
        obj->Next = next;
        return;

    }
  }
  cur->Next = obj;
  obj->Next = NULL;
}

static void TimerInsertNewHeadTimer( TimerEvent_t *obj )
{
  TimerEvent_t* cur = TimerListHead;

  if( cur != NULL )
  {
    cur->IsRunning = false;
  }

  obj->Next = cur;
  TimerListHead = obj;
  TimerSetTimeout( TimerListHead );
}

void TimerIrqHandler( void )
{
    TimerEvent_t* cur;
    //update timer context for callbacks

    TimeStampsUpdate();

    int n=0;
    // remove all the expired object from the list
    while( ( TimerListHead != NULL ) && ( (TimerListHead->Timestamp < RtcGetElapsedTime(  )) || TimerListHead->Timestamp==0  ))
    {
        n++;
        cur = TimerListHead;
        TimerListHead = TimerListHead->Next;
        exec_cb( cur->Callback );
        //update timestamps after callbacks
        TimeStampsUpdate();
    }

    if( TimerListHead != NULL )
    {
        if( TimerListHead->IsRunning != true || n==0)
        {
            TimerSetTimeout( TimerListHead );
        }
    }
}

void TimerStop( TimerEvent_t *obj ) 
{
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();

    TimerEvent_t* prev = TimerListHead;
    TimerEvent_t* cur = TimerListHead;

    // List is empty or the Obj to stop does not exist 
    if( ( TimerListHead == NULL ) || ( obj == NULL ) )
    {
        RHINO_CPU_INTRPT_ENABLE();
        return;
    }

    if( TimerListHead == obj ) // Stop the Head                  
    {
        if( TimerListHead->IsRunning == true ) // The head is already running 
        {    
            if( TimerListHead->Next != NULL )
            {
                TimerListHead->IsRunning = false;
                TimerListHead = TimerListHead->Next;

                //update timestamps after stopping timer
                TimeStampsUpdate();
                TimerSetTimeout( TimerListHead );
            }
            else
            {
                RtcStopTimeout( );
                TimerListHead = NULL;
            }
        }
        else // Stop the head before it is started
        {   
            if( TimerListHead->Next != NULL )   
            {
                TimerListHead = TimerListHead->Next;
            }
            else
            {
                TimerListHead = NULL;
            }
        }
    }
    else // Stop an object within the list
    {      
        while( cur != NULL )
        {
            if( cur == obj )
            {
                if( cur->Next != NULL )
                {
                    cur = cur->Next;
                    prev->Next = cur;
                }
                else
                {
                    cur = NULL;
                    prev->Next = cur;
                }
                break;
            }
            else
            {
                prev = cur;
                cur = cur->Next;
            }
        }   
    }

    obj->IsRunning = false;
    RHINO_CPU_INTRPT_ENABLE();
}  
  
static bool TimerExists( TimerEvent_t *obj )
{
    TimerEvent_t* cur = TimerListHead;

    while( cur != NULL )
    {
        if( cur == obj )
        {
            return true;
        }
        cur = cur->Next;
    }
    return false;  
}

void TimerReset( TimerEvent_t *obj )
{
  TimerStop( obj );
  TimerStart( obj );
}

void TimerSetValue( TimerEvent_t *obj, uint32_t value )
{
    TimerStop( obj );
    if(value<MIN_ALARM_DELAY_TIME)
        value=MIN_ALARM_DELAY_TIME;
    obj->Timestamp = value;
    obj->ReloadValue = value;
}

TimerTime_t RtcGetTimerValue( void )
{
    uint8_t state = CyEnterCriticalSection();
    uint32 current_count = CySysTimerGetCount(2);
    RTC_counts += (((RTC_COUNTS_OVERFLOW_VAL + current_count - RTC_last_count) % RTC_COUNTS_OVERFLOW_VAL));
    RTC_last_count = current_count; 
    CyExitCriticalSection(state);
    timerContext =  RTC_counts*RTC_MS_PER_SEC;
    return timerContext;
}
TimerTime_t TimerGetCurrentTime( void )
{
    return RtcGetTimerValue( ) + g_systime_ref;
}

TimerTime_t TimerGetElapsedTime( TimerTime_t savedTime )
{
    return (TimerGetCurrentTime() - savedTime);
}


TimerTime_t TimerTempCompensation( TimerTime_t period, float temperature )
{
    (void)temperature;
    
    return period;
}



void TimerSetSysTime( TimerSysTime_t sysTime )
{
    TimerTime_t cur_time = RtcGetTimerValue( );
    TimerTime_t set_time = (TimerTime_t)sysTime.Seconds*1000 + sysTime.SubSeconds;
    
    g_systime_ref = set_time - cur_time;
}

TimerSysTime_t TimerGetSysTime( void )
{
    TimerSysTime_t sysTime = { 0 };
    TimerTime_t curTime = TimerGetCurrentTime();

    sysTime.Seconds = (uint32_t)(curTime/1000);
    sysTime.SubSeconds = (uint16_t)(curTime%1000);

    return sysTime;
}

TimerSysTime_t SysTimeGetMcuTime( void )
{
    TimerTime_t curTime = RtcGetTimerValue( );
    TimerSysTime_t mcuTime = { 0 };
    mcuTime.Seconds = (uint32_t)(curTime/1000);
    mcuTime.SubSeconds = (uint16_t)(curTime%1000);
    return mcuTime;
}

uint32_t SysTimeToMs( TimerSysTime_t sysTime )
{
    TimerTime_t sysTimer = (TimerTime_t)sysTime.Seconds*1000 + sysTime.SubSeconds;
    return sysTimer - g_systime_ref;
}

TimerSysTime_t SysTimeFromMs( uint32_t timeMs )
{
    TimerSysTime_t sysTime = { 0 };
    TimerTime_t curTime = (TimerTime_t)timeMs - g_systime_ref;

    sysTime.Seconds = (uint32_t)(curTime/1000);
    sysTime.SubSeconds = (uint16_t)(curTime%1000);

    return sysTime;
}




//RTC functions
static void RtcSetTimeout(TimerEvent_t *obj)
{
    obj->IsRunning = true;
    if ( obj->Timestamp < (RtcGetElapsedTime()+MIN_ALARM_DELAY) )
    {
        obj->Timestamp = RtcGetElapsedTime()+MIN_ALARM_DELAY;
    };
    RtcStartWakeUpAlarm(obj->Timestamp);
}

static void TimerSetTimeout( TimerEvent_t *obj )
{
    obj->IsRunning = true;
    obj->IsRunning = true;
    if ( obj->Timestamp < (RtcGetElapsedTime()+MIN_ALARM_DELAY) )
    {
        obj->Timestamp = RtcGetElapsedTime()+MIN_ALARM_DELAY;
    }
    RtcStartWakeUpAlarm(obj->Timestamp);
}

static void RtcStartWakeUpAlarm(uint32_t timeout)
{
    Asr_Timer_RegisterAlarmCallback(TimerIrqHandler);
    if (timeout <= MIN_ALARM_DELAY_TIME)
        timeout = MIN_ALARM_DELAY_TIME;
    Asr_SetTimeout(timeout);
}

static void RtcStopTimeout()
{
    Asr_Timer_Disable();
}

static TimerTime_t RtcGetTimerContext( void )
{
    return RtcTimerContext;
}

static TimerTime_t RtcSetTimerContext( void )
{
    RtcTimerContext = RtcGetTimerValue();
    
    return RtcTimerContext;
}

static TimerTime_t RtcGetElapsedTime( void )
{
    return (RtcGetTimerValue() - RtcTimerContext); 
}


static void RTC_Update_ASR(void)
{
    uint32 current_count = CySysTimerGetCount(2);
    RTC_counts += (RTC_COUNTS_OVERFLOW_VAL + current_count - RTC_last_count) % RTC_COUNTS_OVERFLOW_VAL;
    RTC_last_count = current_count;  
}

void RtcInit( void )
{
    CySysTimerDisable(CY_SYS_TIMER2_MASK);
    CySysTimerResetCounters(CY_SYS_TIMER2_RESET);
    CySysTimerSetToggleBit(31);//0~31
    CySysTimerSetInterruptCallback(2, RTC_Update_ASR);
    CySysTimerEnableIsr(2);
    CySysTimerSetMode(2, CY_SYS_TIMER_MODE_INT);
    CySysTimerEnable(CY_SYS_TIMER2_MASK);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
