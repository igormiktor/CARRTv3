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
#include "OutputUtils.hpp"
#include "CarrtPicoReset.h"

#include "Clock.h"

#include "BNO055.h"
#include "I2C.h"
#include "SerialLinkPico.h"

#include "CarrtError.h"
#include "SerialMessages.h"
#include "SerialMessageProcessor.h"
#include "DebugUtils.hpp"

#include "PicoState.h"

#include "CoreAtomic.hpp"



#if 0   // Use this to test that compile definitions inherit properly in cmake build

#if defined( DEBUGUTILS_ON ) && DEBUGUTILS_ON
    #pragma message "DEBUGUTILS_ON = ON"
#elif defined( DEBUGUTILS_ON )
    #pragma message "DEBUGUTILS_N = OFF"
#else
    #pragma message "DEBUGUTILS_N = undef"
#endif

#if defined( USE_CARRTPICO_STDIO ) && USE_CARRTPICO_STDIO
    #pragma message "USE_CARRTPICO_STDIO = ON"
#elif defined( USE_CARRTPICO_STDIO )
    #pragma message "USE_CARRTPICO_STDIO = OFF"
#else
    #pragma message "USE_CARRTPICO_STDIO = undef"
#endif

#if defined( DEBUGPICO ) && DEBUGPICO
    #pragma message "DEBUGPICO = ON"
#elif defined( DEBUGPICO )
    #pragma message "DEBUGPICO = OFF"
#else
    #pragma message "DEBUGPICO = undef"
#endif

#if defined( BUILDING_FOR_PICO ) && BUILDING_FOR_PICO
    #pragma message "BUILDING_FOR_PICO = ON"
#elif defined( BUILDING_FOR_PICO )
    #pragma message "BUILDING_FOR_PICO = OFF"
#else
    #pragma message "BUILDING_FOR_PICO = undef"
#endif

#endif  // Checking compile definitions


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

        // Default starting values (at least for now, RPi0 can change these via msg)
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
    constexpr int kWaitTimeInMin{ 1 };
    for ( int i{ 0 }; i < kWaitTimeInMin*60*10; ++i )
    {
        Clock::sleep( 100ms );
        HeartBeatLed::toggle();

        // See if we get a sent a reset message
        if ( rpi0.isReadable() )
        {
            auto msgType = rpi0.getMsgType();
            if ( msgType && *msgType == std::to_underlying( MsgId::kResetPicoMsg ) )
            {
                PicoReset::reset( rpi0 );
            }
        }

        // If no reset msg, we keep strobing the LED
    }

    // If we get here, just force reset
    output2cout( "Fatal reset" );
    PicoReset::fatalReset();

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

        // The encoders are set up on Core1

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
        Core1::queueEventForCore1( EvtId::kBNO055InitializeEvent, BNO055::kWaitAfterPowerOnReset );

        // Tell Core1 to initialize the encoders (so the interrupts go to Core1 )
        Core1::queueEventForCore1( EvtId::kInitEncoders );
    }



    [[maybe_unused]]        // May not call this function during some debugging (leave smp in DumpByte mode)
    void setupMessageProcessor( SerialMessageProcessor& smp )
    {
        // Only register those messages we actually can receive
        // Messages that are only outgoing don't need to be registered
        smp.registerMessage<PingMsg>( MsgId::kPingMsg );
        smp.registerMessage<PingReplyMsg>( MsgId::kPingReplyMsg );
    //  smp.registerMessage<PicoReadyMsg>( MsgId::kPicoReady );
    //  smp.registerMessage<PicoNavStatusUpdateMsg>( MsgId::kPicoNavStatusUpdate );
    //  smp.registerMessage<PicoSaysStopMsg>( MsgId::kPicoSaysStop );
        smp.registerMessage<MsgControlMsg>( MsgId::kMsgControlMsg );
        smp.registerMessage<ResetPicoMsg>( MsgId::kResetPicoMsg );
    //  smp.registerMessage<TimerEventMsg>( MsgId::kTimerEventMsg );
        smp.registerMessage<TimerControlMsg>( MsgId::kTimerControl );
        smp.registerMessage<BeginCalibrationMsg>( MsgId::kBeginCalibration );
        smp.registerMessage<RequestCalibrationStatusMsg>( MsgId::kRequestCalibStatus );
    //  smp.registerMessage<CalibrationInfoUpdateMsg>( MsgId::kCalibrationInfoUpdate );
        smp.registerMessage<SetAutoCalibrateMsg>( MsgId::kSetAutoCalibrate );
        smp.registerMessage<ResetBNO055Msg>( MsgId::kResetBNO055 );
    //  smp.registerMessage<NavUpdateMsg>( MsgId::kTimerNavUpdate );
        smp.registerMessage<NavUpdateControlMsg>( MsgId::kNavUpdateControl );
        smp.registerMessage<DrivingStatusUpdateMsg>( MsgId::kDrivingStatusUpdate );
    //  smp.registerMessage<EncoderUpdateMsg>( MsgId::kEncoderUpdate );
        smp.registerMessage<EncoderUpdateControlMsg>( MsgId::kEncoderUpdateControl );
        smp.registerMessage<BatteryLevelRequestMsg>( MsgId::kBatteryLevelRequest );
    //  smp.registerMessage<BatteryLevelUpdateMsg>( MsgId::kBatteryLevelUpdate );
    //  smp.registerMessage<BatteryLowAlertMsg>( MsgId::kBatteryLowAlert );
    //  smp.registerMessage<ErrorReportMsg>( MsgId::kErrorReportFromPico );
        smp.registerMessage<TestPicoErrorRptMsg>( MsgId::kTestPicoReportError );
        smp.registerMessage<TestPicoMessagesMsg>( MsgId::kTestPicoMessages );
        smp.registerMessage<DebugLinkMsg>( MsgId::kDebugSerialLink );
    }



    void setupEventProcessor( EventProcessor& ep )
    {
        ep.registerHandler<NullEventHandler>( EvtId::kNullEvent );

        ep.registerHandler<QuarterSecondTimerHandler>( EvtId::kQuarterSecondTimerEvent );
        ep.registerHandler<OneSecondTimerHandler>( EvtId::kOneSecondTimerEvent );
        ep.registerHandler<EightSecondTimerHandler>( EvtId::kEightSecondTimerEvent );

        ep.registerHandler<NavUpdateHandler>( EvtId::kNavUpdateEvent );
        ep.registerHandler<InitializeBNO055Handler>( EvtId::kBNO055InitializeEvent );
        ep.registerHandler<BNO055ResetHandler>( EvtId::kBNO055ResetEvent );
        ep.registerHandler<BeginCalibrationHandler>( EvtId::kBNO055BeginCalibrationEvent );
        ep.registerHandler<SendCalibrationInfoHandler>( EvtId::kSendCalibrationInfoEvent );

        ep.registerHandler<PulsePicoLedHandler>( EvtId::kPulsePicoLedEvent );

        ep.registerHandler<PicoResetHandler>( EvtId::kPicoResetEvent );

        ep.registerHandler<ErrorEventHandler>( EvtId::kErrorEvent );
    }



    void sendReady( SerialLinkPico& link )
    {   uint32_t timeTick{ Clock::millis() };
        PicoReadyMsg ready( timeTick );
        ready.sendOut( link );
    }

}   // namespace