#include <cstdint>
#include <iostream>

#include "Batteries.h"
#include "CarrtError.h"
#include "CarrtPicoDefines.h"
#include "Clock.h"
#include "Event.h"
#include "EventManager.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/uart.h"
#include "pico/binary_info.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include "shared/SerialMessage.h"

// GPIO pins for Vout (3.3V)
#define GPIO_VOUT_PIN1 14      // GPIO14 (pin 19)
#define GPIO_VOUT_PIN2 15    // GPIO15 (pin 20)

bi_decl( bi_1pin_with_name( CARRTPICO_HEARTBEAT_LED, "On-board LED for blinking" ) );
bi_decl( bi_2pins_with_names( CARRTPICO_SERIAL_LINK_UART_TX_PIN, "uart1 (data) TX",
                              CARRTPICO_SERIAL_LINK_UART_RX_PIN, "uart1 (data) RX" ) );
bi_decl( bi_2pins_with_names( CARRTPICO_I2C_SDA, "i2c0 SDA", CARRTPICO_I2C_SCL, "i2c0 SCL" ) );

uint64_t elapsedTime = 0;

bool timerCallback( repeating_timer_t* )
{
    static int eighthSecCount{ 0 };
    std::uint32_t tick{ Clock::millis() };

    ++eighthSecCount;
    eighthSecCount %= 64;

    // Queue nav update events every 1/8 second
    // Event parameter counts eighth seconds ( 0, 1, 2, 3, 4, 5, 6, 7 )
    // Events().queueEvent( EvtId::kNavUpdateEvent, eighthSecCount % 8, 0,
    //                      EventManager::kHighPriority, tick );

    if ( ( eighthSecCount % 2 ) == 0 )
    {
        // Event parameter counts quarter seconds ( 0, 1, 2, 3 )
        // Events().queueEvent( EvtId::kQuarterSecondTimerEvent, ( eighthSecCount % 4 ), tick );
    }

    if ( ( eighthSecCount % 8 ) == 0 )
    {
        // Event parameter counts seconds to 8 ( 0, 1, 2, 3, 4, 5, 6, 7 )
        Events().queueEvent( EvtId::kOneSecondTimerEvent, ( eighthSecCount / 8 ), tick );
    }

    if ( eighthSecCount == 0 )
    {
        Events().queueEvent( EvtId::kEightSecondTimerEvent, 0, tick );
    }

    return true;
}

void startCore1()
{
    // std::cout << "Started Core " << get_core_num() << std::endl;

    alarm_pool_t* core1AlarmPool = alarm_pool_create( TIMER_IRQ_2, 4 );

    repeating_timer_t timer;
    // Repeating 1/8 sec timer; negative timeout means exact delay between triggers
    if ( !alarm_pool_add_repeating_timer_ms( core1AlarmPool, -125, timerCallback, NULL, &timer ) )
    {
        std::cout << "Failed to add timer\n" << std::endl;
    }

    while ( 1 )
    {
        sleep_ms( 100 );
    }
}


int main()
{
    stdio_init_all();

    std::cout << "This is CARRT Pico: Battery voltage test" << std::endl;
    std::cout << "Size of int is " << sizeof( int ) << std::endl;
    std::cout << "Start time is: " << Clock::millis() << std::endl;

    Batteries::initBatteries();

    gpio_init( CARRTPICO_HEARTBEAT_LED );
    gpio_set_dir( CARRTPICO_HEARTBEAT_LED, GPIO_OUT );

    multicore_launch_core1( startCore1 );
    std::cout << "Timer start time is: " << Clock::millis() << std::endl;

    // Configure the "Vout" pisn that we use to drive GPIO_TEST_PIN
    gpio_init( GPIO_VOUT_PIN1 );
    gpio_set_dir( GPIO_VOUT_PIN1, GPIO_OUT );
    gpio_put( GPIO_VOUT_PIN1, 1 );
    gpio_init( GPIO_VOUT_PIN2 );
    gpio_set_dir( GPIO_VOUT_PIN2, GPIO_OUT );
    gpio_put( GPIO_VOUT_PIN2, 1 );

    //    bool ledState = false;
    std::cout << "Event processing start time is: " << Clock::millis() << std::endl;
    while ( true )
    {
        EvtId eventCode;
        int eventParam;
        std::uint32_t eventTime;

        if ( Events().getNextEvent( &eventCode, &eventParam, &eventTime ) )
        {
            switch ( eventCode )
            {
                case EvtId::kNavUpdateEvent:
                    // std::cout << "Nav " << eventParam << std::endl;
                    break;

                case EvtId::kQuarterSecondTimerEvent:
                    // std::cout << "1/4 " << eventParam << std::endl;
                    break;

                case EvtId::kOneSecondTimerEvent:
                    std::cout << "1 s " << eventParam << ' ' << eventTime << std::endl;
                    // gpio_put( CARRTPICO_HEARTBEAT_LED, ledState );
                    // ledState = !ledState;
                    break;

                case EvtId::kEightSecondTimerEvent:
                    std::cout << "8 s " << eventTime << std::endl;
                    {
                        float icVolts = Batteries::getIcBatteryVoltage();
                        float motorVolts = Batteries::getMotorBatteryVoltage();
                        std::cout << "IC V: " << icVolts << std::endl;
                        std::cout << "Motor V" << motorVolts << std::endl;
                    }
                    break;

                default:
                    break;
            }
            if ( Events().hasEventQueueOverflowed() )
            {
                std::cout << "Event queue overflowed" << std::endl;
                Events().resetEventQueueOverflowFlag();
            }
        }

        sleep_ms( 25 );
    }

    return 0;
}
