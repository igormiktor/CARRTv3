/*
    PicoSerialMessages.cpp - Serial Messages for CARRT3 communications
    between the RPI and Pico.  This file contains the Pico messages.

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

#include "BNO055.h"
#include "EventManager.h"
#include "PicoOutputUtils.hpp"
#include "PicoState.h"

#include "DebugUtils.hpp"








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
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, kUnknownMessage ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void UnknownMsg::readIn( SerialLink& link ) 
{
    // Unknown message; don't try to read it in.
    mNeedsAction = true;
}

void UnknownMsg::sendOut( SerialLink& link )
{
    // We don't send this msg out on link; instead send error report
    ErrorReportMsg errMsg( false, std::get<1>( mContent.mMsg ) );
    errMsg.sendOut( link );
}

void UnknownMsg::takeAction( EventManager& events, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // Only action is to send the message out
        sendOut( link );
        mNeedsAction = false;
    }
}




/*********************************************************************************************/




NoContentMsg::NoContentMsg( std::uint8_t id ) noexcept
: SerialMessage( static_cast<MessageId>( id ) ), mId{ id }, mNeedsAction{ false } 
{
    // Nothing to do
}

NoContentMsg::NoContentMsg( MessageId id ) noexcept
: SerialMessage( id ), mId{ id }, mNeedsAction{ true }
{
    // Nothing to do
}


void NoContentMsg::readIn( SerialLink& link ) 
{
    // Nothing to do (we already have the ID if we call this function)
}

void NoContentMsg::sendOut( SerialLink& link )
{
    // Send the ID, it's the only thing we need to send
    link.put( static_cast<std::uint8_t>( mId ) );
}

bool NoContentMsg::needsAction() const noexcept
{ 
    return mNeedsAction; 
}

std::uint8_t NoContentMsg::getId() const noexcept
{ 
    return static_cast<std::uint8_t>( mId ); 
}




/*********************************************************************************************/
/*********************************************************************************************/





NullMsg::NullMsg() noexcept
: NoContentMsg( kNullMsg )
{
    mNeedsAction = true;
}

NullMsg::NullMsg( MessageId id ) noexcept
: NoContentMsg( kNullMsg )
{
    // Nothing to do
}


void NullMsg::takeAction( EventManager& events, SerialLink& link )
{
    // Only action is to send it
    if ( mNeedsAction )
    {
        output2cout( "Send Null msg" );
        sendOut( link );
        mNeedsAction = false;
    }
}




/*********************************************************************************************/




PicoReadyMsg::PicoReadyMsg() noexcept 
: SerialMessage( kPicoReady ), mContent( kPicoReady ), mNeedsAction{ false } 
{}

PicoReadyMsg::PicoReadyMsg( TheData t ) noexcept 
: SerialMessage( kPicoReady ), mContent( kPicoReady, t ), mNeedsAction{ true }
{} 

PicoReadyMsg::PicoReadyMsg( std::uint32_t time ) noexcept 
: SerialMessage( kPicoReady ), mContent( kPicoReady, std::make_tuple( static_cast<std::uint32_t>( time ) ) ), mNeedsAction{ true } 
{}

PicoReadyMsg::PicoReadyMsg( MessageId id ) 
: SerialMessage( id ), mContent( kPicoReady ), mNeedsAction{ false }
{ 
    if ( id != kPicoReady ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, kPicoReady ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void PicoReadyMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = false;
    output2cout( "Error:  Pico should never receive PicoReadyMsg" );
}

void PicoReadyMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}

void PicoReadyMsg::takeAction( EventManager&, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // Only action is to send it
        sendOut( link );
        mNeedsAction = false;
    }
}




/*********************************************************************************************/




PicoNavStatusUpdateMsg::PicoNavStatusUpdateMsg() noexcept 
: SerialMessage( kPicoNavStatusUpdate ), mContent( kPicoNavStatusUpdate ), mNeedsAction{ false } 
{}

PicoNavStatusUpdateMsg::PicoNavStatusUpdateMsg( TheData t ) noexcept 
: SerialMessage( kPicoNavStatusUpdate ), mContent( kPicoNavStatusUpdate, t ), mNeedsAction{ true }
{} 

