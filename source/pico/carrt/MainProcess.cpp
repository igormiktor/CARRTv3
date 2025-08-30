/*
    MainProcess.cpp - CARRT-Pico's main function.  Runs the primary event loop
    and dispaches to other code as needed.

    Copyright (c) 2024 Igor Mikolic-Torreira.  All right reserved.

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




#include "MainProcess.h"

#include "CarrtPicoDefines.h"
#include "CarrtPicoReset.h"
#include "Core1.h"
#include "EventManager.h"
#include "HeartBeatLed.h"

#include "SerialCommands.h"
#include "SerialCommandProcessor.h"
#include "SerialLinkPico.h"
#include "CarrtError.h"

#include "PicoOutputUtils.hpp"
#include "PicoState.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/uart.h"




namespace MainProcess
{
    void runMainEventLoop( EventManager& events, SerialCommandProcessor& scp, SerialLinkPico& rpi0 );
    void checkForErrors( EventManager& events, SerialLinkPico& rpi0 );
    void processEvent( EventManager& events, SerialLinkPico& rpi0 );
    void processMessage( EventManager& events, SerialCommandProcessor& scp, SerialLinkPico& rpi0 );
    void dispatchEvent( SerialLinkPico& rpi0, int eventCode, int eventParam, uint32_t eventTime );

    void doNavUpdateEvent( SerialLinkPico& rpi0, int eventParam, uint32_t eventTime );
    void doQuarterSecondTimerEvent( SerialLinkPico& rpi0, int eventParam, uint32_t eventTime );
    void doOneSecondTimerEvent( SerialLinkPico& rpi0, int eventParam, uint32_t eventTime );
    void doEightSecondTimerEvent( SerialLinkPico& rpi0, int eventParam, uint32_t eventTime );
    void doEventQueueOverflowed( SerialLinkPico& rpi0 );
    void doUnknownEvent( SerialLinkPico& rpi0, int eventCode );

    void doNullMsg();
    void doBeginCalibration();
    void doPauseMsg();
    void doResumeMsg();
    void doResetMsg();

    void doReplyWithCalibrationStatus();
    void doProcessCalibrationProfile();
    void doReplyWithCalibrationProfile();

    void doDrivingStatusUpdate();
    void doRequestBatteryLevel();
    void doRequestMotorBatteryLevel();
    void doIdentifyPicoCore();
    void doTestPicoReportError();
    void doExtendedMsg();
    void doUnknownMessage( uint8_t cmd );

    void doUnknownExtendedMsg( uint8_t cmd );

};




void MainProcess::runMainEventLoop( EventManager& events, SerialCommandProcessor& scp, SerialLinkPico& rpi0 )
{
    while ( 1 )
    {
        checkForErrors( events, rpi0 );
        processEvent( events, rpi0 );
        processMessage( events, scp, rpi0 );
    }
}


void MainProcess::checkForErrors( EventManager& events, SerialLinkPico& rpi0 )
{
    // Notionally a place to check for memory exhaustion, etc..
}



void MainProcess::processEvent( EventManager& events, SerialLinkPico& rpi0 )
{
    int eventCode;
    int eventParam;
    uint32_t eventTime;

    if ( events.getNextEvent( &eventCode, &eventParam, &eventTime ) )
    {
        dispatchEvent( rpi0, eventCode, eventParam, eventTime );
    }

    if ( events.hasEventQueueOverflowed() )
    {
        doEventQueueOverflowed( rpi0 );
        events.resetEventQueueOverflowFlag();
    }
}



void MainProcess::processMessage( EventManager& events, SerialCommandProcessor& scp, SerialLinkPico& rpi0 )
{
    auto cmd{ scp.receiveCommandIfAvailable() };
    if ( cmd )
    {
        cmd.value()->takeAction( events, rpi0 );
    }
}



void MainProcess::dispatchEvent( SerialLinkPico& rpi0, int eventCode, int eventParam, uint32_t eventTime )
{
    switch ( eventCode )
    {
        case kNavUpdateEvent:
            doNavUpdateEvent( rpi0, eventParam, eventTime );
            break;
            
        case kQuarterSecondTimerEvent:
            doQuarterSecondTimerEvent( rpi0, eventParam, eventTime );
            break;
            
        case kOneSecondTimerEvent:
            doOneSecondTimerEvent( rpi0, eventParam, eventTime );
            break;
            
        case kEightSecondTimerEvent:
            doEightSecondTimerEvent( rpi0, eventParam, eventTime );
            break;


        default:
            doUnknownEvent( rpi0, eventCode );
    }
}


#if 0
void MainProcess::dispatchMessage( uint8_t cmd )
{
    switch ( cmd )
    {
        // Messages (to Pico); acknowledged from Pico with same Msg & second byte (non-zero 2nd byte -> error code)
        case kNullMsg:                      // Null message; simply acknowledge (no extra bytes)
            doNullMsg();
            break;

        case kStartCore1:                // Pico to start Core1  
            doStartCore1Msg();
            break;

        case kBeginCalibration:             // Pico to begin calibration (of BNO055)
            doBeginCalibration();
            break;

        case kPauseMsg:                     // Pico to pause event processing
            doPauseMsg();
            break;

        case kResumeMsg:                    // Pico to resume event processing  
            doResumeMsg();
            break;

        case kResetMsg:                     // Pico to reses itself (ack by sending kPicoReady)
            doResetMsg();
            break;


        case kRequestCalibStatus:           // Request status of BNO055 calibration
            doReplyWithCalibrationStatus();
            break;

        case kSendCalibProfileToPico:       // Sending a calibration profile to Pico (follow by calibration data)
            doProcessCalibrationProfile();
            break;

        case kRequestCalibProfileFmPico:    // Request a calibration profile from Pico (reply followed by calibration data)
            doReplyWithCalibrationProfile();
            break;


        case kDrivingStatusUpdate:          // From RPi0 to Pico (2nd byte provides driving status)
            doDrivingStatusUpdate();
            break;

        case kRequestBatteryLevel:          // Pico to reply same with IC battery V in following bytes
            doRequestBatteryLevel();
            break;

        case kRequestMotorBatteryLevel:     // Pico to reply same with motor battery V in following bytes
            doRequestMotorBatteryLevel();
            break;

        case kIdentifyPicoCore:             // RPi0 sends to Pico asking to identify core (0 or 1) running uart
            doIdentifyPicoCore();
            break;
    
        case kTestPicoReportError:          // RPi0 sends to Pico asking to report an error (bytes 2-5 contain error code to send back)
            doTestPicoReportError();
            break;

        case kExtendedMsg:
            doExtendedMsg();
            break;
    
        default:
            doUnknownMessage( cmd );
            break;
    }
}
#endif // #if 0


void MainProcess::doNavUpdateEvent( SerialLinkPico& rpi0, int eventParam, uint32_t eventTime )
{
    if ( PicoState::access()->wantNavEvents() )
    {

/*
    SerialLink::putMsg( kTimerNavUpdate );
    SerialLink::put( eventTime );
    SerialLink::put( eventParam );
*/
    }
}



