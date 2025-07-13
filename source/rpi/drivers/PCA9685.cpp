/*
    PCA9685.cpp - Driver for PCA9685-based PWM IC for CARRTv3.
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


#include "PCA9685.h"




#include <cstdint>
#include <cmath>

#include "Clock.h"
#include "I2c.h"



namespace PCA9685
{
    // Registers

    const std::uint8_t kMode1               = 0x00;
    const std::uint8_t kMode2               = 0x01;
    const std::uint8_t kSubAdr1             = 0x02;
    const std::uint8_t kSubAdr2             = 0x03;
    const std::uint8_t kSubAdr3             = 0x04;
    const std::uint8_t kPreScale            = 0xFE;

    const std::uint8_t kPwmPin00OnLo        = 0x06;
    const std::uint8_t kPwmPin00OnHi        = 0x07;
    const std::uint8_t kPwmPin00OffLo       = 0x08;
    const std::uint8_t kPwmPin00OffHi       = 0x09;

    const std::uint8_t kPwmPin01OnLo        = 0x0a;
    const std::uint8_t kPwmPin01OnHi        = 0x0b;
    const std::uint8_t kPwmPin01OffLo       = 0x0c;
    const std::uint8_t kPwmPin01OffHi       = 0x0d;

    const std::uint8_t kPwmPin02OnLo        = 0x0e;
    const std::uint8_t kPwmPin02OnHi        = 0x0f;
    const std::uint8_t kPwmPin02OffLo       = 0x10;
    const std::uint8_t kPwmPin02OffHi       = 0x11;

    const std::uint8_t kPwmPin03OnLo        = 0x12;
    const std::uint8_t kPwmPin03OnHi        = 0x13;
    const std::uint8_t kPwmPin03OffLo       = 0x14;
    const std::uint8_t kPwmPin03OffHi       = 0x15;

    const std::uint8_t kPwmPin04OnLo        = 0x16;
    const std::uint8_t kPwmPin04OnHi        = 0x17;
    const std::uint8_t kPwmPin04OffLo       = 0x18;
    const std::uint8_t kPwmPin04OffHi       = 0x19;

    const std::uint8_t kPwmPin05OnLo        = 0x1a;
    const std::uint8_t kPwmPin05OnHi        = 0x1b;
    const std::uint8_t kPwmPin05OffLo       = 0x1c;
    const std::uint8_t kPwmPin05OffHi       = 0x1d;

    const std::uint8_t kPwmPin06OnLo        = 0x1e;
    const std::uint8_t kPwmPin06OnHi        = 0x1f;
    const std::uint8_t kPwmPin06OffLo       = 0x20;
    const std::uint8_t kPwmPin06OffHi       = 0x21;

    const std::uint8_t kPwmPin07OnLo        = 0x22;
    const std::uint8_t kPwmPin07OnHi        = 0x23;
    const std::uint8_t kPwmPin07OffLo       = 0x24;
    const std::uint8_t kPwmPin07OffHi       = 0x25;

    const std::uint8_t kPwmPin08OnLo        = 0x26;
    const std::uint8_t kPwmPin08OnHi        = 0x27;
    const std::uint8_t kPwmPin08OffLo       = 0x28;
    const std::uint8_t kPwmPin08OffHi       = 0x29;

    const std::uint8_t kPwmPin09OnLo        = 0x2a;
    const std::uint8_t kPwmPin09OnHi        = 0x2b;
    const std::uint8_t kPwmPin09OffLo       = 0x2c;
    const std::uint8_t kPwmPin09OffHi       = 0x2d;

    const std::uint8_t kPwmPin10OnLo        = 0x2e;
    const std::uint8_t kPwmPin10OnHi        = 0x2f;
    const std::uint8_t kPwmPin10OffLo       = 0x30;
    const std::uint8_t kPwmPin10OffHi       = 0x31;

    const std::uint8_t kPwmPin11OnLo        = 0x32;
    const std::uint8_t kPwmPin11OnHi        = 0x33;
    const std::uint8_t kPwmPin11OffLo       = 0x34;
    const std::uint8_t kPwmPin11OffHi       = 0x35;

    const std::uint8_t kPwmPin12OnLo        = 0x36;
    const std::uint8_t kPwmPin12OnHi        = 0x37;
    const std::uint8_t kPwmPin12OffLo       = 0x38;
    const std::uint8_t kPwmPin12OffHi       = 0x39;

    const std::uint8_t kPwmPin13OnLo        = 0x3a;
    const std::uint8_t kPwmPin13OnHi        = 0x3b;
    const std::uint8_t kPwmPin13OffLo       = 0x3c;
    const std::uint8_t kPwmPin13OffHi       = 0x3d;

    const std::uint8_t kPwmPin14OnLo        = 0x3e;
    const std::uint8_t kPwmPin14OnHi        = 0x3f;
    const std::uint8_t kPwmPin14OffLo       = 0x40;
    const std::uint8_t kPwmPin14OffHi       = 0x41;

    const std::uint8_t kPwmPin15OnLo        = 0x42;
    const std::uint8_t kPwmPin15OnHi        = 0x43;
    const std::uint8_t kPwmPin15OffLo       = 0x44;
    const std::uint8_t kPwmPin15OffHi       = 0x45;


    const std::uint8_t kAllLedOnLo          = 0xFA;
    const std::uint8_t kAllLedOnHi          = 0xFB;
    const std::uint8_t kAllLedOffLo         = 0xFC;
    const std::uint8_t kAllLedOffHi         = 0xFD;


    // Mode 1 bits

    const std::uint8_t kMode1_Restart        = 7;
    const std::uint8_t kMode1_ExtClk         = 6;
    const std::uint8_t kMode1_AutoIncr       = 5;
    const std::uint8_t kMode1_Sleep          = 4;
    const std::uint8_t kMode1_Sub1           = 3;
    const std::uint8_t kMode1_Sub2           = 2;
    const std::uint8_t kMode1_Sub3           = 1;
    const std::uint8_t kMode1_AllCallBit     = 0;

    // Other

    const uint16_t kFullOn          = 0x1000;
    const uint16_t kFullOff         = 0x1000;

};



void PCA9685::reset( std::uint8_t address )
{
    std::uint8_t desiredMode = (1 << kMode1_AutoIncr) | (1 << kMode1_AllCallBit);
    I2c::write( address, kMode1, desiredMode );

    // Oscillator needs time to settle
    Clock::delayMicroseconds( 500 );

    setAllPwm( address, 0, kFullOff );
}



void PCA9685::init( std::uint8_t address )
{
    reset( address );
}




void PCA9685::setAllPwm( std::uint8_t address, std::uint16_t on, std::uint16_t off )
{
    std::uint8_t data[4];

    data[0] = on & 0xFF;
    data[1] = on >> 8;
    data[2] = off & 0xFF;
    data[3] = off >> 8;

    I2c::write( address, kAllLedOnLo, data, 4 );
}




void PCA9685::setPwm( std::uint8_t address, std::uint8_t pinNbr, std::uint16_t on, std::uint16_t off )
{
    std::uint8_t data[4];

    data[0] = on & 0xFF;
    data[1] = on >> 8;
    data[2] = off & 0xFF;
    data[3] = off >> 8;

    I2c::write( address, (kPwmPin00OnLo + 4*pinNbr), data, 4 );
}





void PCA9685::setOff( std::uint8_t address, std::uint8_t pinNbr )
{
    setPwm( address, pinNbr, 0, kFullOff );
}




void PCA9685::setOn( std::uint8_t address, std::uint8_t pinNbr )
{
    setPwm( address, pinNbr, kFullOn, 0 );
}




void PCA9685::setPin( std::uint8_t address, std::uint8_t pinNbr, bool value )
{
    if ( value )
    {
        setPwm( address, pinNbr, kFullOn, 0 );
    }
    else
    {
        setPwm( address, pinNbr, 0, kFullOff );
    }
}




void PCA9685::setPwmDutyOnCycle( std::uint8_t address, std::uint8_t pinNbr, float onRatio )
{
    uint16_t on = 0;
    uint16_t off = 0;

    if ( onRatio >= 100 )
    {
       on = kFullOn;
    }
    else if ( onRatio <= 0 )
    {
        off = kFullOff;
    }
    else
    {
        off = static_cast<uint16_t>( 4095 * onRatio + 0.5 );
    }

    setPwm( address, pinNbr, on, off );
}





void PCA9685::setPwmFreq( std::uint8_t address, float freq )
{
    // Calculate the appropriate prescaler
    float prescaleValue = 25000000;
    prescaleValue /= 4096;
    prescaleValue /= freq;
    prescaleValue -= 1;
    std::uint8_t prescaler = std::floor( prescaleValue + 0.5 );

    std::uint8_t originalMode;
    I2c::read( address, kMode1, &originalMode );

    std::uint8_t sleepMode = ( originalMode & 0x7F ) | 0x10;

    // Set the PCA9685 to sleep so we can set the prescaler
    I2c::write( address, kMode1, sleepMode );
    I2c::write( address, kPreScale, prescaler );

    // Restore the original mode
    I2c::write( address, kMode1, originalMode );

    // Oscillator needs time to settle
    Clock::delayMilliseconds( 5 );

    // Set the MODE1 registor to clear restart
    I2c::write( address, kMode1, static_cast<std::uint8_t>( originalMode | (1 << kMode1_Restart) ) );
    Clock::delayMilliseconds( 100 );
}
