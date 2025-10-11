/*
    Core1.cpp - Code that runs on CARRT-Pico Core 1

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




#include "Core1.h"

#include "CarrtPicoDefines.h"
#include "EventManager.h"
#include "Delays.h"

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




namespace
{
    struct EventForCore1
    {
        int     kind;
        int     param;
    };

    queue_t sCore0toCore1Events{};
    alarm_pool_t* sCore1AlarmPool{ nullptr };

    std::int64_t alarmCallback( alarm_id_t alarm, void* userData );
    bool timerCallback( repeating_timer_t* ); 
    void core1Main();
}



/*
*   To be called from Core0
*
*   This code actually runs on Core0
*/

void Core1::launchCore1()
{
    queue_init( &sCore0toCore1Events, sizeof( EventForCore1 ), SIZE_OF_CORE0_TO_CORE1_QUEUE );

    multicore_launch_core1( core1Main );

    // Wait for it to start up
    uint32_t flag{ multicore_fifo_pop_blocking() };

    if ( flag != CORE1_SUCCESS )
    {
        throw CarrtError( makePicoErrorId( PicoError::kPicoMulticoreError, 1, 1 ), "CARRT Pico failed to start Core1" );
    }
}



void Core1::queueEventForCore1( int event, int waitMs )
{
    EventForCore1 evt{ .kind = event, .param = waitMs };
    if ( !queue_try_add( &sCore0toCore1Events, &evt ) )
    {
        // These get added very rarely, so impossible to have a full queue unless something else is very wrong
        throw CarrtError( makePicoErrorId( PicoError::kPicoMulticoreError, 1, 2 ), "CARRT Pico failed to post to Core1 queue" );
    }
}



// Code above here runs on Core0

///////////////////////////////////////////////////////////////////////////////////////////////

// Code below here runs on Core1



namespace
{

    /*
    *   The main() for Core1 
    *
    *   This code actually runs on Core1
    */

    void core1Main() 
    {
        repeating_timer_t timer{};

        sCore1AlarmPool = alarm_pool_create( TIMER_IRQ_2, 4 );
        if ( sCore1AlarmPool && alarm_pool_add_repeating_timer_ms( sCore1AlarmPool, -125, timerCallback, NULL, &timer ) )
        {
            // Success; send the success code
            multicore_fifo_push_blocking( CORE1_SUCCESS );
        }
        else
        {
            // Failure; send the failure code
            multicore_fifo_push_blocking( CORE1_FAILURE );

            // Pico core0 will report the error to RPi0 
            // RPi0 will do whatever is appropriate with it (potentially cycle power to Pico)
        }

        while ( 1 )
        {
            if ( queue_is_empty( &sCore0toCore1Events ) )
            {
                // Let Core1 sleep, Core1 is just processing timer/alarm callbacks and interrupts...
                CarrtPico::sleep( 10ms );
            }
            else
            {
                EventForCore1 evt{ 0, 0 };
                queue_remove_blocking( &sCore0toCore1Events, &evt );
                switch( evt.kind )
                {
                    case kBNO055InitDelay: 
                        alarm_pool_add_alarm_in_ms( sCore1AlarmPool, static_cast<std::uint32_t>( evt.param ), alarmCallback, reinterpret_cast<void *>( kBeginCalibrationEvent ), true );

                    default:
                        break;
                }
            }
//          tight_loop_contents();       // Tight loop really isn't needed here
        }
    }



    std::int64_t alarmCallback( alarm_id_t, void* userData )
    {
        // Argument isn't used

        int eventType{ reinterpret_cast<int>( userData ) };
        Events().queueEvent( eventType );
        return 0;
    }



    bool timerCallback( repeating_timer_t* ) 
    {
        // Argument is not used...

        static int eighthSecCount{ 0 };

        uint32_t timeTick{ to_ms_since_boot( get_absolute_time() ) };

        ++eighthSecCount;
        eighthSecCount %= 64;

        // Queue nav update events every 1/8 second
        // Event parameter counts eighth seconds ( 0, 1, 2, 3, 4, 5, 6, 7 )
        Events().queueEvent( kNavUpdateEvent, eighthSecCount % 8, timeTick, EventManager::kHighPriority );

        // Quarter second events
        if ( ( eighthSecCount % 2 ) == 0 )
        {
            // Event parameter counts quarter seconds ( 0, 1, 2, 3 )
            Events().queueEvent( kQuarterSecondTimerEvent, ( (eighthSecCount / 2) % 4 ), timeTick );
        }

        // 1 second events
        if ( ( eighthSecCount % 8 ) == 0 )
        {
            // Event parameter counts seconds to 8 ( 0, 1, 2, 3, 4, 5, 6, 7 )
            Events().queueEvent( kOneSecondTimerEvent, ( eighthSecCount / 8 ), timeTick );
            Events().queueEvent( kPulsePicoLedEvent );

            if ( PicoState::calibrationInProgress() )
            {
                Events().queueEvent(  kSendCalibrationInfoEvent );
            }
        }

        // 8 second events
        if ( eighthSecCount == 0 )
        {
            Events().queueEvent( kEightSecondTimerEvent, 0, timeTick );
            Events().queueEvent( kSendCalibrationInfoEvent );
        }

        return true;
    }


}

