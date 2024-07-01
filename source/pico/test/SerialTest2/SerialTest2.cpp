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
    Events().queueEvent( Event::kNavUpdateEvent, eighthSecCount % 8, EventManager::kHighPriority );

    if ( ( eighthSecCount % 2 ) == 0 )
    {
        // Event parameter counts quarter seconds ( 0, 1, 2, 3 )
        Events().queueEvent( Event::kQuarterSecondTimerEvent, ( eighthSecCount % 4 ) );
    }

    if ( ( eighthSecCount % 8 ) == 0 )
    {
        // Event parameter counts seconds to 8 ( 0, 1, 2, 3, 4, 5, 6, 7 )
        Events().queueEvent( Event::kOneSecondTimerEvent, ( eighthSecCount / 8 ) );
        Events().queueEvent( Event::kIdentifyCoreEvent, get_core_num() );
    }

    if ( eighthSecCount == 0 )
    {
        Events().queueEvent( Event::kEightSecondTimerEvent, 0 );
    }

    return true;
}



void startCore1() 
{
    std::cout << "Started Core " << get_core_num() << std::endl;

    // Test sending UART from core 1
    uart_putc_raw( UART_DATA, SerialCommand::kIdentifyCore );
    uart_putc_raw( UART_DATA, 1 );

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


union Transfer
{
    char    c[4];
    int     i;
    float   f;
};


int main()
{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init( I2C_PORT, 400*1000 );
    
    gpio_set_function( I2C_SDA, GPIO_FUNC_I2C ) ;
    gpio_set_function( I2C_SCL, GPIO_FUNC_I2C );
    gpio_pull_up( I2C_SDA );
    gpio_pull_up( I2C_SCL );


    // Initialise UART for data
    uart_init( UART_DATA, UART_DATA_BAUD_RATE );
    // Set the GPIO pin mux to the UART
    gpio_set_function( UART_DATA_TX_PIN, GPIO_FUNC_UART );
    gpio_set_function( UART_DATA_RX_PIN, GPIO_FUNC_UART );


    // puts("This is CARRT Pico!");
    std::cout << "This is CARRT Pico: event queue test" << std::endl;
    std::cout << "This is core " << get_core_num() << std::endl;

    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init( LED_PIN );
    gpio_set_dir( LED_PIN, GPIO_OUT );

    multicore_launch_core1( startCore1 );

    // Avoid immediate risk of collision with UART test from Core 1
    sleep_ms( 100 );

    bool ledState = false;
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
                    uart_putc_raw( UART_DATA, SerialCommand::kTimerNavUpdate );
                    uart_putc_raw( UART_DATA, static_cast<char>( eventParam ) );
                    break;
                    
                case Event::kQuarterSecondTimerEvent:
                    std::cout << "1/4 " << eventParam << std::endl;
                    uart_putc_raw( UART_DATA, SerialCommand::kTimer1_4s );
                    uart_putc_raw( UART_DATA, static_cast<char>( eventParam ) );
                    break;
                    
                case Event::kOneSecondTimerEvent:
                    std::cout << "1 s " << eventParam << std::endl;
                    uart_putc_raw( UART_DATA, SerialCommand::kTimer1s );
                    uart_putc_raw( UART_DATA, static_cast<char>( eventParam ) );
                    gpio_put( LED_PIN, ledState );
                    ledState = !ledState;
                    break;
                    
                case Event::kEightSecondTimerEvent:
                    std::cout << "8 s " << eventParam << std::endl;
                    uart_putc_raw( UART_DATA, SerialCommand::kTimer8s );
                    uart_putc_raw( UART_DATA, static_cast<char>( eventParam ) );
                    break;

                case Event::kIdentifyCoreEvent:
                    std::cout << "Core " << eventParam << std::endl;
                    uart_putc_raw( UART_DATA, SerialCommand::kIdentifyCore );
                    uart_putc_raw( UART_DATA, 0 );
                    break;
            }
            if ( Events().hasEventQueueOverflowed() )
            {
                std::cout << "Event queue overflowed" << std::endl;
                Events().resetEventQueueOverflowFlag();
            }
        }

        if ( uart_is_readable( UART_DATA ) )
        {
            Transfer t;
            char cmd = uart_getc( UART_DATA );

            switch( cmd )
            {
                case 'T':
                case 't':
                    uart_putc_raw( UART_DATA, 'T' );
                    //uart_putc_raw( UART_DATA, '\r' );
                    //uart_putc_raw( UART_DATA, '\n' );
                    break;

                case 'F':
                case 'f':
                    uart_putc_raw( UART_DATA, 'F' );
                    t.f = 2.71828;
                    for ( int i = 0; i < 4; ++i )
                    {
                        uart_putc_raw( UART_DATA, t.c[i] );
                    }
                    //uart_putc_raw( UART_DATA, '\r' );
                    //uart_putc_raw( UART_DATA, '\n' );
                    break;

                case 'K':
                case 'k':
                    uart_putc_raw( UART_DATA, 'K' );
                    t.i = 660327733;
                    for ( int i = 0; i < 4; ++i )
                    {
                        uart_putc_raw( UART_DATA, t.c[i] );
                    }
                    //uart_putc_raw( UART_DATA, '\r' );
                    //uart_putc_raw( UART_DATA, '\n' );
                    break;

                case 'I':
                case 'i':
                    uart_putc_raw( UART_DATA, 'I' );
                    t.i = 123456789;
                    for ( int i = 0; i < 4; ++i )
                    {
                        uart_putc_raw( UART_DATA, t.c[i] );
                    }
                    //uart_putc_raw( UART_DATA, '\r' );
                    //uart_putc_raw( UART_DATA, '\n' );
                    break;

                case 'J':
                case 'j':
                    uart_putc_raw( UART_DATA, 'J' );
                    t.i = -123456789;
                    for ( int i = 0; i < 4; ++i )
                    {
                        uart_putc_raw( UART_DATA, t.c[i] );
                    }
                    //uart_putc_raw( UART_DATA, '\r' );
                    //uart_putc_raw( UART_DATA, '\n' );
                    break;

                case 'N':
                case 'n':
                    uart_putc_raw( UART_DATA, '\r' );
                    uart_putc_raw( UART_DATA, '\n' );
                    break;

                default:
                    uart_putc_raw( UART_DATA, '[' );
                    if ( cmd == '\r' || cmd == '\n' )
                    {
                        cmd = '*';
                    }
                    uart_putc_raw( UART_DATA, cmd );
                    uart_putc_raw( UART_DATA, ']' );
                    uart_putc_raw( UART_DATA, '\r' );
                    uart_putc_raw( UART_DATA, '\n' );
                    break;
            }
        }
        sleep_ms( 25 );
    }

    return 0;
}
