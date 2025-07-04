/*
    Servo.cpp - Functions for controlling the servo that the
    Lidar sensor is mounted on.

    Copyright (c) 2025 Igor Mikolic-Torreira.  All right reserved.

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




/*
    Inspired by, and had its origins in, the Adafruit library for the LSM303DLHC,
    but it has been heavily modified.
*/



#include "Servo.h"

#include <math.h>

#include "Clock.h"
#include "I2c.h"

#include "DebugUtils.hpp"





// Extend the Servo namespace with private functions

namespace Servo
{

    constexpr std::uint8_t kServoI2cAddress      = 0x40;


    constexpr std::uint8_t kPCA9685_SubAdr1      = 0x02;
    constexpr std::uint8_t kPCA9685_SubAdr2      = 0x03;
    constexpr std::uint8_t kPCA9685_SubAdr3      = 0x04;

    constexpr std::uint8_t kPCA9685_Mode1        = 0x0;
    constexpr std::uint8_t kPCA9685_Prescale     = 0xFE;

    constexpr std::uint8_t kLed_On_L             = 0x06;
    constexpr std::uint8_t kLed_On_H             = 0x07;
    constexpr std::uint8_t kLed_Off_L            = 0x08;
    constexpr std::uint8_t kLed_Off_H            = 0x09;

    constexpr std::uint8_t kAllLeds_On_L         = 0xFA;
    constexpr std::uint8_t kAllLeds_On_H         = 0xFB;
    constexpr std::uint8_t kAllLeds_Off_L        = 0xFC;
    constexpr std::uint8_t kAllLeds_Off_H        = 0xFD;


    // Servo pin assignments
    constexpr std::uint8_t  kRangeSensorServoPin = 0;


    std::uint8_t readByte( std::uint8_t addr );
    void writeByte( std::uint8_t addr, std::uint8_t d );



    std::uint8_t readByte( std::uint8_t addr )
    {
        std::uint8_t temp;
        I2c::read( kServoI2cAddress, addr, 1, &temp );
        return temp;
    }


    void writeByte( std::uint8_t addr, std::uint8_t d )
    {
        I2c::write( kServoI2cAddress, addr, d );
    }


    int mCurrentAngle;

    std::uint16_t convertToPulseLenFromDegreesRelative( std::int8_t degrees );

};



void Servo::init()
{
    reset();
}



void Servo::reset()
{
    writeByte( kPCA9685_Mode1, 0x0 );
    mCurrentAngle = 0;
    setPWMFreq( 50 );                   // Servo wants a pulse every 20ms, = 50 Hz updates
    slew( 0 );
    debugM( "Servo initialized to 0" );
}


void Servo::disconnect()
{
    setPWMFreq( 0 );
}

void Servo::setPWMFreq( float freq )
{
    // Calculate the appropriate prescaler
    float prescaleValue = 25000000;
    prescaleValue /= 4096;
    prescaleValue /= freq;
    prescaleValue -= 1;
    std::uint8_t prescaler = floor( prescaleValue + 0.5 );

    std::uint8_t originalMode = readByte( kPCA9685_Mode1 );
    std::uint8_t sleepMode = ( originalMode & 0x7F ) | 0x10;

    // Set the PCA9685 to sleep so we can set the prescaler
    writeByte( kPCA9685_Mode1, sleepMode );
    writeByte( kPCA9685_Prescale, prescaler );

    // Restore the original mode
    writeByte( kPCA9685_Mode1, originalMode );
    Clock::delayMilliseconds( 5 );

    // Set the MODE1 registor to enable auto-increment
    writeByte( kPCA9685_Mode1, originalMode | 0xa1 );
    Clock::delayMilliseconds( 100 );
}



void Servo::setPWM( uint16_t on, uint16_t off )
{
    std::uint8_t data[4];

    data[0] = on;
    data[1] = on >> 8;
    data[2] = off;
    data[3] = off >> 8;

    I2c::write( kServoI2cAddress, ( kLed_On_L + 4*kRangeSensorServoPin ), data, 4 );
}









// cppcheck-suppress unusedFunction
int Servo::getCurrentAngle()
{
    // Negate the angles to reverse direction,  
    // positive angles are to the right to match compass angles
    return -mCurrentAngle;
}




int Servo::slew( int angleDegrees )
{
    // Protect against over slewing of the radar
    if ( angleDegrees > 90 )
    {
//        angleDegrees = 90;
    }
    if ( angleDegrees < -90 )
    {
//        angleDegrees = -90;
    }

    // Negate the angles to reverse direction:  positive angles are to the right
    // to match compass angles
    mCurrentAngle = -angleDegrees;

    std::uint16_t pulseLen = convertToPulseLenFromDegreesRelative( mCurrentAngle );
    debugV( mCurrentAngle, pulseLen );
    setPWM( 0, pulseLen );

    return mCurrentAngle;
}




std::uint16_t Servo::convertToPulseLenFromDegreesRelative( int8_t degrees )
{
#define SERVO_F_S3003   1

#if SERVO_HPS_2018
/*
 *    -90 = 90
 *      0 = 315
 *     90 = 544
 * 
 *      pluseLen = (227/90) * degrees + 317
 * 
 *
 */

    std::int16_t pulse = (227.0/90.0) * degrees  + 315;
    return static_cast<std::uint16_t>( pulse );
#endif  // SERVO_HPS_2018

#if SERVO_F_S3003
/*
 *    -90 = 
 *      0 = 290
 *     90 = 530
 * 
 *      pluseLen = (8/3) * degrees + 290
 * 
 *
 */

    std::int16_t pulse = (8.0/3.0) * degrees  + 290;
    return static_cast<std::uint16_t>( pulse );
#endif  // SERVO_F_S3003

}





