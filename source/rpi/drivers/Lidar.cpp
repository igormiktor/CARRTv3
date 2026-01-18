/*
    Lidar.cpp - Functions for controlling CARRT's
    Garmin Lidar Lite v3 range sensor

    Copyright (c) 2026 Igor Mikolic-Torreira.  All right reserved.

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

#include "Lidar.h"

#include <chrono>

#include "CarrtError.h"
#include "Clock.h"
#include "DebugUtils.hpp"
#include "I2c.h"

// Useful for early testing and calibration
// #define CARRT_DISABLE_LIDAR_SERVO   1

// Ensure Servo is active unless explicitly turned off
#if !defined( CARRT_DISABLE_LIDAR_SERVO )
    #define CARRT_DISABLE_LIDAR_SERVO 0
#endif

#if CARRT_DISABLE_LIDAR_SERVO
    #warning CARRT_DISABLE_LIDAR_SERVO defined and non-zero: Servo functionality disabled in Lidar driver
#else
    #include "Servo.h"
#endif

/*
 * Designed as a namespace instead of a class because there is only one
 * radar and need to access the radar from various places in CARRT.
 *
 * A class requires a single global instance; this way we can access
 * strictly through the function interface without an instances
 *
 * Note: this means this driver code is only good for ONE radar.
 * Use of multiple radars would require conversion to a class
 */

/*
 * WARNING: the Garman Lidar Lite v3 does NOT understand I2C "restart".
 * So register reads have to be sent as two separate I2C messages: first
 * a "write" message sending the register to be read; followed by a second
 * "read" message asking for data.
 *
 * The advanced form "read" message that transmits the register to be read
 * and then reads the data fails with the lidar because it signals an
 * I2C "restart" to switch from transmitting the register to receiving the
 * data.  The lidar responds to the "restart" condition by becoming confused,
 * without explicitly signaling an error.  Data read from the lidar from
 * that point on is garbage.
 */

// Add some "local" functions and constants to the namespace

namespace Lidar
{
    constexpr int kLidarDoesntKnowDistance{ 1 };

    constexpr std::uint8_t kLidarI2cAddress{ 0x62 };

    constexpr int kLidarWaitTimedOutErr{ 666 };

    constexpr std::chrono::milliseconds kLidarWaitBetweenPings{ 150ms };

    // clang-format off
    enum LidarRegisters
    {
        kDeviceCommand                      = 0x00,
        kStatus                             = 0x01,
        kMaxAcquisitionCount                = 0x02,     // Default:  0x80
        kAcquisitionMode                    = 0x04,     // Default:  0x08
        kVelocity                           = 0x09,
        kPeakCorrelation                    = 0x0c,
        kCorrRecordNoiseFloor               = 0x0d,
        kRecvSignalStrength                 = 0x0e,
        kDistanceMeasuredHighByte           = 0x0f,
        kDistanceMeasuredLowByte            = 0x10,
        kDistanceMeasuredWord               = 0x8f,
        kBurstMeasurmentCountControl        = 0x11,     // Default:  0x01
        kReferenceAcquisitionCount          = 0x12,     // Default:  0x05
        kPreviousDistanceMeasuredHigh       = 0x14,
        kPreviousDistanceMeasuredLow        = 0x15,
        kPreviousDistanceMeasuredWord       = 0x94,
        kUnitSerialNumberHighByte           = 0x16,
        kUnitSerialNumberLowByte            = 0x17,
        kUnitSerialNumberWord               = 0x96,
        kUnitSerialNumberWriteHighByte      = 0x18,
        kUnitSerialNumberWriteLowByte       = 0x19,
        kUnitSerialNumberWriteWord          = 0x98,
        kI2cAddressReadSet                  = 0x1a,
        kPeakDetectThresholdBypass          = 0x1c,     // Default:  0x00
        kDefaultI2cResponseControl          = 0x1e,     // Default:  0x00
        kStateCommand                       = 0x40,
        kAutoMeasurementDelay               = 0x45,     // Default:  0x14
        kSecondLargestCorrelationPeak       = 0x4c,
        kCorrelationData                    = 0x52,
        kCorrelationSign                    = 0x53,
        kCorrelationDataWordReversed        = 0xd2,
        kCorrelRecordMemBankSelect          = 0x5d,
        kPowerStateControl                  = 0x65      // Default:  0x80
    };

    enum LidarCommands
    {
        kResetCmd                           = 0x00,
        kMeasureWithoutBiasCorrectionCmd    = 0x03,
        kMeasureWithBiasCorrectionCmd       = 0x04
    };

