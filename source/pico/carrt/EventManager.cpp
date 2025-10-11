/*
    EventManager.cpp - An event handling system for CARRT-Pico.

    Copyright (c) 2023 Igor Mikolic-Torreira.  All right reserved.

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





#define EVENTMANAGER_EVENT_QUEUE_SIZE		24


#include "EventManager.h"

#include "pico/stdlib.h"
#include "pico/util/queue.h"


#if 0
#include "Utils/DebuggingMacros.h"



#if CARRT_ENABLE_EVENTMANAGER_DEBUG

#define EVTMGR_DEBUG_PRINT( x )             DEBUG_PRINT( x )
#define EVTMGR_DEBUG_PRINTLN( x )           DEBUG_PRINTLN( x )
#define EVTMGR_DEBUG_PRINT_P( x )           DEBUG_PRINT_P( x )
#define EVTMGR_DEBUG_PRINTLN_P( x )         DEBUG_PRINTLN_P( x )
#define EVTMGR_DEBUG_PRINT_PTR( x )         DEBUG_PRINT( reinterpret_cast<unsigned long>( x ), HEX );
#define EVTMGR_DEBUG_PRINTLN_PTR( x )       DEBUG_PRINTLN( reinterpret_cast<unsigned long>( x ), HEX );

#else

#define EVTMGR_DEBUG_PRINT( x )
#define EVTMGR_DEBUG_PRINTLN( x )
#define EVTMGR_DEBUG_PRINT_P( x )
#define EVTMGR_DEBUG_PRINTLN_P( x )
#define EVTMGR_DEBUG_PRINT_PTR( x )
#define EVTMGR_DEBUG_PRINTLN_PTR( x )

#endif  // CARRT_ENABLE_EVENTMANAGER_DEBUG
#endif  // #if 0




// Global EventManger (shared by core0 and core1)
// Accessed through function Events()

EventManager gEventManagerInstance;




EventManager::~EventManager()
{
    queue_free( &mHighPriorityQueue );
    queue_free( &mLowPriorityQueue ); 
}



EventManager::EventManager()
: mQueueOverflowOccurred{ false }
{
    queue_init( &mHighPriorityQueue, sizeof( Event ), EVENTMANAGER_EVENT_QUEUE_SIZE );
    queue_init( &mLowPriorityQueue, sizeof( Event ), EVENTMANAGER_EVENT_QUEUE_SIZE ); 
}



bool EventManager::getNextEvent( int* eventCode, int* param, uint32_t* time )
{
    Event e;

    // Try high-pri first; if no high-pri, event try low-pri
    if ( queue_try_remove( &mHighPriorityQueue, reinterpret_cast<void*>( &e ) )
        || queue_try_remove( &mLowPriorityQueue, reinterpret_cast<void*>( &e ) ) )
    {
        *eventCode = e.mCode;
        *param = e.mParam;
        if ( time )
        {
            *time = e.mTime;
        }
        return true;
    }

    return false;
}



void EventManager::reset()
{
    queue_free( &mHighPriorityQueue );
    queue_free( &mLowPriorityQueue ); 

    queue_init( &mHighPriorityQueue, sizeof( Event ), EVENTMANAGER_EVENT_QUEUE_SIZE );
    queue_init( &mLowPriorityQueue, sizeof( Event ), EVENTMANAGER_EVENT_QUEUE_SIZE ); 

    mQueueOverflowOccurred = false;
}



bool EventManager::isEventQueueEmpty( EventPriority pri )
{
    return ( pri == kHighPriority ) ? queue_is_empty( &mHighPriorityQueue ) : queue_is_empty( &mLowPriorityQueue );
}



bool EventManager::isEventQueueFull( EventPriority pri )
{
    return ( pri == kHighPriority ) ? queue_is_full( &mHighPriorityQueue ) : queue_is_full( &mLowPriorityQueue );
}



int EventManager::getNumEventsInQueue( EventPriority pri )
{
    return ( pri == kHighPriority ) ? queue_get_level( &mHighPriorityQueue ) : queue_get_level( &mLowPriorityQueue );
}



bool EventManager::queueEvent( int eventCode, int eventParam, uint32_t eventTime, EventPriority pri )
{
    Event e{ eventCode, eventParam, eventTime };

    // Don't block: caller deals with failure to add
    bool success = ( pri == kHighPriority ) ?
        queue_try_add( &mHighPriorityQueue, reinterpret_cast<void*>( &e ) ) 
        : queue_try_add( &mLowPriorityQueue, reinterpret_cast<void*>( &e ) );

    if ( !success )
    {
        // Queue overflow flag is intentionally "sicky": once on, stays on until reset
        // This allows us to check for this at our convenience
        mQueueOverflowOccurred = true;
    }

    return success;
}



bool EventManager::hasEventQueueOverflowed()
{
    return mQueueOverflowOccurred;
}



// cppcheck-suppress unusedFunction
void EventManager::resetEventQueueOverflowFlag()
{
    mQueueOverflowOccurred = false;
}