void MainProcess::doQuarterSecondTimerEvent( SerialLinkPico& rpi0, int eventParam, uint32_t eventTime )
{
    if ( PicoState::access()->wantTimerEvents() )
    {
        TimerEventCmd timerEvt( TimerEventCmd::k1QuarterSecondEvent, eventParam, eventTime );
        timerEvt.sendOut( rpi0 ); 
    }
}



void MainProcess::doOneSecondTimerEvent( SerialLinkPico& rpi0, int eventParam, uint32_t eventTime )
{
    if ( PicoState::access()->wantTimerEvents() )
    {
        TimerEventCmd timerEvt( TimerEventCmd::k1SecondEvent, eventParam, eventTime );
        timerEvt.sendOut( rpi0 ); 
    }

    HeartBeatLed::toggle();
}



void MainProcess::doEightSecondTimerEvent( SerialLinkPico& rpi0, int eventParam, uint32_t eventTime )
{
    if ( PicoState::access()->wantTimerEvents() )
    {
        TimerEventCmd timerEvt( TimerEventCmd::k8SecondEvent, eventParam, eventTime );
        timerEvt.sendOut( rpi0 ); 
    }
}




void MainProcess::doUnknownEvent( SerialLinkPico& rpi0, int eventCode )
{
    output2cout( "Warning: Pico received unknown event: ",  eventCode );
    
    int errCode{ makePicoErrorId( kPicoMainProcessError, 1, eventCode ) };
    ErrorReportCmd errRpt( kPicoNonFatalError, errCode );
    errRpt.sendOut( rpi0 );
}



void MainProcess::doEventQueueOverflowed( SerialLinkPico& rpi0 )
{
    output2cout( "Event queue overflowed" );

    int errCode{ makePicoErrorId( kPicoMainProcessError, 2, 0 ) };
    ErrorReportCmd errRpt( kPicoNonFatalError, errCode );
    errRpt.sendOut( rpi0 );
}



void MainProcess::doNullMsg()
{
    output2cout( "Received Null cmd from RPi0" );

    // do nothing
}



void MainProcess::doPauseMsg()
{
    output2cout( "Received Pause cmd from RPi0" );

    // TODO  Try to pause (throw exception if fails?)
}



void MainProcess::doBeginCalibration()
{
    output2cout( "Received begin calibration msg from RPi0" );

    // TODO  Start the calibration process
}



void MainProcess::doResumeMsg()
{
    output2cout( "Received Resume cmd from RPi0" );

    // TODO  ry to resume; throw if fails
}



void MainProcess::doResetMsg()
{
    output2cout( "Received Reset cmd from RPi0" );

    // Trigger a reset of the Pico
    PicoReset::reset();

    // Wait for the reset to happen
    while ( 1 )
    {
        sleep_ms( 100 );
    }
}



void MainProcess::doReplyWithCalibrationStatus()
{
    output2cout( "Received request calibration status" );

// TODO  Get calibration status and report it
}



void MainProcess::doProcessCalibrationProfile()
{
    output2cout( "Received a calibration profile from RPi0" );

    // TODO  Extract calibration profile and sent it to BNO055


    // TODO     print calibration profile
}



void MainProcess::doReplyWithCalibrationProfile()
{
    output2cout( "Received request for the current calibration profile" );

    // TODO  Get calibration profile from BNO055 and relay it to RPi0

    // TODO     print calibration profile
}






void MainProcess::doDrivingStatusUpdate()
{
    output2cout( "Received Drive status update from RPi0" );

    // TODO  do something with it
}



void MainProcess::doRequestBatteryLevel()
{
    output2cout( "Received request for battery V from RPi0" );

    // TODO  do something with it
}



void MainProcess::doRequestMotorBatteryLevel()
{
    output2cout( "Received request for motor battery V from RPi0" );

    // TODO  do something with it
}



void MainProcess::doTestPicoReportError()
{
    output2cout( "Received test Pico error report cmd from RPi0" );
            
    throw CarrtError( makePicoErrorId( PicoError::kPicoTestError, 1, 0 ), "CARRT Pico test error sent by request" );
}