    // clang-format on

    void passConfigurationParameters( std::uint8_t maxAcqCnt,
                                      std::uint8_t acqMode,
                                      std::uint8_t detectThresholdBypass,
                                      std::uint8_t refAcqCnt );

    int waitUntilLidarReadyToRead();

    int readDistanceData();

    int getUncorrectedDistanceInCm( int* uncorrectedDistInCm,
                                    bool useBiasCorrection = true );

    int rangeCalibrationCorrection( int unCorrectedDistInCm );

};    // namespace Lidar

void Lidar::init()
{
    reset();

#if !defined( CARRT_DISABLE_LIDAR_SERVO ) || CARRT_DISABLE_LIDAR_SERVO == 0
    Servo::init();
#endif
}

// cppcheck-suppress unusedFunction
int Lidar::getCurrentAngle()
{
#if !defined( CARRT_DISABLE_LIDAR_SERVO ) || CARRT_DISABLE_LIDAR_SERVO == 0
    return Servo::getCurrentAngle();
#else
    return 0;
#endif
}

int Lidar::slew( int angleDegrees )
{
#if !defined( CARRT_DISABLE_LIDAR_SERVO ) || CARRT_DISABLE_LIDAR_SERVO == 0
    return Servo::slew( angleDegrees );
#else
    return 0;
#endif
}

int Lidar::waitUntilLidarReadyToRead()
{
    const std::uint8_t kMaxWaitLoopCount = 100;

    bool lidarBusy = true;

    I2c::write( kLidarI2cAddress, static_cast<std::uint8_t>( kStatus ) );
    for ( std::uint8_t i = 0; lidarBusy && i < kMaxWaitLoopCount; ++i )
    {
        std::uint8_t lidarStatus;
        int numRead = I2c::readWithOutRestart( kLidarI2cAddress, kStatus, 1,
                                               &lidarStatus );
        if ( numRead == 1 )
        {
            lidarBusy = lidarStatus & 0x01;
        }
        else
        {
            debugM( "No I2C error, but numRead != 1 as expected" );
            throw CarrtError(
                makeRpi0ErrorId( kLidarError, 1, numRead ),
                "No I2C error, but Lidar numRead != 1 (expected)" );
        }
    }

    return ( lidarBusy ? kLidarWaitTimedOutErr : 0 );
}

int Lidar::readDistanceData()
{
    std::uint8_t rawDistance[ 2 ];
    int numRead = I2c::readWithOutRestart(
        kLidarI2cAddress, kDistanceMeasuredWord, 2, rawDistance );

    if ( numRead != 2 )
    {
        debugM( "No I2C error, but numRead != 2 as expected" );
        throw CarrtError( makeRpi0ErrorId( kLidarError, 2, numRead ),
                          "No I2C error, but Lidar numRead != 2 (expected)" );
    }

    return static_cast<int>(
        ( static_cast<std::uint16_t>( rawDistance[ 0 ] ) << 8 )
        | rawDistance[ 1 ] );
}

int Lidar::getUncorrectedDistanceInCm( int* uncorrectedDistInCm,
                                       bool useBiasCorrection )
{
    std::uint8_t biasChoice{ useBiasCorrection
                                 ? kMeasureWithBiasCorrectionCmd
                                 : kMeasureWithoutBiasCorrectionCmd };
    I2c::write( kLidarI2cAddress, kDeviceCommand, biasChoice );

    int err = waitUntilLidarReadyToRead();
    if ( err )
    {
        debugM( "Lidar not ready to read" );
        throw CarrtError( makeRpi0ErrorId( kLidarError, 3, err ),
                          "Lidar timed-out, not ready to read" );
    }

    // Lidar ready to read a value
    *uncorrectedDistInCm = readDistanceData();

    // Lidar returns 1 if it can't figure out a distance
    if ( *uncorrectedDistInCm == kLidarDoesntKnowDistance )
    {
        err = kNoValidDistance;
    }

    return err;
}

int Lidar::getDistanceInCm( int* distInCm, bool useBiasCorrection )
{
    int uncorrectedDistInCm;
    int err =
        getUncorrectedDistanceInCm( &uncorrectedDistInCm, useBiasCorrection );

    if ( !err )
    {
        *distInCm = rangeCalibrationCorrection( uncorrectedDistInCm );
    }

    return err;
}

