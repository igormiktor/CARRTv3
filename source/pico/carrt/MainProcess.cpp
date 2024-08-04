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






#include "CarrtPicoDefines.h"
#include "CarrtPicoReset.h"
#include "Core1.h"
#include "DebugMacros.h"
#include "EventManager.h"
#include "HeartBeatLed.h"

#include "shared/SerialMessage.h"
#include "shared/SerialLink.h"
#include "shared/CarrtError.h"

#include <iostream>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/uart.h"




namespace MainProcess
{
    void runMainEventLoop();
    void checkForErrors();
    void processEvent();
    void processMessage();
    void dispatchEvent( int eventCode, int eventParam, uint32_t eventTime );
    void dispatchMessage( uint8_t cmd );
    void dispatchExtendedMsg( uint8_t cmd );

    void doNavUpdateEvent( int eventParam, uint32_t eventTime );
    void doQuarterSecondTimerEvent( int eventParam, uint32_t eventTime );
    void doOneSecondTimerEvent( int eventParam, uint32_t eventTime );
    void doEightSecondTimerEvent( int eventParam, uint32_t eventTime );
    void doIdentifyPicoCoreEvent( int eventParam, uint32_t eventTime );
    void doEventQueueOverflowed();
    void doUnknownEvent( int eventCode );

