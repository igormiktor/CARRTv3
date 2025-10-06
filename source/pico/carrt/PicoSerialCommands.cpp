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

#include "EventManager.h"
#include "PicoOutputUtils.hpp"
#include "PicoState.h"

#include "DebugUtils.hpp"








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
    ErrorReportCmd errCmd( false, std::get<1>( mContent.mMsg ) );
    errCmd.sendOut( link );
}

void UnknownCmd::takeAction( EventManager& events, SerialLink& link ) 
{
    // Only action is to send the message out
    sendOut( link );
    mNeedsAction = false;
}




/*********************************************************************************************/




NoContentCmd::NoContentCmd( std::uint8_t id ) noexcept
: SerialCommand( static_cast<CommandId>( id ) ), mId{ id }, mNeedsAction{ false } 
{
    // Nothing to do
}

NoContentCmd::NoContentCmd( CommandId id ) noexcept
: SerialCommand( id ), mId{ id }, mNeedsAction{ true }
{
    // Nothing to do
}


void NoContentCmd::readIn( SerialLink& link ) 
{
    // Nothing to do (we already have the ID if we call this function)
}

void NoContentCmd::sendOut( SerialLink& link )
{
    // Send the ID, it's the only thing we need to send
    link.put( static_cast<std::uint8_t>( mId ) );
}

bool NoContentCmd::needsAction() const noexcept
{ 
    return mNeedsAction; 
}

std::uint8_t NoContentCmd::getId() const noexcept
{ 
    return static_cast<std::uint8_t>( mId ); 
}




/*********************************************************************************************/
/*********************************************************************************************/





NullCmd::NullCmd() noexcept
: NoContentCmd( kNullMsg )
{
    // Nothing to do
}

NullCmd::NullCmd( CommandId id ) noexcept
: NoContentCmd( kNullMsg )
{
    // Nothing to do
}


void NullCmd::takeAction( EventManager& events, SerialLink& link )
{
    // Null Cmd, so nothing to do
}




/*********************************************************************************************/




PicoReadyCmd::PicoReadyCmd() noexcept 
: SerialCommand( kPicoReady ), mContent( kPicoReady ), mNeedsAction{ false } 
{}

PicoReadyCmd::PicoReadyCmd( TheData t ) noexcept 
: SerialCommand( kPicoReady ), mContent( kPicoReady, t ), mNeedsAction{ true }
{} 

PicoReadyCmd::PicoReadyCmd( std::uint32_t time ) noexcept 
: SerialCommand( kPicoReady ), mContent( kPicoReady, std::make_tuple( static_cast<std::uint32_t>( time ) ) ), mNeedsAction{ true } 
{}

PicoReadyCmd::PicoReadyCmd( CommandId id ) 
: SerialCommand( id ), mContent( kPicoReady ), mNeedsAction{ false }
{ 
    if ( id != kPicoReady ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialCommandError, 1, kPicoReady ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void PicoReadyCmd::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = false;
    output2cout( "Error:  Pico should never receive PicoReadyCmd" );
}

void PicoReadyCmd::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}

void PicoReadyCmd::takeAction( EventManager&, SerialLink& link ) 
{
    // Only action is to send it
    sendOut( link );
    mNeedsAction = false;
}




/*********************************************************************************************/




PicoReadyNavCmd::PicoReadyNavCmd() noexcept 
: SerialCommand( kPicoReadyNav ), mContent( kPicoReadyNav ), mNeedsAction{ false } 
{}

PicoReadyNavCmd::PicoReadyNavCmd( TheData t ) noexcept 
: SerialCommand( kPicoReadyNav ), mContent( kPicoReadyNav, t ), mNeedsAction{ true }
{} 

PicoReadyNavCmd::PicoReadyNavCmd( std::uint32_t time ) noexcept 
: SerialCommand( kPicoReadyNav ), mContent( kPicoReadyNav, static_cast<std::uint32_t>( time ) ), mNeedsAction{ true } 
{}

PicoReadyNavCmd::PicoReadyNavCmd( CommandId id ) 
: SerialCommand( id ), mContent( kPicoReadyNav ), mNeedsAction{ false }
{ 
    if ( id != kPicoReadyNav ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialCommandError, 1, kPicoReadyNav ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void PicoReadyNavCmd::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
}

void PicoReadyNavCmd::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}

void PicoReadyNavCmd::takeAction( EventManager&, SerialLink& link ) 
{
    // Only action is to send it
    sendOut( link );
    mNeedsAction = false;
}




