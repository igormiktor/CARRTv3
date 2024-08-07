#include "EventManager.h"
#include "CarrtPicoDefines.h"

#include <iostream>
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/uart.h"




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
        Events().queueEvent( Event::kQuarterSecondTimerEvent, (eighthSecCount % 8) / 2 );
    }

    if ( ( eighthSecCount % 8 ) == 0 )
    {
        // Event parameter counts seconds to 8 ( 0, 1, 2, 3, 4, 5, 6, 7 )
        Events().queueEvent( Event::kOneSecondTimerEvent, ( eighthSecCount / 8 ) );


        Events().queueEvent( Event::kIdentifyPicoCoreEvent, get_core_num() );
    }

    if ( eighthSecCount == 0 )
    {
        Events().queueEvent( Event::kEightSecondTimerEvent, 0 );
    }

    return true;
}



void startCore1() 
{
    std::cout << "Started Code " << get_core_num() << std::endl;

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

      
    gpio_set_function( CARRTPICO_I2C_SDA, GPIO_FUNC_I2C ) ;
    gpio_set_function( CARRTPICO_I2C_SCL, GPIO_FUNC_I2C );
    gpio_pull_up( CARRTPICO_I2C_SDA );
    gpio_pull_up( CARRTPICO_I2C_SCL );


    // Initialise UART for data
    uart_init( CARRTPICO_SERIAL_LINK_UART, CARRTPICO_SERIAL_LINK_UART_BAUD_RATE );
    // Set the GPIO pin mux to the UART
    gpio_set_function( CARRTPICO_SERIAL_LINK_UART_TX_PIN, GPIO_FUNC_UART );
    gpio_set_function( CARRTPICO_SERIAL_LINK_UART_RX_PIN, GPIO_FUNC_UART );


#if 0
    repeating_timer_t timer;
    // Repeating 1/8 sec timer; negative timeout means exact delay between triggers
    if ( !add_repeating_timer_ms( -125, timerCallback, NULL, &timer ) ) 
    {
        std::cout << "Failed to add timer\n" << std::endl;
        return 1;
    }
#endif // 0


    // puts("This is CARRT Pico!");
    std::cout << "This is CARRT Pico: event queue test" << std::endl;
    std::cout << "This is core " << get_core_num() << std::endl;

    gpio_init( CARRTPICO_HEARTBEAT_LED );
    gpio_set_dir( CARRTPICO_HEARTBEAT_LED, GPIO_OUT );

    multicore_launch_core1( startCore1 );

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
                    break;
                    
                case Event::kQuarterSecondTimerEvent:
                    std::cout << "1/4 " << eventParam << std::endl;
                    break;
                    
                case Event::kOneSecondTimerEvent:
                    std::cout << "1 s " << eventParam << std::endl;
                    gpio_put( CARRTPICO_HEARTBEAT_LED, ledState );
                    ledState = !ledState;
                    break;
                    
                case Event::kEightSecondTimerEvent:
                    std::cout << "8 s " << eventParam << std::endl;
                    break;

                case Event::kIdentifyPicoCoreEvent:
                    std::cout << "Core " << eventParam << std::endl;
            }
            if ( Events().hasEventQueueOverflowed() )
            {
                std::cout << "Event queue overflowed" << std::endl;
                Events().resetEventQueueOverflowFlag();
            }
        }

        if ( uart_is_readable( CARRTPICO_SERIAL_LINK_UART ) )
        {
            Transfer t;
            char cmd = uart_getc( CARRTPICO_SERIAL_LINK_UART );

            switch( cmd )
            {
                case 'T':
                case 't':
                    uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, 'T' );
                    //uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, '\r' );
                    //uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, '\n' );
                    break;

                case 'F':
                case 'f':
                    uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, 'F' );
                    t.f = 2.71828;
                    for ( int i = 0; i < 4; ++i )
                    {
                        uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, t.c[i] );
                    }
                    //uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, '\r' );
                    //uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, '\n' );
                    break;

                case 'K':
                case 'k':
                    uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, 'K' );
                    t.i = 660327733;
                    for ( int i = 0; i < 4; ++i )
                    {
                        uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, t.c[i] );
                    }
                    //uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, '\r' );
                    //uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, '\n' );
                    break;

                case 'I':
                case 'i':
                    uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, 'I' );
                    t.i = 123456789;
                    for ( int i = 0; i < 4; ++i )
                    {
                        uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, t.c[i] );
                    }
                    //uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, '\r' );
                    //uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, '\n' );
                    break;

                case 'J':
                case 'j':
                    uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, 'J' );
                    t.i = -123456789;
                    for ( int i = 0; i < 4; ++i )
                    {
                        uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, t.c[i] );
                    }
                    //uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, '\r' );
                    //uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, '\n' );
                    break;

                case 'N':
                case 'n':
                    uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, '\r' );
                    uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, '\n' );
                    break;

                default:
                    uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, '[' );
                    if ( cmd == '\r' || cmd == '\n' )
                    {
                        cmd = '*';
                    }
                    uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, cmd );
                    uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, ']' );
                    uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, '\r' );
                    uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, '\n' );
                    break;
            }
        }
        sleep_ms( 25 );
    }

    return 0;
}
