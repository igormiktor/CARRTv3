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
#include "OutputUtils.hpp"
#include "PicoState.h"

#include "DebugUtils.hpp"






UnknownMsg::UnknownMsg() noexcept 
: SerialMessage( MsgId::kUnknownMessage ), mContent( MsgId::kUnknownMessage ), mNeedsAction{ false } 
{}

UnknownMsg::UnknownMsg( TheData t ) noexcept 
: SerialMessage( MsgId::kUnknownMessage ), mContent( MsgId::kUnknownMessage, t ), mNeedsAction{ true } 
{} 

UnknownMsg::UnknownMsg( std::uint8_t rcvdId, int errorCode ) noexcept 
: SerialMessage( MsgId::kUnknownMessage ), mContent( MsgId::kUnknownMessage, std::make_tuple( rcvdId, errorCode ) ), 
    mNeedsAction{ true } 
{}

UnknownMsg::UnknownMsg( MsgId id ) 
: SerialMessage( id ), mContent( MsgId::kUnknownMessage ), mNeedsAction{ false }
{ 
    if ( id != MsgId::kUnknownMessage ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, std::to_underlying( MsgId::kUnknownMessage ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void UnknownMsg::readIn( SerialLink& link ) 
{
    // Unknown message; don't try to read it in.
    mNeedsAction = true;

    debug2cout( "Received unknown message: ", getIdNum() );
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
: SerialMessage( static_cast<MsgId>( id ) ), mId{ id }, mNeedsAction{ true } 
{
    // Nothing to do (MsgID is only info we need to be able to send)
}

NoContentMsg::NoContentMsg( MsgId id ) noexcept
: SerialMessage( id ), mId{ id }, mNeedsAction{ true }
{
    // Nothing to do
}


void NoContentMsg::readIn( SerialLink& link ) 
{
    // Nothing to do (we already have the ID if we call this function)
    debug2cout( "Got NoContentMsg", getIdNum() );
}

void NoContentMsg::sendOut( SerialLink& link )
{
    // Send the ID, it's the only thing we need to send
    link.put( static_cast<std::uint8_t>( mId ) );

    debug2cout( "Sent NoContentMsg", getIdNum() );
}

bool NoContentMsg::needsAction() const noexcept
{ 
    return mNeedsAction; 
}

MsgId NoContentMsg::getId() const noexcept
{ 
    return mId; 
}




/*********************************************************************************************/
/*********************************************************************************************/





PingMsg::PingMsg() noexcept
: NoContentMsg( MsgId::kPingMsg ), mSender( true )
{
    // Nothing else to do
}

PingMsg::PingMsg( MsgId id ) noexcept
: NoContentMsg( MsgId::kPingMsg ), mSender( false )
{
    // Nothing else to do
}


void PingMsg::takeAction( EventManager& events, SerialLink& link )
{
    // Action depends on whether we are sender or receiver
    if ( mNeedsAction )
    {
        if ( mSender )
        {
            sendOut( link );
            mNeedsAction = false;

            output2cout( "Sent ping to RPi0" );
        }
        else
        {
            // If we are receiver, we send PingReplyMsg
            PingReplyMsg pingReply{};
            pingReply.needsAction();
        }
    }
}




/*********************************************************************************************/




PingReplyMsg::PingReplyMsg() noexcept
: NoContentMsg( MsgId::kPingReplyMsg ), mSender( true )
{
    // Nothing else to do
}

PingReplyMsg::PingReplyMsg( MsgId id ) noexcept
: NoContentMsg( MsgId::kPingReplyMsg ), mSender( false )
{
    // Nothing to do
}


void PingReplyMsg::takeAction( EventManager& events, SerialLink& link )
{
    // Action depends on whether we are sender or receiver
    if ( mNeedsAction )
    {
        if ( mSender )
        {
            sendOut( link );
            mNeedsAction = false;

            output2cout( "Send ping reply to RPi0" );
        }
        else
        {
            // If we are receiver, we simply log it
            output2cout( "Rcvd ping reply from RPi0" );

            // Could do something fancier like track we sent ping and match this reply to it
            // But meant for debugging, so just leave it in our std::cout stream
        }
    }
}




/*********************************************************************************************/




PicoReadyMsg::PicoReadyMsg() noexcept 
: SerialMessage( MsgId::kPicoReady ), mContent( MsgId::kPicoReady ), mNeedsAction{ false } 
{}

PicoReadyMsg::PicoReadyMsg( TheData t ) noexcept 
: SerialMessage( MsgId::kPicoReady ), mContent( MsgId::kPicoReady, t ), mNeedsAction{ true }
{} 

PicoReadyMsg::PicoReadyMsg( std::uint32_t time ) noexcept 
: SerialMessage( MsgId::kPicoReady ), mContent( MsgId::kPicoReady, std::make_tuple( static_cast<std::uint32_t>( time ) ) ), mNeedsAction{ true } 
{}

PicoReadyMsg::PicoReadyMsg( MsgId id ) 
: SerialMessage( id ), mContent( MsgId::kPicoReady ), mNeedsAction{ false }
{ 
    if ( id != MsgId::kPicoReady ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, std::to_underlying( MsgId::kPicoReady ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void PicoReadyMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = false;

    output2cout( "Error: Pico got PicoReadyMsg", getIdNum(), std::get<0>( mContent.mMsg ) );
}

void PicoReadyMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );

    debug2cout( "Sent PicoReadyMsg", getIdNum(), std::get<0>( mContent.mMsg ) );
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
: SerialMessage( MsgId::kPicoNavStatusUpdate ), mContent( MsgId::kPicoNavStatusUpdate ), mNeedsAction{ false } 
{}

PicoNavStatusUpdateMsg::PicoNavStatusUpdateMsg( TheData t ) noexcept 
: SerialMessage( MsgId::kPicoNavStatusUpdate ), mContent( MsgId::kPicoNavStatusUpdate, t ), mNeedsAction{ true }
{} 

PicoNavStatusUpdateMsg::PicoNavStatusUpdateMsg( bool status, std::uint8_t m, std::uint8_t a, std::uint8_t g, std::uint8_t s ) noexcept 
: SerialMessage( MsgId::kPicoNavStatusUpdate ), mContent( MsgId::kPicoNavStatusUpdate, std::make_tuple( status, m, a, g, s ) ), mNeedsAction{ true } 
{}

PicoNavStatusUpdateMsg::PicoNavStatusUpdateMsg( MsgId id ) 
: SerialMessage( id ), mContent( MsgId::kPicoNavStatusUpdate ), mNeedsAction{ false }
{ 
    if ( id != MsgId::kPicoNavStatusUpdate ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, std::to_underlying( MsgId::kPicoNavStatusUpdate ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void PicoNavStatusUpdateMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    output2cout( "Error: Pico got PicoNavStatusUpdateMsg", 
        getIdNum(), std::get<0>( mContent.mMsg ), std::get<1>( mContent.mMsg ), std::get<2>( mContent.mMsg ), 
        std::get<3>( mContent.mMsg ), std::get<4>( mContent.mMsg ) );
}

void PicoNavStatusUpdateMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );

    debug2cout( "Pico sent PicoNavStatusUpdateMsg", 
        getIdNum(), std::get<0>( mContent.mMsg ), std::get<1>( mContent.mMsg ), std::get<2>( mContent.mMsg ), 
        std::get<3>( mContent.mMsg ), std::get<4>( mContent.mMsg ) );
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
: NoContentMsg( MsgId::kPicoSaysStop )
{
    mNeedsAction = true;
}

PicoSaysStopMsg::PicoSaysStopMsg( MsgId id ) noexcept
: NoContentMsg( id )
{
    // Nothing to do
}


void PicoSaysStopMsg::takeAction( EventManager& events, SerialLink& link )
{
    if ( mNeedsAction )
    {
        sendOut( link );
        mNeedsAction = false;

        output2cout( "Pico sends stop to RPI0" );
    }
}




/*********************************************************************************************/





MsgControlMsg::MsgControlMsg() noexcept 
: SerialMessage( MsgId::kMsgControlMsg ), mContent( MsgId::kMsgControlMsg ), mNeedsAction{ false } 
{}

MsgControlMsg::MsgControlMsg( TheData t ) noexcept 
: SerialMessage( MsgId::kMsgControlMsg ), mContent( MsgId::kMsgControlMsg, t ), mNeedsAction{ true } 
{} 

MsgControlMsg::MsgControlMsg( bool val ) noexcept 
: SerialMessage( MsgId::kMsgControlMsg ), mContent( MsgId::kMsgControlMsg, std::make_tuple( static_cast<std::uint8_t>( val ) ) ), mNeedsAction{ true } 
{}

MsgControlMsg::MsgControlMsg( MsgId id ) 
: SerialMessage( id ), mContent( MsgId::kMsgControlMsg ), mNeedsAction{ false }
{ 
    if ( id != MsgId::kMsgControlMsg ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, std::to_underlying( MsgId::kMsgControlMsg ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void MsgControlMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    debug2cout( "Pico got MsgControlMsg", getIdNum(), static_cast<int>( std::get<0>( mContent.mMsg ) ) );
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
        mNeedsAction = false;

        output2cout( "MsgControlMsg received, new values are" );  
        output2cout( " sendTimerMsgs", static_cast<bool>( val & kTimerMsgMask ) );
        output2cout( " sendNavMsgs", static_cast<bool>( val & kNavMsgMask ) );
        output2cout( " sendNavStatusMsgs", static_cast<bool>( val & kNavStatusMask ) );
        output2cout( " sendEncoderMsgs", static_cast<bool>( val & kEncoderMsgMask ) );
        output2cout( " sendCalibrationMsgs", static_cast<bool>( val & kCalibrationMsgMask ) );
    }
}




/*********************************************************************************************/




ResetMsg::ResetMsg() noexcept
: NoContentMsg( MsgId::kResetMsg )
{
    mNeedsAction = true;
}

ResetMsg::ResetMsg( MsgId id ) noexcept
: NoContentMsg( id )
{
    // Nothing to do
}


void ResetMsg::takeAction( EventManager& events, SerialLink& link )
{
    if ( mNeedsAction )
    {
        events.queueEvent( EvtId::kPicoResetEvent, 0, 0, EventManager::kHighPriority );
        mNeedsAction = false;

        output2cout( "Pico got message from RPi0 to reset" );
    }
}




/*********************************************************************************************/




TimerEventMsg::TimerEventMsg() noexcept 
: SerialMessage( MsgId::kTimerEventMsg ), mContent( MsgId::kTimerEventMsg ), mNeedsAction{ false } 
{}

TimerEventMsg::TimerEventMsg( TheData t ) noexcept 
: SerialMessage( MsgId::kTimerEventMsg ), mContent( MsgId::kTimerEventMsg, t ), mNeedsAction{ true }
{} 

TimerEventMsg::TimerEventMsg( std::uint8_t which, int count, uint32_t time ) noexcept 
: SerialMessage( MsgId::kTimerEventMsg ), mContent( MsgId::kTimerEventMsg, std::make_tuple( which, count, time ) ), mNeedsAction{ true } 
{}

TimerEventMsg::TimerEventMsg( MsgId id ) 
: SerialMessage( id ), mContent( MsgId::kTimerEventMsg ), mNeedsAction{ false }
{ 
    if ( id != MsgId::kTimerEventMsg ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, std::to_underlying( MsgId::kTimerEventMsg ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void TimerEventMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    output2cout( "Error: Pico should never receive TimerEventMsg", 
        getIdNum(), static_cast<int>( std::get<0>( mContent.mMsg ) ), std::get<1>( mContent.mMsg ), std::get<2>( mContent.mMsg ) );
}

void TimerEventMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );

    debug2cout( "Sent TimerEventMsg", 
        getIdNum(), static_cast<int>( std::get<0>( mContent.mMsg ) ), std::get<1>( mContent.mMsg ), std::get<2>( mContent.mMsg ) );
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
: SerialMessage( MsgId::kTimerControl ), mContent( MsgId::kTimerControl ), mNeedsAction{ false } 
{}

TimerControlMsg::TimerControlMsg( TheData t ) noexcept 
: SerialMessage( MsgId::kTimerControl ), mContent( MsgId::kTimerControl, t ), mNeedsAction{ true } 
{} 

TimerControlMsg::TimerControlMsg( bool val ) noexcept 
: SerialMessage( MsgId::kTimerControl ), mContent( MsgId::kTimerControl, std::make_tuple( static_cast<std::uint8_t>( val ) ) ), mNeedsAction{ true } 
{}

TimerControlMsg::TimerControlMsg( MsgId id ) 
: SerialMessage( id ), mContent( MsgId::kTimerControl ), mNeedsAction{ false }
{ 
    if ( id != MsgId::kTimerControl ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, std::to_underlying( MsgId::kTimerControl ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void TimerControlMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    debug2cout( "Pico got TimerControlMsg", getIdNum(), static_cast<bool>( std::get<0>( mContent.mMsg ) ) );
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
        mNeedsAction = false;

        output2cout( "Timer events to RPi0 turned to", val );    
    }
}




/*********************************************************************************************/





BeginCalibrationMsg::BeginCalibrationMsg() noexcept
: NoContentMsg( MsgId::kBeginCalibration )
{
    mNeedsAction = true;
}

BeginCalibrationMsg::BeginCalibrationMsg( MsgId id ) noexcept
: NoContentMsg( id )
{
    // Nothing to do
}


void BeginCalibrationMsg::takeAction( EventManager& events, SerialLink& link )
{
    if ( mNeedsAction )
    {
        events.queueEvent( EvtId::kBNO055BeginCalibrationEvent );
        mNeedsAction = false;

        output2cout( "Got begin calibration msg: Trigger calibration" );
    }
}




/*********************************************************************************************/





RequestCalibrationStatusMsg::RequestCalibrationStatusMsg() noexcept
: NoContentMsg( MsgId::kRequestCalibStatus )
{
    mNeedsAction = true;
}

RequestCalibrationStatusMsg::RequestCalibrationStatusMsg( MsgId id ) noexcept
: NoContentMsg( id )
{
    // Nothing to do
}


void RequestCalibrationStatusMsg::takeAction( EventManager& events, SerialLink& link )
{
    if ( mNeedsAction )
    {
        // Even if PicoState::wantNavStatusMsgs() is false, we always respond to direct request
        auto calibData{ BNO055::getCalibration() };
        bool status = BNO055::calibrationGood( calibData );

        PicoState::navCalibrated( status );
        PicoNavStatusUpdateMsg navReadyStatus( status, calibData.mag, calibData.accel, calibData.gyro, calibData.system );
        navReadyStatus.takeAction( events, link );

        mNeedsAction = false;
 
        output2cout( "Got a request calib status msg" );
    }
}




/*********************************************************************************************/




SendCalibrationInfoMsg::SendCalibrationInfoMsg() noexcept
: SerialMessage( MsgId::kSendCalibInfo ), mContent( MsgId::kSendCalibInfo ), mNeedsAction{ false }
{}

SendCalibrationInfoMsg::SendCalibrationInfoMsg( TheData t ) noexcept
: SerialMessage( MsgId::kSendCalibInfo ), mContent( MsgId::kSendCalibInfo, t ), mNeedsAction{ true }
{}


SendCalibrationInfoMsg::SendCalibrationInfoMsg( std::uint8_t mag, std::uint8_t accel, std::uint8_t gyro, std::uint8_t sys ) noexcept
: SerialMessage( MsgId::kSendCalibInfo ), mContent( MsgId::kSendCalibInfo, std::make_tuple( mag, accel, gyro, sys) ), mNeedsAction{ true }
{}


SendCalibrationInfoMsg::SendCalibrationInfoMsg( MsgId id )
: SerialMessage( id ), mContent( MsgId::kSendCalibInfo ), mNeedsAction{ false }
{
    if ( id != MsgId::kSendCalibInfo ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1,std::to_underlying( MsgId::kSendCalibInfo ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void SendCalibrationInfoMsg::readIn( SerialLink& link )
{
    mContent.readIn( link );
    mNeedsAction = true;

    output2cout( "Error: received sendCalibrationInfoMsg", getIdNum(), 
        static_cast<int>( std::get<0>( mContent.mMsg ) ), static_cast<int>( std::get<1>( mContent.mMsg ) ), 
        static_cast<int>( std::get<2>( mContent.mMsg ) ), static_cast<int>( std::get<3>( mContent.mMsg ) ) );
}


void SendCalibrationInfoMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );

    debug2cout( "Sent sendCalibrationInfoMsg", getIdNum(), 
        static_cast<int>( std::get<0>( mContent.mMsg ) ), static_cast<int>( std::get<1>( mContent.mMsg ) ), 
        static_cast<int>( std::get<2>( mContent.mMsg ) ), static_cast<int>( std::get<3>( mContent.mMsg ) ) );
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




SetAutoCalibrateMsg::SetAutoCalibrateMsg() noexcept 
: SerialMessage( MsgId::kSetAutoCalibrate ), mContent( MsgId::kSetAutoCalibrate ), mNeedsAction{ false } 
{}

SetAutoCalibrateMsg::SetAutoCalibrateMsg( TheData t ) noexcept 
: SerialMessage( MsgId::kSetAutoCalibrate ), mContent( MsgId::kSetAutoCalibrate, t ), mNeedsAction{ true } 
{} 

SetAutoCalibrateMsg::SetAutoCalibrateMsg( bool val ) noexcept 
: SerialMessage( MsgId::kSetAutoCalibrate ), mContent( MsgId::kSetAutoCalibrate, std::make_tuple( static_cast<std::uint8_t>( val ) ) ), mNeedsAction{ true } 
{}

SetAutoCalibrateMsg::SetAutoCalibrateMsg( MsgId id ) 
: SerialMessage( id ), mContent( MsgId::kSetAutoCalibrate ), mNeedsAction{ false }
{ 
    if ( id != MsgId::kSetAutoCalibrate ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, std::to_underlying( MsgId::kSetAutoCalibrate ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void SetAutoCalibrateMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    debug2cout( "Got SetAutoCalibrateMsg", getIdNum(), static_cast<bool>( std::get<0>( mContent.mMsg) ) );
}

void SetAutoCalibrateMsg::sendOut( SerialLink& link )
{
    // This never sent from Pico
}

void SetAutoCalibrateMsg::takeAction( EventManager& events, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        bool val = std::get<0>( mContent.mMsg );
        PicoState::wantAutoCalibrate( val );
        mNeedsAction = false;

        output2cout( "Pico autocalibration set to ", val );    
    }
}




/*********************************************************************************************/





ResetBNO055Msg::ResetBNO055Msg() noexcept
: NoContentMsg( MsgId::kResetBNO055 )
{
    mNeedsAction = true;
}

ResetBNO055Msg::ResetBNO055Msg( MsgId id ) noexcept
: NoContentMsg( id )
{
    // Nothing to do
}


void ResetBNO055Msg::takeAction( EventManager& events, SerialLink& link )
{
    if ( mNeedsAction )
    {
        events.queueEvent( EvtId::kBNO055ResetEvent );
        mNeedsAction = false;

        output2cout( "Got ResetBNO055Msg" );
    }
}




/*********************************************************************************************/




NavUpdateMsg::NavUpdateMsg() noexcept
: SerialMessage( MsgId::kTimerNavUpdate ), mContent( MsgId::kTimerNavUpdate ), mNeedsAction{ false }
{}

NavUpdateMsg::NavUpdateMsg( TheData t ) noexcept
: SerialMessage( MsgId::kTimerNavUpdate ), mContent( MsgId::kTimerNavUpdate, t ), mNeedsAction{ true }
{}


NavUpdateMsg::NavUpdateMsg(  float heading, std::uint32_t time  ) noexcept
: SerialMessage( MsgId::kTimerNavUpdate ), mContent( MsgId::kTimerNavUpdate, std::make_tuple( heading, time ) ), mNeedsAction{ true }
{}


NavUpdateMsg::NavUpdateMsg( MsgId id )
: SerialMessage( MsgId::kTimerNavUpdate ), mContent( MsgId::kTimerNavUpdate ), mNeedsAction{ false }
{
    if ( id != MsgId::kTimerNavUpdate ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, std::to_underlying( MsgId::kTimerNavUpdate ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void NavUpdateMsg::readIn( SerialLink& link )
{
    mContent.readIn( link );
    mNeedsAction = true;

    output2cout( "Error: got NavUpdateMsg", getIdNum(), std::get<0>( mContent.mMsg ), std::get<1>( mContent.mMsg ) );
}


void NavUpdateMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );

    debug2cout( "Sent NavUpdateMsg", getIdNum(), std::get<0>( mContent.mMsg ), std::get<1>( mContent.mMsg ) );
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
: SerialMessage( MsgId::kNavUpdateControl ), mContent( MsgId::kNavUpdateControl ), mNeedsAction{ false } 
{}

NavUpdateControlMsg::NavUpdateControlMsg( TheData t ) noexcept 
: SerialMessage( MsgId::kNavUpdateControl ), mContent( MsgId::kNavUpdateControl, t ), mNeedsAction{ true } 
{} 

NavUpdateControlMsg::NavUpdateControlMsg( bool val ) noexcept 
: SerialMessage( MsgId::kNavUpdateControl ), mContent( MsgId::kNavUpdateControl, std::make_tuple( static_cast<std::uint8_t>( val ) ) ), mNeedsAction{ true } 
{}

NavUpdateControlMsg::NavUpdateControlMsg( MsgId id ) 
: SerialMessage( id ), mContent( MsgId::kNavUpdateControl ), mNeedsAction{ false }
{ 
    if ( id != MsgId::kNavUpdateControl ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, std::to_underlying( MsgId::kNavUpdateControl ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void NavUpdateControlMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    debug2cout( "Got NavUpdateControlMsg", getIdNum(), static_cast<bool>( std::get<0>( mContent.mMsg) ) );
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
        mNeedsAction = false;

        output2cout( "Nav update events to RPi0 set to ", val );    
    }
}




/*********************************************************************************************/




DrivingStatusUpdateMsg::DrivingStatusUpdateMsg() noexcept 
: SerialMessage( MsgId::kDrivingStatusUpdate ), mContent( MsgId::kDrivingStatusUpdate ), mNeedsAction{ false } 
{}

DrivingStatusUpdateMsg::DrivingStatusUpdateMsg( TheData t ) noexcept 
: SerialMessage( MsgId::kDrivingStatusUpdate ), mContent( MsgId::kDrivingStatusUpdate, t ), mNeedsAction{ true } 
{} 

DrivingStatusUpdateMsg::DrivingStatusUpdateMsg( Drive driveStatus ) noexcept 
: SerialMessage( MsgId::kDrivingStatusUpdate ), mContent( MsgId::kDrivingStatusUpdate, std::make_tuple( std::to_underlying( driveStatus ) ) ), mNeedsAction{ true } 
{}

DrivingStatusUpdateMsg::DrivingStatusUpdateMsg( MsgId id ) 
: SerialMessage( id ), mContent( MsgId::kDrivingStatusUpdate ), mNeedsAction{ false }
{ 
    if ( id != MsgId::kDrivingStatusUpdate ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, std::to_underlying( MsgId::kDrivingStatusUpdate ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void DrivingStatusUpdateMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    debug2cout( "Got DrivingStatusUpdateMsg", getIdNum(), static_cast<int>( std::get<0>( mContent.mMsg ) ) );
}

void DrivingStatusUpdateMsg::sendOut( SerialLink& link )
{
    // This never sent from Pico
}

void DrivingStatusUpdateMsg::takeAction( EventManager& events, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        std::uint8_t driveStatus = std::get<0>( mContent.mMsg );

        // TODO take whatever action is appropirate
        output2cout( "RPi0 sent driving status ", static_cast<int>( driveStatus ) ); 
        output2cout( "TODO - implement action" );   
        mNeedsAction = false;
    }
}




/*********************************************************************************************/




EncoderUpdateMsg::EncoderUpdateMsg() noexcept 
: SerialMessage( MsgId::kEncoderUpdate ), mContent( MsgId::kEncoderUpdate ), mNeedsAction{ false } 
{}

EncoderUpdateMsg::EncoderUpdateMsg( TheData t ) noexcept 
: SerialMessage( MsgId::kEncoderUpdate ), mContent( MsgId::kEncoderUpdate, t ), mNeedsAction{ true }
{} 

EncoderUpdateMsg::EncoderUpdateMsg( int left, int right, uint32_t time ) noexcept 
: SerialMessage( MsgId::kEncoderUpdate ), mContent( MsgId::kEncoderUpdate, std::make_tuple( left, right, time ) ), mNeedsAction{ true } 
{}

EncoderUpdateMsg::EncoderUpdateMsg( MsgId id ) 
: SerialMessage( id ), mContent( MsgId::kEncoderUpdate ), mNeedsAction{ false }
{ 
    if ( id != MsgId::kEncoderUpdate ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, std::to_underlying( MsgId::kEncoderUpdate ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void EncoderUpdateMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    output2cout( "Error: got EncoderUpdateMsg", std::get<0>( mContent.mMsg ), std::get<1>( mContent.mMsg ), std::get<2>( mContent.mMsg ) );
}

void EncoderUpdateMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );

    debug2cout( "Sent EncoderUpdateMsg", std::get<0>( mContent.mMsg ), std::get<1>( mContent.mMsg ), std::get<2>( mContent.mMsg ) );
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




EncoderUpdateControlMsg::EncoderUpdateControlMsg() noexcept 
: SerialMessage( MsgId::kEncoderUpdateControl ), mContent( MsgId::kEncoderUpdateControl ), mNeedsAction{ false } 
{}

EncoderUpdateControlMsg::EncoderUpdateControlMsg( TheData t ) noexcept 
: SerialMessage( MsgId::kEncoderUpdateControl ), mContent( MsgId::kEncoderUpdateControl, t ), mNeedsAction{ true } 
{} 

EncoderUpdateControlMsg::EncoderUpdateControlMsg( bool val ) noexcept 
: SerialMessage( MsgId::kEncoderUpdateControl ), mContent( MsgId::kEncoderUpdateControl, std::make_tuple( static_cast<std::uint8_t>( val ) ) ), mNeedsAction{ true } 
{}

EncoderUpdateControlMsg::EncoderUpdateControlMsg( MsgId id ) 
: SerialMessage( id ), mContent( MsgId::kEncoderUpdateControl ), mNeedsAction{ false }
{ 
    if ( id != MsgId::kEncoderUpdateControl ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, std::to_underlying( MsgId::kEncoderUpdateControl ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void EncoderUpdateControlMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    debug2cout( "Got EncoderUpdateControlMsg", getIdNum(), static_cast<bool>( std::get<0>( mContent.mMsg ) ) );
}

void EncoderUpdateControlMsg::sendOut( SerialLink& link )
{
    // This never sent from Pico
}

void EncoderUpdateControlMsg::takeAction( EventManager& events, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        bool val = std::get<0>( mContent.mMsg );
        PicoState::sendEncoderMsgs( val );
        mNeedsAction = false;

        output2cout( "Encoder update events to RPi0 turned to ", val );    
    }
}




/*********************************************************************************************/




BatteryLevelRequestMsg::BatteryLevelRequestMsg() noexcept 
: SerialMessage( MsgId::kBatteryLevelRequest ), mContent( MsgId::kBatteryLevelRequest ), mNeedsAction{ false } 
{}

BatteryLevelRequestMsg::BatteryLevelRequestMsg( TheData t ) noexcept 
: SerialMessage( MsgId::kBatteryLevelRequest ), mContent( MsgId::kBatteryLevelRequest, t ), mNeedsAction{ true } 
{} 

BatteryLevelRequestMsg::BatteryLevelRequestMsg( Battery whichBattery ) noexcept 
: SerialMessage( MsgId::kBatteryLevelRequest ), mContent( MsgId::kBatteryLevelRequest, std::make_tuple( std::to_underlying( whichBattery ) ) ), mNeedsAction{ true } 
{}

BatteryLevelRequestMsg::BatteryLevelRequestMsg( MsgId id ) 
: SerialMessage( id ), mContent( MsgId::kBatteryLevelRequest ), mNeedsAction{ false }
{ 
    if ( id != MsgId::kBatteryLevelRequest ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, std::to_underlying( MsgId::kBatteryLevelRequest ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void BatteryLevelRequestMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    debug2cout( "Got BatteryLevelRequestMsg", static_cast<int>( std::get<0>( mContent.mMsg ) ) );
}

void BatteryLevelRequestMsg::sendOut( SerialLink& link )
{
    // This never sent from Pico
}

void BatteryLevelRequestMsg::takeAction( EventManager& events, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        std::uint8_t whichBattery = std::get<0>( mContent.mMsg );
        if ( whichBattery == std::to_underlying( Battery::kIcBattery ) || whichBattery == std::to_underlying( Battery::kBothBatteries ) )
        {
            // TODO
        }
        else if ( whichBattery == std::to_underlying( Battery::kMotorBattery ) || whichBattery == std::to_underlying( Battery::kBothBatteries ) )
        {
            // TODO
        }
        else
        {
            output2cout( "Bad battery request code", static_cast<int>( whichBattery ) );
            ErrorReportMsg err( false, makePicoErrorId( kPicoSerialMessageError, 2, whichBattery ) );
        }

        // TODO take whatever action is appropirate
        output2cout( "RPi0 requested battery level for ", static_cast<int>( whichBattery ) ); 
        output2cout( "TODO - implement action" );   
        mNeedsAction = false;
    }
}




/*********************************************************************************************/




BatteryLevelUpdateMsg::BatteryLevelUpdateMsg() noexcept 
: SerialMessage( MsgId::kBatteryLevelUpdate ), mContent( MsgId::kBatteryLevelUpdate ), mNeedsAction{ false } 
{}

BatteryLevelUpdateMsg::BatteryLevelUpdateMsg( TheData t ) noexcept 
: SerialMessage( MsgId::kBatteryLevelUpdate ), mContent( MsgId::kBatteryLevelUpdate, t ), mNeedsAction{ true } 
{} 

BatteryLevelUpdateMsg::BatteryLevelUpdateMsg( Battery whichBattery, float level ) noexcept 
: SerialMessage( MsgId::kBatteryLevelUpdate ), mContent( MsgId::kBatteryLevelUpdate, std::make_tuple( std::to_underlying( whichBattery ), level ) ), 
    mNeedsAction{ true } 
{}

BatteryLevelUpdateMsg::BatteryLevelUpdateMsg( MsgId id ) 
: SerialMessage( id ), mContent( MsgId::kBatteryLevelUpdate ), mNeedsAction{ false }
{ 
    if ( id != MsgId::kBatteryLevelUpdate ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, std::to_underlying( MsgId::kBatteryLevelUpdate ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void BatteryLevelUpdateMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    output2cout( "Error: got BatteryLevelUpdateMsg", getIdNum(), static_cast<int>( std::get<0>( mContent.mMsg ) ), std::get<1>( mContent.mMsg ) );
}

void BatteryLevelUpdateMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );

    debug2cout( "Sent BatteryLevelUpdateMsg", getIdNum(), static_cast<int>( std::get<0>( mContent.mMsg ) ), std::get<1>( mContent.mMsg ) );
}

void BatteryLevelUpdateMsg::takeAction( EventManager&, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // Only action is to send it
        sendOut( link );
        mNeedsAction = false;
    }
}




/*********************************************************************************************/




BatteryLowAlertMsg::BatteryLowAlertMsg() noexcept 
: SerialMessage( MsgId::kBatteryLowAlert ), mContent( MsgId::kBatteryLowAlert ), mNeedsAction{ false } 
{}

BatteryLowAlertMsg::BatteryLowAlertMsg( TheData t ) noexcept 
: SerialMessage( MsgId::kBatteryLowAlert ), mContent( MsgId::kBatteryLowAlert, t ), mNeedsAction{ true } 
{} 

BatteryLowAlertMsg::BatteryLowAlertMsg( Battery whichBattery, float level ) noexcept 
: SerialMessage( MsgId::kBatteryLowAlert ), mContent( MsgId::kBatteryLowAlert, std::make_tuple( std::to_underlying( whichBattery ), level ) ), 
    mNeedsAction{ true } 
{}

BatteryLowAlertMsg::BatteryLowAlertMsg( MsgId id ) 
: SerialMessage( id ), mContent( MsgId::kBatteryLowAlert ), mNeedsAction{ false }
{ 
    if ( id != MsgId::kBatteryLowAlert ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, std::to_underlying( MsgId::kBatteryLowAlert ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void BatteryLowAlertMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    output2cout( "Error: got BatteryLowAlertMsg", getIdNum(), static_cast<int>( std::get<0>( mContent.mMsg ) ) );
}

void BatteryLowAlertMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );

    output2cout( "Sent BatteryLowAlertMsg", getIdNum(), static_cast<int>( std::get<0>( mContent.mMsg ) ) );
}

void BatteryLowAlertMsg::takeAction( EventManager&, SerialLink& link ) 
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
: SerialMessage( MsgId::kErrorReportFromPico ), mContent( MsgId::kErrorReportFromPico ), mNeedsAction{ false } 
{}

ErrorReportMsg::ErrorReportMsg( TheData t ) noexcept 
: SerialMessage( MsgId::kErrorReportFromPico ), mContent( MsgId::kErrorReportFromPico, t ), mNeedsAction{ true } 
{} 

ErrorReportMsg::ErrorReportMsg( bool val, int errorCode ) noexcept 
: SerialMessage( MsgId::kErrorReportFromPico ), mContent( MsgId::kErrorReportFromPico, std::make_tuple( static_cast<std::uint8_t>( val ), errorCode ) ), 
    mNeedsAction{ true } 
{}

ErrorReportMsg::ErrorReportMsg( MsgId id ) 
: SerialMessage( id ), mContent( MsgId::kErrorReportFromPico ), mNeedsAction{ false }
{ 
    if ( id != MsgId::kErrorReportFromPico ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, std::to_underlying( MsgId::kErrorReportFromPico ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void ErrorReportMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    output2cout( "Error: got ErrorReportMsg", getIdNum(), static_cast<bool>( std::get<0>( mContent.mMsg ) ), std::get<1>( mContent.mMsg ) );
}

void ErrorReportMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );

    debug2cout( "Sent ErrorReportMsg", getIdNum(), static_cast<bool>( std::get<0>( mContent.mMsg ) ), std::get<1>( mContent.mMsg ) );
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
: SerialMessage( MsgId::kTestPicoReportError ), mContent( MsgId::kTestPicoReportError ), mNeedsAction{ false } 
{}

TestPicoErrorRptMsg::TestPicoErrorRptMsg( TheData t ) noexcept 
: SerialMessage( MsgId::kTestPicoReportError ), mContent( MsgId::kTestPicoReportError, t ), mNeedsAction{ true } 
{} 

TestPicoErrorRptMsg::TestPicoErrorRptMsg( bool makeItFatal, int errorCodeToReport ) noexcept 
: SerialMessage( MsgId::kTestPicoReportError ), mContent( MsgId::kTestPicoReportError, std::make_tuple( static_cast<std::uint8_t>( makeItFatal ), errorCodeToReport ) ), 
    mNeedsAction{ true } 
{}

TestPicoErrorRptMsg::TestPicoErrorRptMsg( MsgId id ) 
: SerialMessage( id ), mContent( MsgId::kTestPicoReportError ), mNeedsAction{ false }
{ 
    if ( id != MsgId::kTestPicoReportError ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, std::to_underlying( MsgId::kTestPicoReportError ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void TestPicoErrorRptMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    debug2cout( "Got TestPicoErrorRptMsg", getIdNum(), static_cast<bool>( std::get<0>( mContent.mMsg ) ), std::get<1>( mContent.mMsg ) );
}

void TestPicoErrorRptMsg::sendOut( SerialLink& link )
{
    // Pico never sends this message out (only receives)
    // Instead seend the requested error msg
    auto [ fatal, errorCode ] = mContent.mMsg;
    ErrorReportMsg errRptAsRqstd( fatal, errorCode );
    errRptAsRqstd.sendOut( link );

    output2cout( "Sent test error msg: fatal?", fatal, "error code", errorCode );
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
: SerialMessage( MsgId::kDebugSerialLink ), mContent( MsgId::kDebugSerialLink ), mNeedsAction{ false } 
{}

DebugLinkMsg::DebugLinkMsg( TheData t ) noexcept 
: SerialMessage( MsgId::kDebugSerialLink ), mContent( MsgId::kDebugSerialLink, t ), mNeedsAction{ true } 
{} 

DebugLinkMsg::DebugLinkMsg( int val1, int val2 ) noexcept 
: SerialMessage( MsgId::kDebugSerialLink ), mContent( MsgId::kDebugSerialLink, std::make_tuple( val1, val2 ) ), mNeedsAction{ true } 
{}

DebugLinkMsg::DebugLinkMsg( MsgId id ) 
: SerialMessage( id ), mContent( MsgId::kDebugSerialLink ), mNeedsAction{ false }          
{ 
    if ( id != MsgId::kDebugSerialLink ) 
    { 
        throw CarrtError( makePicoErrorId( kPicoSerialMessageError, 1, std::to_underlying( MsgId::kDebugSerialLink ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void DebugLinkMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    output2cout( "Got DebugLinkMsg", std::get<0>( mContent.mMsg ), std::get<1>( mContent.mMsg ) );
}

void DebugLinkMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );

    output2cout( "Sent DebugLinkMsg", std::get<0>( mContent.mMsg ), std::get<1>( mContent.mMsg ) );
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

