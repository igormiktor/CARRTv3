#include "EventManager.h"

#include "shared/SerialCommand.h"

#include <iostream>
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/uart.h"


// UART defines
#define UART_DATA               uart1
#define UART_DATA_BAUD_RATE     115200
#define UART_DATA_TX_PIN        4
#define UART_DATA_RX_PIN        5

// GPIO Interrupt pin
#define GPIO_TEST_PIN           18      // GPIO18 (pin 24)

// Debounce time (in ms)
#define GPIO_DEBOUNCE_TIME      10      // milliseconds (seems to work well)

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT    i2c0
#define I2C_SDA     8
#define I2C_SCL     9


bi_decl( bi_1pin_with_name( PICO_DEFAULT_LED_PIN, "On-board LED for blinking" ) );
bi_decl( bi_2pins_with_names( UART_DATA_TX_PIN, "uart1 (data) TX", UART_DATA_RX_PIN, "uart1 (data) RX" ) );
bi_decl( bi_2pins_with_names( I2C_SDA, "i2c0 SDA", I2C_SCL, "i2c0 SCL" ) );




bool timerCallback( repeating_timer_t* ) 
{
    static int eighthSecCount = 0;

    ++eighthSecCount;
    eighthSecCount %= 64;

    // Queue nav update events every 1/8 second
    // Event parameter counts eighth seconds ( 0, 1, 2, 3, 4, 5, 6, 7 )
    // Events().queueEvent( Event::kNavUpdateEvent, eighthSecCount % 8, EventManager::kHighPriority );

    if ( ( eighthSecCount % 2 ) == 0 )
    {
        // Event parameter counts quarter seconds ( 0, 1, 2, 3 )
        // Events().queueEvent( Event::kQuarterSecondTimerEvent, ( eighthSecCount % 4 ) );
    }

    if ( ( eighthSecCount % 8 ) == 0 )
    {
        // Event parameter counts seconds to 8 ( 0, 1, 2, 3, 4, 5, 6, 7 )
        Events().queueEvent( Event::kOneSecondTimerEvent, ( eighthSecCount / 8 ) );
     }

    if ( eighthSecCount == 0 )
    {
        // Events().queueEvent( Event::kEightSecondTimerEvent, 0 );
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


void gpioInterruptCallback( uint gpio, uint32_t events )
{
    static uint32_t lastInterrupt = 0;

    if ( gpio == GPIO_TEST_PIN )
    {
        uint32_t tick = to_ms_since_boot( get_absolute_time() );
        if ( tick - lastInterrupt < GPIO_DEBOUNCE_TIME )
        {
            // Debounce
            return;
        }

        lastInterrupt = tick;

        if ( events & GPIO_IRQ_EDGE_FALL )
        {
            Events().queueEvent( kGpioInterruptTestFallingEvent, static_cast<int>( tick ) );
        }
        else if ( events & GPIO_IRQ_EDGE_RISE )
        {
            Events().queueEvent( kGpioInterruptTestRisingEvent, static_cast<int>( tick )  );
        }
        else
        {
            Events().queueEvent( kGpioInterruptTestFailureEvent, static_cast<int>( events )  );
        }
    }
    else
    {
        Events().queueEvent( kGpioInterruptWrongPinEvent, gpio );
    }

    return;
}



int main()
{
    stdio_init_all();

    // puts("This is CARRT Pico!");
    std::cout << "This is CARRT Pico: GPIO interrupt test" << std::endl;
    std::cout << "Size of int is " << sizeof( int ) << std::endl;
    std::cout << "Start time is: " << to_ms_since_boot( get_absolute_time() ) << std::endl;

    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init( LED_PIN );
    gpio_set_dir( LED_PIN, GPIO_OUT );

    multicore_launch_core1( startCore1 );
    std::cout << "Timer start time is: " << to_ms_since_boot( get_absolute_time() ) << std::endl;

    // Configure interrupt on GPIO_TEST_PIN
    gpio_init( GPIO_TEST_PIN );
    gpio_set_dir( GPIO_TEST_PIN, GPIO_IN );
    gpio_pull_down( GPIO_TEST_PIN );
    gpio_set_irq_enabled_with_callback( GPIO_TEST_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, 
        true, gpioInterruptCallback );

    bool ledState = false;
    std::cout << "Event processing start time is: " << to_ms_since_boot( get_absolute_time() ) << std::endl;
    while ( true ) 
    {
        int eventCode;
        int eventParam;

        if ( Events().getNextEvent( &eventCode, &eventParam ) )
        {
            switch ( eventCode )
            {
                case Event::kNavUpdateEvent:
                    std::cout << "Nav " << eventParam << std::endl;
                    break;
                    
                case Event::kQuarterSecondTimerEvent:
                    std::cout << "1/4 " << eventParam << std::endl;
                    break;
                    
                case Event::kOneSecondTimerEvent:
                    std::cout << "1 s " << eventParam << std::endl;
                    // gpio_put( LED_PIN, ledState );
                    // ledState = !ledState;
                    break;
                    
                case Event::kEightSecondTimerEvent:
                    std::cout << "8 s " << eventParam << std::endl;
                    break;

                case Event::kIdentifyCoreEvent:
                    std::cout << "Core " << eventParam << std::endl;
                    break;

                case Event::kGpioInterruptTestFallingEvent:
                    gpio_put( LED_PIN, 0 );
                    std::cout << "GPIO Falling Event " << static_cast<uint>( eventParam ) << std::endl;
                    break;

                case Event::kGpioInterruptTestRisingEvent:
                    gpio_put( LED_PIN, 1 );
                    std::cout << "GPIO Rising Event " << static_cast<uint>( eventParam ) << std::endl;
                    break;

                case Event::kGpioInterruptTestFailureEvent:
                    gpio_put( LED_PIN, 0 );
                    std::cout << "GPIO neither rising nor falling (error) " << static_cast<uint>( eventParam ) << std::endl;
                    break;

                case Event::kGpioInterruptWrongPinEvent:
                    gpio_put( LED_PIN, 0 );
                    std::cout << "GPIO Interrupt wrong pin " << eventParam << std::endl;
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
