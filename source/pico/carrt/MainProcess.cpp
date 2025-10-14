/*
    MainProcess.cpp - CARRT-Pico's main function.  Runs the primary event loop
    and dispaches to other code as needed.

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




#include "MainProcess.h"

#include "BNO055.h"
#include "CarrtPicoDefines.h"
#include "CarrtPicoReset.h"
#include "Core1.h"
#include "Delays.h"
#include "EventManager.h"
#include "EventProcessor.h"
#include "HeartBeatLed.h"

#include "SerialCommands.h"
#include "SerialCommandProcessor.h"
#include "SerialLinkPico.h"
#include "CarrtError.h"

#include "PicoOutputUtils.hpp"
#include "PicoState.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/uart.h"




namespace MainProcess
{
    void runMainEventLoop( EventManager& events, EventProcessor& ep, SerialCommandProcessor& scp, SerialLinkPico& rpi0 );
    void checkForErrors( EventManager& events, SerialLinkPico& rpi0 );
    void doHouseKeeping();

    void doEventQueueOverflowed( SerialLinkPico& rpi0 );
    void doUnknownEvent( SerialLinkPico& rpi0, int eventCode );

    void doTestPicoReportError();
};




void MainProcess::runMainEventLoop( EventManager& events, EventProcessor& ep, SerialCommandProcessor& scp, SerialLinkPico& rpi0 )
{
    while ( 1 )
    {
        checkForErrors( events, rpi0 );
        ep.dispatchOneEvent( events, rpi0 );
        scp.dispatchOneSerialCommand( events, rpi0 );
        doHouseKeeping();
        // CarrtPico::sleep( 10ms );
    }
}


void MainProcess::checkForErrors( EventManager& events, SerialLinkPico& rpi0 )
{
    // Notionally a place to check for memory exhaustion, etc..

}


void MainProcess::doHouseKeeping()
{
    // Synch calibration and calibration in progress ()
    PicoState::calibrationInProgress( !PicoState::navCalibrated() );
}


void MainProcess::doEventQueueOverflowed( SerialLinkPico& link )
{
    output2cout( "Event queue overflowed" );

    int errCode{ makePicoErrorId( kPicoMainProcessError, 1, 0 ) };
    ErrorReportCmd errRpt( kPicoNonFatalError, errCode );
    errRpt.sendOut( link );
}


void MainProcess::doTestPicoReportError()
{
    output2cout( "Received test Pico error report cmd from RPi0" );
            
    throw CarrtError( makePicoErrorId( PicoError::kPicoTestError, 1, 0 ), "CARRT Pico test error sent by request" );
}