PicoNavStatusUpdateMsg::PicoNavStatusUpdateMsg( bool status, std::uint8_t m, std::uint8_t a, std::uint8_t g, std::uint8_t s ) noexcept 
: SerialMessage( kPicoNavStatusUpdate ), mContent( kPicoNavStatusUpdate, std::make_tuple( status, m, a, g, s ) ), mNeedsAction{ true } 
{}

PicoNavStatusUpdateMsg::PicoNavStatusUpdateMsg( MessageId id ) 
: SerialMessage( id ), mContent( kPicoNavStatusUpdate ), mNeedsAction{ false }
{ 
    if ( id != kPicoNavStatusUpdate ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, kPicoNavStatusUpdate ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void PicoNavStatusUpdateMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
}

void PicoNavStatusUpdateMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}

void PicoNavStatusUpdateMsg::takeAction( EventManager&, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // Only action is to send it
        sendOut( link );
        mNeedsAction = false;
    }
}




/*********************************************************************************************/





PicoSaysStopMsg::PicoSaysStopMsg() noexcept
: NoContentMsg( kPicoSaysStop )
{
    mNeedsAction = true;
}

PicoSaysStopMsg::PicoSaysStopMsg( MessageId id ) noexcept
: NoContentMsg( id )
{
    // Nothing to do
}


void PicoSaysStopMsg::takeAction( EventManager& events, SerialLink& link )
{
    if ( mNeedsAction )
    {
        output2cout( "Pico sends stop to RPI0" );
        sendOut( link );
        mNeedsAction = false;
    }
}




/*********************************************************************************************/





MsgControlMsg::MsgControlMsg() noexcept 
: SerialMessage( kMsgControlMsg ), mContent( kMsgControlMsg ), mNeedsAction{ false } 
{}

MsgControlMsg::MsgControlMsg( TheData t ) noexcept 
: SerialMessage( kMsgControlMsg ), mContent( kMsgControlMsg, t ), mNeedsAction{ true } 
{} 

MsgControlMsg::MsgControlMsg( bool val ) noexcept 
: SerialMessage( kMsgControlMsg ), mContent( kMsgControlMsg, std::make_tuple( static_cast<std::uint8_t>( val ) ) ), mNeedsAction{ true } 
{}

MsgControlMsg::MsgControlMsg( MessageId id ) 
: SerialMessage( id ), mContent( kMsgControlMsg ), mNeedsAction{ false }
{ 
    if ( id != kMsgControlMsg ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, kMsgControlMsg ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void MsgControlMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
}

void MsgControlMsg::sendOut( SerialLink& link )
{
    // This never sent from Pico
}

void MsgControlMsg::takeAction( EventManager& events, SerialLink& link ) 
{
    if ( mNeedsAction )
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
}




/*********************************************************************************************/




ResetMsg::ResetMsg() noexcept
: NoContentMsg( kResetMsg )
{
    mNeedsAction = true;
}

ResetMsg::ResetMsg( MessageId id ) noexcept
: NoContentMsg( id )
{
    // Nothing to do
}


void ResetMsg::takeAction( EventManager& events, SerialLink& link )
{
    if ( mNeedsAction )
    {
        output2cout( "Pico got message from RPi0 to reset" );
        events.queueEvent( Event::kPicoResetEvent, 0, 0, EventManager::kHighPriority );
        mNeedsAction = false;
    }
}




/*********************************************************************************************/




TimerEventMsg::TimerEventMsg() noexcept 
: SerialMessage( kTimerEventMsg ), mContent( kTimerEventMsg ), mNeedsAction{ false } 
{}

TimerEventMsg::TimerEventMsg( TheData t ) noexcept 
: SerialMessage( kTimerEventMsg ), mContent( kTimerEventMsg, t ), mNeedsAction{ true }
{} 

TimerEventMsg::TimerEventMsg( std::uint8_t which, int count, uint32_t time ) noexcept 
: SerialMessage( kTimerEventMsg ), mContent( kTimerEventMsg, std::make_tuple( which, count, time ) ), mNeedsAction{ true } 
{}

TimerEventMsg::TimerEventMsg( MessageId id ) 
: SerialMessage( id ), mContent( kTimerEventMsg ), mNeedsAction{ false }
{ 
    if ( id != kTimerEventMsg ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, kTimerEventMsg ), "Id mismatch at creation" ); 
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
    mContent.sendOut( link );
}

