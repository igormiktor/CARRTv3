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
#include "Core1.h"
#include "Encoders.h"
#include "EventManager.h"
#include "HeartBeatLed.h"
#include "MainProcess.h"
#include "PicoOutputUtils.hpp"
#include "CarrtPicoReset.h"

#include "BNO055.h"
#include "I2C.h"
#include "SerialLinkPico.h"

#include "CarrtError.h"
#include "SerialCommands.h"
#include "SerialCommandProcessor.h"
#include "DebugUtils.hpp"

#include "PicoState.h"

#include "CoreAtomic.hpp"



#include "pico/stdlib.h"
//#include "pico/multicore.h"
// #include "pico/util/queue.h"
// #include "hardware/clocks.h"
#include "hardware/i2c.h"
// #include "hardware/timer.h"
// #include "hardware/uart.h"




void initializeFailSafeHardware();
void initializeFailableHardware();
void setupCommandProcessor( SerialCommandProcessor& cmdr );
void sendReady( SerialLinkPico& link );







int main()
{
    CoreAtomic::CAtomicInitializer theInitializationIsDone;

    initializeFailSafeHardware();

    // Open the serial link to RPi0
    SerialLinkPico rpi0;

    try
    {
        // Force creation of the singleton
        PicoState::access()->allSendEventsOff();

        initializeFailableHardware();

        output2cout( "CARRT Pico started, hardware initialized, both cores running." );
    
        // Set up command processor
        SerialCommandProcessor scp( rpi0 );
        // TODO load it up with commands...
    
        // Report we are started and ready to receive commands
        sendReady( rpi0 );

        MainProcess::runMainEventLoop( Events(), scp, rpi0  );
    }

    catch( const CarrtError& e )
    {
        // Report the error...
        ErrorReportCmd err( kPicoFatalError, e.errorCode() ); 
        err.sendOut( rpi0 );

        output2cout( "Fatal error ", e.errorCode(), ' ', e.what() );
    }

    catch( const std::exception& e )
    {
        int errCode{ makePicoErrorId( kPicoMainError, 1, 0 ) };
        ErrorReportCmd err( kPicoFatalError, errCode ); 
        err.sendOut( rpi0 );

        output2cout( "Fatal error of unexpected nature", e.what() );
    }

    catch( ... )
    {    
        int errCode{ makePicoErrorId( kPicoMainError, 2, 0 ) };
        ErrorReportCmd err( kPicoFatalError, errCode ); 
        err.sendOut( rpi0 );

        output2cout( "Fatal error of unknown type" );
    }


    output2cout( "Pico frozen with fast LED strobe" );
    // Just spin and put HeartBeatLed on fast strobe
    while ( 1 )
    {
        sleep_ms( 100 );
        HeartBeatLed::toggle();

        // See if we get a sent a reset command
        if ( rpi0.isReadable() )
        {
            auto msgType = rpi0.getMsgType();
            if ( msgType && *msgType == kResetMsg )
            {
                PicoReset::reset( rpi0 );
            }
        }

        // If no reset msg, we keep strobing the LED
    }

    return 0;
}




void initializeFailSafeHardware()
{
    // Nothing in this function throws or even fails

#if USE_CARRTPICO_STDIO
    // Initialize C/C++ stdio (used for status output and debugging)
    stdio_init_all();
#endif 

    // Set up Pico's I2C (to talk with BNO055)
    I2C::initI2C();

    // Initialize the heartbeat LED
    HeartBeatLed::initialize();

    // Set up the encoders;
    Encoders::initEncoders();
}




void initializeFailableHardware()
{
    // These function calls may throw

    // Initialize the BNO055 (but we need to verify calibration is complete later on)
    BNO055::init();

    // Launch Core1
    launchCore1();
}



void setupCommandProcessor( SerialCommandProcessor& scp )
{
    scp.registerCommand<NullCmd>( kNullMsg );
    scp.registerCommand<PicoReadyCmd>( kPicoReady );
    scp.registerCommand<TimerControlCmd>( kTimerControl );
    scp.registerCommand<DebugLinkCmd>( kDebugSerialLink );

}



void sendReady( SerialLinkPico& link )
{
    PicoReadyCmd ready( kPicoReady );
    ready.sendOut( link );
}