/*
    Motors.h - Functions for controlling CARRTv3's drive motors

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





#ifndef Motors_h
#define Motors_h

#include <inttypes.h>


namespace Motors
{

    const uint8_t kFullSpeed = 255;
    const uint8_t kHalfSpeed = 200;

    int init();

    int setSpeedAllMotors( uint8_t speed );

    int goForward();
    int goBackward();
    int stop();

    int rotateLeft();
    int rotateRight();


    enum MotorCmd
    {
        kCmdForward = 1,
        kCmdBackward,
        kCmdBrake,
        kCmdRelease
    };

    int setRearRightMotorSpeed( uint8_t s );
    int setFrontRightMotorSpeed( uint8_t s );
    int setFrontLeftMotorSpeed( uint8_t s );
    int setRearLeftMotorSpeed( uint8_t s );

    int runRearRightMotor( MotorCmd cmd );
    int runRearLeftMotor( MotorCmd cmd );
    int runFrontRightMotor( MotorCmd cmd );
    int runFrontLeftMotor( MotorCmd cmd );

};


#endif
