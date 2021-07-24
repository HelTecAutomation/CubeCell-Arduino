/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010, LeafLabs, LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/


#include "CubeCellServoTimers.h"
#include "Arduino.h"
#include "math.h"


#define CYCLES_PER_MICROSECOND 10000
#define MAX_OVERFLOW    ((1 << 16) - 1)
#define CYC_MSEC        (1000 * CYCLES_PER_MICROSECOND)
#define TAU_MSEC        22.1
#define TAU_USEC        (TAU_MSEC * 1000)
#define TAU_CYC         (TAU_MSEC * CYC_MSEC)
#define SERVO_PRESCALER (TAU_CYC / MAX_OVERFLOW + 1)
#define SERVO_OVERFLOW  ((uint16_t)round((double)TAU_CYC / SERVO_PRESCALER))

// Unit conversions
#define US_TO_COMPARE(us) ((uint16_t)map((us), 0, TAU_USEC, 0, SERVO_OVERFLOW))
#define COMPARE_TO_US(c)  ((uint32_t)map((c), 0, SERVO_OVERFLOW, 0, TAU_USEC))
#define ANGLE_TO_US(a)    ((uint16_t)(map((a), this->minAngle, this->maxAngle, \
                                        this->minPW, this->maxPW)))
#define US_TO_ANGLE(us)   ((int16_t)(map((us), this->minPW, this->maxPW,  \
                                       this->minAngle, this->maxAngle)))

Servo::Servo() 
{
    this->resetFields();
}

bool Servo::attach(uint8_t pin, uint16_t minPW, uint16_t maxPW, int16_t minAngle, int16_t maxAngle)
{
  // put your setup code here, to run once:

  /*PWM CLK frequency default is 12M;
   *PWM CLK frequency value:
   *PWM_CLK_FREQ_48M
   *PWM_CLK_FREQ_24M
   *PWM_CLK_FREQ_16M
   *PWM_CLK_FREQ_12M
   *PWM_CLK_FREQ_8M
   *PWM_CLK_FREQ_6M
   *PWM_CLK_FREQ_4M
   *PWM_CLK_FREQ_3M
   *PWM_CLK_FREQ_2M
   *PWM_CLK_FREQ_1M
   */
  setPWM_Frequency(PWM_CLK_FREQ_3M);

  //pwm period can be 0xFF~0xFFFF, default is 0xFFFF
  setPWM_ComparePeriod(0xFFFF);
  
  // pinMode(PWM1,OUTPUT);

    pinMode(pin, OUTPUT);

    if (this->attached()) {
        this->detach();
    }

    this->pin = pin;
    this->minPW = (minPW);
    this->maxPW = (maxPW );
    this->minAngle = minAngle;
    this->maxAngle = maxAngle;

    return true;
}

bool Servo::detach() {
    if (!this->attached()) {
        return false;
    }


    this->resetFields();

    return true;
}

void Servo::write(int degrees) {
    degrees = constrain(degrees, this->minAngle, this->maxAngle);
    this->writeMicroseconds(ANGLE_TO_US(degrees));
}

int Servo::read() const {
    int a = US_TO_ANGLE(this->readMicroseconds());
    // map() round-trips in a weird way we mostly correct for here;
    // the round-trip is still sometimes off-by-one for write(1) and
    // write(179).
    return a == this->minAngle || a == this->maxAngle ? a : a + 1;
}

void Servo::writeMicroseconds(uint16_t pulseWidth) {
    if (!this->attached()) {
        // ASSERT(0);
        return;
    } 
    pulseWidth = constrain(pulseWidth, this->minPW, this->maxPW);
    analogWrite(this->pin, US_TO_COMPARE(pulseWidth));
}

uint16_t Servo::readMicroseconds() const {
    if (!this->attached()) {
        // ASSERT(0);
        return 0;
    }
    uint16_t compare =0;
    if(this->pin ==GPIO3)
    {
        compare =PWM2_ReadCompare();
    }
    else if(this->pin ==GPIO2)
    {
        compare =PWM1_ReadCompare();
    }
    else
    {
        return 0xFFFF; //error
    }
    
    return COMPARE_TO_US(compare);
}

void Servo::resetFields(void) {
    this->pin = NOT_ATTACHED;
    this->minAngle = MIN_ANGLE;
    this->maxAngle = MAX_ANGLE;
    this->minPW = MIN_PULSE_WIDTH;
    this->maxPW = MAX_PULSE_WIDTH;
}