/*********************************************************************************************/





PicoSaysStopCmd::PicoSaysStopCmd() noexcept
: NoContentCmd( kPicoSaysStop )
{
    // Nothing to do
}

PicoSaysStopCmd::PicoSaysStopCmd( CommandId id ) noexcept
: NoContentCmd( id )
{
    // Nothing to do
}


void PicoSaysStopCmd::takeAction( EventManager& events, SerialLink& link )
{
    // Nothing to do (outgoing command)
}




/*********************************************************************************************/





PauseCmd::PauseCmd() noexcept
: NoContentCmd( kPauseMsg )
{
    // Nothing to do
}

PauseCmd::PauseCmd( CommandId id ) noexcept
: NoContentCmd( id )
{
    // Nothing to do
}


void PauseCmd::takeAction( EventManager& events, SerialLink& link )
{
    // TODO implement a pause in Pico

    output2cout( "Got a pause cmd: TODO -- implement a pause in Pico (not yet coded)" );
}




/*********************************************************************************************/





ResumeCmd::ResumeCmd() noexcept
: NoContentCmd( kResumeMsg )
{
    // Nothing to do
}

ResumeCmd::ResumeCmd( CommandId id ) noexcept
: NoContentCmd( id )
{
    // Nothing to do
}


void ResumeCmd::takeAction( EventManager& events, SerialLink& link )
{
    // TODO resume Pico (from a pause)

    output2cout( "Got a resume cmd: TODO resume Pico (from a pause)" );
}




/*********************************************************************************************/




ResetCmd::ResetCmd() noexcept
: NoContentCmd( kResetMsg )
{
    // Nothing to do
}

ResetCmd::ResetCmd( CommandId id ) noexcept
: NoContentCmd( id )
{
    // Nothing to do
}


void ResetCmd::takeAction( EventManager& events, SerialLink& link )
{
    output2cout( "Pico commanded to reset by RPi0" );
    events.queueEvent( Event::kPicoResetEvent, 0, 0, EventManager::kHighPriority );
    mNeedsAction = false;
}




/*********************************************************************************************/




TimerEventCmd::TimerEventCmd() noexcept 
: SerialCommand( kTimerEvent ), mContent( kTimerEvent ), mNeedsAction{ false } 
{}

TimerEventCmd::TimerEventCmd( TheData t ) noexcept 
: SerialCommand( kTimerEvent ), mContent( kTimerEvent, t ), mNeedsAction{ true }
{} 

TimerEventCmd::TimerEventCmd( std::uint8_t which, int count, uint32_t time ) noexcept 
: SerialCommand( kTimerEvent ), mContent( kTimerEvent, std::make_tuple( which, count, time ) ), mNeedsAction{ true } 
{}

TimerEventCmd::TimerEventCmd( CommandId id ) 
: SerialCommand( id ), mContent( kTimerEvent ), mNeedsAction{ false }
{ 
    if ( id != kTimerEvent ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialCommandError, 1, kTimerEvent ), "Id mismatch at creation" ); 
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
    mContent.sendOut( link );
}

void TimerEventCmd::takeAction( EventManager&, SerialLink& link ) 
{
    // Only action is to send it
    sendOut( link );
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
        throw CarrtError( makePicoErrorId( kPicoSerialCommandError, 1, kTimerControl ), "Id mismatch at creation" ); 
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
    // This never sent from Pico
}

void TimerControlCmd::takeAction( EventManager& events, SerialLink& link ) 
{
    bool val = std::get<0>( mContent.mMsg );
    PicoState::sendTimerEvents( val );
    output2cout( "Timer events to RPi0 turned to ", val );    
    mNeedsAction = false;
}




/*********************************************************************************************/





BeginCalibrationCmd::BeginCalibrationCmd() noexcept
: NoContentCmd( kBeginCalibration )
{
    // Nothing to do
}

BeginCalibrationCmd::BeginCalibrationCmd( CommandId id ) noexcept
: NoContentCmd( id )
{
    // Nothing to do
}


void BeginCalibrationCmd::takeAction( EventManager& events, SerialLink& link )
{
    output2cout( "Got a calibration cmd: Trigger calibration" );
    events.queueEvent( Event::kBeginCalibrationEvent );
    mNeedsAction = false;
}




/*********************************************************************************************/





