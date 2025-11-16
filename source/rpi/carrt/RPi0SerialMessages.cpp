/*
    RPi0SerialMessages.cpp - Serial Messages for CARRT3 communications
    between the RPI and Pico.  This file contains the RPi0 message handling.

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



#include "OutputUtils.hpp"
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
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, std::to_underlying( MsgId::kUnknownMessage ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void UnknownMsg::readIn( SerialLink& link ) 
{
    // Unknown message; don't try to read it in.
    mNeedsAction = true;

    debug2cout( "Got unknown message: ", getIdNum() );
}

void UnknownMsg::sendOut( SerialLink& link )
{
   // RPi0 never sends this out
   output2cout( "Error: RPI0 tried to send UnknownMsg" );
}

void UnknownMsg::takeAction( EventManager& events, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // TODO take some real action (report somewhere?)
        output2cout( "RPi0 got UnknownMsg", getIdNum() );
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

            output2cout( "RPi0 sent PingMsg" );
        }
        else
        {
            // If we are receiver, we send PingReplyMsg
            PingReplyMsg pingReply{};
            pingReply.takeAction( events, link );

            output2cout( "Rpi0 got PingMsg, sent PingReplyMsg" );
        }

        mNeedsAction = false;
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

            output2cout( "RPi0 sent PingReplyMsg" );
        }
        else
        {
            // If we are receiver, we simply log it
            output2cout( "RPi0 got PingReplyMsg" );

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
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, std::to_underlying( MsgId::kPicoReady ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void PicoReadyMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    debug2cout( "Got PicoReadyMsg", getIdNum(), std::get<0>( mContent.mMsg ) );
}

void PicoReadyMsg::sendOut( SerialLink& link )
{
    // RPi0 should never send
    output2cout( "RPi0 trying to send PicoReadyMsg", getIdNum(), std::get<0>( mContent.mMsg ) );
}

void PicoReadyMsg::takeAction( EventManager&, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // TODO -- RPi0 needs to take action
        mNeedsAction = false;

        output2cout( "TODO: RPi0 action on PicoReadyMsg" );
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
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, std::to_underlying( MsgId::kPicoNavStatusUpdate ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void PicoNavStatusUpdateMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    debug2cout( "Got PicoNavStatusUpdateMsg", 
        getIdNum(), std::get<0>( mContent.mMsg ), std::get<1>( mContent.mMsg ), std::get<2>( mContent.mMsg ), 
        std::get<3>( mContent.mMsg ), std::get<4>( mContent.mMsg ) );
}

void PicoNavStatusUpdateMsg::sendOut( SerialLink& link )
{
    output2cout( "Error: RPi0 sending PicoNavStatusUpdateMsg", 
        getIdNum(), std::get<0>( mContent.mMsg ), std::get<1>( mContent.mMsg ), std::get<2>( mContent.mMsg ), 
        std::get<3>( mContent.mMsg ), std::get<4>( mContent.mMsg ) );
}

void PicoNavStatusUpdateMsg::takeAction( EventManager&, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // TODO -- take action
        mNeedsAction = false;

        output2cout( "TODO: RPi0 action on PicoNavStatusUpdateMsg" );
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
        // TODO take action
        mNeedsAction = false;

        output2cout( "TODO: RPi0 action on PicoSayStopMsg" );
    }
}




/*********************************************************************************************/





MsgControlMsg::MsgControlMsg() noexcept 
: SerialMessage( MsgId::kMsgControlMsg ), mContent( MsgId::kMsgControlMsg ), mNeedsAction{ false } 
{}

MsgControlMsg::MsgControlMsg( TheData t ) noexcept 
: SerialMessage( MsgId::kMsgControlMsg ), mContent( MsgId::kMsgControlMsg, t ), mNeedsAction{ true } 
{} 

MsgControlMsg::MsgControlMsg( std::uint8_t values ) noexcept 
: SerialMessage( MsgId::kMsgControlMsg ), mContent( MsgId::kMsgControlMsg, std::make_tuple( values ) ), mNeedsAction{ true } 
{}

