/*
    EventClock.cpp - Runs CARRT's internal clock system.

    Copyright (c) 2016 Igor Mikolic-Torreira.  All right reserved.

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





#include "EventClock.h"

#include "EventManager.h"


#include <avr/interrupt.h>
#include <util/atomic.h>



// Select Timer2 or Timer5 to drive the Rover's internal clock

#if !defined( TRINKET_CLOCK_USE_TIMER2 ) && !defined( TRINKET_CLOCK_USE_TIMER1 )

#define TRINKET_CLOCK_USE_TIMER2

#endif





#ifdef TRINKET_CLOCK_USE_TIMER2


void EventClock::init()
{
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE )
    {
        // Clear Timer2 and configure it for Normal mode

        TCCR2A = 0;         // set entire TCCR2A register to 0
        TCCR2B = 0;         // same for TCCR2B
        TCNT2  = 0;         // initialize counter value to 0

        // Normal mode already set (bits WGM22, WGM21, and WGM20 cleared)

        // Set prescaler = 32 (set bits CS21 and CS20)
        TCCR2B |= (1 << CS21) | (1 << CS20);

        // Enable timer overflow interrupt
        TIMSK2 |= (1 << TOIE2);
    }
}



void EventClock::stop()
{
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE )
    {
        // Disable timer overflow interrupt
        TIMSK2 &= ~(1 << TOIE2);
    }
}




ISR( TIMER2_OVF_vect )
{
    // 1/8 second = 183 interrupts + count only 27 on the last interrupt

    static uint8_t interruptCount = -183;           // Yes, this does the right thing
    static uint8_t eighthSecCount = 0;

    ++interruptCount;

    // Count up from -183 to make the comparison that happens most the time a comparison to zero
    if ( interruptCount == 0 )
    {
        // On 184rd cycle, only count 27, not the full 256
        TCNT2 = 256 - 27;
    }
    else if ( interruptCount == 1 )
    {
        // Hit an eighth second
        interruptCount = -183;       // Reset the count of interrupts

#if 0

        // Slower but more explicit implementation, retained as documentation of the logic

        ++eighthSecCount;
        eighthSecCount %= 64;

        // Queue nav update events every 1/8 second
        // Event parameter counts eighth seconds ( 0, 1, 2, 3, 4, 5, 6, 7 )
        EventManager::queueEvent( EventManager::kNavUpdateEvent, eighthSecCount % 8, EventManager::kHighPriority );

        if ( ( eighthSecCount % 2 ) == 0 )
        {
            // Event parameter counts quarter seconds ( 0, 1, 2, 3 )
            EventManager::queueEvent( EventManager::kQuarterSecondTimerEvent, (eighthSecCount % 8) / 2 );
        }

        if ( ( eighthSecCount % 8 ) == 0 )
        {
            // Event parameter counts seconds to 8 ( 0, 1, 2, 3, 4, 5, 6, 7 )
            EventManager::queueEvent( EventManager::kOneSecondTimerEvent, ( eighthSecCount / 8 ) );
        }

        if ( eighthSecCount == 0 )
        {
            EventManager::queueEvent( EventManager::kEightSecondTimerEvent, 0 );
        }

#else

        ++eighthSecCount;
        eighthSecCount &= 0x3F;                         // x &= 0x3F ==  x modulo 64

        // Queue nav update events every 1/8 second
        // Event parameter counts eighth seconds ( 0, 1, 2, 3, 4, 5, 6, 7 )
        EventManager::queueEvent( EventManager::kEighthSecondTimerEvent, eighthSecCount & 0x07, EventManager::kHighPriority );

        if ( ( eighthSecCount & 0x01 ) == 0 )           // x & 0x01 == x modulo 2
        {
            // Event parameter counts quarter seconds ( 0, 1, 2, 3 )
            // Note:  (x & 0x07) >> 1 == (x % 8) / 2
            EventManager::queueEvent( EventManager::kQuarterSecondTimerEvent, (eighthSecCount & 0x07) >> 1 );
        }

        if ( ( eighthSecCount & 0x07 ) == 0 )           // x & 0x07 == x mod 8
        {
            // Event parameter counts seconds to 8 ( 0, 1, 2, 3, 4, 5, 6, 7 )
            // Note: (x >> 3) = x / 8
            EventManager::queueEvent( EventManager::kOneSecondTimerEvent, ( eighthSecCount >> 3 ) );
        }

        if ( eighthSecCount == 0 )
        {
            EventManager::queueEvent( EventManager::kEightSecondTimerEvent, 0 );
        }

#endif

    }
}


#endif







#ifdef TRINKET_CLOCK_USE_TIMER1


void EventClock::init()
{
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE )
    {
        // Set Timer1 for 8 Hz interrupts by using prescaler value 8 and
        // 2 overflows plus count to 56428 on the third count.

        // Clear Timer1 and configure it for Compare Match mode

        TCCR1A = 0;         // set entire TCCR1A register to 0
        TCCR1B = 0;         // same for TCCR1B
        TCNT1  = 0;         // initialize counter value to 0

        // Clear CS52 and set CS11 bit for prescaler value: 8
        TCCR1B |= (1 << CS11);

        // Enable timer overflow interrupt
        TIMSK1 |= (1 << TOIE1);
    }
}



void EventClock::stop()
{
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE )
    {
        // Disable timer overflow interrupt
        TIMSK1 &= ~(1 << TOIE1);
    }
}



ISR( TIMER1_OVF_vect )
{
    // 1/8 second = 2 interrupts + count only 56428 on the last interrupt

    static uint16_t interruptCount = -2;           // Yes, this does the right thing
    static uint8_t eighthSecCount = 0;

    ++interruptCount;

    // Count up from -2 to make the comparison that happens most the time a comparison to zero
    if ( interruptCount == 0 )
    {
        // On 3rdth cycle, only count 56428, not the full 65535
        TCNT2 = 65536 - 56428;
    }
    else if ( interruptCount == 1 )
    {
        // Hit an eighth second
        interruptCount = -2;       // Reset the count of interrupts
#if 0

    // Slower but more explicit implementation, retained as documentation of the logic

    ++eighthSecCount;
    eighthSecCount %= 64;

    // Queue nav update events every 1/8 second
    // Event parameter counts eighth seconds ( 0, 1, 2, 3, 4, 5, 6, 7 )
    EventManager::queueEvent( EventManager::kNavUpdateEvent, eighthSecCount % 8, EventManager::kHighPriority );

    if ( ( eighthSecCount % 2 ) == 0 )
    {
        // Event parameter counts quarter seconds ( 0, 1, 2, 3 )
        EventManager::queueEvent( EventManager::kQuarterSecondTimerEvent, (eighthSecCount % 8) / 2 );
    }

    if ( ( eighthSecCount % 8 ) == 0 )
    {
        // Event parameter counts seconds to 8 ( 0, 1, 2, 3, 4, 5, 6, 7 )
        EventManager::queueEvent( EventManager::kOneSecondTimerEvent, ( eighthSecCount / 8 ) );
    }

    if ( eighthSecCount == 0 )
    {
        EventManager::queueEvent( EventManager::kEightSecondTimerEvent, 0 );
    }

#else

    ++eighthSecCount;
    eighthSecCount &= 0x3F;                         // x &= 0x3F ==  x modulo 64

    // Queue nav update events every 1/8 second
    // Event parameter counts eighth seconds ( 0, 1, 2, 3, 4, 5, 6, 7 )
    EventManager::queueEvent( EventManager::kEighthSecondTimerEvent, eighthSecCount & 0x07, EventManager::kHighPriority );

    if ( ( eighthSecCount & 0x01 ) == 0 )           // x & 0x01 == x modulo 2
    {
        // Event parameter counts quarter seconds ( 0, 1, 2, 3 )
        // Note:  (x & 0x07) >> 1 == (x % 8) / 2
        EventManager::queueEvent( EventManager::kQuarterSecondTimerEvent, (eighthSecCount & 0x07) >> 1 );
    }

    if ( ( eighthSecCount & 0x07 ) == 0 )           // x & 0x07 == x mod 8
    {
        // Event parameter counts seconds to 8 ( 0, 1, 2, 3, 4, 5, 6, 7 )
        // Note: (x >> 3) = x / 8
        EventManager::queueEvent( EventManager::kOneSecondTimerEvent, ( eighthSecCount >> 3 ) );
    }

    if ( eighthSecCount == 0 )
    {
        EventManager::queueEvent( EventManager::kEightSecondTimerEvent, 0 );
    }

#endif

}



#endif



