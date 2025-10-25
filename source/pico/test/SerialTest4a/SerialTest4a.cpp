#include "CarrtPicoDefines.h"
#include "EventManager.h"

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

#include "utils/CoreAtomic.hpp"

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
    Events().queueEvent( EvtId::kNavUpdateEvent, eighthSecCount % 8, EventManager::kHighPriority );

    if ( ( eighthSecCount % 2 ) == 0 )
    {
        // Event parameter counts quarter seconds ( 0, 1, 2, 3 )
        Events().queueEvent( EvtId::kQuarterSecondTimerEvent, ( eighthSecCount % 4 ) );
    }

    if ( ( eighthSecCount % 8 ) == 0 )
    {
        // Event parameter counts seconds to 8 ( 0, 1, 2, 3, 4, 5, 6, 7 )
        Events().queueEvent( EvtId::kOneSecondTimerEvent, ( eighthSecCount / 8 ) );
    }

    if ( eighthSecCount == 0 )
    {
        Events().queueEvent( EvtId::kEightSecondTimerEvent, 0 );
    }

    return true;
}



void startCore1() 
{
    std::cout << "Started Core " << get_core_num() << std::endl;

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



int main()
{
    CoreAtomic::CAtomicInitializer theInitializationIsDone;

    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init( CARRTPICO_I2C_PORT, CARRTPICO_I2C_SPEED );
    gpio_set_function( CARRTPICO_I2C_SDA, GPIO_FUNC_I2C ) ;
    gpio_set_function( CARRTPICO_I2C_SCL, GPIO_FUNC_I2C );
    gpio_pull_up( CARRTPICO_I2C_SDA );
    gpio_pull_up( CARRTPICO_I2C_SCL );


    // Initialise UART for data
    SerialLinkPico rpi0;


    // puts("This is CARRT Pico!");
    std::cout << "This is CARRT Pico: event queue test" << std::endl;
    std::cout << "This is core " << get_core_num() << std::endl;

    gpio_init( CARRTPICO_HEARTBEAT_LED );
    gpio_set_dir( CARRTPICO_HEARTBEAT_LED, GPIO_OUT );

    multicore_launch_core1( startCore1 );


    bool ledState = false;
    while ( true ) 
    {
        EvtId eventCode;
        int eventParam;

        if ( Events().getNextEvent( &eventCode, &eventParam ) )
        {
            switch ( eventCode )
            {
                case EvtId::kNavUpdateEvent:
                    std::cout << "Nav " << eventParam << std::endl;
                    // uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, SerialMessage::kTimerNavUpdate );
                    // uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, static_cast<char>( eventParam ) );
                    break;
                    
                case EvtId::kQuarterSecondTimerEvent:
                    std::cout << "1/4 " << eventParam << std::endl;
                    // uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, SerialMessage::kTimer1_4s );
                    // uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, static_cast<char>( eventParam ) );
                    break;
                    
                case EvtId::kOneSecondTimerEvent:
                    std::cout << "1 s " << eventParam << std::endl;
                    // uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, SerialMessage::kTimer1s );
                    // uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, static_cast<char>( eventParam ) );
                    gpio_put( CARRTPICO_HEARTBEAT_LED, ledState );
                    ledState = !ledState;
                    break;
                    
                case EvtId::kEightSecondTimerEvent:
                    std::cout << "8 s " << eventParam << std::endl;
                    // uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, SerialMessage::kTimer8s );
                    // uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, static_cast<char>( eventParam ) );
                    break;

                default:
                    // Do others
                    break;
            }
            if ( Events().hasEventQueueOverflowed() )
            {
                std::cout << "Event queue overflowed" << std::endl;
                Events().resetEventQueueOverflowFlag();
            }
        }

        if ( uart_is_readable( CARRTPICO_SERIAL_LINK_UART ) )
        {
            uint32_t cmd{ 0 };
            auto gotCmd = rpi0.getMsgType();
            if ( gotCmd )
            {
                cmd = *gotCmd;
            }

            switch( cmd )
            {
                case 'T':
                case 't':
                    rpi0.putMsgType( 'T' );
                    break;

                case 'F':
                case 'f':
                {
                    // Read a float
                    auto gotF = rpi0.getFloat();
                    if ( gotF )
                    {
                        float f{ *gotF };
                        // Send back 'F' and float + 1
                        f += 1;
                        rpi0.putMsgType( 'F' );
                        rpi0.put( f );                        
                    }
                    else
                    {
                        float f{ 666.666 };
                        // Send back 'X' and this float
                        rpi0.putMsgType( 'X' );
                        rpi0.put( f );                        
                    }
                    break;
                }

                case 'K':
                case 'k':
                {
                    // Read an unsigned int
                    auto gotU = rpi0.get4Bytes();
                    if ( gotU )
                    {
                        std::uint32_t u{ *gotU };
                        // Send back 'K' and u + 1
                        u += 1;
                        rpi0.putMsgType( 'K' );
                        rpi0.put( u );                        
                    }
                    else
                    {
                        std::uint32_t u{ 666 };
                        // Send back 'X' and this u
                        rpi0.putMsgType( 'X' );
                        rpi0.put( u );                        
                    }
                    break;
                }

                case 'I':
                case 'i':
                {
                    // Read an int
                    // Send back 'I"
                    rpi0.putMsgType( 'I' );
                    auto gotI = rpi0.getInt();
                    if ( gotI )
                    {
                        int i{ *gotI };
                        // Send back i + 1
                        i += 1;
                        rpi0.put( i );                        
                    }
                    else
                    {
                        int i{ -666 };
                        // Send back this float
                        rpi0.put( i );                        
                    }
                    break;
                }

                case 'J':
                case 'j':
                {
                    // Read an int
                    auto gotI2 = rpi0.getInt();
                    if ( gotI2 )
                    {
                        int i2{ *gotI2 };
                        // Send back 'J' and i + 1
                        i2 += 1;
                        rpi0.putMsgType( 'J' );
                        rpi0.put( i2 );                        
                    }
                    else
                    {
                        int i2{ 666666 };
                        // Send back 'X' and this float
                        rpi0.putMsgType( 'X' );
                        rpi0.put( i2 );                        
                    }
                    break;
                }
                case 'N':
                case 'n':
                    uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, '\r' );
                    uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, '\n' );
                    break;

                case 0:
                    // do nothing
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
