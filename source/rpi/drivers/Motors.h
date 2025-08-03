/*
    Motors.h - Functions for controlling CARRT3's drive motors

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





#ifndef Motors_h
#define Motors_h



#include <CarrtError.h>



class MotorError : public CarrtError
{
public:
    explicit MotorError( int errCode, const std::string& what )
        : CarrtError( errCode, what ) {}

    explicit MotorError( int errCode, const char* what )
        : CarrtError( errCode, what ) {}
};


namespace Motors
{
    const int kFullSpeed = 255;
    const int kHalfSpeed = 200;

    void init();

    void setSpeedAllMotors( int speed );

    void goForward();
    void goBackward();
    void stop();

    void rotateLeft();
    void rotateRight();


    enum MotorCmd
    {
        kCmdForward = 1,
        kCmdBackward,
        kCmdBrake,
        kCmdRelease
    };

    void setRearRightMotorSpeed( int s );
    void setFrontRightMotorSpeed( int s );
    void setFrontLeftMotorSpeed( int s );
    void setRearLeftMotorSpeed( int s );

    void runRearRightMotor( MotorCmd cmd );
    void runRearLeftMotor( MotorCmd cmd );
    void runFrontRightMotor( MotorCmd cmd );
    void runFrontLeftMotor( MotorCmd cmd );

};


#endif
