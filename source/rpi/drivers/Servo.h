/*
    Servo.h - Functions for controlling the servo that the
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




#ifndef Servo_h
#define Servo_h

#include <cstdint>




namespace Servo
{


    void init( bool pulseMode = false );

    void reset( bool pulseMode = false );

    void disconnect();

    void setPwmFreq( float freq );

    void setPwmDutyOnCycle( float onRatio );

    void setPwm( std::uint16_t on, std::uint16_t off );

    std::uint16_t setPulseLen( std::uint16_t pulseLen );
    std::uint16_t getPulseLen();

    // Slew angles measured relative to 000 = straight ahead;
    // left/counter-clockwise = positive, right/clockwise = negative
    int slew( int angleDegrees );

    int getCurrentAngle();

};


#endif
