/*
  SHT2x - A Humidity Library for Arduino.

  Supported Sensor modules:
    SHT21-Breakout Module - http://www.moderndevice.com/products/sht21-humidity-sensor
    SHT2x-Breakout Module - http://www.misenso.com/products/001

  Created by Christopher Ladden at Modern Device on December 2009.
  
  Modified by www.misenso.com on October 2011:
    - code optimisation
    - compatibility with Arduino 1.0

 * This file is part of Sodaq_SHT2x.
 *
 * Sodaq_SHT2x is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or(at your option) any later version.
 *
 * Sodaq_SHT2x is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Sodaq_SHT2x.  If not, see
 * <http://www.gnu.org/licenses/>.
 */


#ifndef SHT2X_H
#define SHT2X_H

#include <stdint.h>

class SHT2xClass
{
  private:
    uint16_t readSensor(uint8_t command);

  public:
    float GetHumidity(void);
    float GetTemperature(void);
    float GetDewPoint(void);
};

extern SHT2xClass SHT2x;

#endif
