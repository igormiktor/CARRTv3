#include <cstdint>
#include <iostream>

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

// GPIO Interrupt pin
#define GPIO_TEST_PIN 18    // GPIO18 (pin 24)

// Debounce time (in ms)
#define GPIO_DEBOUNCE_TIME 10    // milliseconds (seems to work well)

bi_decl( bi_1pin_with_name( CARRTPICO_HEARTBEAT_LED, "On-board LED for blinking" ) );
bi_decl( bi_2pins_with_names( CARRTPICO_SERIAL_LINK_UART_TX_PIN, "uart1 (data) TX",
                              CARRTPICO_SERIAL_LINK_UART_RX_PIN, "uart1 (data) RX" ) );
bi_decl( bi_2pins_with_names( CARRTPICO_I2C_SDA, "i2c0 SDA", CARRTPICO_I2C_SCL, "i2c0 SCL" ) );

uint64_t elapsedTime = 0;

bool timerCallback( repeating_timer_t* )
{
    static int eighthSecCount = 0;

    ++eighthSecCount;
    eighthSecCount %= 64;

    // Queue nav update events every 1/8 second
    // Event parameter counts eighth seconds ( 0, 1, 2, 3, 4, 5, 6, 7 )
    // Events().queueEvent( EvtId::kNavUpdateEvent, eighthSecCount % 8, 0,
    //                      EventManager::kHighPriority );

    if ( ( eighthSecCount % 2 ) == 0 )
    {
        // Event parameter counts quarter seconds ( 0, 1, 2, 3 )
        // Events().queueEvent( EvtId::kQuarterSecondTimerEvent, ( eighthSecCount % 4 ) );
    }

    if ( ( eighthSecCount % 8 ) == 0 )
    {
        // Event parameter counts seconds to 8 ( 0, 1, 2, 3, 4, 5, 6, 7 )
        Events().queueEvent( EvtId::kOneSecondTimerEvent, ( eighthSecCount / 8 ) );
    }

    if ( eighthSecCount == 0 )
    {
        // Events().queueEvent( EvtId::kEightSecondTimerEvent, 0 );
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
        // tight_loop_contents();
        sleep_ms( 100 );
    }
}

void gpioInterruptCallback( uint gpio, std::uint32_t events )
{
    static std::uint32_t lastInterrupt = 0;

    std::uint64_t startTime = to_us_since_boot( get_absolute_time() );

    if ( gpio == GPIO_TEST_PIN )
    {
        std::uint32_t tick = Clock::millis();
        if ( tick - lastInterrupt < GPIO_DEBOUNCE_TIME )
        {
            // Debounce
            return;
        }

        lastInterrupt = tick;

        if ( events & GPIO_IRQ_EDGE_FALL )
        {
            Events().queueEvent( EvtId::kGpioInterruptTestFallingEvent, static_cast<int>( events ), tick );
        }
        if ( events & GPIO_IRQ_EDGE_RISE )
        {
            Events().queueEvent( EvtId::kGpioInterruptTestRisingEvent, static_cast<int>( events ), tick );
        }
        if ( !( events & GPIO_IRQ_EDGE_FALL || events & GPIO_IRQ_EDGE_RISE ) )
        {
            Events().queueEvent( EvtId::kGpioInterruptTestFailureEvent, static_cast<int>( events ), tick );
        }
    }
    else
    {
        Events().queueEvent( EvtId::kGpioInterruptWrongPinEvent, gpio );
    }

    elapsedTime = to_us_since_boot( get_absolute_time() ) - startTime;

    return;
}

int main()
{
    stdio_init_all();

    // puts("This is CARRT Pico!");
    std::cout << "This is CARRT Pico: GPIO interrupt test" << std::endl;
    std::cout << "Size of int is " << sizeof( int ) << std::endl;
    std::cout << "Start time is: " << Clock::millis() << std::endl;

    gpio_init( CARRTPICO_HEARTBEAT_LED );
    gpio_set_dir( CARRTPICO_HEARTBEAT_LED, GPIO_OUT );

    multicore_launch_core1( startCore1 );
    std::cout << "Timer start time is: " << Clock::millis() << std::endl;

    // Configure interrupt on GPIO_TEST_PIN
    gpio_init( GPIO_TEST_PIN );
    gpio_set_dir( GPIO_TEST_PIN, GPIO_IN );
    gpio_pull_down( GPIO_TEST_PIN );
    gpio_set_irq_enabled_with_callback( GPIO_TEST_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,
                                        true, gpioInterruptCallback );

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
                    std::cout << "Nav " << eventParam << std::endl;
                    break;

                case EvtId::kQuarterSecondTimerEvent:
                    std::cout << "1/4 " << eventParam << std::endl;
                    break;

                case EvtId::kOneSecondTimerEvent:
                    std::cout << "1 s " << eventParam << std::endl;
                    // gpio_put( CARRTPICO_HEARTBEAT_LED, ledState );
                    // ledState = !ledState;
                    break;

                case EvtId::kEightSecondTimerEvent:
                    std::cout << "8 s " << eventParam << std::endl;
                    break;

                case EvtId::kIdentifyPicoCoreEvent:
                    std::cout << "Core " << eventParam << std::endl;
                    break;

                case EvtId::kGpioInterruptTestFallingEvent:
                    gpio_put( CARRTPICO_HEARTBEAT_LED, 0 );
                    std::cout << "GPIO Falling Event " << eventTime << std::endl;
                    std::cout << "ElapsedTime (usecs) " << elapsedTime << std::endl;
                    break;

                case EvtId::kGpioInterruptTestRisingEvent:
                    gpio_put( CARRTPICO_HEARTBEAT_LED, 1 );
                    std::cout << "GPIO Rising Event " << eventTime << std::endl;
                    std::cout << "ElapsedTime (usecs) " << elapsedTime << std::endl;
                    break;

                case EvtId::kGpioInterruptTestFailureEvent:
                    gpio_put( CARRTPICO_HEARTBEAT_LED, 0 );
                    std::cout << "GPIO neither rising nor falling (error) "
                              << static_cast<uint>( eventParam ) << std::endl;
                    break;

                    //                case EvtId::kGpioInterruptWrongPinEvent:
                    //                    gpio_put( CARRTPICO_HEARTBEAT_LED, 0 );
                    //                    std::cout << "GPIO Interrupt wrong pin " << eventParam <<
                    //                    std::endl; break;

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
