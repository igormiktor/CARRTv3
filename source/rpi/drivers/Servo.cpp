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

#include "Clock.h"
#include "DebugUtils.hpp"
#include "PCA9685.h"





// Extend the Servo namespace with private functions

namespace Servo
{

    // Servo I2C address (actual the PCA9685 address)
    constexpr std::uint8_t kLidarServoI2cAddress    =  PCA9685::kPCA9685_I2cAddress;
    // Servo pin assignments
    constexpr std::uint8_t kLidarServoPin           = 0;

    int mCurrentAngle;          // +deg -> clockwise; -deg -> counterclockwise; 0 deg -> straight ahead
    std::uint16_t mPulseLen;

    std::uint16_t convertToPulseLenFromDegreesRelative( int degrees );

};




void Servo::init( bool pulseMode )
{
    reset( pulseMode );
}



void Servo::reset( bool pulseMode )
{
    PCA9685::reset( kLidarServoI2cAddress );
    mCurrentAngle = 0;
    PCA9685::setPwmFreq( kLidarServoI2cAddress, 50 );                   // Servo wants a pulse every 20ms, = 50 Hz updates
    debugM( "Servo initialized to:" );
    if ( pulseMode )
    {
        setPulseLen( 340 );             // Generic safe value for servos I play with
    }
    else
    {    
        slew( 0 );
    }    
}



void Servo::disconnect()
{
    setPwmFreq( 0 );
}



void Servo::setPwmFreq( float freq )
{
    PCA9685::setPwmFreq( kLidarServoI2cAddress, freq );
}



void Servo::setPwm( std::uint16_t on, std::uint16_t off )
{
    PCA9685::setPwm( kLidarServoI2cAddress, kLidarServoPin, on, off );
}



void Servo::setPwmDutyOnCycle( float onRatio )
{
    PCA9685::setPwmDutyOnCycle( kLidarServoI2cAddress, kLidarServoPin, onRatio );
}



int Servo::getCurrentAngle()
{
    // Negate the angles to reverse direction,  
    // positive angles are to the right to match compass angles
    return mCurrentAngle;
}



int Servo::slew( int angleDegrees )
{
    // Protect against over slewing of the radar
    if ( angleDegrees > 90 )
    {
        angleDegrees = 90;
    }
    if ( angleDegrees < -90 )
    {
        angleDegrees = -90;
    }

    mCurrentAngle = angleDegrees;

    mPulseLen = convertToPulseLenFromDegreesRelative( mCurrentAngle );
    setPwm( 0, mPulseLen );
    debugV( mCurrentAngle, mPulseLen );

    return mCurrentAngle;
}



std::uint16_t Servo::setPulseLen( std::uint16_t pulseLen )
{
    mPulseLen = pulseLen;
    setPwm( 0, mPulseLen );
    // Only call this function for testing & calibration, so leave useful debug output here
    debugV( mPulseLen );
    return mPulseLen;
}



std::uint16_t Servo::getPulseLen()
{
    return mPulseLen;
}



std::uint16_t Servo::convertToPulseLenFromDegreesRelative( int degrees )
{
#define SERVO_MG_996R       1
#define SERVO_HPS_2018      0

#if SERVO_MG_996R
    /*
    *     90 = 105
    *      0 = 328
    *    -90 = 551
    * 
    *      pulseLen = (105-551)/180 * degrees
    */
    constexpr auto slope{ (105.0 - 551.0)/180.0 };
    constexpr auto intercept{ 328.0 };
    std::int16_t pulse = slope * degrees + intercept + 0.5;
    return static_cast<std::uint16_t>( pulse );
#endif  // SERVO_HPS_2018


#if SERVO_HPS_2018
    /*
    *     90 = 90
    *      0 = 315
    *    -90 = 544
    * 
    *      pulseLen = (-227/90) * degrees + 317
    * 
    *
    */

    std::int16_t pulse = (-227.0/90.0) * degrees  + 315;
    return static_cast<std::uint16_t>( pulse );
#endif  // SERVO_HPS_2018
}





