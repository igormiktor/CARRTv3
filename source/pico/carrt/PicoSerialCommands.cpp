/*
    PicoSerialCommands.cpp - Serial Commands for CARRT3 communications
    between the RPI and Pico.  This file contains the Pico commands.

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




#include "PicoSerialCommands.h"

#include "CarrtError.h"


DebugLinkCommand::DebugLinkCommand() noexcept 
: SerialCommand( kDebugSerialLink ), mDebugData( kDebugSerialLink ), mNeedsAction{ false } 
{}

DebugLinkCommand::DebugLinkCommand( DebugData t ) noexcept 
: SerialCommand( kDebugSerialLink ), mDebugData( kDebugSerialLink, t ) 
{} 

DebugLinkCommand::DebugLinkCommand( std::uint8_t val1, std::uint8_t val2 ) noexcept 
: SerialCommand( kDebugSerialLink ), mDebugData( kDebugSerialLink, std::make_tuple( val1, val2 ) ) 
{}

DebugLinkCommand::DebugLinkCommand( CommandId id ) 
: SerialCommand( id ), mDebugData( kDebugSerialLink ), mNeedsAction{ false }
{ 
    if ( id != kDebugSerialLink ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialCommandError, 1, kDebugSerialLink ), "Id mismatch at creation" ); 
    } 
}


/*
void DebugLinkCommand::readIn( SerialLink& link ) 
{
    mDebugData.readIn( link );
    mNeedsAction = true;
}
*/


void DebugLinkCommand::sendOut( SerialLink& link )
{
    mDebugData.sendOut( link );
}



void DebugLinkCommand::takeAction( SerialLink& link ) 
{
    sendOut( link );
    mNeedsAction = false;
}






