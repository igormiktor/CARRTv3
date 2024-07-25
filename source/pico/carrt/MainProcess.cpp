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

#include "EventManager.h"
#include "HeartBeatLed.h"

#include "shared/SerialCommand.h"
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
    void processCommand();
    void dispatchEvent( int eventCode, int eventParam, uint32_t eventTime );
    void dispatchCommand( uint8_t cmd );
    void dispatchExtendedCmd( uint8_t cmd );

    void doNavUpdateEvent( int eventParam, uint32_t eventTime );
    void doQuarterSecondTimerEvent( int eventParam, uint32_t eventTime );
    void doOneSecondTimerEvent( int eventParam, uint32_t eventTime );
    void doEightSecondTimerEvent( int eventParam, uint32_t eventTime );
    void doIdentifyPicoCoreEvent( int eventParam, uint32_t eventTime );
    void doEventQueueOverflowed();
    void doUnknownEvent( int eventCode );

    void doNullCmd();
    void doStartCore1Cmd();
    void doBeginCalibration();
    void doPauseCmd();
    void doResumeCmd();
    void doResetCmd();

    void doReplyWithCalibrationStatus();
    void doProcessCalibrationProfile();
    void doReplyWithCalibrationProfile();

    void doDrivingStatusUpdate();
    void doRequestBatteryLevel();
    void doRequestMotorBatteryLevel();
    void doIdentifyPicoCore();
    void doTestPicoReportError();
    void doExtendedCmd();
    void doUnknownCommand( uint8_t cmd );

    void doUnknownExtendedCmd( uint8_t cmd );

};




