/*
    EventProcessor.h - A class to register event handlers and process/dispatch
    events accordingly.

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


#ifndef EventProcessor_h
#define EventProcessor_h

#include <memory>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include "CarrtError.h"
#include "Event.h"
#include "EventHandler.h"



class EventProcessor
{
public:

    using EventHandlerPtr = typename std::unique_ptr<EventHandler>;

    EventProcessor( int reserveSize );

    ~EventProcessor() = default;
    
    EventProcessor( const EventProcessor& ) = delete;
    EventProcessor( EventProcessor&& ) = delete;
    EventProcessor& operator=( const EventProcessor& ) = delete;
    EventProcessor& operator=( EventProcessor&& ) = delete;

    void dispatchOneEvent( EventManager& events, SerialLink& link ) const;

    template <typename T> 
    void registerHandler( EvtId id ) 
    {
        static_assert( std::is_base_of<EventHandler, T>::value, "EventProcessor::registerHandler: handlers must derive from EventHandler" );
        int idNum = std::to_underlying( id );
        if ( mHandlers.find( idNum ) != mHandlers.end() )
        {
            throw CarrtError( makeSharedErrorId( kEventHandlerDupeError, 1, idNum ), "Id dupe at event registation" );
        }
        mHandlers[idNum] = std::make_unique<T>();
    }

    // Useful in case T constructor needs arguments
    template <typename T> 
    void registerHandler( EvtId id, T* ptr )
    {
        static_assert( std::is_base_of<EventHandler, T>::value, "EventProcessor::registerHandler: handlers must derive from EventHandler" );
        int idNum = std::to_underlying( id );
        if ( mHandlers.find( idNum ) != mHandlers.end() )
        {
            throw CarrtError( makeSharedErrorId( kEventHandlerDupeError, 2, idNum ), "Id dupe at event registation" );
        }
        mHandlers[idNum] = std::unique_ptr<T>( ptr );
    }



private:

    void handleUnknownEvent( EventManager& events, SerialLink& link, EvtId eventCode, int eventParam, std::uint32_t eventTime ) const;

    std::unordered_map<int, EventHandlerPtr> mHandlers;
};




#endif  // EventProcessor_h
