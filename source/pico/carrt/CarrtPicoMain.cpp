/*
    CarrtPicoMain.cpp - CARRT-Pico's main function.  Initializes and hands off to
    the Main Process.

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
#include "Core1.h"
#include "Encoders.h"
#include "EventHandlers.h"
#include "EventManager.h"
#include "EventProcessor.h"

#include "HeartBeatLed.h"
#include "MainProcess.h"
#include "PicoOutputUtils.hpp"
#include "CarrtPicoReset.h"

#include "Delays.h"

#include "BNO055.h"
#include "I2C.h"
#include "SerialLinkPico.h"

#include "CarrtError.h"
#include "SerialMessages.h"
#include "SerialMessageProcessor.h"
#include "DebugUtils.hpp"

#include "PicoState.h"

#include "CoreAtomic.hpp"



#include "pico/stdlib.h"
//#include "pico/multicore.h"
// #include "pico/util/queue.h"
// #include "hardware/clocks.h"
#include "hardware/i2c.h"
// #include "hardware/timer.h"
// #include "hardware/uart.h"




namespace
{
    void initializeFailSafeHardware();
    void initializeFailableHardware();
    void setupMessageProcessor( SerialMessageProcessor& smp );
    void setupEventProcessor( EventProcessor& ep );
    void sendReady( SerialLinkPico& link );
}



constexpr int kSerialMessageHandlerReserveSize  = 20;
constexpr int kEventHandlerReserveSize          = 24;



int main()
{
    CoreAtomic::CAtomicInitializer theInitializationIsDone;
    PicoState::initialize();

    initializeFailSafeHardware();

    // Open the serial link to RPi0
    SerialLinkPico rpi0;

    try
    {
        initializeFailableHardware();

        output2cout( "CARRT Pico started, hardware initialized, both cores running." );
    
        // Set up message processor
        SerialMessageProcessor smp( kSerialMessageHandlerReserveSize, rpi0 );
        setupMessageProcessor( smp );

        // Set up event processor
        EventProcessor ep( kEventHandlerReserveSize );
        setupEventProcessor( ep );
    
        // Report we are started and ready to receive messages
        sendReady( rpi0 );

        // TESTING
        PicoState::allMsgsSendOn();

        MainProcess::runMainEventLoop( Events(), ep, smp, rpi0 );
    }

    catch( const CarrtError& e )
    {
        // Report the error...
        ErrorReportMsg err( kPicoFatalError, e.errorCode() ); 
        err.sendOut( rpi0 );

        output2cout( "Fatal error ", e.errorCode(), ' ', e.what() );
    }

    catch( const std::exception& e )
    {
        int errCode{ makePicoErrorId( kPicoMainError, 1, 0 ) };
        ErrorReportMsg err( kPicoFatalError, errCode ); 
        err.sendOut( rpi0 );

        output2cout( "Fatal error of unexpected nature", e.what() );
    }

    catch( ... )
    {    
        int errCode{ makePicoErrorId( kPicoMainError, 2, 0 ) };
        ErrorReportMsg err( kPicoFatalError, errCode ); 
        err.sendOut( rpi0 );

        output2cout( "Fatal error of unknown type" );
    }


    output2cout( "Pico frozen and displaying fast LED strobe" );
    // Just spin and put HeartBeatLed on fast strobe
    while ( 1 )
    {
        CarrtPico::sleep( 100ms );
        HeartBeatLed::toggle();

        // See if we get a sent a reset message
        if ( rpi0.isReadable() )
        {
            auto msgType = rpi0.getMsgType();
            if ( msgType && *msgType == kResetMsg )
            {
                PicoReset::reset( rpi0 );
            }
        }

        // If no reset msg, we keep strobing the LED
    }

    return 0;
}




namespace
{

    void initializeFailSafeHardware()
    {
        // Nothing in this function throws or even fails

    #if USE_CARRTPICO_STDIO
        // Initialize C/C++ stdio (used for status output and debugging)
        stdio_init_all();
    #endif 

        // Set up Pico's I2C (to talk with BNO055)
        I2C::initI2C();

        // Initialize the heartbeat LED
        HeartBeatLed::initialize();

        // Set up the encoders;
        Encoders::initEncoders();

        // Initialize state
        PicoState::initialize();
    }




    void initializeFailableHardware()
    {
        // These function calls may throw

        // Launch Core1
        Core1::launchCore1();

        // If we get here, guaranteed Core1 is running and in its main event loop
        // So perfect time to queue this future event ti triggle initialization of BNO055.
        // BNO055 needs nearly 1 sec to be ready to accept I2C )
        Core1::queueEventForCore1( kBNO055InitializeEvent, BNO055::kWaitAfterPowerOnReset );
    }



    void setupMessageProcessor( SerialMessageProcessor& smp )
    {
        // Only register those messages we actually can receive
        // Messages that are only outgoing don't need to be registered
        smp.registerMessage<NullMsg>( kNullMsg );
    //  smp.registerMessage<PicoReadyMsg>( kPicoReady );
    //  smp.registerMessage<PicoNavStatusUpdateMsg>( kPicoNavStatusUpdate );
    //  smp.registerMessage<PicoSaysStopMsg>( kPicoSaysStop );
        smp.registerMessage<MsgControlMsg>( kMsgControlMsg );
        smp.registerMessage<ResetMsg>( kResetMsg );
        smp.registerMessage<BeginCalibrationMsg>( kBeginCalibration );
        smp.registerMessage<RequestCalibrationStatusMsg>( kRequestCalibStatus );
    //  smp.registerMessage<SendCalibrationInfoMsg>( kSendCalibInfo );
        smp.registerMessage<ResetBNO055Msg>( kResetBNO055 );
    //  smp.registerMessage<TimerEventMsg>( kTimerEvent );
        smp.registerMessage<TimerControlMsg>( kTimerControl );
    //  smp.registerMessage<NavUpdateMsg>( kTimerNavUpdate );
        smp.registerMessage<NavUpdateControlMsg>( kNavUpdateControl );
        smp.registerMessage<DebugLinkMsg>( kDebugSerialLink );
    }



    void setupEventProcessor( EventProcessor& ep )
    {
        ep.registerHandler<NullEventHandler>( kNullEvent );

        ep.registerHandler<QuarterSecondTimerHandler>( kQuarterSecondTimerEvent );
        ep.registerHandler<OneSecondTimerHandler>( kOneSecondTimerEvent );
        ep.registerHandler<EightSecondTimerHandler>( kEightSecondTimerEvent );

        ep.registerHandler<NavUpdateHandler>( kNavUpdateEvent );
        ep.registerHandler<InitializeBNO055Handler>( kBNO055InitializeEvent );
        ep.registerHandler<BNO055ResetHandler>( kBNO055ResetEvent );
        ep.registerHandler<BeginCalibrationHandler>( kBNO055BeginCalibrationEvent );
        ep.registerHandler<SendCalibrationInfoHandler>( kSendCalibrationInfoEvent );

        ep.registerHandler<PulsePicoLedHandler>( kPulsePicoLedEvent );

        ep.registerHandler<PicoResetHandler>( kPicoResetEvent );

        ep.registerHandler<ErrorEventHandler>( kErrorEvent );
    }



    void sendReady( SerialLinkPico& link )
    {
        PicoReadyMsg ready( kPicoReady );
        ready.sendOut( link );
    }

}   // namespace