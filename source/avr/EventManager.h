/*
 * EventManager.h
 *

 * An event handling system for avr microcontrollers.
 *
 * Author: igormt@alumni.caltech.edu
 * Copyright (c) 2017 Igor Mikolic-Torreira
 *
 * This library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser
 * General Public License along with this library; if not,
 * write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */


#ifndef EventManager_h
#define EventManager_h

#include <stdint.h>

// Size of the listener list.  Adjust as appropriate for your application.
// Requires a total of sizeof(*f())+sizeof(int)+sizeof(bool) bytes of RAM for each unit of size
#ifndef EVENTMANAGER_LISTENER_LIST_SIZE
#define EVENTMANAGER_LISTENER_LIST_SIZE		8
#endif

// Size of the event two queues.  Adjust as appropriate for your application.
// Requires a total of 4 * sizeof(int) bytes of RAM for each unit of size
#ifndef EVENTMANAGER_EVENT_QUEUE_SIZE
#define EVENTMANAGER_EVENT_QUEUE_SIZE		8
#endif



namespace EventManager
{

    enum
    {
        kEventNone,
        kEighthSecondTimerEvent,
        kQuarterSecondTimerEvent,
        kOneSecondTimerEvent,
        kEightSecondTimerEvent
    };
};





namespace EventManager
{

    // Type for an event listener (a.k.a. callback) function
    typedef void ( *EventListener )( int eventCode, int eventParam );

    // EventManager recognizes two kinds of events.  By default, events are
    // are queued as low priority, but these constants can be used to explicitly
    // set the priority when queueing events
    //
    // NOTE high priority events are always handled before any low priority events.
    enum EventPriority { kHighPriority, kLowPriority };


    // Add a listener
    // Returns true if the listener is successfully installed, false otherwise (e.g. the dispatch table is full)
    bool addListener( int eventCode, EventListener listener );

    // Remove (event, listener) pair (all occurrences)
    // Other listeners with the same function or event code will not be affected
    bool removeListener( int eventCode, EventListener listener );

    // Remove all occurrances of a listener
    // Removes this listener regardless of the event code; returns number removed
    // Useful when one listener handles many different events
    int removeListener( EventListener listener );

    // Enable or disable a listener
    // Return true if the listener was successfully enabled or disabled, false if the listener was not found
    bool enableListener( int eventCode, EventListener listener, bool enable );

    // Returns the current enabled/disabled state of the (eventCode, listener) combo
    bool isListenerEnabled( int eventCode, EventListener listener );

    // The default listener is a callback function that is called when an event with no listener is processed
    // These functions set, clear, and enable/disable the default listener
    bool setDefaultListener( EventListener listener );
    void removeDefaultListener();
    void enableDefaultListener( bool enable );

    // Is the ListenerList empty?
    bool isListenerListEmpty();

    // Is the ListenerList full?
    bool isListenerListFull();

    int numListeners();

    // Returns true if no events are in the queue
    bool isEventQueueEmpty( EventPriority pri = kLowPriority );

    // Returns true if no more events can be inserted into the queue
    bool isEventQueueFull( EventPriority pri = kLowPriority );

    // Actual number of events in queue
    int getNumEventsInQueue( EventPriority pri = kLowPriority );

    // tries to insert an event into the queue;
    // returns true if successful, false if the
    // queue if full and the event cannot be inserted
    bool queueEvent( int eventCode, int eventParam, EventPriority pri = kLowPriority );

    // this must be called regularly (usually by calling it inside the loop() function)
    int processEvent();

    // this function can be called to process ALL events in the queue
    // WARNING:  if interrupts are adding events as fast as they are being processed
    // this function might never return.  YOU HAVE BEEN WARNED.
    int processAllEvents();

};




#endif
