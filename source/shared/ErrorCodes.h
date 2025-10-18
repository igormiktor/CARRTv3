/*
    ErrorCodes.h - Error codes for CARRT3

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



#ifndef ErrorCodes_h
#define ErrorCodes_h


#include <cstdint>


// Error code scheme....

/*
    All error codes are "int" which is 32-bit on both RPi Zero and RP2040.  
    So error codes can range from -2,147,483,648 to 2,147,483,647

    On RPi Zero...

        Pigpio library error codes are all negative, in the range -1 to -3,999

        Within an RPi Zero module identify the function where error occurs by increments of -10,0000
        and never expect to have more than 99 functions in a module, so max -990,000 (plus pigpio error)

        So identify modules by increments of -1,000,000, starting with -1,000,000


    On Pico (RP2040)...

        Pico error codes from the SDK (e.g., PICO_ERROR_*) are also all negative, in the range -1 to -9

        Within a Pico module identify the function where error occurs by increments of -100 
        and allow up to 99 functions per module, so max -9,900 (plus PICO_ERROR_...)

        So identfy modules where errors occur by increments of -10,000, starting with -10,000

        And we treat all Pico errors as a single "module" of RPi Zero errors (code < -1,000,000)

    So...
    
        Error codes < -999999 mean a Pico error
        Error codes   -xxyyzz mean an error in Pico module xx, function yy, error code -zz

        Error codes <  -1000000 mean a Rpi Zero error
        Error codes   -xxyyzzzz (where xx > 1) mean an error in RPi Zero, module xx, function yy, error code -zzzz

*/


constexpr int kRPi0FunctionIdErrIncrement       = -10000;
constexpr int kRPi0ModuleIdErrIncrement         = -1000000;

constexpr int kPicoFunctionIdErrIncrement       = -100;
constexpr int kPicoModuleIdErrIncrement         = -10000;




enum PicoError : int
{

    // Pico Module Errors (all are * kPicoModuleIdErrIncrement)

    kPicoMainError              = 1,
    kPicoMulticoreError         = 2,
    kPicoI2cBNO055Error         = 3, 
    kPicoMainProcessError       = 4,
    kPicoSerialMessageError     = 5,
    kPicoEventProcessorError    = 6,

    kPicoCritSectionError       = 10,

    kPicoGotErrorEvent          = 20,

    kPicoTestError              = 98,
    kPicoLastError              = 99
};

constexpr uint32_t kPicoFatalError              = 1;
constexpr uint32_t kPicoNonFatalError           = 0;




enum RPi0Error : int
{

    // RPi Zero Module errors (all are * kRPi0ModuleIdErrIncrement)

    kI2cError                   = 1,   
    kBuzzerError                = 2,
    kLidarError                 = 3,
    kRpi0SerialError            = 4,
    kRPi0SerialMessageError     = 5, 

    kTestError                  = 98,
    kLastError                  = 99
};




enum CommonError : int
{
    kSerialMsgReadError         = 80,
    kSerialMsgDupeError         = 81,
    kSerialMsgUnknownError      = 82,
    kEventHandlerDupeError      = 83
};


#endif // ErrorCodes.h

