/*
    PCA9685.cpp - Driver for PCA9685-based PWM IC for CARRTv3.
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


#include "PCA9685.h"




#include <cinttypes>
#include <cmath>

#include "Clock.h"
#include "I2c.h"



namespace PCA9685
{
    // Registers

    const uint8_t kMode1            = 0x00;
    const uint8_t kMode2            = 0x01;
    const uint8_t kSubAdr1          = 0x02;
    const uint8_t kSubAdr2          = 0x03;
    const uint8_t kSubAdr3          = 0x04;
    const uint8_t kPreScale         = 0xFE;

    const uint8_t kLed00OnLo        = 0x06;
    const uint8_t kLed00OnHi        = 0x07;
    const uint8_t kLed00OffLo       = 0x08;
    const uint8_t kLed00OffHi       = 0x09;

    const uint8_t kLed01OnLo        = 0x0a;
    const uint8_t kLed01OnHi        = 0x0b;
    const uint8_t kLed01OffLo       = 0x0c;
    const uint8_t kLed01OffHi       = 0x0d;

    const uint8_t kLed02OnLo        = 0x0e;
    const uint8_t kLed02OnHi        = 0x0f;
    const uint8_t kLed02OffLo       = 0x10;
    const uint8_t kLed02OffHi       = 0x11;

    const uint8_t kLed03OnLo        = 0x12;
    const uint8_t kLed03OnHi        = 0x13;
    const uint8_t kLed03OffLo       = 0x14;
    const uint8_t kLed03OffHi       = 0x15;

    const uint8_t kLed04OnLo        = 0x16;
    const uint8_t kLed04OnHi        = 0x17;
    const uint8_t kLed04OffLo       = 0x18;
    const uint8_t kLed04OffHi       = 0x19;

    const uint8_t kLed05OnLo        = 0x1a;
    const uint8_t kLed05OnHi        = 0x1b;
    const uint8_t kLed05OffLo       = 0x1c;
    const uint8_t kLed05OffHi       = 0x1d;

    const uint8_t kLed06OnLo        = 0x1e;
    const uint8_t kLed06OnHi        = 0x1f;
    const uint8_t kLed06OffLo       = 0x20;
    const uint8_t kLed06OffHi       = 0x21;

    const uint8_t kLed07OnLo        = 0x22;
    const uint8_t kLed07OnHi        = 0x23;
    const uint8_t kLed07OffLo       = 0x24;
    const uint8_t kLed07OffHi       = 0x25;

    const uint8_t kLed08OnLo        = 0x26;
    const uint8_t kLed08OnHi        = 0x27;
    const uint8_t kLed08OffLo       = 0x28;
    const uint8_t kLed08OffHi       = 0x29;

    const uint8_t kLed09OnLo        = 0x2a;
    const uint8_t kLed09OnHi        = 0x2b;
    const uint8_t kLed09OffLo       = 0x2c;
    const uint8_t kLed09OffHi       = 0x2d;

    const uint8_t kLed10OnLo        = 0x2e;
    const uint8_t kLed10OnHi        = 0x2f;
    const uint8_t kLed10OffLo       = 0x30;
    const uint8_t kLed10OffHi       = 0x31;

    const uint8_t kLed11OnLo        = 0x32;
    const uint8_t kLed11OnHi        = 0x33;
    const uint8_t kLed11OffLo       = 0x34;
    const uint8_t kLed11OffHi       = 0x35;

    const uint8_t kLed12OnLo        = 0x36;
    const uint8_t kLed12OnHi        = 0x37;
    const uint8_t kLed12OffLo       = 0x38;
    const uint8_t kLed12OffHi       = 0x39;

    const uint8_t kLed13OnLo        = 0x3a;
    const uint8_t kLed13OnHi        = 0x3b;
    const uint8_t kLed13OffLo       = 0x3c;
    const uint8_t kLed13OffHi       = 0x3d;

    const uint8_t kLed14OnLo        = 0x3e;
    const uint8_t kLed14OnHi        = 0x3f;
    const uint8_t kLed14OffLo       = 0x40;
    const uint8_t kLed14OffHi       = 0x41;

    const uint8_t kLed15OnLo        = 0x42;
    const uint8_t kLed15OnHi        = 0x43;
    const uint8_t kLed15OffLo       = 0x44;
    const uint8_t kLed15OffHi       = 0x45;


    const uint8_t kAllLedOnLo       = 0xFA;
    const uint8_t kAllLedOnHi       = 0xFB;
    const uint8_t kAllLedOffLo      = 0xFC;
    const uint8_t kAllLedOffHi      = 0xFD;


    // Mode 1 bits

    const uint8_t kMode1_Restart        = 7;
    const uint8_t kMode1_ExtClk         = 6;
    const uint8_t kMode1_AutoIncr       = 5;
    const uint8_t kMode1_Sleep          = 4;
    const uint8_t kMode1_Sub1           = 3;
    const uint8_t kMode1_Sub2           = 2;
    const uint8_t kMode1_Sub3           = 1;
    const uint8_t kMode1_AllCallBit     = 0;

    // Other

    const uint16_t kFullOn          = 0x1000;
    const uint16_t kFullOff         = 0x1000;

};



int PCA9685::reset( std::uint8_t address )
{
    uint8_t desiredMode = (1 << kMode1_AutoIncr) | (1 << kMode1_AllCallBit);
    int err = I2c::write( address, kMode1, desiredMode );

    // Oscillator needs time to settle
    Clock::delayMicroseconds( 500 );

    if ( !err )
    {
        err = setAllPwm( address, 0, kFullOff );
    }

    return err;
}





int PCA9685::init( std::uint8_t address )
{
    return reset( address );
}




int PCA9685::setAllPwm( std::uint8_t address, std::uint16_t on, std::uint16_t off )
{
    uint8_t data[4];

    data[0] = on;
    data[1] = on >> 8;
    data[2] = off;
    data[3] = off >> 8;

    int err = I2c::write( address, kAllLedOnLo, data, 4 );

    return err;
}




int PCA9685::setPwm( std::uint8_t address, uint8_t pinNbr, std::uint16_t on, std::uint16_t off )
{
    uint8_t data[4];

    data[0] = on;
    data[1] = on >> 8;
    data[2] = off;
    data[3] = off >> 8;

    int err = I2c::write( address, (kLed00OnLo + 4*pinNbr), data, 4 );

    return err;
}





int PCA9685::setOff( std::uint8_t address, uint8_t pinNbr )
{
    return setPwm( address, pinNbr, 0, kFullOff );
}




int PCA9685::setOn( std::uint8_t address, uint8_t pinNbr )
{
    return setPwm( address, pinNbr, kFullOn, 0 );
}




int PCA9685::setPwmDutyOnCycle( std::uint8_t address, uint8_t pinNbr, float onRatio )
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


    return setPwm( address, pinNbr, on, off );
}





int PCA9685::setPWMFreq( std::uint8_t address, float freq )
{
    // Calculate the appropriate prescaler
    float prescaleValue = 25000000;
    prescaleValue /= 4096;
    prescaleValue /= freq;
    prescaleValue -= 1;
    uint8_t prescaler = std::floor( prescaleValue + 0.5 );

    uint8_t originalMode;
    int err = I2c::read( address, kMode1, &originalMode );
    if ( !err )
    {
        uint8_t sleepMode = ( originalMode & 0x7F ) | 0x10;

        // Set the PCA9685 to sleep so we can set the prescaler
        err = I2c::write( address, kMode1, sleepMode );
        if ( !err )
        {
            err = I2c::write( address, kPreScale, prescaler );
        }

        // Restore the original mode
        I2c::write( address, kMode1, originalMode );

        // Oscillator needs time to settle
        Clock::delayMicroseconds( 500 );
    }

    // Set the MODE1 registor to clear restart
    err = I2c::write( address, kMode1, static_cast<uint8_t>( originalMode | (1 << kMode1_Restart) ) );

    return err;
}

