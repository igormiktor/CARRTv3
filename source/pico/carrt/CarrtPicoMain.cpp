/*
    CarrtPicoMain.cpp - CARRT-Pico's main function.  Initializes and hands off to
    the Main Process.

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






#include "CarrtPicoDefines.h"
//#include "Core1.h"
#include "Encoders.h"
#include "HeartBeatLed.h"
#include "MainProcess.h"

#include "drivers/BNO055.h"
#include "drivers/I2C.h"

#include "shared/CarrtError.h"
#include "shared/SerialCommand.h"
#include "shared/SerialLink.h"

#include <iostream>
#include "pico/stdlib.h"
//#include "pico/multicore.h"
// #include "pico/util/queue.h"
// #include "hardware/clocks.h"
#include "hardware/i2c.h"
// #include "hardware/timer.h"
// #include "hardware/uart.h"




void initializeHardware();
void initI2C();



int main()
{
 
    initializeHardware();

#if USE_CARRTPICO_STDIO
    std::cout << "CARRT Pico started, hw initialized, running on core " << get_core_num() << std::endl;
#endif

    try
    {
        // Report we are started...
        SerialLink::putCmd( kPicoReady );
        SerialLink::put( to_ms_since_boot( get_absolute_time() ) );

        MainProcess::runMainEventLoop();
    }

    catch( const CarrtError& e )
    {
        // Report the error...
        SerialLink::putCmd( kErrorReportFromPico );
        SerialLink::putByte( kPicoFatalError );
        SerialLink::put( e.errorCode() );

    #if USE_CARRTPICO_STDIO
        std::cout << "Error " << e.errorCode() << ' ' << e.what() << std::endl;
    #endif
    }

    catch( const std::exception& e )
    {
        SerialLink::putCmd( kErrorReportFromPico );
        SerialLink::putByte( kPicoFatalError );
        int errCode = makePicoErrorId( kPicoMainError, 1, 0 );
        SerialLink::put( errCode );
    
    #if USE_CARRTPICO_STDIO
        std::cout << "Error of unknown type " << e.what() << std::endl;
    #endif
    }

    catch( ... )
    {
        SerialLink::putCmd( kErrorReportFromPico );
        SerialLink::putByte( kPicoFatalError );
        int errCode = makePicoErrorId( kPicoMainError, 2, 0 );
        SerialLink::put( errCode );
    
    #if USE_CARRTPICO_STDIO
        std::cout << "Unknown error" << std::endl;
    #endif
    }

    // Just spin and put HeartBeatLed on fast strobe
    while ( 1 )
    {
        sleep_ms( 100 );
        HeartBeatLed::toggle();
    }

    return 0;
}



void initializeHardware()
{
#if USE_CARRTPICO_STDIO
    // Initialize C/C++ stdio (used for debugging)
    stdio_init_all();
#endif 

    // Set up Pico's I2C (to talk with BNO055)
    I2C::initI2C();

    // Initialize UART for RPi0<->Pico serial link
    SerialLink::openSerialLink();

    // Initialize the BNO055 (but we need to verify calibration is complete later on)
    BNO055::init();

    // Initialize the heartbeat LED (so we can show both normal ops and errors)
    HeartBeatLed::initialize();

    // Set up the encoders;
    Encoders::initEncoders();
}






