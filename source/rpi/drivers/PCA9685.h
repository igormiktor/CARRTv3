/*
    PCA9685.h - Driver for PCA9685-based PWM IC for CARRTv3.
    Copyright (c) 2026 Igor Mikolic-Torreira.  All right reserved.

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

#include <cstdint>

namespace PCA9685
{

    // PCA9685 I2C address
    constexpr std::uint8_t kPCA9685_I2cAddress{ 0x40 };

    void init( std::uint8_t address );

    void reset( std::uint8_t address );

    void setPwmFreq( std::uint8_t address, float freq );

    void setAllPwm( std::uint8_t address, std::uint16_t on, std::uint16_t off );

    void setPwm( std::uint8_t address, std::uint8_t pinNbr, std::uint16_t on,
                 std::uint16_t off );

    void setPwmDutyOnCycle( std::uint8_t address, std::uint8_t pinNbr,
                            float onRatio );

    void setOff( std::uint8_t address, std::uint8_t pinNbr );

    void setOn( std::uint8_t address, std::uint8_t pinNbr );

    void setPin( std::uint8_t address, std::uint8_t pinNbr, bool val );

};    // namespace PCA9685

#endif    // PCA9685_h
