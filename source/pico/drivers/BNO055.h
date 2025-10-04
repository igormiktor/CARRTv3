/*
    BNO055.h - BNO055 functions for CARRT Pico.

    Copyright (c) 2024 Igor Mikolic-Torreira.  All right reserved.

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


#ifndef BNO055_h
#define BNO055_h

#include <cstdint>
#include <tuple>
#include <optional>


namespace BNO055
{
    struct Calibration
    {
        unsigned char mag;
        unsigned char accel;
        unsigned char gyro;
        unsigned char system;
    };

    constexpr std::uint8_t  kCalibrationSuccess{ 3 };

    bool inline calibrationSuccess( const Calibration& status ) 
    {
        return ( status.mag + status.accel + status.gyro + status.system ) == 4*kCalibrationSuccess;
    }

    void init();

    float getHeading();

    std::uint8_t getMagCalibration();
    std::uint8_t getAccelCalibration();
    std::uint8_t getGyroCalibration();
    std::uint8_t getSysCalibration();
   
    int getCalibration( unsigned char* gyro, unsigned char* accel, unsigned char* mag );

    Calibration getCalibration();
    
};




#endif  // BNO055_h

