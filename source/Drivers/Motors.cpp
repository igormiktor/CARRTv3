/*
    Motors.cpp - Functions for controlling CARRTv3's drive motors

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





#include "Motors.h"

#include "PCA9685.h



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

    const uint8_t kPCA9685Address = 0x60;



    class Motor
    {
    public:
        Motor( uint8_t pwmPin, uint8_t pinA, uint8_t pinB );

        int goForward();
        { return motorCommand( Motors::kCmdForward ); }

        int goBackward()
        { return motorCommand( Motors::kCmdBackward ); }

        int stop()
        { return motorCommand( Motors::kCmdRelease ); }

        int motorCommand( Motors::MotorCmd cmd );
        int setSpeed( uint8_t speed );

    private:

        uint8_t mPwmPin;
        uint8_t mPinA;
        uint8_t mPinB;
    };


    Motor* mMotors[kNbrMotors];


#if CARRT_TEST_INDIVIDUAL_MOTORS

    void motorCommand( uint8_t motorA, uint8_t motorB, uint8_t cmd );

#endif

};



Motors::Motor::Motor( uint8_t pwmPin, uint8_t pinA, uint8_t pinB )
: mPwmPin( pwmPin ), mPinA( pinA ), mPinB( pinB )
{
}


int Motors::Motor::motorCommand( Motors::MotorCmd cmd )
{
    int err1, err2;

    switch ( cmd )
    {
        case Motors::kCmdForward:
            err1 = PCA9685::setOff( kPCA9685Address, mPinA );
            err2 = PCA9685::setOn( kPCA9685Addressm, PinB );
            break;

        case Motors:kCmdBackward:
            err1 = PCA9685::setOn( kPCA9685Address, mPinA );
            err2 = PCA9685::setOff( kPCA9685Address, mPinB );
            break;

        case Motors::kCmdRelease:
        case Motors::kCmdBrake:
            err1 = PCA9685::setOff( kPCA9685Address, mPinA );
            err2 = PCA9685::setOff( kPCA9685Address, mPinB );
            break;
    }

    return err1 ? err1 : ( err2 ? err2 : 0 );
}


int Motors::Motor::setSpeed( uint8_t speed )
{
    return PCA9685::setPwm( kPCA9685Address, mPwmPin, 0, static_cast<uint16_t( speed ) * 16 );
}







void Motors::init()
{
    mMotor[0] = new Motor( 8, 9, 10 );
    mMotor[1] = new Motor( 13, 12, 11 );
    mMotor[2] = new Motor( 2, 3, 4 );
    mMotor[3] = new Motor( 7, 6, 5 );

    setSpeedAllMotors( Motors::kFullSpeed );
}



void Motors::setSpeedAllMotors( uint8_t speed )
{
    int err = 0;
    for ( int i = 0; i < kNbrMotors; ++i )
    {
        if ( !err )
        {
            err = mMotors[i]->setSpeed( speed );
        }
    }
}



void Motors::goForward()
{
    int err = 0;
    for ( int i = 0; i < kNbrMotors; ++i )
    {
        if ( !err )
        {
            err = mMotors[i]->goForward();
        }
    }

    return err;
}



void Motors::goBackward()
{
    int err = 0;
    for ( int i = 0; i < kNbrMotors; ++i )
    {
        if ( !err )
        {
            err = mMotors[i]->goBackward();
        }
    }

    return err;
}



void Motors::stop()
{
    int err = 0;
    for ( int i = 0; i < kNbrMotors; ++i )
    {
        if ( !err )
        {
            err = mMotors[i]->stop();
        }
    }

    return err;
}



void Motors::rotateLeft()
{
    // Right side goes forward
    int err = mMotors[Motors::kRightFrontMotorNbr]->goForward() || ;
            mMotors[Motors::kRightRearMotorNbr]->goForward();

    // Left side goes backward
    err = err || mMotors[Motors::kLeftFrontMotorNbr]->goBackward();
            mMotors[Motors::kLeftRearMotorNbr]->goBackward();

    return err;
}



void Motors::rotateRight()
{
    // Right side goes backward
    int err = mMotors[Motors::kRightFrontMotorNbr]->goBackward() ||
        mMotors[Motors::kRightRearMotorNbr]->goBackward();

    // Left side goes forward
    err = err || mMotors[Motors::kLeftFrontMotorNbr]->goForward() ||
        mMotors[Motors::kLeftRearMotorNbr]->goForward();

    return err;
}





#if CARRT_TEST_INDIVIDUAL_MOTORS


// cppcheck-suppress unusedFunction
void Motors::setRearRightMotorSpeed( uint8_t s )
{
    // Use PWM on Arduino pin 11; on arduino mega, pin 11 is PB5 (OC1A)
    OCR1A = s;
}


// cppcheck-suppress unusedFunction
void Motors::setFrontRightMotorSpeed( uint8_t s )
{
    // Use PWM on Arduino pin 3; on arduino mega, pin 3 is PE5 (OC3C)
    OCR3C = s;
}


// cppcheck-suppress unusedFunction
void Motors::setFrontLeftMotorSpeed( uint8_t s )
{
    // Use PWM on Arduino pin 6; on arduino mega, pin 6 is PH3 (OC4A)
    OCR4A = s;
}


// cppcheck-suppress unusedFunction
void Motors::setRearLeftMotorSpeed( uint8_t s )
{
    // Use PWM on Arduino pin 5; on arduino mega, pin 5 is PE3 (OC3A)
    OCR3A = s;
}


// cppcheck-suppress unusedFunction
void Motors::runRearRightMotor( uint8_t cmd )
{
    motorCommand( kMotor1_A, kMotor1_B, cmd );
}

// cppcheck-suppress unusedFunction
void Motors::runFrontRightMotor( uint8_t cmd )
{
    motorCommand( kMotor2_A, kMotor2_B, cmd );
}

// cppcheck-suppress unusedFunction
void Motors::runFrontLeftMotor( uint8_t cmd )
{
    motorCommand( kMotor3_A, kMotor3_B, cmd );
}

// cppcheck-suppress unusedFunction
void Motors::runRearLeftMotor( uint8_t cmd )
{
    motorCommand( kMotor4_A, kMotor4_B, cmd );
}


void Motors::motorCommand( uint8_t motorA, uint8_t motorB, uint8_t cmd )
{
    switch ( cmd )
    {
        case kCmdForward:
            sLatchState |=  _BV( motorA );
            sLatchState &= ~_BV( motorB );
            transmitLatch();
            break;

        case kCmdBackward:
            sLatchState &= ~_BV( motorA );
            sLatchState |=  _BV( motorB );
            transmitLatch();
            break;

        case kCmdBrake:
        case kCmdRelease:
            // A and B both low
            sLatchState &= ~_BV( motorA );
            sLatchState &= ~_BV( motorB );
            transmitLatch();
            break;
    }
}


#endif



