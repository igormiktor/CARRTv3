/*
    BNO055.h - BNO055 functions for CARRT Pico.

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


#ifndef BNO055_h
#define BNO055_h

#include <cstdint>
#include <tuple>
#include <optional>


/* 
    Wait from power-on or soft reset to any I2C comms (e.g., initializing BNO055):  650ms

    Wait after mode switch (e.g., from CONFIGMODE to NDOF ):  600ms (according to notes in bno055.c driver and INCLUDED IN DRIVER CODDE)
*/

/*
Keen observation. It has been known for quite some time that the system calibration indicator is jumping around, but the data is still reliable. 
After analysis we found the issue to be simply in the status indicator, not the data itself. We have a software fix for it, if we ever change 
the BNO055 firmware programmed at the factory. For now the workaround is simply to ignore this value.

Therefore we recommend that as long as Magnetometer is 3/3, and Gyroscope is 3/3, the data can be trusted. Pitch and Roll are more accurate 
with accelerometer calibration, but the impact is far smaller than Gyroscope or Magnetometer calibration. Especially in your case you mention 
that the application is a wearable, then I doubt there is a significant impact on the use-case between 1 and 2 degrees of error on the pitch and roll.

From: https://community.bosch-sensortec.com/mems-sensors-forum-jrmujtaw/post/bno055-calibration-staus-not-stable-SxucRX9zo6bAbeT

**********

Calibration values reflect Android definitions (https://developer.android.com/reference/android/hardware/SensorManager#SENSOR_STATUS_ACCURACY_HIGH)

    - UNRELIABLE (0)    The values returned by this sensor cannot be trusted, calibration is needed or the environment doesn't allow readings
    - LOW (1)           This sensor is reporting data with low accuracy, calibration with the environment is needed
    - MEDIUM (2)        This sensor is reporting data with an average level of accuracy, calibration with the environment may improve the readings
    - HIGH (3)          This sensor is reporting data with maximum accuracy        

*/


namespace BNO055
{

    constexpr int           kWaitAfterPowerOnReset{ 650 };      // milliseconds
    constexpr int           kWaitAfterModeChange( 600 );        // milliseconds
    constexpr std::uint8_t  kCalibrationHigh{ 3 };              // 3 = High; 0 = Unreliable


    struct Calibration      // Always listed in order M-A-G-S
    {
        unsigned char mag;
        unsigned char accel;
        unsigned char gyro;
        unsigned char system;
    };

    inline bool calibrationGood( const Calibration& status ) 
    {
        return ( status.mag + status.accel + status.gyro + status.system ) == 4*kCalibrationHigh;
    }

    inline bool calibrationGoodEnough( const Calibration& status )
    {
        // Mag and Gyro are "High", Accel and System are not "Unreliable"
        return (status.mag == kCalibrationHigh) && status.accel && (status.gyro == kCalibrationHigh) && status.system;
    }


    void init();    // May include long delays of 600ms due to internal mode switches    
    void reset();   // Requires kWaitAfterPowerOnReset before then calling init()
               

    float getHeading();

    std::uint8_t getMagCalibration();
    std::uint8_t getAccelCalibration();
    std::uint8_t getGyroCalibration();
    std::uint8_t getSysCalibration();
   
    int getCalibration( unsigned char* gyro, unsigned char* accel, unsigned char* mag );

    Calibration getCalibration();
    
};




#endif  // BNO055_h

