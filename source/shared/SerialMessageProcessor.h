/*
    SerialMessageProcessor.h - Master processor for Serial Messages for both 
    the RPI and Pico.  This file is shared by both the RPI and Pico code bases.

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


#ifndef SerialMessageProcessor_h
#define SerialMessageProcessor_h



#include <unordered_map>
#include <string>
#include <memory>

#include "SerialMessage.h"
#include "CarrtError.h"




class MessageFactory
{
public:

    using MsgPtr = typename std::unique_ptr<SerialMessage>;

    MessageFactory( int reserveSize );
    ~MessageFactory() = default;
    
    MessageFactory( const MessageFactory& ) = delete;
    MessageFactory( MessageFactory&& ) = delete;
    MessageFactory& operator=( const MessageFactory& ) = delete;
    MessageFactory& operator=( MessageFactory&& ) = delete;


    template <typename T>
    void registerMessage( MessageId id )
    {
        static_assert( std::is_base_of<SerialMessage, T>::value, "MessageFactory::registerMessage(): Messages must derive from SerialMessage" );
        if ( mCreators.find( id ) != mCreators.end() )
        {
            throw CarrtError( makeSharedErrorId( kSerialMsgDupeError, 1, id ), "Id dupe at registation" );
        }
        mCreators[id] = &creator<T>;
    }

    MsgPtr createMessage( MessageId id ) 
    {
        auto it = mCreators.find( id );
        if ( it != mCreators.end() )  
        {
            return it->second( id );
        }
        // If we cannot find the id, return a special message, UnknownMsg.
        int err = makeSharedErrorId( kSerialMsgUnknownError, 1, id );
        return std::unique_ptr<SerialMessage>( new UnknownMsg(  id, err ) );
    }


private:

    template <typename T>
    static MsgPtr creator( MessageId id )
    {
        // All SerialMessages must have a constructor that takes a uint8_t parameter (the ID)
        return MsgPtr( new T( id ) );
    }

    typedef MsgPtr (*PCreator)( MessageId );
    std::unordered_map<std::uint8_t, PCreator> mCreators;
};



class EventManager;

class SerialMessageProcessor
{
public:

    using MsgPtr = typename MessageFactory::MsgPtr;

    SerialMessageProcessor( int reserveSize, SerialLink& link );

    ~SerialMessageProcessor() = default;
    
    SerialMessageProcessor( const SerialMessageProcessor& ) = delete;
    SerialMessageProcessor( SerialMessageProcessor&& ) = delete;
    SerialMessageProcessor& operator=( const SerialMessageProcessor& ) = delete;
    SerialMessageProcessor& operator=( SerialMessageProcessor&& ) = delete;

    void dispatchOneSerialMessage( EventManager& events, SerialLink& link );

    std::optional<MsgPtr> receiveMessageIfAvailable();

    template <typename T> 
    void registerMessage( MessageId id )
    {
        mFactory.registerMessage<T>( id );
    }



private:

    inline MsgPtr createMessageFromSerialLink( std::uint8_t id ) { return createMessageFromSerialLink( static_cast<MessageId>( id ) ); }
    MsgPtr createMessageFromSerialLink( MessageId id );


    MessageFactory      mFactory;
    SerialLink&         mLink;


};






#endif      // SerialMessageProcessor_h