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

#include "BNO055.h"
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




PicoNavStatusUpdateCmd::PicoNavStatusUpdateCmd() noexcept 
: SerialCommand( kPicoNavStatusUpdate ), mContent( kPicoNavStatusUpdate ), mNeedsAction{ false } 
{}

PicoNavStatusUpdateCmd::PicoNavStatusUpdateCmd( TheData t ) noexcept 
: SerialCommand( kPicoNavStatusUpdate ), mContent( kPicoNavStatusUpdate, t ), mNeedsAction{ true }
{} 

PicoNavStatusUpdateCmd::PicoNavStatusUpdateCmd( bool status, std::uint8_t m, std::uint8_t a, std::uint8_t g, std::uint8_t s ) noexcept 
: SerialCommand( kPicoNavStatusUpdate ), mContent( kPicoNavStatusUpdate, std::make_tuple( status, m, a, g, s ) ), mNeedsAction{ true } 
{}

PicoNavStatusUpdateCmd::PicoNavStatusUpdateCmd( CommandId id ) 
: SerialCommand( id ), mContent( kPicoNavStatusUpdate ), mNeedsAction{ false }
{ 
    if ( id != kPicoNavStatusUpdate ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialCommandError, 1, kPicoNavStatusUpdate ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void PicoNavStatusUpdateCmd::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
}

void PicoNavStatusUpdateCmd::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}

void PicoNavStatusUpdateCmd::takeAction( EventManager&, SerialLink& link ) 
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
    output2cout( "Pico sends stop to RPI0" );
    sendOut( link );
    mNeedsAction = false;
}




/*********************************************************************************************/





MsgControlCmd::MsgControlCmd() noexcept 
: SerialCommand( kMsgControlMsg ), mContent( kMsgControlMsg ), mNeedsAction{ false } 
{}

MsgControlCmd::MsgControlCmd( TheData t ) noexcept 
: SerialCommand( kMsgControlMsg ), mContent( kMsgControlMsg, t ), mNeedsAction{ true } 
{} 

MsgControlCmd::MsgControlCmd( bool val ) noexcept 
: SerialCommand( kMsgControlMsg ), mContent( kMsgControlMsg, std::make_tuple( static_cast<std::uint8_t>( val ) ) ), mNeedsAction{ true } 
{}

MsgControlCmd::MsgControlCmd( CommandId id ) 
: SerialCommand( id ), mContent( kMsgControlMsg ), mNeedsAction{ false }
{ 
    if ( id != kMsgControlMsg ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialCommandError, 1, kMsgControlMsg ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void MsgControlCmd::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
}

void MsgControlCmd::sendOut( SerialLink& link )
{
    // This never sent from Pico
}

void MsgControlCmd::takeAction( EventManager& events, SerialLink& link ) 
{
    std::uint8_t val = std::get<0>( mContent.mMsg );

    PicoState::sendTimerMsgs( val & kTimerMsgMask );
    PicoState::sendNavMsgs( val & kNavMsgMask );
    PicoState::sendNavStatusMsgs( val & kNavStatusMask );
    PicoState::sendEncoderMsgs( val & kEncoderMsgMask );
    PicoState::sendCalibrationMsgs( val & kCalibrationMsgMask );

    output2cout( "Timer events to RPi0 turned to ", val );    
    mNeedsAction = false;
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
    PicoState::sendTimerMsgs( val );
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
    events.queueEvent( kBNO055BeginCalibrationEvent );
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

    // Even if PicoState::wantNavStatusMsgs() is false, we always respond to direct request
    auto calibData{ BNO055::getCalibration() };
    bool status = BNO055::calibrationGood( calibData );

    PicoState::navCalibrated( status );
    PicoNavStatusUpdateCmd navReadyStatus( status, calibData.mag, calibData.accel, calibData.gyro, calibData.system );
    navReadyStatus.takeAction( events, link );

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





ResetBNO055Cmd::ResetBNO055Cmd() noexcept
: NoContentCmd( kResetBNO055 )
{
    // Nothing to do
}

ResetBNO055Cmd::ResetBNO055Cmd( CommandId id ) noexcept
: NoContentCmd( id )
{
    // Nothing to do
}


void ResetBNO055Cmd::takeAction( EventManager& events, SerialLink& link )
{
    output2cout( "Got a reset BNO055 cmd" );
    events.queueEvent( kBNO055ResetEvent );
    mNeedsAction = false;
}




/*********************************************************************************************/




NavUpdateCmd::NavUpdateCmd() noexcept
: SerialCommand( kTimerNavUpdate ), mContent( kTimerNavUpdate ), mNeedsAction{ false }
{}

NavUpdateCmd::NavUpdateCmd( TheData t ) noexcept
: SerialCommand( kTimerNavUpdate ), mContent( kTimerNavUpdate, t ), mNeedsAction{ true }
{}


NavUpdateCmd::NavUpdateCmd(  float heading, std::uint32_t time  ) noexcept
: SerialCommand( kTimerNavUpdate ), mContent( kTimerNavUpdate, std::make_tuple( heading, time ) ), mNeedsAction{ true }
{}


NavUpdateCmd::NavUpdateCmd( CommandId id )
: SerialCommand( kTimerNavUpdate ), mContent( kTimerNavUpdate ), mNeedsAction{ false }
{
    if ( id != kTimerNavUpdate ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialCommandError, 1, kTimerNavUpdate ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void NavUpdateCmd::readIn( SerialLink& link )
{
    mContent.readIn( link );
    mNeedsAction = true;
}


void NavUpdateCmd::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}



void NavUpdateCmd::takeAction( EventManager& events, SerialLink& link )
{
    sendOut( link );
    mNeedsAction = false;
}




/*********************************************************************************************/




NavUpdateControlCmd::NavUpdateControlCmd() noexcept 
: SerialCommand( kNavUpdateControl ), mContent( kNavUpdateControl ), mNeedsAction{ false } 
{}

NavUpdateControlCmd::NavUpdateControlCmd( TheData t ) noexcept 
: SerialCommand( kNavUpdateControl ), mContent( kNavUpdateControl, t ), mNeedsAction{ true } 
{} 

NavUpdateControlCmd::NavUpdateControlCmd( bool val ) noexcept 
: SerialCommand( kNavUpdateControl ), mContent( kNavUpdateControl, std::make_tuple( static_cast<std::uint8_t>( val ) ) ), mNeedsAction{ true } 
{}

NavUpdateControlCmd::NavUpdateControlCmd( CommandId id ) 
: SerialCommand( id ), mContent( kNavUpdateControl ), mNeedsAction{ false }
{ 
    if ( id != kNavUpdateControl ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialCommandError, 1, kNavUpdateControl ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void NavUpdateControlCmd::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
}

void NavUpdateControlCmd::sendOut( SerialLink& link )
{
    // This never sent from Pico
}

void NavUpdateControlCmd::takeAction( EventManager& events, SerialLink& link ) 
{
    bool val = std::get<0>( mContent.mMsg );
    PicoState::sendNavMsgs( val );
    output2cout( "Nav update events to RPi0 turned to ", val );    
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

        default: 
            val1 *= -1;
            val2 *= -1;
            DebugLinkCmd reply( val2, val1 );
            reply.sendOut( link );
            break;
    }
    mNeedsAction = false;
}




/*********************************************************************************************/

