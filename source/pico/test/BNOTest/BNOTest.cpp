#include "BNO055.h"
#include "CarrtPicoDefines.h"
#include "Core1.h"
#include "CoreAtomic.hpp"
#include "EventManager.h"
#include "I2C.h"
#include "PicoOutputUtils.hpp"
#include "CarrtPicoReset.h"
#include "PicoState.h"
#include "SerialCommands.h"
#include "SerialCommandProcessor.h"
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


bool gSendTimerEvents{ false };


bool checkAndReportCalibration( EventManager& events, SerialLink& link )
{
    auto status{ BNO055::getCalibration() };
    SendCalibrationStatusCmd calibStatus( status.mag, status.accel, status.gyro, status.system );
    calibStatus.takeAction( events, link );
    output2cout( 
        "Calib status (M, A, G, S): ", 
        static_cast<int>( status.mag ), 
        static_cast<int>( status.accel ),
        static_cast<int>( status.gyro ), 
        static_cast<int>( status.system ) 
    );
    return BNO055::calibrationGood( status );
}



int main()
{
    try
    {
        CoreAtomic::CAtomicInitializer theInitializationIsDone;
        PicoState::initialize();

        stdio_init_all();

        std::cout << "This is CARRT Pico" << std::endl;
        std::cout << "This is core " << get_core_num() << std::endl;

        // Initialise UART for data
        SerialLinkPico rpi0;

        // Init I2C
        I2C::initI2C();

        gpio_init( CARRTPICO_HEARTBEAT_LED );
        gpio_set_dir( CARRTPICO_HEARTBEAT_LED, GPIO_OUT );

        SerialCommandProcessor scp( rpi0 );
        scp.registerCommand<TimerControlCmd>( kTimerControl );
        scp.registerCommand<DebugLinkCmd>( kDebugSerialLink );

        // Make sure we wait long enough for BNO055 to go through powerup
        sleep_ms( 650 );        // Minimum wait for BNO055 to powerup

        BNO055::init();

        // Tell RPi0 we are ready to receive commands
        PicoReadyCmd picoReady( to_ms_since_boot( get_absolute_time() ) );
        picoReady.sendOut( rpi0 );

        bool ledState = false;

        Core1::launchCore1();

        sleep_ms( 500 );

        Core1::queueEventForCore1( Core1::kBNO055InitDelay, 7000 );

        while ( true ) 
        {
            uint32_t timeTick{ to_ms_since_boot( get_absolute_time() ) };
            int eventCode{};
            int eventParam{};

            if ( Events().getNextEvent( &eventCode, &eventParam ) )
            {
                switch ( eventCode )
                {
                    case Event::kNavUpdateEvent:
                        // std::cout << "Nav " << eventParam << std::endl;
                        break;
                        
                    case Event::kQuarterSecondTimerEvent:
                        // std::cout << "1/4 " << eventParam << std::endl;
                        if ( gSendTimerEvents )
                        {
                            // std::cout << "1/4 " << eventParam << ", " << gSendTimerEvents << std::endl;
                            TimerEventCmd qtrSec( TimerEventCmd::k1QuarterSecondEvent, eventParam, timeTick );
                            qtrSec.sendOut( rpi0 );
                        }
                        break;
                        
                    case Event::kOneSecondTimerEvent:
                        // std::cout << "1 s " << eventParam << std::endl;
                        if ( gSendTimerEvents )
                        {
                            TimerEventCmd oneSec( TimerEventCmd::k1SecondEvent, eventParam, timeTick );
                            oneSec.sendOut( rpi0 );
                        }
                        gpio_put( CARRTPICO_HEARTBEAT_LED, ledState );
                        ledState = !ledState;

                        if ( PicoState::navCalibrated() )
                        {
                            // Testing
                            // Send nav events...
                            float heading{ BNO055::getHeading() };
                            std::cout << "Heading: " << heading << std::endl;
                            int calibM{ BNO055::getMagCalibration() };
                            std::cout << "Calib-M: " << calibM << std::endl;
                            int calibS = BNO055::getSysCalibration();
                            std::cout << "Calib-S: " << calibS << std::endl;
                        }
                        else if ( PicoState::calibrationInProgress() )
                        {
                            bool status = checkAndReportCalibration( Events(), rpi0 );
                            if ( status )
                            {
                                PicoState::navCalibrated( true );
                                PicoState::calibrationInProgress( false );

                                output2cout( "BNO055 calibrated!" );
                                PicoReadyNavCmd navReady( to_ms_since_boot( get_absolute_time() ) );
                                navReady.takeAction( Events(), rpi0 );
                            }
                        }
                        break;
                        
                    case Event::kEightSecondTimerEvent:
                        // std::cout << "8 s " << eventParam << std::endl;
                        if ( gSendTimerEvents )
                        {
                            TimerEventCmd eightSec( TimerEventCmd::k8SecondEvent, eventParam, timeTick );
                            eightSec.sendOut( rpi0 );
                        }
                        break;


                    case Event::kSendCalibrationInfoEvent:
                    {
                        bool status = checkAndReportCalibration( Events(), rpi0 );
                        bool oldStatus = PicoState::navCalibrated( status );
                        if ( status != oldStatus )
                        {
                            if ( status )
                            {
                                output2cout( "Gone from not calibrated to calibrated" );
                                PicoState::calibrationInProgress( false ); 
                                
                                PicoReadyNavCmd navReady( to_ms_since_boot( get_absolute_time() ) );
                                navReady.takeAction( Events(), rpi0 );
                            }
                            else
                            {
                                output2cout( "Gone from calibrated to not calibrated" );
                                // TODO something...
                                PicoState::calibrationInProgress( true );
                            }
                        }
                        break;   
                    }  
                        
                    case Event::kBeginCalibrationEvent:
                        output2cout( "Got begin calibration event" );
                        PicoState::navCalibrated( false );
                        // Reset BNO055??
                        break;


                    case Event::kPicoResetEvent:
                        // TODO Reset Pico
                        PicoReset::reset( rpi0 );
                        break;
                }

                if ( Events().hasEventQueueOverflowed() )
                {
                    output2cout( "Event queue overflowed" );
                    ErrorReportCmd errCmd( false, makePicoErrorId( kPicoMainProcessError, 1, 1 ) );
                    errCmd.sendOut( rpi0 );
                    Events().resetEventQueueOverflowFlag();
                }
            }

            auto cmd{ scp.receiveCommandIfAvailable() };
            if ( cmd )
            {
                cmd.value()->takeAction( Events(), rpi0 );
            }
            sleep_ms( 25 );
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

    return 0;
}