void TimerEventMsg::takeAction( EventManager&, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // Only action is to send it
        sendOut( link );
        mNeedsAction = false;
    }
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
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, kTimerControl ), "Id mismatch at creation" ); 
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
    // This never sent from Pico
}

void TimerControlMsg::takeAction( EventManager& events, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        bool val = std::get<0>( mContent.mMsg );
        PicoState::sendTimerMsgs( val );
        output2cout( "Timer events to RPi0 turned to ", val );    
        mNeedsAction = false;
    }
}




/*********************************************************************************************/





BeginCalibrationMsg::BeginCalibrationMsg() noexcept
: NoContentMsg( kBeginCalibration )
{
    mNeedsAction = true;
}

BeginCalibrationMsg::BeginCalibrationMsg( MessageId id ) noexcept
: NoContentMsg( id )
{
    // Nothing to do
}


void BeginCalibrationMsg::takeAction( EventManager& events, SerialLink& link )
{
    if ( mNeedsAction )
    {
        output2cout( "Got a calibration msg: Trigger calibration" );
        events.queueEvent( kBNO055BeginCalibrationEvent );
        mNeedsAction = false;
    }
}




/*********************************************************************************************/





RequestCalibrationStatusMsg::RequestCalibrationStatusMsg() noexcept
: NoContentMsg( kRequestCalibStatus )
{
    mNeedsAction = true;
}

RequestCalibrationStatusMsg::RequestCalibrationStatusMsg( MessageId id ) noexcept
: NoContentMsg( id )
{
    // Nothing to do
}


void RequestCalibrationStatusMsg::takeAction( EventManager& events, SerialLink& link )
{
    if ( mNeedsAction )
    {
        output2cout( "Got a request calib status msg" );

        // Even if PicoState::wantNavStatusMsgs() is false, we always respond to direct request
        auto calibData{ BNO055::getCalibration() };
        bool status = BNO055::calibrationGood( calibData );

        PicoState::navCalibrated( status );
        PicoNavStatusUpdateMsg navReadyStatus( status, calibData.mag, calibData.accel, calibData.gyro, calibData.system );
        navReadyStatus.takeAction( events, link );

        mNeedsAction = false;
    }
}




/*********************************************************************************************/




SendCalibrationInfoMsg::SendCalibrationInfoMsg() noexcept
: SerialMessage( kSendCalibInfo ), mContent( kSendCalibInfo ), mNeedsAction{ false }
{}

SendCalibrationInfoMsg::SendCalibrationInfoMsg( TheData t ) noexcept
: SerialMessage( kSendCalibInfo ), mContent( kSendCalibInfo, t ), mNeedsAction{ true }
{}


SendCalibrationInfoMsg::SendCalibrationInfoMsg( std::uint8_t mag, std::uint8_t accel, std::uint8_t gyro, std::uint8_t sys ) noexcept
: SerialMessage( kSendCalibInfo ), mContent( kSendCalibInfo, std::make_tuple( mag, accel, gyro, sys) ), mNeedsAction{ true }
{}


