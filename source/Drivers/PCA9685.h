/*
    PCA9685.h - Driver for PCA9685-based PWM IC for CARRTv3.
    Copyright (c) 2017 Igor Mikolic-Torreira.  All right reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/




#ifndef PCA9685_h
#define PCA9685_h

#include <cinttypes>



namespace PCA9685
{

    int init( std::uint8_t address );

    int reset( std::uint8_t address );


    int setPWMFreq( std::uint8_t address, float freq );


    int setAllPwm( std::uint8_t address, std::uint16_t on, std::uint16_t off );

    int setPwm( std::uint8_t address, uint8_t pinNbr, std::uint16_t on, std::uint16_t off );

    int setPwmDutyOnCycle( std::uint8_t address, uint8_t pinNbr, float onRatio );

    int setOff( std::uint8_t address, uint8_t pinNbr );

    int setOn( std::uint8_t address, uint8_t pinNbr );

};


#endif  // PCA9685_h