int Lidar::getMedianDistanceInCm( int* distInCm, std::uint8_t nbrMedianSamples,
                                  bool useBiasCorrection )
{
    int samples[ nbrMedianSamples ];
    samples[ 0 ] = kNoValidDistance;

    bool gotValidRange = false;
    int err = 0;

    std::uint8_t iMax = nbrMedianSamples;
    std::uint8_t i = 0;
    while ( i < iMax )
    {
        int last;
        int err = getUncorrectedDistanceInCm( &last, useBiasCorrection );
        if ( err )
        {
            // Error getting range.
            // Skip, don't include as part of median.
            --iMax;
        }
        else
        {
            std::uint8_t j;

            // Distance valid, include as part of median.

            gotValidRange = true;

            // Only do sorting on second or later entries
            if ( i > 0 )
            {
                // Don't start sort till second ping.
                for ( j = i; j > 0 && samples[ j - 1 ] < last; --j )
                {
                    // Insertion sort loop.
                    samples[ j ] =
                        samples[ j - 1 ];    // Shift array to correct position
                                             // for sort insertion.
                }
            }
            else
            {
                j = 0;    // First ping is starting point for sort.
            }
            samples[ j ] =
                last;    // Add last ping to array in sorted position.
            ++i;         // Move to next ping.
        }

        if ( i < iMax )
        {
            // Millisecond delay between pings.
            Clock::sleep( kLidarWaitBetweenPings );
        }
    }

    if ( gotValidRange )
    {
        // Return the median distance, but apply correction
        *distInCm = rangeCalibrationCorrection( samples[ iMax >> 1 ] );

        return 0;
    }
    else
    {
        // If no valid ranges, then last attempt also errored and thus err
        // contains the last error code
        return err;
    }
}

int Lidar::rangeCalibrationCorrection( int unCorrectedDistInCm )
{
#define CARRT_LIDAR_UNCALIBRATED 1

#if CARRT_LIDAR_UNCALIBRATED
    return unCorrectedDistInCm;
#else
    // Correction using a piece-wise linear function

    const int kBreakPoint = 75;    // cm

    // Linear corrections empirically derived
    const float kM1 = 1.200648697;
    const float kB1 = -19.5157141259;

    const float kM2 = 0.9779984453;
    const float kB2 = -3.6283644408;

    if ( unCorrectedDistInCm < kBreakPoint )
    {
        return kM1 * unCorrectedDistInCm + kB1;
    }
    else
    {
        return kM2 * unCorrectedDistInCm + kB2;
    }
#endif
}

void Lidar::reset()
{
    // Lidar takes approximately 22ms to reset
    I2c::write( kLidarI2cAddress, kDeviceCommand,
                static_cast<std::uint8_t>( kResetCmd ) );

    Clock::sleep( 25ms );

    setConfiguration( Lidar::kDefault );

    // Experience has shown Lidar needs a "warm up"
    for ( std::uint8_t i = 0; i < 10; ++i )
    {
        int rng;
        Clock::sleep( kLidarWaitBetweenPings );
        Lidar::getDistanceInCm( &rng );
    }
}

void Lidar::setConfiguration( Configuration config )
{
    switch ( config )
    {
        default:
        case kDefault:
            passConfigurationParameters( 0x80, 0x08, 0x00, 0x05 );
            break;

        case kShortRangeAndHighSpeed:
            passConfigurationParameters( 0x1d, 0x08, 0x00, 0x05 );
            break;

        case kShortRangeAndHighestSpeed:
            passConfigurationParameters( 0x0d, 0x04, 0x00, 0x03 );
            break;

        case kDefaultRangeAndHigherSpeedAtShortRange:
            passConfigurationParameters( 0x80, 0x00, 0x00, 0x05 );
            break;

        case kMaximumRange:
            passConfigurationParameters( 0xff, 0x08, 0x00, 0x05 );
            break;

        case kHighSensitivityButHigherError:
            passConfigurationParameters( 0x80, 0x08, 0x80, 0x05 );
            break;

        case kLowSensitivityButLowerError:
            passConfigurationParameters( 0x80, 0x08, 0xb0, 0x05 );
            break;
    }
}

void Lidar::passConfigurationParameters( std::uint8_t maxAcqCnt,
                                         std::uint8_t acqMode,
                                         std::uint8_t detectThresholdBypass,
                                         std::uint8_t refAcqCnt )
{
    I2c::write( kLidarI2cAddress, kMaxAcquisitionCount, maxAcqCnt );
    I2c::write( kLidarI2cAddress, kAcquisitionMode, acqMode );
    I2c::write( kLidarI2cAddress, kPeakDetectThresholdBypass,
                detectThresholdBypass );
    I2c::write( kLidarI2cAddress, kReferenceAcquisitionCount, refAcqCnt );
}