RequestCalibrationStatusCmd::RequestCalibrationStatusCmd() noexcept
: NoContentCmd( kRequestCalibStatus )
{
    // Nothing to do
}

RequestCalibrationStatusCmd::RequestCalibrationStatusCmd( CommandId id ) noexcept
: NoContentCmd( id )
{
    // Nothing to do
}


void RequestCalibrationStatusCmd::takeAction( EventManager& events, SerialLink& link )
{
    output2cout( "Got a request calib status cmd" );
    events.queueEvent( Event::kSendCalibrationInfoEvent );
    mNeedsAction = false;
}




/*********************************************************************************************/





SendCalibrationStatusCmd::SendCalibrationStatusCmd() noexcept
: SerialCommand( kSendCalibStatus ), mContent( kSendCalibStatus ), mNeedsAction{ false }
{}

SendCalibrationStatusCmd::SendCalibrationStatusCmd( TheData t ) noexcept
: SerialCommand( kSendCalibStatus ), mContent( kSendCalibStatus, t ), mNeedsAction{ true }
{}


SendCalibrationStatusCmd::SendCalibrationStatusCmd( std::uint8_t mag, std::uint8_t accel, std::uint8_t gyro, std::uint8_t sys ) noexcept
: SerialCommand( kSendCalibStatus ), mContent( kSendCalibStatus, std::make_tuple( mag, accel, gyro, sys) ), mNeedsAction{ true }
{}


SendCalibrationStatusCmd::SendCalibrationStatusCmd( CommandId id )
: SerialCommand( id ), mContent( kSendCalibStatus ), mNeedsAction{ false }
{
    if ( id != kSendCalibStatus ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialCommandError, 1, kSendCalibStatus ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void SendCalibrationStatusCmd::readIn( SerialLink& link )
{
    mContent.readIn( link );
    mNeedsAction = true;
}


void SendCalibrationStatusCmd::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}



void SendCalibrationStatusCmd::takeAction( EventManager& events, SerialLink& link )
{
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
        throw CarrtError( makePicoErrorId( kPicoSerialCommandError, 1, kErrorReportFromPico ), "Id mismatch at creation" ); 
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
    mContent.sendOut( link );
}

void ErrorReportCmd::takeAction( EventManager&, SerialLink& link ) 
{
    // Only action is to send the message out
    sendOut( link );
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
        throw CarrtError( makePicoErrorId( kPicoSerialCommandError, 1, kDebugSerialLink ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void DebugLinkCmd::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
//    auto [ v1, v2 ] = mContent.mMsg;
//    debugM( "DebugLinkCmd got" );
//    debugV( v1, v2 );
}

void DebugLinkCmd::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}

void DebugLinkCmd::takeAction( EventManager& events, SerialLink& link ) 
{
    // Lets negate the values, flip the order, and send them back
    auto [ val1, val2 ] = mContent.mMsg;

 
    switch ( val1 )
    {
        case kNullMsg:
            break;

        case kBeginCalibration:
            break;

        case kPauseMsg:
            break;

        case kResumeMsg:
            break;

        case kResetMsg:
            break;


        // Msgs from Pico
        case kPicoReady:
            break;

        case kPicoReadyNav:
            break;

        case kPicoSaysStop:
            break;


        // Timer events (from Pico)
        case kTimerEvent:
            break;

        case kTimerControl:
            break;


        // Calibration cmds       
        case kRequestCalibStatus:
            break;

        case kSendCalibProfileToPico:
            break;

        case kRequestCalibProfileFmPico:
            break;


        // Navigation events
        case kTimerNavUpdate:
            break;

        case kDrivingStatusUpdate:
            break;

        case kEncoderUpdate:
            break;


        // Battery info
        case kRequestBatteryLevel:
            break;

        case kBatteryLevelUpdate:
            break;

        case kBatteryLowAlert:
            break;


        // Error reports
        case kErrorReportFromPico:
            break;


        // Unknown command
        case kUnknownCommand:
            break;


        // Debugging events
        case kTestPicoReportError:
            break;

        case kExtendedMsg:
            break;

        default: 
            val1 *= -1;
            val2 *= -1;
            DebugLinkCmd reply( val2, val1 );
            // auto [ v1, v2 ] = reply.mContent.mMsg;
            // debugM( "DebugLinkCmd sending" );
            // debugV( v1, v2 );
            reply.sendOut( link );
            break;
    }
    mNeedsAction = false;
}




/*********************************************************************************************/

