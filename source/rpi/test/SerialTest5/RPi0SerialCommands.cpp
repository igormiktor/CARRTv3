/*
    RPi0SerialCommands.cpp - Serial Commands for CARRT3 communications
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

#include <iostream>

#include "CarrtError.h"

#include "DebugUtils.hpp"







/*********************************************************************************************/




TimerEventCmd::TimerEventCmd() noexcept 
: SerialCommand( kTimerEvent ), mContent( kTimerEvent ), mNeedsAction{ false } 
{}

TimerEventCmd::TimerEventCmd( TheData t ) noexcept 
: SerialCommand( kTimerEvent ), mContent( kTimerEvent, t ), mNeedsAction{ true }
{} 

TimerEventCmd::TimerEventCmd( std::uint8_t which, int count ) noexcept 
: SerialCommand( kTimerEvent ), mContent( kTimerEvent, std::make_tuple( which, count ) ), mNeedsAction{ true } 
{}

TimerEventCmd::TimerEventCmd( CommandId id ) 
: SerialCommand( id ), mContent( kTimerEvent ), mNeedsAction{ false }
{ 
    if ( id != kTimerEvent ) 
    { 
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialCommandError, 1, kTimerEvent ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}



void TimerEventCmd::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
}



void TimerEventCmd::sendOut( SerialLink& link )
{
    // RPi0 never sends these out; only receives
}



void TimerEventCmd::takeAction( SerialLink& link ) 
{
    // For the test just print out
    auto [ kind, count ] = mContent.mMsg;
    std::cout << "Timer msg ";
    switch ( kind )
    {
        case k1QuarterSecondEvent:
            std::cout << "1/4 ";
            break;

        case k1SecondEvent:
            std::cout << "1 ";
            break;

        case k8SecondEvent: 
            std::cout << "8 ";

        default:
            std::cout << "?? " << kind << " ?? ";
            break;
    }
    std::cout << "sec; count " << count << std::endl;
    mNeedsAction = false;
}





/*********************************************************************************************/




TimerControlCmd::TimerControlCmd() noexcept 
: SerialCommand( kTimerControl ), mContent( kTimerControl ), mNeedsAction{ false } 
{}

TimerControlCmd::TimerControlCmd( TheData t ) noexcept 
: SerialCommand( kTimerControl ), mContent( kTimerControl, t ), mNeedsAction{ true } 
{} 

TimerControlCmd::TimerControlCmd( bool val ) noexcept 
: SerialCommand( kTimerControl ), mContent( kTimerControl, std::make_tuple( static_cast<std::uint8_t>( val ) ) ), mNeedsAction{ true } 
{}

TimerControlCmd::TimerControlCmd( CommandId id ) 
: SerialCommand( id ), mContent( kTimerControl ), mNeedsAction{ false }
{ 
    if ( id != kTimerControl ) 
    { 
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialCommandError, 1, kTimerControl ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}



void TimerControlCmd::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
}



void TimerControlCmd::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}



void TimerControlCmd::takeAction( SerialLink& link ) 
{
    // RPi0 action is to send it out...
    sendOut( link );
    mNeedsAction = false;
}




/*********************************************************************************************/




ErrorReportCmd::ErrorReportCmd() noexcept 
: SerialCommand( kErrorReportFromPico ), mContent( kErrorReportFromPico ), mNeedsAction{ false } 
{}

ErrorReportCmd::ErrorReportCmd( TheData t ) noexcept 
: SerialCommand( kErrorReportFromPico ), mContent( kErrorReportFromPico, t ), mNeedsAction{ true } 
{} 

ErrorReportCmd::ErrorReportCmd( bool val, int errorCode ) noexcept 
: SerialCommand( kErrorReportFromPico ), mContent( kErrorReportFromPico, std::make_tuple( static_cast<std::uint8_t>( val ), errorCode ) ), 
    mNeedsAction{ true } 
{}

ErrorReportCmd::ErrorReportCmd( CommandId id ) 
: SerialCommand( id ), mContent( kErrorReportFromPico ), mNeedsAction{ false }
{ 
    if ( id != kErrorReportFromPico ) 
    { 
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialCommandError, 1, kErrorReportFromPico ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}



void ErrorReportCmd::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
}



void ErrorReportCmd::sendOut( SerialLink& link )
{
    // These never sent from RPi0 (only recevied)
}



void ErrorReportCmd::takeAction( SerialLink& link ) 
{
    auto [ fatal, errCode ] = mContent.mMsg;
    std::cout << "Error from Pico: " << ( fatal ? "fatal" : "non-fatal" ) << "; error code " << errCode << std::endl;

    mNeedsAction = false;
}




/*********************************************************************************************/




DebugLinkCmd::DebugLinkCmd() noexcept 
: SerialCommand( kDebugSerialLink ), mContent( kDebugSerialLink ), mNeedsAction{ false } 
{}

DebugLinkCmd::DebugLinkCmd( TheData t ) noexcept 
: SerialCommand( kDebugSerialLink ), mContent( kDebugSerialLink, t ), mNeedsAction{ true } 
{} 

DebugLinkCmd::DebugLinkCmd( int val1, int val2 ) noexcept 
: SerialCommand( kDebugSerialLink ), mContent( kDebugSerialLink, std::make_tuple( val1, val2 ) ), mNeedsAction{ true } 
{}

DebugLinkCmd::DebugLinkCmd( CommandId id ) 
: SerialCommand( id ), mContent( kDebugSerialLink ), mNeedsAction{ false }          
{ 
    if ( id != kDebugSerialLink ) 
    { 
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialCommandError, 1, kDebugSerialLink ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}



void DebugLinkCmd::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
}



void DebugLinkCmd::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}



void DebugLinkCmd::takeAction( SerialLink& link ) 
{
    // Let's display what we got
    auto [ val1, val2 ] = mContent.mMsg;
    std::cout << "Debug: got: " << val1 << " and " << val2 << std::endl;
    mNeedsAction = false;
}




/*********************************************************************************************/




UnknownCmd::UnknownCmd() noexcept 
: SerialCommand( kUnknownCommand ), mContent( kUnknownCommand ), mNeedsAction{ false } 
{}

UnknownCmd::UnknownCmd( TheData t ) noexcept 
: SerialCommand( kUnknownCommand ), mContent( kUnknownCommand, t ), mNeedsAction{ true } 
{} 

UnknownCmd::UnknownCmd( std::uint8_t rcvdId, int errorCode ) noexcept 
: SerialCommand( kUnknownCommand ), mContent( kUnknownCommand, std::make_tuple( rcvdId, errorCode ) ), 
    mNeedsAction{ true } 
{}

UnknownCmd::UnknownCmd( CommandId id ) 
: SerialCommand( id ), mContent( kUnknownCommand ), mNeedsAction{ false }
{ 
    if ( id != kUnknownCommand ) 
    { 
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialCommandError, 1, kUnknownCommand ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}



void UnknownCmd::readIn( SerialLink& link ) 
{
   // Unknown command, don't try to read it
}



void UnknownCmd::sendOut( SerialLink& link )
{
    // We don't send this Cmd out on link; doing this is an error
    debugM( "Attempting to send an UnknownCmd on serial link" );
}



void UnknownCmd::takeAction( SerialLink& link ) 
{
    // this should do something to trigger error processing 
    auto [ id, errCode ] = mContent.mMsg;
    std::cout << "We received an unknown command, id " << id << " and error " << errCode << std::endl;

    mNeedsAction = false;
}




/*********************************************************************************************/
