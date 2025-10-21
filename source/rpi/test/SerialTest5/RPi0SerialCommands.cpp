/*
    RPi0SerialMessages.cpp - Serial Commands for CARRT3 communications
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




#include "SerialMessages.h"

#include <iostream>

#include "CarrtError.h"

#include "DebugUtils.hpp"




class EventManager;


/*********************************************************************************************/




TimerEventMsg::TimerEventMsg() noexcept 
: SerialMessage( kTimerEvent ), mContent( kTimerEvent ), mNeedsAction{ false } 
{}

TimerEventMsg::TimerEventMsg( TheData t ) noexcept 
: SerialMessage( kTimerEvent ), mContent( kTimerEvent, t ), mNeedsAction{ true }
{} 

TimerEventMsg::TimerEventMsg( std::uint8_t which, int count, std::uint32_t time ) noexcept 
: SerialMessage( kTimerEvent ), mContent( kTimerEvent, std::make_tuple( which, count, time ) ), mNeedsAction{ true } 
{}

TimerEventMsg::TimerEventMsg( MessageId id ) 
: SerialMessage( id ), mContent( kTimerEvent ), mNeedsAction{ false }
{ 
    if ( id != kTimerEvent ) 
    { 
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, kTimerEvent ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}



void TimerEventMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
}



void TimerEventMsg::sendOut( SerialLink& link )
{
    // RPi0 never sends these out; only receives
}



void TimerEventMsg::takeAction( EventManager&, SerialLink& link ) 
{
    // For the test just print out
    auto [ kind, count, time ] = mContent.mMsg;
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
            break;

        default:
            std::cout << "?? " << static_cast<int>(kind) << " ?? ";
            break;
    }
    std::cout << "sec; count: " << count << " time: " << time << std::endl;
    mNeedsAction = false;
}





/*********************************************************************************************/




TimerControlMsg::TimerControlMsg() noexcept 
: SerialMessage( kTimerControl ), mContent( kTimerControl ), mNeedsAction{ false } 
{}

TimerControlMsg::TimerControlMsg( TheData t ) noexcept 
: SerialMessage( kTimerControl ), mContent( kTimerControl, t ), mNeedsAction{ true } 
{} 

TimerControlMsg::TimerControlMsg( bool val ) noexcept 
: SerialMessage( kTimerControl ), mContent( kTimerControl, std::make_tuple( static_cast<std::uint8_t>( val ) ) ), mNeedsAction{ true } 
{}

TimerControlMsg::TimerControlMsg( MessageId id ) 
: SerialMessage( id ), mContent( kTimerControl ), mNeedsAction{ false }
{ 
    if ( id != kTimerControl ) 
    { 
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, kTimerControl ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}



void TimerControlMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
}



void TimerControlMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}



void TimerControlMsg::takeAction( EventManager&, SerialLink& link ) 
{
    // RPi0 action is to send it out...
    sendOut( link );
    mNeedsAction = false;
}




/*********************************************************************************************/




ErrorReportMsg::ErrorReportMsg() noexcept 
: SerialMessage( kErrorReportFromPico ), mContent( kErrorReportFromPico ), mNeedsAction{ false } 
{}

ErrorReportMsg::ErrorReportMsg( TheData t ) noexcept 
: SerialMessage( kErrorReportFromPico ), mContent( kErrorReportFromPico, t ), mNeedsAction{ true } 
{} 

ErrorReportMsg::ErrorReportMsg( bool val, int errorCode ) noexcept 
: SerialMessage( kErrorReportFromPico ), mContent( kErrorReportFromPico, std::make_tuple( static_cast<std::uint8_t>( val ), errorCode ) ), 
    mNeedsAction{ true } 
{}

ErrorReportMsg::ErrorReportMsg( MessageId id ) 
: SerialMessage( id ), mContent( kErrorReportFromPico ), mNeedsAction{ false }
{ 
    if ( id != kErrorReportFromPico ) 
    { 
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, kErrorReportFromPico ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}



void ErrorReportMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
}



void ErrorReportMsg::sendOut( SerialLink& link )
{
    // These never sent from RPi0 (only recevied)
}



void ErrorReportMsg::takeAction( EventManager&, SerialLink& link ) 
{
    auto [ fatal, errCode ] = mContent.mMsg;
    std::cout << "Error from Pico: " << ( fatal ? "fatal" : "non-fatal" ) << "; error code " << errCode << std::endl;

    mNeedsAction = false;
}




/*********************************************************************************************/




DebugLinkMsg::DebugLinkMsg() noexcept 
: SerialMessage( kDebugSerialLink ), mContent( kDebugSerialLink ), mNeedsAction{ false } 
{}

DebugLinkMsg::DebugLinkMsg( TheData t ) noexcept 
: SerialMessage( kDebugSerialLink ), mContent( kDebugSerialLink, t ), mNeedsAction{ true } 
{} 

DebugLinkMsg::DebugLinkMsg( int val1, int val2 ) noexcept 
: SerialMessage( kDebugSerialLink ), mContent( kDebugSerialLink, std::make_tuple( val1, val2 ) ), mNeedsAction{ true } 
{}

DebugLinkMsg::DebugLinkMsg( MessageId id ) 
: SerialMessage( id ), mContent( kDebugSerialLink ), mNeedsAction{ false }          
{ 
    if ( id != kDebugSerialLink ) 
    { 
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, kDebugSerialLink ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}



void DebugLinkMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
}



void DebugLinkMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}



void DebugLinkMsg::takeAction( EventManager&, SerialLink& link ) 
{
    // Let's display what we got
    auto [ val1, val2 ] = mContent.mMsg;
    std::cout << "Debug: got: " << val1 << " and " << val2 << std::endl;
    mNeedsAction = false;
}




/*********************************************************************************************/




UnknownMsg::UnknownMsg() noexcept 
: SerialMessage( kUnknownMessage ), mContent( kUnknownMessage ), mNeedsAction{ false } 
{}

UnknownMsg::UnknownMsg( TheData t ) noexcept 
: SerialMessage( kUnknownMessage ), mContent( kUnknownMessage, t ), mNeedsAction{ true } 
{} 

UnknownMsg::UnknownMsg( std::uint8_t rcvdId, int errorCode ) noexcept 
: SerialMessage( kUnknownMessage ), mContent( kUnknownMessage, std::make_tuple( rcvdId, errorCode ) ), 
    mNeedsAction{ true } 
{}

UnknownMsg::UnknownMsg( MessageId id ) 
: SerialMessage( id ), mContent( kUnknownMessage ), mNeedsAction{ false }
{ 
    if ( id != kUnknownMessage ) 
    { 
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, kUnknownMessage ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}



void UnknownMsg::readIn( SerialLink& link ) 
{
   // Unknown command, don't try to read it
}



void UnknownMsg::sendOut( SerialLink& link )
{
    // We don't send this Msg out on link; doing this is an error
    debugM( "Attempting to send an UnknownMsg on serial link" );
}



void UnknownMsg::takeAction( EventManager&, SerialLink& link ) 
{
    // this should do something to trigger error processing 
    auto [ id, errCode ] = mContent.mMsg;
    std::cout << "We received an unknown command, id " << id << " and error " << errCode << std::endl;

    mNeedsAction = false;
}




/*********************************************************************************************/
