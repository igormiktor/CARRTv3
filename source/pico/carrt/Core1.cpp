/*
    Core1.cpp - Code that runs on CARRT-Pico Core 1

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




#include "Core1.h"

#include "CarrtPicoDefines.h"
#include "EventManager.h"

#include "CarrtError.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/uart.h"



bool timerCallback( repeating_timer_t* ); 
void core1Main();



/*
*   To be called from Core0 
*
*   This code actually runs on Core0
*/

void launchCore1()
{
    multicore_launch_core1( core1Main );

    // Wait for it to start up
    uint32_t flag{ multicore_fifo_pop_blocking() };

    if ( flag != CORE1_SUCCESS )
    {
        throw CarrtError( makePicoErrorId( PicoError::kPicoMulticoreError, 1, 1 ), "CARRT Pico failed to start Core1" );
    }
}



// Code above here runs on Core0

// Code below here runs on Core1


/*
*   The main() for Core1 
*
*   This code actually runs on Core1
*/

void core1Main() 
{
    repeating_timer_t timer{};


    alarm_pool_t* core1AlarmPool = alarm_pool_create( TIMER_IRQ_2, 4 );
    if ( core1AlarmPool && alarm_pool_add_repeating_timer_ms( core1AlarmPool, -125, timerCallback, NULL, &timer ) )
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

        // Let core1 sleep, we're just processing timer callbacks...
        sleep_ms( 20 );
        // tight_loop_contents();       // Tight loop really isn't needed here
    }
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

    if ( ( eighthSecCount % 2 ) == 0 )
    {
        // Event parameter counts quarter seconds ( 0, 1, 2, 3 )
        Events().queueEvent( kQuarterSecondTimerEvent, ( (eighthSecCount / 2) % 4 ), timeTick );
    }

    if ( ( eighthSecCount % 8 ) == 0 )
    {
        // Event parameter counts seconds to 8 ( 0, 1, 2, 3, 4, 5, 6, 7 )
        Events().queueEvent( kOneSecondTimerEvent, ( eighthSecCount / 8 ), timeTick );
        // Events().queueEvent( Event::kPulsePicoLed, 0 );     // Don't need time tick
    }

    if ( eighthSecCount == 0 )
    {
        Events().queueEvent( kEightSecondTimerEvent, 0, timeTick );
    }

    return true;
}