MsgControlMsg::MsgControlMsg( MsgId id ) 
: SerialMessage( id ), mContent( MsgId::kMsgControlMsg ), mNeedsAction{ false }
{ 
    if ( id != MsgId::kMsgControlMsg ) 
    { 
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, std::to_underlying( MsgId::kMsgControlMsg ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void MsgControlMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = false;

    // This shouldn't happen
    output2cout( "Error: RPi0 got MsgControlMsg", getIdNum(), static_cast<int>( std::get<0>( mContent.mMsg ) ) );
}

void MsgControlMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );

    debug2cout( "Rpi0 sent MsgControlMsg", static_cast<int>( std::get<0>( mContent.mMsg ) ) );
}

void MsgControlMsg::takeAction( EventManager& events, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        sendOut( link );
        mNeedsAction = false;

        std::uint8_t values{ std::get<0>( mContent.mMsg ) };
        output2cout( "MsgControlMsg sent with values" );  
        output2cout( " sendTimerMsgs", static_cast<bool>( values & kTimerMsgMask ) );
        output2cout( " sendNavMsgs", static_cast<bool>( values & kNavMsgMask ) );
        output2cout( " sendNavStatusMsgs", static_cast<bool>( values & kNavStatusMask ) );
        output2cout( " sendEncoderMsgs", static_cast<bool>( values & kEncoderMsgMask ) );
        output2cout( " sendCalibrationMsgs", static_cast<bool>( values & kCalibrationMsgMask ) );
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

    output2cout( "Error: RPi0 got ResetMsg", getIdNum() );
}


