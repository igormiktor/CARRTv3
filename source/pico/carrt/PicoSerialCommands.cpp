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




#include "SerialCommands.h"

#include "CarrtError.h"








/*********************************************************************************************/




TimerEventCmd::TimerEventCmd() noexcept 
: SerialCommand( kTimerEvent ), mTheData( kTimerEvent ), mNeedsAction{ false } 
{}

TimerEventCmd::TimerEventCmd( TheData t ) noexcept 
: SerialCommand( kTimerEvent ), mTheData( kTimerEvent, t ), mNeedsAction{ true }
{} 

TimerEventCmd::TimerEventCmd( std::uint8_t which, int count ) noexcept 
: SerialCommand( kTimerEvent ), mTheData( kTimerEvent, std::make_tuple( which, count ) ), mNeedsAction{ true } 
{}

TimerEventCmd::TimerEventCmd( CommandId id ) 
: SerialCommand( id ), mTheData( kTimerEvent ), mNeedsAction{ false }
{ 
    if ( id != kTimerEvent ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialCommandError, 1, kTimerEvent ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}



void TimerEventCmd::readIn( SerialLink& link ) 
{
    mTheData.readIn( link );
    mNeedsAction = true;
}



void TimerEventCmd::sendOut( SerialLink& link )
{
    mTheData.sendOut( link );
}



void TimerEventCmd::takeAction( SerialLink& link ) 
{
    // Only action is to send it
    sendOut( link );
    mNeedsAction = false;
}




/*********************************************************************************************/




TimerControlCmd::TimerControlCmd() noexcept 
: SerialCommand( kTimerControl ), mTheData( kTimerControl ), mNeedsAction{ false } 
{}

TimerControlCmd::TimerControlCmd( TheData t ) noexcept 
: SerialCommand( kTimerControl ), mTheData( kTimerControl, t ), mNeedsAction{ true } 
{} 

TimerControlCmd::TimerControlCmd( bool val ) noexcept 
: SerialCommand( kTimerControl ), mTheData( kTimerControl, std::make_tuple( static_cast<std::uint8_t>( val ) ) ), mNeedsAction{ true } 
{}

TimerControlCmd::TimerControlCmd( CommandId id ) 
: SerialCommand( id ), mTheData( kTimerControl ), mNeedsAction{ false }
{ 
    if ( id != kTimerControl ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialCommandError, 1, kTimerControl ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}



void TimerControlCmd::readIn( SerialLink& link ) 
{
    mTheData.readIn( link );
    mNeedsAction = true;
}



void TimerControlCmd::sendOut( SerialLink& link )
{
    mTheData.sendOut( link );
}



void TimerControlCmd::takeAction( SerialLink& link ) 
{
    // This is a kluge for testing only
    extern bool gSendTimerEvents;

    gSendTimerEvents = std::get<0>( mTheData.mData );
    
    sendOut( link );
    mNeedsAction = false;
}




/*********************************************************************************************/




ErrorReportCmd::ErrorReportCmd() noexcept 
: SerialCommand( kErrorReportFromPico ), mTheData( kErrorReportFromPico ), mNeedsAction{ false } 
{}

ErrorReportCmd::ErrorReportCmd( TheData t ) noexcept 
: SerialCommand( kErrorReportFromPico ), mTheData( kErrorReportFromPico, t ), mNeedsAction{ true } 
{} 

ErrorReportCmd::ErrorReportCmd( bool val, int errorCode ) noexcept 
: SerialCommand( kErrorReportFromPico ), mTheData( kErrorReportFromPico, std::make_tuple( static_cast<std::uint8_t>( val ), errorCode ) ), 
    mNeedsAction{ true } 
{}

ErrorReportCmd::ErrorReportCmd( CommandId id ) 
: SerialCommand( id ), mTheData( kErrorReportFromPico ), mNeedsAction{ false }
{ 
    if ( id != kErrorReportFromPico ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialCommandError, 1, kErrorReportFromPico ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}



void ErrorReportCmd::readIn( SerialLink& link ) 
{
    mTheData.readIn( link );
    mNeedsAction = true;
}



void ErrorReportCmd::sendOut( SerialLink& link )
{
    mTheData.sendOut( link );
}



void ErrorReportCmd::takeAction( SerialLink& link ) 
{
    // Only action is to send the message out
    sendOut( link );
    mNeedsAction = false;
}




/*********************************************************************************************/




DebugLinkCmd::DebugLinkCmd() noexcept 
: SerialCommand( kDebugSerialLink ), mTheData( kDebugSerialLink ), mNeedsAction{ false } 
{}

DebugLinkCmd::DebugLinkCmd( TheData t ) noexcept 
: SerialCommand( kDebugSerialLink ), mTheData( kDebugSerialLink, t ), mNeedsAction{ true } 
{} 

DebugLinkCmd::DebugLinkCmd( int val1, int val2 ) noexcept 
: SerialCommand( kDebugSerialLink ), mTheData( kDebugSerialLink, std::make_tuple( val1, val2 ) ), mNeedsAction{ true } 
{}

DebugLinkCmd::DebugLinkCmd( CommandId id ) 
: SerialCommand( id ), mTheData( kDebugSerialLink ), mNeedsAction{ false }          
{ 
    if ( id != kDebugSerialLink ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialCommandError, 1, kDebugSerialLink ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}



void DebugLinkCmd::readIn( SerialLink& link ) 
{
    mTheData.readIn( link );
    mNeedsAction = true;
}



void DebugLinkCmd::sendOut( SerialLink& link )
{
    mTheData.sendOut( link );
}



void DebugLinkCmd::takeAction( SerialLink& link ) 
{
    // Lets negate the values, flip the order, and send them back
    auto [ val1, val2 ] = mTheData.mData;
    val1 *= -1;
    val2 *= -1;
    mTheData.mData = std::make_tuple( val2, val1 );
    sendOut( link );
    mNeedsAction = false;
}




/*********************************************************************************************/




UnknownCmd::UnknownCmd() noexcept 
: SerialCommand( kUnknownCommand ), mTheData( kUnknownCommand ), mNeedsAction{ false } 
{}

UnknownCmd::UnknownCmd( TheData t ) noexcept 
: SerialCommand( kUnknownCommand ), mTheData( kUnknownCommand, t ), mNeedsAction{ true } 
{} 

UnknownCmd::UnknownCmd( std::uint8_t rcvdId, int errorCode ) noexcept 
: SerialCommand( kUnknownCommand ), mTheData( kUnknownCommand, std::make_tuple( rcvdId, errorCode ) ), 
    mNeedsAction{ true } 
{}

UnknownCmd::UnknownCmd( CommandId id ) 
: SerialCommand( id ), mTheData( kUnknownCommand ), mNeedsAction{ false }
{ 
    if ( id != kUnknownCommand ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialCommandError, 1, kUnknownCommand ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}



void UnknownCmd::readIn( SerialLink& link ) 
{
    // Unknown command; don't try to read it in.
}



void UnknownCmd::sendOut( SerialLink& link )
{
    // We don't send this Cmd out on link; instead send error report
    ErrorReportCmd errCmd( false, std::get<1>( mTheData.mData ) );
    errCmd.takeAction( link );
}



void UnknownCmd::takeAction( SerialLink& link ) 
{
    // Only action is to send the message out
    sendOut( link );
    mNeedsAction = false;
}




/*********************************************************************************************/

