/*
    EventHandlers.cpp - Actual handlers for Pico events implementation.

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



#include "EventHandlers.h"

#include "BNO055.h"
#include "CarrtPicoReset.h"
#include "Core1.h"
#include "CarrtError.h"
#include "EventManager.h"
#include "HeartBeatLed.h"
#include "PicoOutputUtils.hpp"
#include "PicoState.h"
#include "SerialCommands.h"
#include "SerialLink.h"



void NullEventHandler::handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const
{
    output2cout( "Got a Null event" );
}


// ********************** Timer event handlers


void QuarterSecondTimerHandler::handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const
{
    if ( PicoState::wantTimerEvents() )
    {
        TimerEventCmd timerEvt( TimerEventCmd::k1QuarterSecondEvent, eventParam, eventTime );
        timerEvt.sendOut( link ); 
    }
}


void OneSecondTimerHandler::handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const
{
    if ( PicoState::wantTimerEvents() )
    {
        TimerEventCmd timerEvt( TimerEventCmd::k1SecondEvent, eventParam, eventTime );
        timerEvt.sendOut( link ); 
    }
}


void EightSecondTimerHandler::handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const
{
    if ( PicoState::wantTimerEvents() )
    {
        TimerEventCmd timerEvt( TimerEventCmd::k8SecondEvent, eventParam, eventTime );
        timerEvt.sendOut( link ); 
    }
}


// ********************** BNO055/navigation event handlers


void NavUpdateHandler::handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const
{
    if ( PicoState::navCalibrated() && PicoState::wantNavEvents() )
    {
        float heading = BNO055::getHeading();
        NavUpdateCmd navUpdate( heading, eventTime );
        navUpdate.sendOut( link );
        output2cout( "Sent Hdg: ", heading );
    }
}


void InitializeBNO055Handler::handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const
{
    // Note this call includes internal delay of ~600ms
    BNO055::init();

    // Because delay built into BNO055init() at driver level, can trigger BeginCalibration without delay
    events.queueEvent( kBNO055BeginCalibrationEvent );
    
    output2cout( "Got BNO055 initialize event" );
}


void BNO055ResetHandler::handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const
{
    // Note this call is followed by 650ms wait before we can call init()
    BNO055::reset();
    PicoState::navCalibrated( false );
    Core1::queueEventForCore1( kBNO055InitializeEvent, BNO055::kWaitAfterPowerOnReset );

    output2cout( "Got BNO055 reset event" );
}


void BeginCalibrationHandler::handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const
{
    output2cout( "Got begin calibration event" );
    PicoState::navCalibrated( false );
    PicoState::calibrationInProgress( true );
}


void SendCalibrationInfoHandler::handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const
{
    auto calibData{ BNO055::getCalibration() };
    bool status = BNO055::calibrationGood( calibData );

    bool oldStatus = PicoState::navCalibrated( status );
    if ( status != oldStatus )
    {
        // Send message to RPi0 that Nav Status changed and set Pico state accordingly
        PicoNavStatusUpdateCmd navReadyStatus( status, calibData.mag, calibData.accel, calibData.gyro, calibData.system );
        navReadyStatus.takeAction( events, link );
        PicoState::calibrationInProgress( !status ); 
        
        if ( status )
        {
            output2cout( "Gone from not calibrated to CALIBRATED" );
        }
        else
        {
            output2cout( "Gone from calibrated to NOT CALIBRATED" );
        }
    }
    else
    {
        // If calibration status unchanged, just send normal calibration report
        SendCalibrationStatusCmd calibStatus( calibData.mag, calibData.accel, calibData.gyro, calibData.system );
        calibStatus.takeAction( events, link );
    }

    output2cout( "Calib status (M, A, G, S): ", static_cast<int>( calibData.mag ), static_cast<int>( calibData.accel ), 
        static_cast<int>( calibData.gyro ), static_cast<int>( calibData.system ) );
}


// ********************** Encoder event handlers


// Go here


// ********************** Pulse LED event handlers


void PulsePicoLedHandler::handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const
{
    HeartBeatLed::toggle();
}


// ********************** Battery event handlers


// Go here


// ********************** Pico Reset event handlers


void PicoResetHandler::handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const
{
    PicoReset::reset( link );
}


// ********************** Error event handlers


void ErrorEventHandler::handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const
{
    output2cout( "Got an error event in the event queue" );

    int errCode{ makePicoErrorId( kPicoGotErrorEvent, 1, eventParam ) };
    ErrorReportCmd errRpt( kPicoNonFatalError, errCode );
    errRpt.sendOut( link );
}