SendCalibrationInfoMsg::SendCalibrationInfoMsg( MessageId id )
: SerialMessage( id ), mContent( kSendCalibInfo ), mNeedsAction{ false }
{
    if ( id != kSendCalibInfo ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, kSendCalibInfo ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void SendCalibrationInfoMsg::readIn( SerialLink& link )
{
    mContent.readIn( link );
    mNeedsAction = true;
}


void SendCalibrationInfoMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}



void SendCalibrationInfoMsg::takeAction( EventManager& events, SerialLink& link )
{
    if ( mNeedsAction )
    {
        sendOut( link );
        mNeedsAction = false;
    }
}




/*********************************************************************************************/





ResetBNO055Msg::ResetBNO055Msg() noexcept
: NoContentMsg( kResetBNO055 )
{
    mNeedsAction = true;
}

ResetBNO055Msg::ResetBNO055Msg( MessageId id ) noexcept
: NoContentMsg( id )
{
    // Nothing to do
}


void ResetBNO055Msg::takeAction( EventManager& events, SerialLink& link )
{
    if ( mNeedsAction )
    {
        output2cout( "Got a reset BNO055 msg" );
        events.queueEvent( kBNO055ResetEvent );
        mNeedsAction = false;
    }
}




/*********************************************************************************************/




NavUpdateMsg::NavUpdateMsg() noexcept
: SerialMessage( kTimerNavUpdate ), mContent( kTimerNavUpdate ), mNeedsAction{ false }
{}

NavUpdateMsg::NavUpdateMsg( TheData t ) noexcept
: SerialMessage( kTimerNavUpdate ), mContent( kTimerNavUpdate, t ), mNeedsAction{ true }
{}


NavUpdateMsg::NavUpdateMsg(  float heading, std::uint32_t time  ) noexcept
: SerialMessage( kTimerNavUpdate ), mContent( kTimerNavUpdate, std::make_tuple( heading, time ) ), mNeedsAction{ true }
{}


NavUpdateMsg::NavUpdateMsg( MessageId id )
: SerialMessage( kTimerNavUpdate ), mContent( kTimerNavUpdate ), mNeedsAction{ false }
{
    if ( id != kTimerNavUpdate ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, kTimerNavUpdate ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void NavUpdateMsg::readIn( SerialLink& link )
{
    mContent.readIn( link );
    mNeedsAction = true;
}


void NavUpdateMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}



void NavUpdateMsg::takeAction( EventManager& events, SerialLink& link )
{
    if ( mNeedsAction )
    {
        sendOut( link );
        mNeedsAction = false;
    }
}




/*********************************************************************************************/




NavUpdateControlMsg::NavUpdateControlMsg() noexcept 
: SerialMessage( kNavUpdateControl ), mContent( kNavUpdateControl ), mNeedsAction{ false } 
{}

NavUpdateControlMsg::NavUpdateControlMsg( TheData t ) noexcept 
: SerialMessage( kNavUpdateControl ), mContent( kNavUpdateControl, t ), mNeedsAction{ true } 
{} 

NavUpdateControlMsg::NavUpdateControlMsg( bool val ) noexcept 
: SerialMessage( kNavUpdateControl ), mContent( kNavUpdateControl, std::make_tuple( static_cast<std::uint8_t>( val ) ) ), mNeedsAction{ true } 
{}

NavUpdateControlMsg::NavUpdateControlMsg( MessageId id ) 
: SerialMessage( id ), mContent( kNavUpdateControl ), mNeedsAction{ false }
{ 
    if ( id != kNavUpdateControl ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, kNavUpdateControl ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void NavUpdateControlMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
}

void NavUpdateControlMsg::sendOut( SerialLink& link )
{
    // This never sent from Pico
}

void NavUpdateControlMsg::takeAction( EventManager& events, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        bool val = std::get<0>( mContent.mMsg );
        PicoState::sendNavMsgs( val );
        output2cout( "Nav update events to RPi0 turned to ", val );    
        mNeedsAction = false;
    }
}




/*********************************************************************************************/




DrivingStatusUpdateMsg::DrivingStatusUpdateMsg() noexcept 
: SerialMessage( kDrivingStatusUpdate ), mContent( kDrivingStatusUpdate ), mNeedsAction{ false } 
{}

DrivingStatusUpdateMsg::DrivingStatusUpdateMsg( TheData t ) noexcept 
: SerialMessage( kDrivingStatusUpdate ), mContent( kDrivingStatusUpdate, t ), mNeedsAction{ true } 
{} 

DrivingStatusUpdateMsg::DrivingStatusUpdateMsg( std::uint8_t driveStatus ) noexcept 
: SerialMessage( kDrivingStatusUpdate ), mContent( kDrivingStatusUpdate, std::make_tuple( static_cast<std::uint8_t>( driveStatus ) ) ), mNeedsAction{ true } 
{}

DrivingStatusUpdateMsg::DrivingStatusUpdateMsg( MessageId id ) 
: SerialMessage( id ), mContent( kDrivingStatusUpdate ), mNeedsAction{ false }
{ 
    if ( id != kDrivingStatusUpdate ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, kDrivingStatusUpdate ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void DrivingStatusUpdateMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
}

void DrivingStatusUpdateMsg::sendOut( SerialLink& link )
{
    // This never sent from Pico
}

void DrivingStatusUpdateMsg::takeAction( EventManager& events, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        std::uint8_t val = std::get<0>( mContent.mMsg );

        // TODO take whatever action is appropirate
        output2cout( "RPi0 sent driving status ", val ); 
        output2cout( "TODO - implement action" );   
        mNeedsAction = false;
    }
}




/*********************************************************************************************/




EncoderUpdateMsg::EncoderUpdateMsg() noexcept 
: SerialMessage( kEncoderUpdate ), mContent( kEncoderUpdate ), mNeedsAction{ false } 
{}

EncoderUpdateMsg::EncoderUpdateMsg( TheData t ) noexcept 
: SerialMessage( kEncoderUpdate ), mContent( kEncoderUpdate, t ), mNeedsAction{ true }
{} 

EncoderUpdateMsg::EncoderUpdateMsg( int left, int right, uint32_t time ) noexcept 
: SerialMessage( kEncoderUpdate ), mContent( kEncoderUpdate, std::make_tuple( left, right, time ) ), mNeedsAction{ true } 
{}

EncoderUpdateMsg::EncoderUpdateMsg( MessageId id ) 
: SerialMessage( id ), mContent( kEncoderUpdate ), mNeedsAction{ false }
{ 
    if ( id != kEncoderUpdate ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, kEncoderUpdate ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void EncoderUpdateMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
}

void EncoderUpdateMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}

void EncoderUpdateMsg::takeAction( EventManager&, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // Only action is to send it
        sendOut( link );
        mNeedsAction = false;
    }
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
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, kErrorReportFromPico ), "Id mismatch at creation" ); 
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
    mContent.sendOut( link );
}

