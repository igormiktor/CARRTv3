#include "BNO055.h"
#include "CarrtPicoDefines.h"
#include "Core1.h"
#include "CoreAtomic.hpp"
#include "Delays.h"
#include "EventManager.h"
#include "I2C.h"
#include "PicoOutputUtils.hpp"
#include "CarrtPicoReset.h"
#include "PicoState.h"
#include "SerialMessages.h"
#include "SerialMessageProcessor.h"
#include "SerialLinkPico.h"

#include <iostream>

#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/uart.h"


bi_decl( bi_1pin_with_name( CARRTPICO_HEARTBEAT_LED, "On-board LED for blinking" ) );
bi_decl( bi_2pins_with_names( CARRTPICO_SERIAL_LINK_UART_TX_PIN, "uart1 (data) TX", CARRTPICO_SERIAL_LINK_UART_RX_PIN, "uart1 (data) RX" ) );
bi_decl( bi_2pins_with_names( CARRTPICO_I2C_SDA, "i2c0 SDA", CARRTPICO_I2C_SCL, "i2c0 SCL" ) );





void checkAndReportCalibration( EventManager& events, SerialLink& link )
{
    auto calibData{ BNO055::getCalibration() };
    bool status = BNO055::calibrationGood( calibData );

    bool oldStatus = PicoState::navCalibrated( status );
    if ( status != oldStatus )
    {
        // Send message to RPi0 that Nav Status change and set Pico state accordingly
        PicoNavStatusUpdateMsg navReadyStatus( status, calibData.mag, calibData.accel, calibData.gyro, calibData.system );
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
        SendCalibrationInfoMsg calibStatus( calibData.mag, calibData.accel, calibData.gyro, calibData.system );
        calibStatus.takeAction( events, link );
    }

    output2cout( "Calib status (M, A, G, S): ", static_cast<int>( calibData.mag ), static_cast<int>( calibData.accel ), 
        static_cast<int>( calibData.gyro ), static_cast<int>( calibData.system ) );
}


void initializeBNO055( EventManager& events )
{
    // Note this call likely to include internal delay of 600ms
    BNO055::init();

    // Because delay built into init(), can trigger BeginCalibration without delay
    events.queueEvent( kBNO055BeginCalibrationEvent );
}


void resetBNO055( EventManager& events )
{
    // Note this call is followed by 650ms delay before we can call init()
    BNO055::reset();
    PicoState::navCalibrated( false );
    Core1::queueEventForCore1( kBNO055InitializeEvent, BNO055::kWaitAfterPowerOnReset );
}




int main()
{
    using CarrtPico::delay;

    try
    {
        CoreAtomic::CAtomicInitializer theInitializationIsDone;
        PicoState::initialize();

        stdio_init_all();

        output2cout( "This is CARRT Pico" );

        // Start Core1 and queue a future event to trigger initialization of BNO055
        Core1::launchCore1();
        // If we get here, guaranteed Core1 is running and in its main event loop
        // So perfect time to queue this future event
        Core1::queueEventForCore1( kBNO055InitializeEvent, BNO055::kWaitAfterPowerOnReset );

        // Initialise UART for data
        SerialLinkPico rpi0;

        // Init I2C
        I2C::initI2C();

        gpio_init( CARRTPICO_HEARTBEAT_LED );
        gpio_set_dir( CARRTPICO_HEARTBEAT_LED, GPIO_OUT );

        SerialMessageProcessor smp( 10, rpi0 );
        smp.registerMessage<TimerControlMsg>( MsgId::kTimerControl );
        smp.registerMessage<DebugLinkMsg>( MsgId::kDebugSerialLink );

        // Tell RPi0 we are ready to receive messages
        PicoReadyMsg picoReady( to_ms_since_boot( get_absolute_time() ) );
        picoReady.sendOut( rpi0 );


        // Test only:  queue a BO055 reset in 15 seconds
        Core1::queueEventForCore1( kBNO055ResetEvent, 15000 );

        bool ledState = false;

        while ( true ) 
        {
            uint32_t timeTick{ to_ms_since_boot( get_absolute_time() ) };
            int eventCode{};
            int eventParam{};

            if ( Events().getNextEvent( &eventCode, &eventParam ) )
            {
                switch ( eventCode )
                {
                    case kBNO055InitializeEvent:
                        output2cout( "Got BNO055 initialize event" );
                        initializeBNO055( Events() );
                        break;

                    case kBNO055ResetEvent:
                        output2cout( "Got BNO055 reset event" );
                        resetBNO055( Events() );
                        break;

                    case Event::kNavUpdateEvent:
                        if ( PicoState::navCalibrated() && PicoState::wantNavMsgs() )
                        {
                            float heading = BNO055::getHeading();
                            output2cout( "Hdg: ", heading, "T" );
                            NavUpdateMsg navUpdate( heading, timeTick );
                            navUpdate.takeAction( Events(), rpi0 );
                        }
                        break;
                        
                    case Event::kQuarterSecondTimerEvent:
                        // std::cout << "1/4 " << eventParam << std::endl;
                        if ( PicoState::wantTimerMsgs() )
                        {
                            // std::cout << "1/4 " << eventParam << ", " << PicoState::wantTimerMsgs() << std::endl;
                            TimerEventMsg qtrSec( TimerEventMsg::k1QuarterSecondEvent, eventParam, timeTick );
                            qtrSec.sendOut( rpi0 );
                        }
                        break;
                        
                    case Event::kOneSecondTimerEvent:
                        if ( PicoState::wantTimerMsgs() )
                        {
                            TimerEventMsg oneSec( TimerEventMsg::k1SecondEvent, eventParam, timeTick );
                            oneSec.sendOut( rpi0 );
                        }
                        break;

                    case kPulsePicoLedEvent:
                        gpio_put( CARRTPICO_HEARTBEAT_LED, ledState );
                        ledState = !ledState;
                        break;
                        
                    case Event::kEightSecondTimerEvent:
                        if ( PicoState::wantTimerMsgs() )
                        {
                            TimerEventMsg eightSec( TimerEventMsg::k8SecondEvent, eventParam, timeTick );
                            eightSec.sendOut( rpi0 );
                        }
                        break;

                    case Event::kSendCalibrationInfoEvent:
                        checkAndReportCalibration( Events(), rpi0 );
                        break;

                    case Event::kBNO055BeginCalibrationEvent:
                        output2cout( "Got begin calibration event" );
                        PicoState::navCalibrated( false );
                        PicoState::calibrationInProgress( true );
                        break;

                    case Event::kPicoResetEvent:
                        // TODO Reset Pico
                        PicoReset::reset( rpi0 );
                        break;
                }

                if ( Events().hasEventQueueOverflowed() )
                {
                    output2cout( "Event queue overflowed" );
                    ErrorReportMsg errMsg( false, makePicoErrorId( kPicoMainProcessError, 1, 1 ) );
                    errMsg.sendOut( rpi0 );
                    Events().resetEventQueueOverflowFlag();
                }
            }

            auto msg{ smp.receiveMessageIfAvailable() };
            if ( msg )
            {
                msg.value()->takeAction( Events(), rpi0 );
            }
            CarrtPico::sleep( 25ms );
        }
    }


    catch ( const CarrtError& err )
    {
        std::cerr << "Error: " << err.errorCode() << ", " << err.what() << std::endl;
    }

    catch ( const std::exception& err )
    {
        std::cerr << "Error: " << err.what() << std::endl;
    }

    catch (...)
    {
        std::cerr << "Error of unknown type." << std::endl;
    }

    PicoReset::fatalReset();

    return 0;
}
