/*
    MainProcess.cpp - CARRT-Pico's main function.  Runs the primary event loop
    and dispaches to other code as needed.

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
#include "EventManager.h"
#include "Core1.h"
#include "shared/SerialCommand.h"
#include "shared/SerialLink.h"
#include "shared/CarrtError.h"

#include <iostream>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/uart.h"




namespace MainProcess
{
    void runMainEventLoop();
    void checkForErrors();
    void processEvent();
    void processCommand();
    void dispatchCommand( uint32_t cmd );
};




void MainProcess::runMainEventLoop()
{
    while ( 1 )
    {
        checkForErrors();
        processEvent();
        processCommand();
    }
}


void MainProcess::checkForErrors()
{
    // Notionally a place to check for memory exhaustion, etc..
}



void MainProcess::processEvent()
{
    int eventCode;
    int eventParam;

    if ( Events().getNextEvent( &eventCode, &eventParam ) )
    {
        switch ( eventCode )
        {
            case Event::kNavUpdateEvent:
                std::cout << "Nav " << eventParam << std::endl;
                break;
                
            case Event::kQuarterSecondTimerEvent:
                std::cout << "1/4 " << eventParam << std::endl;
                break;
                
            case Event::kOneSecondTimerEvent:
                std::cout << "1 s " << eventParam << std::endl;
                break;
                
            case Event::kEightSecondTimerEvent:
                std::cout << "8 s " << eventParam << std::endl;
                break;

            case Event::kIdentifyPicoCoreEvent:
                std::cout << "Core " << eventParam << std::endl;
        }
        if ( Events().hasEventQueueOverflowed() )
        {
            std::cout << "Event queue overflowed" << std::endl;
            SerialLink::putCmd( kErrorReportFromPico );
            SerialLink::putByte( kPicoNonFatalError );
            SerialLink::put( makePicoErrorId( PicoError::kPicoMainProcessError, 3, 0 ) );
            Events().resetEventQueueOverflowFlag();
        }
    }
}




void MainProcess::processCommand()
{
    if ( SerialLink::isReadable() )
    {
        Transfer t;
        uint8_t cmd = SerialLink::getCmd();

        dispatchCommand( cmd );
    }
}


void MainProcess::dispatchCommand( uint32_t cmd )
{
    // Placeholder
}