void ErrorReportMsg::takeAction( EventManager&, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // Only action is to send the message out
        sendOut( link );
        mNeedsAction = false;
    }
}




/*********************************************************************************************/




TestPicoErrorRptMsg::TestPicoErrorRptMsg() noexcept 
: SerialMessage( kTestPicoReportError ), mContent( kTestPicoReportError ), mNeedsAction{ false } 
{}

TestPicoErrorRptMsg::TestPicoErrorRptMsg( TheData t ) noexcept 
: SerialMessage( kTestPicoReportError ), mContent( kTestPicoReportError, t ), mNeedsAction{ true } 
{} 

TestPicoErrorRptMsg::TestPicoErrorRptMsg( bool makeItFatal, int errorCodeToReport ) noexcept 
: SerialMessage( kTestPicoReportError ), mContent( kTestPicoReportError, std::make_tuple( static_cast<std::uint8_t>( makeItFatal ), errorCodeToReport ) ), 
    mNeedsAction{ true } 
{}

TestPicoErrorRptMsg::TestPicoErrorRptMsg( MessageId id ) 
: SerialMessage( id ), mContent( kTestPicoReportError ), mNeedsAction{ false }
{ 
    if ( id != kTestPicoReportError ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, kTestPicoReportError ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void TestPicoErrorRptMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
}

void TestPicoErrorRptMsg::sendOut( SerialLink& link )
{
    // Pico never sends this message out (only receives)
    // Instead seend the requested error msg
    auto [ fatal, errorCode ] = mContent.mMsg;
    ErrorReportMsg errRptAsRqstd( fatal, errorCode );
    errRptAsRqstd.sendOut( link );
    output2cout( "As test sent error msg: fatal?: ", fatal, ", error code: ", errorCode );
}

void TestPicoErrorRptMsg::takeAction( EventManager&, SerialLink& link ) 
{
    // Create the requested error report and send it
    if ( mNeedsAction )
    {
        sendOut( link );
        mNeedsAction = false;
    }
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
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, kDebugSerialLink ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void DebugLinkMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;
//    auto [ v1, v2 ] = mContent.mMsg;
//    debugM( "DebugLinkMsg got" );
//    debugV( v1, v2 );
}

void DebugLinkMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}

void DebugLinkMsg::takeAction( EventManager& events, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // Lets negate the values, flip the order, and send them back
        auto [ val1, val2 ] = mContent.mMsg;

        switch ( val1 )
        {

            default: 
                val1 *= -1;
                val2 *= -1;
                DebugLinkMsg reply( val2, val1 );
                reply.sendOut( link );
                break;
        }
        mNeedsAction = false;
    }
}




/*********************************************************************************************/

