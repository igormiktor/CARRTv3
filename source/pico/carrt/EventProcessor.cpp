/*
    EventProcessor.h - Implementation of class to register event handlers
    and process/dispatch  events accordingly.

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

#include "EventProcessor.h"

#include <utility>

#include "CarrtError.h"
#include "Clock.h"
#include "EventManager.h"
#include "OutputUtils.hpp"
#include "SerialMessages.h"

EventProcessor::EventProcessor( int reserveSize )
{
    mHandlers.reserve( reserveSize );
}

void EventProcessor::dispatchOneEvent( EventManager& events,
                                       SerialLink& link ) const
{
    EvtId eventCode;
    int eventParam;
    std::uint32_t eventTime;

    if ( events.getNextEvent( &eventCode, &eventParam, &eventTime ) )
    {
        auto handler{ mHandlers.find( std::to_underlying( eventCode ) ) };
        if ( handler == mHandlers.end() )
        {
            handleUnknownEvent( events, link, eventCode, eventParam,
                                eventTime );
        }
        else
        {
            handler->second->handleEvent( events, link, eventCode, eventParam,
                                          eventTime );
        }
    }
}

void EventProcessor::handleUnknownEvent( EventManager& events, SerialLink& link,
                                         EvtId eventCode, int eventParam,
                                         std::uint32_t eventTime ) const
{
    output2cout( "Warning: Pico received unknown event: ",
                 std::to_underlying( eventCode ) );

    int errCode{ makePicoErrorId( kPicoEventProcessorError, 1,
                                  std::to_underlying( eventCode ) ) };
    ErrorReportMsg errRpt( kPicoNonFatalError, errCode, Clock::millis() );
    errRpt.sendOut( link );
}
