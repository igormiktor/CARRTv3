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
        ~Motor();

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


Motors::Motor::~Motor()
{
    stop();
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







int Motors::init()
{
    mMotor[0] = new Motor( 8, 9, 10 );
    mMotor[1] = new Motor( 13, 12, 11 );
    mMotor[2] = new Motor( 2, 3, 4 );
    mMotor[3] = new Motor( 7, 6, 5 );

    return setSpeedAllMotors( Motors::kFullSpeed );
}



int Motors::setSpeedAllMotors( uint8_t speed )
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



int Motors::goForward()
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



int Motors::goBackward()
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



int Motors::stop()
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



int Motors::rotateLeft()
{
    // Right side goes forward
    int err = mMotors[Motors::kRightFrontMotorNbr]->goForward() || ;
            mMotors[Motors::kRightRearMotorNbr]->goForward();

    // Left side goes backward
    err = err || mMotors[Motors::kLeftFrontMotorNbr]->goBackward();
            mMotors[Motors::kLeftRearMotorNbr]->goBackward();

    return err;
}



int Motors::rotateRight()
{
    // Right side goes backward
    int err = mMotors[Motors::kRightFrontMotorNbr]->goBackward() ||
        mMotors[Motors::kRightRearMotorNbr]->goBackward();

    // Left side goes forward
    err = err || mMotors[Motors::kLeftFrontMotorNbr]->goForward() ||
        mMotors[Motors::kLeftRearMotorNbr]->goForward();

    return err;
}





// cppcheck-suppress unusedFunction
int Motors::setRearRightMotorSpeed( uint8_t s )
{
    return mMotors[Motors::kRightRearMotorNbr]->setSpeed( s );
}


// cppcheck-suppress unusedFunction
int Motors::setFrontRightMotorSpeed( uint8_t s )
{
    return mMotors[Motors::kRightFrontMotorNbr]->setSpeed( s );
}


// cppcheck-suppress unusedFunction
int Motors::setFrontLeftMotorSpeed( uint8_t s )
{
    return mMotors[Motors::kLeftFrontMotorNbr]->setSpeed( s );
}


// cppcheck-suppress unusedFunction
int Motors::setRearLeftMotorSpeed( uint8_t s )
{
    return mMotors[Motors::kLeftRearMotorNbr]->setSpeed( s );
}


// cppcheck-suppress unusedFunction
int Motors::runRearRightMotor( Motors::MotorCmd cmd )
{
    return mMotors[Motors::kRightRearMotorNbr]->motorCommand( cmd );
}

// cppcheck-suppress unusedFunction
int Motors::runFrontRightMotor( Motors::MotorCmd cmd )
{
    return mMotors[Motors::kRightFrontMotorNbr]->motorCommand( cmd );
}

// cppcheck-suppress unusedFunction
int Motors::runFrontLeftMotor( Motors::MotorCmd cmd )
{
    return mMotors[Motors::kLeftFrontMotorNbr]->motorCommand( cmd );
}

// cppcheck-suppress unusedFunction
int Motors::runRearLeftMotor( Motors::MotorCmd cmd )
{
    return mMotors[Motors::kLeftRearMotorNbr]->motorCommand( cmd );
}