void ResetMsg::takeAction( EventManager& events, SerialLink& link )
{
    if ( mNeedsAction )
    {
        sendOut( link );
        mNeedsAction = false;

        output2cout( "RPi0 sent ResetMsg", getIdNum() );
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
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, std::to_underlying( MsgId::kTimerEventMsg ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void TimerEventMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    debug2cout( "RPi0 got TimerEventMsg", getIdNum(), 
        static_cast<int>( std::get<0>( mContent.mMsg ) ), std::get<1>( mContent.mMsg ), std::get<2>( mContent.mMsg ) );
}

void TimerEventMsg::sendOut( SerialLink& link )
{
    // RPi0 should never send this msg

    output2cout( "Error: RPi0 sending TimerEventMsg", getIdNum(), 
        static_cast<int>( std::get<0>( mContent.mMsg ) ), std::get<1>( mContent.mMsg ), std::get<2>( mContent.mMsg ) );
}

void TimerEventMsg::takeAction( EventManager&, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // Process the timer event
        // TODO process timer event
        mNeedsAction = false;

        output2cout( "TODO: RPi0 action on TimerEventMsg" );
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
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, std::to_underlying( MsgId::kTimerControl ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void TimerControlMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = false;

    output2cout( "Error: Rpi0 got TimerControlMsg", getIdNum(), static_cast<bool>( std::get<0>( mContent.mMsg ) ) );
}

void TimerControlMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );

    debug2cout( "RPi0 sent TimerControlMsg", getIdNum(), static_cast<bool>( std::get<0>( mContent.mMsg ) ) );
}

void TimerControlMsg::takeAction( EventManager& events, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        sendOut( link );
        mNeedsAction = false;

        output2cout( "RPi0 sent TimerControlMsg, turned timer events", ( static_cast<bool>( std::get<0>( mContent.mMsg ) ) ? "off" : "on" ) );    
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
    mNeedsAction = false;

    output2cout( "Error: RPi0 got BeginCalibrationMsg", getIdNum() );
}


void BeginCalibrationMsg::takeAction( EventManager& events, SerialLink& link )
{
    if ( mNeedsAction )
    {
        sendOut( link );
        mNeedsAction = false;

        output2cout( "RPi0 set BeginCalibrationMsg", getIdNum() );
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
    mNeedsAction = false;

    output2cout( "Error: RPi0 got RequestCalibrationStatusMsg", getIdNum() );
}


void RequestCalibrationStatusMsg::takeAction( EventManager& events, SerialLink& link )
{
    if ( mNeedsAction )
    {
        // Just send it
        sendOut( link );
        mNeedsAction = false;

        output2cout( "RPi0 sent RequestCalibrationStatusMsg", getIdNum() );
    }
}




/*********************************************************************************************/




CalibrationInfoUpdateMsg::CalibrationInfoUpdateMsg() noexcept
: SerialMessage( MsgId::kCalibrationInfoUpdate ), mContent( MsgId::kCalibrationInfoUpdate ), mNeedsAction{ false }
{}

CalibrationInfoUpdateMsg::CalibrationInfoUpdateMsg( TheData t ) noexcept
: SerialMessage( MsgId::kCalibrationInfoUpdate ), mContent( MsgId::kCalibrationInfoUpdate, t ), mNeedsAction{ true }
{}


CalibrationInfoUpdateMsg::CalibrationInfoUpdateMsg( std::uint8_t mag, std::uint8_t accel, std::uint8_t gyro, std::uint8_t sys ) noexcept
: SerialMessage( MsgId::kCalibrationInfoUpdate ), mContent( MsgId::kCalibrationInfoUpdate, std::make_tuple( mag, accel, gyro, sys) ), mNeedsAction{ true }
{}


CalibrationInfoUpdateMsg::CalibrationInfoUpdateMsg( MsgId id )
: SerialMessage( id ), mContent( MsgId::kCalibrationInfoUpdate ), mNeedsAction{ false }
{
    if ( id != MsgId::kCalibrationInfoUpdate ) 
    { 
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1,std::to_underlying( MsgId::kCalibrationInfoUpdate ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void CalibrationInfoUpdateMsg::readIn( SerialLink& link )
{
    mContent.readIn( link );
    mNeedsAction = true;

    debug2cout( "RPi0 got CalibrationInfoUpdateMsg", getIdNum(), 
        static_cast<int>( std::get<0>( mContent.mMsg ) ), static_cast<int>( std::get<1>( mContent.mMsg ) ), 
        static_cast<int>( std::get<2>( mContent.mMsg ) ), static_cast<int>( std::get<3>( mContent.mMsg ) ) );
}


void CalibrationInfoUpdateMsg::sendOut( SerialLink& link )
{
    // RPi0 should never send this
    
    output2cout( "Error: RPi0 tried to send CalibrationInfoUpdateMsg", getIdNum(), 
        static_cast<int>( std::get<0>( mContent.mMsg ) ), static_cast<int>( std::get<1>( mContent.mMsg ) ), 
        static_cast<int>( std::get<2>( mContent.mMsg ) ), static_cast<int>( std::get<3>( mContent.mMsg ) ) );
}


void CalibrationInfoUpdateMsg::takeAction( EventManager& events, SerialLink& link )
{
    if ( mNeedsAction )
    {
        // TODO process the data in the message
        mNeedsAction = false;

        output2cout ( "TODO: RPi0 process CalibrationInfoMsg", getIdNum(), 
        static_cast<int>( std::get<0>( mContent.mMsg ) ), static_cast<int>( std::get<1>( mContent.mMsg ) ), 
        static_cast<int>( std::get<2>( mContent.mMsg ) ), static_cast<int>( std::get<3>( mContent.mMsg ) ) ); 
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
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, std::to_underlying( MsgId::kSetAutoCalibrate ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void SetAutoCalibrateMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    output2cout( "Error: RPi0 got SetAutoCalibrateMsg", getIdNum(), static_cast<bool>( std::get<0>( mContent.mMsg) ) );
}

void SetAutoCalibrateMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}

void SetAutoCalibrateMsg::takeAction( EventManager& events, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // Send it
        sendOut( link );
        mNeedsAction = false;

        output2cout( "Sent Pico SetAutoCalibrateMsg set to", static_cast<bool>( std::get<0>( mContent.mMsg ) ) );    
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
    // RPi0 should never get this

    output2cout( "Error: RPi0 got ResetBNO055Msg", getIdNum() );
}


void ResetBNO055Msg::takeAction( EventManager& events, SerialLink& link )
{
    if ( mNeedsAction )
    {
        sendOut( link );
        mNeedsAction = false;

        output2cout( "Sent ResetBNO055Msg to Pico" );
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
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, std::to_underlying( MsgId::kTimerNavUpdate ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void NavUpdateMsg::readIn( SerialLink& link )
{
    mContent.readIn( link );
    mNeedsAction = true;

    debug2cout( "Got NavUpdateMsg", getIdNum(), std::get<0>( mContent.mMsg ), std::get<1>( mContent.mMsg ) );
}


void NavUpdateMsg::sendOut( SerialLink& link )
{
    // RPi0 never sends this

    output2cout( "Error: RPi0 sending NavUpdateMsg", getIdNum(), std::get<0>( mContent.mMsg ), std::get<1>( mContent.mMsg ) );
}



void NavUpdateMsg::takeAction( EventManager& events, SerialLink& link )
{
    if ( mNeedsAction )
    {
        // TODO  do something with the fino
        sendOut( link );
        mNeedsAction = false;

        output2cout( "Got NavUpdateMsg", getIdNum(), std::get<0>( mContent.mMsg ), std::get<1>( mContent.mMsg ) );
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
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, std::to_underlying( MsgId::kNavUpdateControl ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void NavUpdateControlMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = false;

    debug2cout( "Error: RPi0 got NavUpdateControlMsg", getIdNum(), static_cast<bool>( std::get<0>( mContent.mMsg) ) );
}

void NavUpdateControlMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}

void NavUpdateControlMsg::takeAction( EventManager& events, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // Send it
        sendOut( link );
        mNeedsAction = false;

        output2cout( "NavUpdateControlMsg send to Pico, set to", static_cast<bool>( std::get<0>( mContent.mMsg ) ) );    
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
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, std::to_underlying( MsgId::kDrivingStatusUpdate ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void DrivingStatusUpdateMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = false;

    output2cout( "Error: RPi0 got DrivingStatusUpdateMsg", getIdNum(), static_cast<int>( std::get<0>( mContent.mMsg ) ) );
}

void DrivingStatusUpdateMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
    
    debug2cout( "RPi0 sent DrivingStatusUpdateMsg", getIdNum(), static_cast<int>( std::get<0>( mContent.mMsg ) ) );
}

void DrivingStatusUpdateMsg::takeAction( EventManager& events, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        sendOut( link );
        mNeedsAction = false;

        output2cout( "RPi0 sent DrivingStatusUpdateMsg", static_cast<int>( std::get<0>( mContent.mMsg ) ) ); 
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
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, std::to_underlying( MsgId::kEncoderUpdate ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void EncoderUpdateMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    debug2cout( "RPi0 got EncoderUpdateMsg", std::get<0>( mContent.mMsg ), std::get<1>( mContent.mMsg ), std::get<2>( mContent.mMsg ) );
}

void EncoderUpdateMsg::sendOut( SerialLink& link )
{
    // RPi0 never sends this

    output2cout( "Error: RPi0 sends EncoderUpdateMsg", std::get<0>( mContent.mMsg ), std::get<1>( mContent.mMsg ), std::get<2>( mContent.mMsg ) );
}

void EncoderUpdateMsg::takeAction( EventManager&, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // TODO take action with thi
        // TODO
        mNeedsAction = false;

        output2cout( "TODO process EncoderUpdateMsg", std::get<0>( mContent.mMsg ), std::get<1>( mContent.mMsg ), std::get<2>( mContent.mMsg ) );
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
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, std::to_underlying( MsgId::kEncoderUpdateControl ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void EncoderUpdateControlMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = false;

    output2cout( "Error: RPi0 got EncoderUpdateControlMsg", getIdNum(), static_cast<bool>( std::get<0>( mContent.mMsg ) ) );
}

void EncoderUpdateControlMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}

void EncoderUpdateControlMsg::takeAction( EventManager& events, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // Simply send
        sendOut( link );
        mNeedsAction = false;

        output2cout( "RPi0 sent EncoderUpdateControlMsg with", static_cast<bool>( std::get<0>( mContent.mMsg ) ) );    
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
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, std::to_underlying( MsgId::kBatteryLevelRequest ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void BatteryLevelRequestMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = false;

    output2cout( "Error: RPi0 got BatteryLevelRequestMsg", static_cast<int>( std::get<0>( mContent.mMsg ) ) );
}

void BatteryLevelRequestMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );
}

void BatteryLevelRequestMsg::takeAction( EventManager& events, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // Send it
        sendOut( link );
        mNeedsAction = false;

        output2cout( "RPi0 sent BatteryLevelRequestMsg", static_cast<int>( std::get<0>( mContent.mMsg ) ) );
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
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, std::to_underlying( MsgId::kBatteryLevelUpdate ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void BatteryLevelUpdateMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    debug2cout( "RPi0 got BatteryLevelUpdateMsg", getIdNum(), static_cast<int>( std::get<0>( mContent.mMsg ) ), std::get<1>( mContent.mMsg ) );
}

void BatteryLevelUpdateMsg::sendOut( SerialLink& link )
{
    // RPi0 never sends this

    output2cout( "Error: RPi0 sending BatteryLevelUpdateMsg", getIdNum(), static_cast<int>( std::get<0>( mContent.mMsg ) ), std::get<1>( mContent.mMsg ) );
}

void BatteryLevelUpdateMsg::takeAction( EventManager&, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // TODO act on this
        mNeedsAction = false;

        output2cout( "TODO: RPi0 act on BatteryLevelUpdateMsg" );
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
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, std::to_underlying( MsgId::kBatteryLowAlert ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void BatteryLowAlertMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    debug2cout( "RPi0 got BatteryLowAlertMsg", getIdNum(), static_cast<int>( std::get<0>( mContent.mMsg ) ), std::get<1>( mContent.mMsg ) );
}

void BatteryLowAlertMsg::sendOut( SerialLink& link )
{
    // RPi0 never sends this

    output2cout( "Error: RPi0 sending BatteryLowAlertMsg", getIdNum(), static_cast<int>( std::get<0>( mContent.mMsg ) ), std::get<1>( mContent.mMsg ) );
}

void BatteryLowAlertMsg::takeAction( EventManager&, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // TODO process this
        mNeedsAction = false;

        output2cout( "TODO: RPi0 act on BatteryLowAlertMsg" );
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
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, std::to_underlying( MsgId::kErrorReportFromPico ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void ErrorReportMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    output2cout( "RPi0 got ErrorReportMsg", getIdNum(), static_cast<bool>( std::get<0>( mContent.mMsg ) ), std::get<1>( mContent.mMsg ) );
}

void ErrorReportMsg::sendOut( SerialLink& link )
{
    // RPi0 never sends this

    output2cout( "Error: RPi0 sending ErrorReportMsg", getIdNum(), static_cast<bool>( std::get<0>( mContent.mMsg ) ), std::get<1>( mContent.mMsg ) );
}

void ErrorReportMsg::takeAction( EventManager&, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // TODO handle the error
        mNeedsAction = false;

        output2cout( "TODO: RPi0 act on ErrorReportMsg", 
            ( static_cast<bool>( std::get<0>( mContent.mMsg ) ) ? "Fatal" : "Not Fatal" ), std::get<1>( mContent.mMsg ) );
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
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, std::to_underlying( MsgId::kTestPicoReportError ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void TestPicoErrorRptMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = false;

    output2cout( "Error: RPi0 got TestPicoErrorRptMsg", getIdNum(), static_cast<bool>( std::get<0>( mContent.mMsg ) ), std::get<1>( mContent.mMsg ) );
}

void TestPicoErrorRptMsg::sendOut( SerialLink& link )
{
    sendOut( link );

    debug2cout( "RPiO sent TestPicoErrorRptMsg requesting",  static_cast<bool>( std::get<0>( mContent.mMsg ) ), std::get<1>( mContent.mMsg ) );
}

void TestPicoErrorRptMsg::takeAction( EventManager&, SerialLink& link ) 
{
    // Sent it
    if ( mNeedsAction )
    {
        sendOut( link );
        mNeedsAction = false;

        output2cout( "RPi0 SentTestPicoErrorRptMsg requesting", 
            ( static_cast<bool>( std::get<0>( mContent.mMsg ) ) ? "Fatal" : "Not Fatal" ), "error code", std::get<1>( mContent.mMsg ) );
    }
}




/*********************************************************************************************/




DebugLinkMsg::DebugLinkMsg() noexcept 
: SerialMessage( MsgId::kDebugSerialLink ), mContent( MsgId::kDebugSerialLink ), mNeedsAction{ false } 
{}

DebugLinkMsg::DebugLinkMsg( TheData t ) noexcept 
: SerialMessage( MsgId::kDebugSerialLink ), mContent( MsgId::kDebugSerialLink, t ), mNeedsAction{ false } 
{} 

DebugLinkMsg::DebugLinkMsg(  int val1_i, std::uint8_t val2_u8, float val3_f, std::uint32_t val4_u32  ) noexcept 
: SerialMessage( MsgId::kDebugSerialLink ), 
                    mContent( MsgId::kDebugSerialLink, std::make_tuple( val1_i, val2_u8, val3_f, val4_u32 ) ), 
                    mNeedsAction{ false } 
{}

DebugLinkMsg::DebugLinkMsg( MsgId id ) 
: SerialMessage( id ), mContent( MsgId::kDebugSerialLink ), mNeedsAction{ false }          
{ 
    if ( id != MsgId::kDebugSerialLink ) 
    { 
        throw CarrtError( makeRpi0ErrorId( kRPi0SerialMessageError, 1, std::to_underlying( MsgId::kDebugSerialLink ) ), "Id mismatch at creation" ); 
    } 
    // Note that it doesn't need action until loaded with data
}


void DebugLinkMsg::readIn( SerialLink& link ) 
{
    mContent.readIn( link );
    mNeedsAction = true;

    output2cout( "RPi0 got DebugLinkMsg", std::get<0>( mContent.mMsg ), static_cast<int>( std::get<1>( mContent.mMsg ) ), 
                    std::get<2>( mContent.mMsg ), std::get<3>( mContent.mMsg ) );
}

void DebugLinkMsg::sendOut( SerialLink& link )
{
    mContent.sendOut( link );

    output2cout( "RPi0 sent DebugLinkMsg", std::get<0>( mContent.mMsg ), static_cast<int>( std::get<1>( mContent.mMsg ) ), 
                    std::get<2>( mContent.mMsg ), std::get<3>( mContent.mMsg ) );
}

void DebugLinkMsg::takeAction( EventManager&, SerialLink& link ) 
{
    if ( mNeedsAction )
    {
        // DebugLinkMsgs work by having RPi0 send and Pico respond
        // This is the one message that RPi0 always sends with "sendOut()", **NEVER** with takeAction()
        // takeAction() is reserved to receiving.
        // So here always assume we are receiving
        // Just display the values, which is already done by readIn() 

        mNeedsAction = false;
        // TODO in future perhaps confirm to user that reply matched expectation?
        output2cout( "TODO: RPi0 act on DebugLinkMsg" );
    }
}




/*********************************************************************************************/

