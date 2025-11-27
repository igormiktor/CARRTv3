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
#include "Clock.h"
#include "EventManager.h"
#include "EventProcessor.h"
#include "HeartBeatLed.h"

#include "SerialMessages.h"
#include "SerialMessageProcessor.h"
#include "SerialLinkPico.h"
#include "CarrtError.h"

#include "OutputUtils.hpp"
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
    void runMainEventLoop( EventManager& events, EventProcessor& ep, SerialMessageProcessor& smp, SerialLinkPico& rpi0 );
    void checkForErrors( EventManager& events, SerialLinkPico& rpi0 );
    void doHouseKeeping( EventManager& events, SerialLinkPico& rpi0 );

    void doEventQueueOverflowed( SerialLinkPico& rpi0 );
    void doUnknownEvent( SerialLinkPico& rpi0, int eventCode );

    void doTestPicoReportError();
};




void MainProcess::runMainEventLoop( EventManager& events, EventProcessor& ep, SerialMessageProcessor& smp, SerialLinkPico& rpi0 )
{
    while ( 1 )
    {
        checkForErrors( events, rpi0 );
        ep.dispatchOneEvent( events, rpi0 );
        smp.dispatchOneSerialMessage( events, rpi0 );
        if ( PicoState::startUpFinished() )
        {
            doHouseKeeping( events, rpi0 );
        }
        // Clock::sleep( 10ms );
    }
}


void MainProcess::checkForErrors( EventManager& events, SerialLinkPico& rpi0 )
{
    // Notionally a place to check for memory exhaustion, etc..

}


void MainProcess::doHouseKeeping( EventManager& events, SerialLinkPico& rpi0 )
{
    // Synch calibration and calibration in progress
    if ( PicoState::navCalibrated() )
    {
        // End calibration process
        PicoState::calibrationInProgress( false );
    }
    else if ( PicoState::wantAutoCalibrate() && !PicoState::calibrationInProgress() )
    {
        // Trigger new calibration
        events.queueEvent( EvtId::kBNO055BeginCalibrationEvent );
    }
}


void MainProcess::doEventQueueOverflowed( SerialLinkPico& link )
{
    output2cout( "Event queue overflowed" );

    int errCode{ makePicoErrorId( kPicoMainProcessError, 1, 0 ) };
    ErrorReportMsg errRpt( kPicoNonFatalError, errCode );
    errRpt.sendOut( link );
}


void MainProcess::doTestPicoReportError()
{
    output2cout( "Received test Pico error report msg from RPi0" );
            
    throw CarrtError( makePicoErrorId( kPicoTestError, 1, 1 ), "CARRT Pico test error sent by request" );
}



