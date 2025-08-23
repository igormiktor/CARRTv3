/*
    SerialCommandProcessor.cpp - Master processor for Serial Commands for both 
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


#include "SerialCommandProcessor.h"


#include <map>
#include <string>
#include <memory>

#include "SerialCommand.h"
#include "SerialLink.h"






SerialCommandProcessor::SerialCommandProcessor( SerialLink& link )
: mLink{ link }
{
    // Nothing else to do
}



SerialCommandProcessor::CmdPtr SerialCommandProcessor::createCommandFromSerialLink( CommandId id )
{
//    SerialCommand* cmdPtr = mFactory.createCommand( id );
//    auto cmd = std::make_unique<SerialCommand>( cmdPtr );
    auto cmd = mFactory.createCommand( id );
    cmd->readIn( mLink );
    return  cmd;
}



std::optional<SerialCommandProcessor::CmdPtr> SerialCommandProcessor::receiveCommandIfAvailable()
{
    auto cmdId = mLink.getMsgType();
    if ( cmdId )
    {
        return createCommandFromSerialLink( *cmdId );
    }
    else
    {
        return std::nullopt; 
    }
}
