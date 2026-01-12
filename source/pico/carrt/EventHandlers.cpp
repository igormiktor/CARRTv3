/*
    EventHandlers.cpp - Actual handlers for Pico events implementation.

    Copyright (c) 2026 Igor Mikolic-Torreira.  All right reserved.

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
#include "CarrtError.h"
#include "CarrtPicoReset.h"
#include "Clock.h"
#include "Core1.h"
#include "EventManager.h"
#include "HeartBeatLed.h"
#include "OutputUtils.hpp"
#include "PicoState.h"
#include "SerialLink.h"
#include "SerialMessages.h"

void NullEventHandler::handleEvent( EventManager& events, SerialLink& link,
                                    EvtId eventCode, int eventParam,
                                    std::uint32_t eventTime ) const
{
    output2cout( "Got a Null event" );
}

// ********************** Timer event handlers

void QuarterSecondTimerHandler::handleEvent( EventManager& events,
                                             SerialLink& link, EvtId eventCode,
                                             int eventParam,
                                             std::uint32_t eventTime ) const
{
    if ( PicoState::wantQtrSecTimerMsgs() )
    {
        TimerEventMsg timerEvt( TimerEventMsg::k1QuarterSecondEvent, eventParam,
                                eventTime );
        timerEvt.sendOut( link );
    }
}

void OneSecondTimerHandler::handleEvent( EventManager& events, SerialLink& link,
                                         EvtId eventCode, int eventParam,
                                         std::uint32_t eventTime ) const
{
    if ( PicoState::want1SecTimerMsgs() )
    {
        TimerEventMsg timerEvt( TimerEventMsg::k1SecondEvent, eventParam,
                                eventTime );
        timerEvt.sendOut( link );
    }
}

void EightSecondTimerHandler::handleEvent( EventManager& events,
                                           SerialLink& link, EvtId eventCode,
                                           int eventParam,
                                           std::uint32_t eventTime ) const
{
    if ( PicoState::want8SecTimerMsgs() )
    {
        TimerEventMsg timerEvt( TimerEventMsg::k8SecondEvent, eventParam,
                                eventTime );
        timerEvt.sendOut( link );
    }
}

// ********************** BNO055/navigation event handlers

void NavUpdateHandler::handleEvent( EventManager& events, SerialLink& link,
                                    EvtId eventCode, int eventParam,
                                    std::uint32_t eventTime ) const
{
    if ( PicoState::navCalibrated() && PicoState::wantNavMsgs() )
    {
        float heading = BNO055::getHeading();
        NavUpdateMsg navUpdate( heading, eventTime );
        navUpdate.sendOut( link );
        output2cout( "Sent Hdg: ", heading );
    }
}

void InitializeBNO055Handler::handleEvent( EventManager& events,
                                           SerialLink& link, EvtId eventCode,
                                           int eventParam,
                                           std::uint32_t eventTime ) const
{
    output2cout( "Got BNO055 initialize event" );

    // Note this call includes internal delay of ~600ms
    BNO055::init();

    // Because delay built into BNO055init() at driver level,
    // we can trigger BeginCalibration without delay
    events.queueEvent( EvtId::kBNO055BeginCalibrationEvent );

    // And we are done with start up (also done after BNO055 reset)
    // once BNO055 is initialized
    PicoState::startUpFinished( true );
}

void BNO055ResetHandler::handleEvent( EventManager& events, SerialLink& link,
                                      EvtId eventCode, int eventParam,
                                      std::uint32_t eventTime ) const
{
    output2cout( "Got BNO055 reset event" );
    // Note this call is followed by 650ms wait before we can call init()
    BNO055::reset();
    PicoState::navCalibrated( false );
    Core1::queueEventForCore1( EvtId::kBNO055InitializeEvent,
                               BNO055::kWaitAfterPowerOnReset );

    // While BNO055 resetting, we revert to "startup mode"
    PicoState::startUpFinished( false );
}

void BeginCalibrationHandler::handleEvent( EventManager& events,
                                           SerialLink& link, EvtId eventCode,
                                           int eventParam,
                                           std::uint32_t eventTime ) const
{
    output2cout( "Got begin calibration event" );
    PicoState::navCalibrated( false );
    PicoState::calibrationInProgress( true );
}

void SendCalibrationInfoHandler::handleEvent( EventManager& events,
                                              SerialLink& link, EvtId eventCode,
                                              int eventParam,
                                              std::uint32_t eventTime ) const
{
    auto calibData{ BNO055::getCalibration() };
    bool status = BNO055::calibrationGood( calibData );

    // Set Pico state accordingly
    bool oldStatus = PicoState::navCalibrated( status );

    if ( status != oldStatus )
    {
        // Send message to RPi0 that Nav Status changed
        if ( PicoState::wantNavStatusMsgs() )
        {
            PicoNavStatusUpdateMsg navReadyStatus(
                status, calibData.mag, calibData.accel, calibData.gyro,
                calibData.system );
            navReadyStatus.sendOut( link );
        }

        if ( status )
        {
            output2cout( "Changed from not calibrated to CALIBRATED" );
        }
        else
        {
            output2cout( "Changed from calibrated to NOT CALIBRATED" );
        }
    }
    else
    {
        if ( PicoState::wantCalibrationMsgs() )
        {
            // If calibration status unchanged, just send normal calibration
            // report
            CalibrationInfoUpdateMsg calibStatus(
                calibData.mag, calibData.accel, calibData.gyro,
                calibData.system );
            calibStatus.sendOut( link );
        }
    }

    //    output2cout( "Calib status (M, A, G, S): ", static_cast<int>(
    //    calibData.mag ), static_cast<int>( calibData.accel ),
    //        static_cast<int>( calibData.gyro ), static_cast<int>(
    //        calibData.system ) );
}

// ********************** Encoder event handlers

// Go here

// ********************** Pulse LED event handlers

void PulsePicoLedHandler::handleEvent( EventManager& events, SerialLink& link,
                                       EvtId eventCode, int eventParam,
                                       std::uint32_t eventTime ) const
{
    HeartBeatLed::toggle();
}

// ********************** Battery event handlers

// Go here

// ********************** Pico Reset event handlers

void PicoResetHandler::handleEvent( EventManager& events, SerialLink& link,
                                    EvtId eventCode, int eventParam,
                                    std::uint32_t eventTime ) const
{
    PicoReset::reset( link );
}

// ********************** Error event handlers

void ErrorEventHandler::handleEvent( EventManager& events, SerialLink& link,
                                     EvtId eventCode, int eventParam,
                                     std::uint32_t eventTime ) const
{
    output2cout( "Got an error event in the event queue", eventParam );

    int errCode{ makePicoErrorId( kPicoGotErrorEvent, 1, eventParam ) };
    ErrorReportMsg errRpt( kPicoNonFatalError, errCode, Clock::millis() );
    errRpt.sendOut( link );
}