    void doNullMsg();
    void doStartCore1Msg();
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




void MainProcess::runMainEventLoop()
{
    while ( 1 )
    {
        checkForErrors();
        processEvent();
        processMessage();
    }
}


void MainProcess::checkForErrors()
{
    // Notionally a place to check for memory exhaustion, etc..
}



void MainProcess::processEvent()
{
    int eventCode;
    int eventParam;
    uint32_t eventTime;

    if ( Events().getNextEvent( &eventCode, &eventParam, &eventTime ) )
    {
        dispatchEvent( eventCode, eventParam, eventTime );
    }

    if ( Events().hasEventQueueOverflowed() )
    {
        doEventQueueOverflowed();
        Events().resetEventQueueOverflowFlag();
    }
}



void MainProcess::processMessage()
{
    if ( SerialLink::isReadable() )
    {
        uint8_t cmd = SerialLink::getMsg();
        dispatchMessage( cmd );
    }
}



void MainProcess::dispatchEvent( int eventCode, int eventParam, uint32_t eventTime )
{
    switch ( eventCode )
    {
        case kNavUpdateEvent:
            doNavUpdateEvent( eventParam, eventTime );
            break;
            
        case kQuarterSecondTimerEvent:
            doQuarterSecondTimerEvent( eventParam, eventTime );
            break;
            
        case kOneSecondTimerEvent:
            doOneSecondTimerEvent( eventParam, eventTime );
            break;
            
        case kEightSecondTimerEvent:
            doEightSecondTimerEvent( eventParam, eventTime );
            break;

        case kIdentifyPicoCoreEvent:
            doIdentifyPicoCoreEvent( eventParam, eventTime );
            break;

        default:
            doUnknownEvent( eventCode );
    }
}



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



void MainProcess::doNavUpdateEvent( int eventParam, uint32_t eventTime )
{
    SerialLink::putMsg( kTimerNavUpdate );
    SerialLink::put( eventTime );
    SerialLink::put( eventParam );
}



void MainProcess::doQuarterSecondTimerEvent( int eventParam, uint32_t eventTime )
{
    SerialLink::putMsg( kTimer1_4s );
    SerialLink::put( eventTime );
    SerialLink::put( eventParam );               
}



void MainProcess::doOneSecondTimerEvent( int eventParam, uint32_t eventTime )
{
    SerialLink::putMsg( kTimer1s );
    SerialLink::put( eventTime );
    SerialLink::put( eventParam );

    HeartBeatLed::toggle();
}



void MainProcess::doEightSecondTimerEvent( int eventParam, uint32_t eventTime )
{
    // std::cout << "8s " << eventParam << std::endl;
    SerialLink::putMsg( kTimer8s );
    SerialLink::put( eventTime );
    SerialLink::put( eventParam );
}



void MainProcess::doIdentifyPicoCoreEvent( int eventParam, uint32_t eventTime )
{
    // std::cout << "Core " << eventParam << std::endl;
    SerialLink::putMsg( kIdentifyPicoCore );
    SerialLink::put( eventParam );
}



void MainProcess::doUnknownEvent( int eventCode )
{
    INFO_PICO_MSG2( "Warning: Pico received unknown event: ",  eventCode );
    
    SerialLink::putMsg( kErrorReportFromPico );
    SerialLink::putByte( kPicoNonFatalError );
    int errCode = makePicoErrorId( kPicoMainProcessError, 1, 0 );
    SerialLink::put( errCode );      
}



void MainProcess::doEventQueueOverflowed()
{
    INFO_PICO_MSG1( "Event queue overflowed" );
      
    SerialLink::putMsg( kErrorReportFromPico );
    SerialLink::putByte( kPicoNonFatalError );
    int errCode = makePicoErrorId( kPicoMainProcessError, 2, 0 );
    SerialLink::put( errCode );
}



void MainProcess::doNullMsg()
{
    DEBUG_PICO_MSG1( "Received Null cmd from RPi0" );

    // Simply acknowledge
    SerialLink::putMsg( kNullMsg );
}



void MainProcess::doStartCore1Msg()
{
    DEBUG_PICO_MSG1( "Received Start Core1 cmd from RPi0" );

    launchCore1();
    
    // Simply acknowledge
    SerialLink::putMsg( kStartCore1 );
}



void MainProcess::doPauseMsg()
{
    DEBUG_PICO_MSG1( "Received Pause cmd from RPi0" );

    // Ack receipt
    SerialLink::putMsg( kPauseMsg );

    // TODO  Try to pause (throw exception if fails)
}



void MainProcess::doBeginCalibration()
{
    DEBUG_PICO_MSG1( "Received begin calibration msg from RPi0" );

    // TODO  Start the calibration process

    // Acknowledge by same message in reply, followed by error code
    unsigned char err = 0;
    SerialLink::putMsg( kBeginCalibration );
    SerialLink::putByte( err );
}



void MainProcess::doResumeMsg()
{
    DEBUG_PICO_MSG1( "Received Resume cmd from RPi0" );

    // Ack receipt
    SerialLink::putMsg( kResumeMsg );

    // TODO  ry to resume; throw if fails
}



void MainProcess::doResetMsg()
{
    DEBUG_PICO_MSG1( "Received Reset cmd from RPi0" );

    // Acknowledge the reset cmd
    SerialLink::putMsg( kResetMsg );

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
    DEBUG_PICO_MSG1( "Received request calibration status" );

// TODO  Get calibration status and report it
}



void MainProcess::doProcessCalibrationProfile()
{
    DEBUG_PICO_MSG1( "Received a calibration profile from RPi0" );

    // TODO  Extract calibration profile and sent it to BNO055


    // TODO     print calibration profile
}



void MainProcess::doReplyWithCalibrationProfile()
{
    DEBUG_PICO_MSG1( "Received request for the current calibration profile" );

    // TODO  Get calibration profile from BNO055 and relay it to RPi0

    // TODO     print calibration profile
}






void MainProcess::doDrivingStatusUpdate()
{
    DEBUG_PICO_MSG1( "Received Drive status update from RPi0" );

    // TODO  do something with it
}



void MainProcess::doRequestBatteryLevel()
{
    DEBUG_PICO_MSG1( "Received request for battery V from RPi0" );

    // TODO  do something with it
}



void MainProcess::doRequestMotorBatteryLevel()
{
    DEBUG_PICO_MSG1( "Received request for motor battery V from RPi0" );

    // TODO  do something with it
}



void MainProcess::doIdentifyPicoCore()
{
    DEBUG_PICO_MSG1( "Received request to ID Core from RPi0" );

    SerialLink::putMsg( kIdentifyPicoCore );
    uint32_t core = get_core_num();
    SerialLink::put( core );

    DEBUG_PICO_MSG2( "Pico Core ID is: ", core );
}



void MainProcess::doTestPicoReportError()
{
    DEBUG_PICO_MSG1( "Received test Pico error report cmd from RPi0" );
            
    throw CarrtError( makePicoErrorId( PicoError::kPicoTestError, 1, 0 ), "CARRT Pico test error sent by request" );
}



void MainProcess::doExtendedMsg()
{
    DEBUG_PICO_MSG1( "Received extended cmd from RPi0" );

    uint8_t cmd = SerialLink::getMsg();
    dispatchExtendedMsg( cmd );
}



void MainProcess::doUnknownMessage( uint8_t cmd )
{
    INFO_PICO_MSG2( "Warning: Received unknown cmd from RPi0: ", cmd );
    
    SerialLink::putMsg( kErrorReportFromPico );
    SerialLink::putByte( kPicoNonFatalError );
    int errCode = makePicoErrorId( kPicoMainProcessError, 3, 0 );
    SerialLink::put( errCode );
}




void MainProcess::dispatchExtendedMsg( uint8_t cmd )
{
    switch ( cmd )
    {
        case kNullExtMsg:
            break;

        // Debugging extended cmds
        case kTestExtendedMsg:
            break;

        case kTriggerError:
            break;

        default:
            doUnknownExtendedMsg( cmd );
            break;
    }
}



void MainProcess::doUnknownExtendedMsg( uint8_t cmd )
{
    DEBUG_PICO_MSG2( "Warning: Received unknown extended cmd from RPi0: ", cmd );
    
    SerialLink::putMsg( kErrorReportFromPico );
    SerialLink::putByte( kPicoNonFatalError );
    int errCode = makePicoErrorId( kPicoMainProcessError, 4, 0 );
    SerialLink::put( errCode );
}
