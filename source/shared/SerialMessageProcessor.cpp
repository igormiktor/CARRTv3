/*
    SerialMessageProcessor.cpp - Master processor for Serial Messages for both 
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


#include "SerialMessageProcessor.h"


#include <map>
#include <string>
#include <memory>

#include "SerialMessage.h"
#include "SerialLink.h"




MessageFactory::MessageFactory( int reserveSize )
{
    mCreators.reserve( reserveSize );
}






SerialMessageProcessor::SerialMessageProcessor( int reserveSize, SerialLink& link )
: mFactory{ reserveSize }, mLink{ link }
{
    // Nothing else to do
}



SerialMessageProcessor::MsgPtr SerialMessageProcessor::createMessageFromSerialLink( MessageId id )
{
//    SerialMessage* MsgPtr = mFactory.createMessage( id );
//    auto msg = std::make_unique<SerialMessage>( MsgPtr );
    auto msg = mFactory.createMessage( id );
    msg->readIn( mLink );
    return  msg;
}



std::optional<SerialMessageProcessor::MsgPtr> SerialMessageProcessor::receiveMessageIfAvailable()
{
    auto msgId = mLink.getMsgType();
    if ( msgId )
    {
        return createMessageFromSerialLink( *msgId );
    }
    else
    {
        return std::nullopt; 
    }
}



void SerialMessageProcessor::dispatchOneSerialMessage( EventManager& events, SerialLink& link )
{
    auto msg{ receiveMessageIfAvailable() };
    if ( msg )
    {
        msg.value()->takeAction( events, link );
    }
}

