#include "CarrtPicoDefines.h"
#include "EventManager.h"
#include "BNO055.h"
#include "I2C.h"
#include "SerialCommands.h"
#include "SerialLinkPico.h"
#include "SerialCommandProcessor.h"

#include <iostream>
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/uart.h"

#include "CoreAtomic.hpp"

bi_decl( bi_1pin_with_name( CARRTPICO_HEARTBEAT_LED, "On-board LED for blinking" ) );
bi_decl( bi_2pins_with_names( CARRTPICO_SERIAL_LINK_UART_TX_PIN, "uart1 (data) TX", CARRTPICO_SERIAL_LINK_UART_RX_PIN, "uart1 (data) RX" ) );
bi_decl( bi_2pins_with_names( CARRTPICO_I2C_SDA, "i2c0 SDA", CARRTPICO_I2C_SCL, "i2c0 SCL" ) );



bool timerCallback( repeating_timer_t* ) 
{
    static int eighthSecCount = 0;

    ++eighthSecCount;
    eighthSecCount %= 64;

    // Queue nav update events every 1/8 second
    // Event parameter counts eighth seconds ( 0, 1, 2, 3, 4, 5, 6, 7 )
    Events().queueEvent( Event::kNavUpdateEvent, eighthSecCount % 8, EventManager::kHighPriority );

    if ( ( eighthSecCount % 2 ) == 0 )
    {
        // Event parameter counts quarter seconds ( 0, 1, 2, 3 )
        Events().queueEvent( Event::kQuarterSecondTimerEvent, ( (eighthSecCount / 2) % 4 ) );
    }

    if ( ( eighthSecCount % 8 ) == 0 )
    {
        // Event parameter counts seconds to 8 ( 0, 1, 2, 3, 4, 5, 6, 7 )
        Events().queueEvent( Event::kOneSecondTimerEvent, ( eighthSecCount / 8 ) );
       }

    if ( eighthSecCount == 0 )
    {
        Events().queueEvent( Event::kEightSecondTimerEvent, 0 );
    }

    return true;
}



void startCore1() 
{
    std::cout << "This is core " << get_core_num() << std::endl;

    alarm_pool_t* core1AlarmPool = alarm_pool_create( TIMER_IRQ_2, 4 );

    repeating_timer_t timer;
    // Repeating 1/8 sec timer; negative timeout means exact delay between triggers
    if ( !alarm_pool_add_repeating_timer_ms( core1AlarmPool, -125, timerCallback, NULL, &timer ) ) 
    {
        std::cout << "Failed to add timer\n" << std::endl;
    }

    while ( 1 )
    {
        // tight_loop_contents();
        sleep_ms( 100 );
    }
}


bool gSendTimerEvents{ false };


int main()
{
    try
    {
        CoreAtomic::CAtomicInitializer theInitializationIsDone;

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
        sleep_ms( 1000 );

        BNO055::init();

        bool calibrated{ false };
        bool ledState = false;

        multicore_launch_core1( startCore1 );

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

                        if ( calibrated )
                        {
                            float heading{ BNO055::getHeading() };
                            std::cout << "Heading: " << heading << std::endl;
                            int calibM{ BNO055::getMagCalibration() };
                            std::cout << "Calib-M: " << calibM << std::endl;
                            int calibS = BNO055::getSysCalibration();
                            std::cout << "Calib-S: " << calibS << std::endl;
                        }
                        else
                        {
                            auto status{ BNO055::getCalibration() };
                            std::cout << "Calib status (M, A, G, S): " << static_cast<int>( status.mag ) << ", " 
                                << static_cast<int>( status.accel ) << ", " 
                                << static_cast<int>( status.gyro ) << ", " 
                                << static_cast<int>( status.system ) << std::endl;

                            if ( BNO055::calibrationSuccess( status ) )
                            {
                                calibrated = true;
                                std::cout << "BNO055 calibrated!" << std::endl;
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
                }
                if ( Events().hasEventQueueOverflowed() )
                {
                    std::cout << "Event queue overflowed" << std::endl;
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
