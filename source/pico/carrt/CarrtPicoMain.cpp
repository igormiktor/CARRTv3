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
#include "MainProcess.h"
// #include "EventManager.h"
#include "Core1.h"
#include "shared/SerialCommand.h"
#include "shared/SerialLink.h"
#include "shared/CarrtError.h"

#include <iostream>
#include "pico/stdlib.h"
#include "pico/multicore.h"
// #include "pico/util/queue.h"
// #include "hardware/clocks.h"
#include "hardware/i2c.h"
// #include "hardware/timer.h"
// #include "hardware/uart.h"




void initializeHardware();
void initI2C();

bool launchCore1();
void runEventLoops();
void reset();



int main()
{
 
    initializeHardware();

    std::cout << "CARRT Pico started, hw initialized, running on core " << get_core_num() << std::endl;

    try
    {
        bool failed = launchCore1();
        if ( failed )
        {
            // Need to bail...
            throw CarrtError( makePicoErrorId( PicoError::kPicoMulticoreError, 1, 0 ), "CARRT Pico core1 start failed" );
        }

        // Report we are started...
        SerialLink::putCmd( kPicoReady );
        SerialLink::put( to_ms_since_boot( get_absolute_time() ) );

        MainProcess::runMainEventLoop();
    }

    catch( const CarrtError& e )
    {
        // Report the error...
        std::cout << "Error " << e.errorCode() << ' ' << e.what() << std::endl;
        SerialLink::putCmd( kErrorReportFromPico );
        SerialLink::putByte( kPicoFatalError );
        SerialLink::put( e.errorCode() );
    }

    catch( const std::exception& e )
    {
        std::cout << e.what() << std::endl;
        SerialLink::putCmd( kErrorReportFromPico );
        SerialLink::putByte( kPicoFatalError );
        SerialLink::put( 666 );
    }

    // Just spin...
    while ( 1 )
    {
        sleep_ms( 100 );
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
    initI2C();

    // Initialize UART for RPi0<->Pico serial link
    SerialLink::openSerialLink();
}



void initI2C()
{
    // I2C Initialisation
    i2c_init( CARRTPICO_I2C_PORT, CARRTPICO_I2C_SPEED );
    
    gpio_set_function( CARRTPICO_I2C_SDA, GPIO_FUNC_I2C ) ;
    gpio_set_function( CARRTPICO_I2C_SCL, GPIO_FUNC_I2C );
    gpio_pull_up( CARRTPICO_I2C_SDA );
    gpio_pull_up( CARRTPICO_I2C_SCL );
}


bool launchCore1()
{
    multicore_launch_core1( core1Main );

    // Wait for it to start up
    uint32_t flag = multicore_fifo_pop_blocking();
    
    return flag != CORE1_SUCCESS ;
}


void runEventLoops();
void reset();