void MainProcess::runMainEventLoop()
{
    while ( 1 )
    {
        checkForErrors();
        processEvent();
        processCommand();
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



void MainProcess::processCommand()
{
    if ( SerialLink::isReadable() )
    {
        uint8_t cmd = SerialLink::getCmd();
        dispatchCommand( cmd );
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



void MainProcess::dispatchCommand( uint8_t cmd )
{
    switch ( cmd )
    {
        // Commands (to Pico); acknowledged from Pico with same Cmd & second byte (non-zero 2nd byte -> error code)
        case kNullCmd:                      // Null command; simply acknowledge (no extra bytes)
            doNullCmd();
            break;

        case kStartCore1Cmd:                // Pico to start Core1  
            doStartCore1Cmd();
            break;

        case kBeginCalibration:             // Pico to begin calibration (of BNO055)
            doBeginCalibration();
            break;

        case kPauseCmd:                     // Pico to pause event processing
            doPauseCmd();
            break;

        case kResumeCmd:                    // Pico to resume event processing  
            doResumeCmd();
            break;

        case kResetCmd:                     // Pico to reses itself (ack by sending kPicoReady)
            doResetCmd();
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

        case kExtendedCmd:
            doExtendedCmd();
            break;
    
        default:
            doUnknownCommand( cmd );
            break;
    }
}



void MainProcess::doNavUpdateEvent( int eventParam, uint32_t eventTime )
{
    SerialLink::putCmd( kTimerNavUpdate );
    SerialLink::put( eventTime );
    SerialLink::put( eventParam );
}



void MainProcess::doQuarterSecondTimerEvent( int eventParam, uint32_t eventTime )
{
    SerialLink::putCmd( kTimer1_4s );
    SerialLink::put( eventTime );
    SerialLink::put( eventParam );               
}



void MainProcess::doOneSecondTimerEvent( int eventParam, uint32_t eventTime )
{
    SerialLink::putCmd( kTimer1s );
    SerialLink::put( eventTime );
    SerialLink::put( eventParam );

    HeartBeatLed::toggle();
}



void MainProcess::doEightSecondTimerEvent( int eventParam, uint32_t eventTime )
{
    // std::cout << "8s " << eventParam << std::endl;
    SerialLink::putCmd( kTimer8s );
    SerialLink::put( eventTime );
    SerialLink::put( eventParam );
}



void MainProcess::doIdentifyPicoCoreEvent( int eventParam, uint32_t eventTime )
{
    // std::cout << "Core " << eventParam << std::endl;
    SerialLink::putCmd( kIdentifyPicoCore );
    SerialLink::put( eventParam );
}



void MainProcess::doUnknownEvent( int eventCode )
{
#if USE_CARRTPICO_STDIO
    std::cout << "Warning: Received unknown event: " << eventCode << std::endl;
#endif
    
    SerialLink::putCmd( kErrorReportFromPico );
    SerialLink::putByte( kPicoNonFatalError );
    int errCode = makePicoErrorId( kPicoMainProcessError, 1, 0 );
    SerialLink::put( errCode );      
}



void MainProcess::doEventQueueOverflowed()
{
#if USE_CARRTPICO_STDIO
    std::cout << "Event queue overflowed" << std::endl;
#endif
     
    SerialLink::putCmd( kErrorReportFromPico );
    SerialLink::putByte( kPicoNonFatalError );
    int errCode = makePicoErrorId( kPicoMainProcessError, 2, 0 );
    SerialLink::put( errCode );
}



void MainProcess::doNullCmd()
{
#if USE_CARRTPICO_STDIO
    std::cout << "Received Null cmd from RPi0" << std::endl;
#endif

    // Simply acknowledge
    SerialLink::putCmd( kNullCmd );
}



void MainProcess::doStartCore1Cmd()
{
#if USE_CARRTPICO_STDIO
    std::cout << "Received Start Core1 cmd from RPi0" << std::endl;
#endif

    int err = launchCore1();
    
    // Simply acknowledge
    SerialLink::putCmd( kStartCore1Cmd );
    SerialLink::putByte( err );

#if USE_CARRTPICO_STDIO
    std::cout << "Start Core1 " << ( err ? "fail" : "success" ) << std::endl;
#endif
}



void MainProcess::doPauseCmd()
{
#if USE_CARRTPICO_STDIO
    std::cout << "Received Pause cmd from RPi0" << std::endl;
#endif

    // Try to pause
    uint8_t pauseFailed = 0;
    SerialLink::putCmd( kPauseCmd );
    SerialLink::putByte( pauseFailed );
}



void MainProcess::doBeginCalibration()
{
#if USE_CARRTPICO_STDIO
    std::cout << "Received Pause cmd from RPi0" << std::endl;
#endif

    // TODO  Start the calibration process

    // Acknowledge by same command in reply, followed by error code
    unsigned char err = 0;
    SerialLink::putCmd( kBeginCalibration );
    SerialLink::putByte( err );
}



void MainProcess::doResumeCmd()
{
#if USE_CARRTPICO_STDIO
    std::cout << "Received Resume cmd from RPi0" << std::endl;
#endif

    // Try to resume
    uint8_t resumeFailed = 0;
    SerialLink::putCmd( kResumeCmd );
    SerialLink::putByte( resumeFailed );
}



void MainProcess::doResetCmd()
{
#if USE_CARRTPICO_STDIO
    std::cout << "Received Reset cmd from RPi0" << std::endl;
#endif

    // Acknowledge the reset cmd
    SerialLink::putCmd( kResetCmd );

    // Trigger a reset of the Pico
    PicoReset::reset();

    // Wait for the reset to happen
    while ( 1 )
    {
        sleep_ms( 100 );
    }
}



void doReplyWithCalibrationStatus()
{
#if USE_CARRTPICO_STDIO
    std::cout << "Received request calibration status" << std::endl;
#endif

// TODO  Get calibration status and report it
}



void doProcessCalibrationProfile()
{
#if USE_CARRTPICO_STDIO
    std::cout << "Received a calibration profile from RPi0" << std::endl;
#endif

    // TODO  Extract calibration profile and sent it to BNO055

    unsigned char err = 0;
    SerialLink::putCmd( kSendCalibProfileToPico );
    SerialLink::putByte( err );

#if USE_CARRTPICO_STDIO
    // TODO     send calibration profile
#endif
}



void doReplyWithCalibrationProfile()
{
#if USE_CARRTPICO_STDIO
    std::cout << "Received request for the current calibration profile" << std::endl;
#endif

// Get calibration profile from BNO055 and relay it to RPi0
}






void MainProcess::doDrivingStatusUpdate()
{
#if USE_CARRTPICO_STDIO
    std::cout << "Received Drive status update from RPi0" << std::endl;
#endif


}



void MainProcess::doRequestBatteryLevel()
{
#if USE_CARRTPICO_STDIO
    std::cout << "Received request for battery V from RPi0" << std::endl;
#endif


}



void MainProcess::doRequestMotorBatteryLevel()
{
#if USE_CARRTPICO_STDIO
    std::cout << "Received request for motor battery V from RPi0" << std::endl;
#endif


}



void MainProcess::doIdentifyPicoCore()
{
#if USE_CARRTPICO_STDIO
    std::cout << "Received request to ID Core from RPi0" << std::endl;
#endif

    SerialLink::putCmd( kIdentifyPicoCore );
    uint32_t core = get_core_num();
    SerialLink::put( core );
}



void MainProcess::doTestPicoReportError()
{
#if USE_CARRTPICO_STDIO
    std::cout << "Received test Pico error report cmd from RPi0" << std::endl;
#endif
            
    throw CarrtError( makePicoErrorId( PicoError::kPicoTestError, 1, 0 ), "CARRT Pico test error sent by request" );
}



void MainProcess::doExtendedCmd()
{
#if USE_CARRTPICO_STDIO
    std::cout << "Received extended cmd from RPi0" << std::endl;
#endif

    uint8_t cmd = SerialLink::getCmd();
    dispatchExtendedCmd( cmd );
}



void MainProcess::doUnknownCommand( uint8_t cmd )
{
#if USE_CARRTPICO_STDIO
    std::cout << "Warning: Received unknown cmd from RPi0: " << cmd << std::endl;
#endif
    
    SerialLink::putCmd( kErrorReportFromPico );
    SerialLink::putByte( kPicoNonFatalError );
    int errCode = makePicoErrorId( kPicoMainProcessError, 3, 0 );
    SerialLink::put( errCode );
}




void MainProcess::dispatchExtendedCmd( uint8_t cmd )
{
    switch ( cmd )
    {
        case kNullExtCmd:
            break;

        // Debugging extended cmds
        case kTestExtendedCmd:
            break;

        case kTriggerError:
            break;

        default:
            doUnknownExtendedCmd( cmd );
            break;
    }
}



void MainProcess::doUnknownExtendedCmd( uint8_t cmd )
{
#if USE_CARRTPICO_STDIO
    std::cout << "Warning: Received unknown extended cmd from RPi0: " << cmd << std::endl;
#endif
    
    SerialLink::putCmd( kErrorReportFromPico );
    SerialLink::putByte( kPicoNonFatalError );
    int errCode = makePicoErrorId( kPicoMainProcessError, 4, 0 );
    SerialLink::put( errCode );
}
