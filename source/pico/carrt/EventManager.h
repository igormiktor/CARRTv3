/*
    EventManager.h - The event management module for CARRT-Pico

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






#ifndef EventManager_h
#define EventManager_h

#include <stdint.h>

#include "Event.h"      // This is where events themselves are defined

#include "pico/stdlib.h"
#include "pico/util/queue.h"




class EventManager
{

public:

    // EventManager recognizes two kinds of events.  By default, events are
    // are queued as low priority, but these constants can be used to explicitly
    // set the priority when queueing events
    //
    // NOTE high priority events are always handled before any low priority events.
    enum EventPriority { kLowPriority, kHighPriority };

    // Constructors, destructors, and special members
    ~EventManager();
    EventManager();
    EventManager( const EventManager& ) = delete;               // Prevent copy
    EventManager& operator=(const EventManager& ) = delete;     // Prevent copy

    // Reset event manager by reseting (purging) queues and clearing overflow flag
    void reset();

    // Returns true if no events are in the queue
    bool isEventQueueEmpty( EventPriority pri = kLowPriority );

    // Returns true if no more events can be inserted into the queue
    bool isEventQueueFull( EventPriority pri = kLowPriority );

    // Actual number of events in queue
    int getNumEventsInQueue( EventPriority pri = kLowPriority );

    // Tries to insert an event into the queue;
    // returns true if successful, false if the
    // queue if full and the event cannot be inserted
    bool queueEvent( int eventCode, int eventParam = 0, uint32_t eventTime = 0, EventPriority pri = kLowPriority );

    // This function returns the next event
    bool getNextEvent( int* eventCode, int* eventParam, uint32_t* eventTime = NULL );

    // Has the event queue overflowed?
    bool hasEventQueueOverflowed();

    // Reset the event queue overflow flag
    void resetEventQueueOverflowFlag();


private:

    // What events look like
    struct Event
    {
        int         mCode;
        int         mParam;
        uint32_t    mTime;
    };

    queue_t  mHighPriorityQueue;
    queue_t  mLowPriorityQueue;

    bool mQueueOverflowOccurred;
};



inline EventManager& Events()
{
    extern EventManager gEventManagerInstance;

    return gEventManagerInstance;
}


#endif
