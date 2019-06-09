/*
    Motors.cpp - Functions for controlling CARRTv3's drive motors

    Copyright (c) 2019 Igor Mikolic-Torreira.  All right reserved.

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



#include <cstdint>


#include "Motors.h"

#include "Drivers/I2c.h"
#include "Drivers/PCA9685.h"



// Extend namespace with additional functions and such
namespace Motors
{

    enum
    {
        kRightRearMotorNbr      = 0,
        kRightFrontMotorNbr     = 1,
        kLeftFrontMotorNbr      = 2,
        kLeftRearMotorNbr       = 3
    };

    const int kNbrMotors = 4;

    const std::uint8_t kMotorHatAddress = 0x60;



    class Motor
    {
    public:
        Motor( int pwmPin, int pinA, int pinB );
        ~Motor();

        void goForward()
        { motorCommand( Motors::kCmdForward ); }

        void goBackward()
        { motorCommand( Motors::kCmdBackward ); }

        void stop()
        { motorCommand( Motors::kCmdRelease ); }

        void motorCommand( Motors::MotorCmd cmd );
        void setSpeed( int speed );

    private:

        uint8_t mPwmPin;
        uint8_t mPinA;
        uint8_t mPinB;
    };


    void motorCommand( uint8_t motorA, uint8_t motorB, uint8_t cmd );

    Motor*  mMotors[ kNbrMotors ];

};








Motors::Motor::Motor( int pwmPin, int pinA, int pinB )
: mPwmPin( static_cast<uint8_t>( pwmPin ) ), mPinA( static_cast<uint8_t>( pinA ) ), mPinB( static_cast<uint8_t>( pinB ) )
{
    // Nothing else to do
}


Motors::Motor::~Motor()
{
    stop();
}


// cppcheck-suppress unusedFunction
void Motors::Motor::motorCommand( Motors::MotorCmd cmd )
{
    switch ( cmd )
    {
        case Motors::kCmdForward:
            // Set pin A = 1;
            // Set pin B = 0;
            PCA9685::setOn( kMotorHatAddress, mPinA );
            PCA9685::setOff( kMotorHatAddress, mPinB );
            break;

        case Motors::kCmdBackward:
            // Set pinA = 0;
            // Set pinB = 1;
            PCA9685::setOff( kMotorHatAddress, mPinA );
            PCA9685::setOn( kMotorHatAddress, mPinB );
            break;

        case Motors::kCmdRelease:
        case Motors::kCmdBrake:
            // Set PinA = 0;
            // Set PinB = 0;
            PCA9685::setOff( kMotorHatAddress, mPinA );
            PCA9685::setOff( kMotorHatAddress, mPinB );
            break;
    }
}


void Motors::Motor::setSpeed( int speed )
{
    PCA9685::setPwm( kMotorHatAddress, mPwmPin, 0, static_cast<uint16_t>( speed * 16 ) );
}







void Motors::init()
{
    PCA9685::init( kMotorHatAddress );

    mMotors[0] = new Motor( 8, 9, 10 );
    mMotors[1] = new Motor( 13, 12, 11 );
    mMotors[2] = new Motor( 2, 3, 4 );
    mMotors[3] = new Motor( 7, 6, 5 );

    setSpeedAllMotors( Motors::kFullSpeed );
}



void Motors::setSpeedAllMotors( int speed )
{
    for ( int i = 0; i < kNbrMotors; ++i )
    {
        mMotors[i]->setSpeed( speed );
    }
}



void Motors::goForward()
{
    for ( int i = 0; i < kNbrMotors; ++i )
    {
        mMotors[i]->goForward();
    }
}



void Motors::goBackward()
{
    for ( int i = 0; i < kNbrMotors; ++i )
    {
        mMotors[i]->goBackward();
    }
}



void Motors::stop()
{
    for ( int i = 0; i < kNbrMotors; ++i )
    {
        mMotors[i]->stop();
    }
}



void Motors::rotateLeft()
{
    // Right side goes forward
    mMotors[Motors::kRightFrontMotorNbr]->goForward();
    mMotors[Motors::kRightRearMotorNbr]->goForward();

    // Left side goes backward
    mMotors[Motors::kLeftFrontMotorNbr]->goBackward();
    mMotors[Motors::kLeftRearMotorNbr]->goBackward();
}



void Motors::rotateRight()
{
    // Right side goes backward
    mMotors[Motors::kRightFrontMotorNbr]->goBackward();
    mMotors[Motors::kRightRearMotorNbr]->goBackward();

    // Left side goes forward
    mMotors[Motors::kLeftFrontMotorNbr]->goForward();
    mMotors[Motors::kLeftRearMotorNbr]->goForward();
}





// cppcheck-suppress unusedFunction
void Motors::setRearRightMotorSpeed( int s )
{
    mMotors[Motors::kRightRearMotorNbr]->setSpeed( s );
}


// cppcheck-suppress unusedFunction
void Motors::setFrontRightMotorSpeed( int s )
{
    mMotors[Motors::kRightFrontMotorNbr]->setSpeed( s );
}


// cppcheck-suppress unusedFunction
void Motors::setFrontLeftMotorSpeed( int s )
{
    mMotors[Motors::kLeftFrontMotorNbr]->setSpeed( s );
}


// cppcheck-suppress unusedFunction
void Motors::setRearLeftMotorSpeed( int s )
{
    mMotors[Motors::kLeftRearMotorNbr]->setSpeed( s );
}


// cppcheck-suppress unusedFunction
void Motors::runRearRightMotor( Motors::MotorCmd cmd )
{
    mMotors[Motors::kRightRearMotorNbr]->motorCommand( cmd );
}

// cppcheck-suppress unusedFunction
void Motors::runFrontRightMotor( Motors::MotorCmd cmd )
{
    mMotors[Motors::kRightFrontMotorNbr]->motorCommand( cmd );
}

// cppcheck-suppress unusedFunction
void Motors::runFrontLeftMotor( Motors::MotorCmd cmd )
{
    mMotors[Motors::kLeftFrontMotorNbr]->motorCommand( cmd );
}

// cppcheck-suppress unusedFunction
void Motors::runRearLeftMotor( Motors::MotorCmd cmd )
{
    mMotors[Motors::kLeftRearMotorNbr]->motorCommand( cmd );
}
