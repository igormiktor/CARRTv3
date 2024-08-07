#include <iostream>
#include <stdexcept>
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#include "hardware/sync.h"
#include "hardware/watchdog.h"


#include "../carrt/CarrtPicoDefines.h"
#include "shared/CarrtError.h"



bi_decl( bi_1pin_with_name( CARRTPICO_HEARTBEAT_LED, "On-board LED for blinking" ) );
bi_decl( bi_2pins_with_names( 0, "uart0 TX", 1, "uart0 RX" ) );
bi_decl( bi_2pins_with_names( CARRTPICO_I2C_SDA, "i2c0 SDA", CARRTPICO_I2C_SCL, "i2c0 SCL" ) );




int64_t alarm_callback( alarm_id_t id, void *user_data ) 
{
    // Put your timeout handler code in here
    return 0;
}

volatile int x = 0;

int main()
{
    stdio_init_all();

    std::cout << "Hello, this is CARRT Pico!" << std::endl;
    std::cout << "Size of signed char: " << sizeof( signed char ) << std::endl;
    std::cout << "Size of signed short int: " << sizeof( signed short int ) << std::endl;
    std::cout << "Size of signed int: " << sizeof( signed int ) << std::endl;
    std::cout << "Size of signed long int: " << sizeof( signed long int ) << std::endl;
    std::cout << "Size of signed long long int: " << sizeof( signed long long int ) << std::endl;
    std::cout << "Size of float: " << sizeof( float ) << std::endl;
    std::cout << "Size of double: " << sizeof( double ) << std::endl;


    try
    {
        // I2C Initialisation. Using it at 400Khz.
        i2c_init( CARRTPICO_I2C_PORT, CARRTPICO_I2C_SPEED );
        
        gpio_set_function( CARRTPICO_I2C_SDA, GPIO_FUNC_I2C );
        gpio_set_function( CARRTPICO_I2C_SCL, GPIO_FUNC_I2C );
        gpio_pull_up( CARRTPICO_I2C_SDA );
        gpio_pull_up( CARRTPICO_I2C_SCL );

        // Timer example code - This example fires off the callback after 2000ms
        add_alarm_in_ms( 2000, alarm_callback, NULL, false );

        gpio_init( CARRTPICO_HEARTBEAT_LED );
        gpio_set_dir( CARRTPICO_HEARTBEAT_LED, GPIO_OUT );

        while ( true ) 
        {
            try 
            {
                std::cerr << "At top of try block" << std::endl;
                x = 0;
                while ( true )
                {
                    ++x;
                    gpio_put( CARRTPICO_HEARTBEAT_LED, 1 );
                    sleep_ms( 250 );
                    gpio_put( CARRTPICO_HEARTBEAT_LED, 0 );
                    sleep_ms( 500 );
                    std::cout << "Hello, world! Light is about to flash on.  " << x << std::endl;
                    sleep_ms( 500 );

                    if ( x == 10 )
                    {
                        throw CarrtError( 666, "Test error" );
                    }
                }
            }

            catch( const CarrtError& e )
            {
                std::cerr << "Error " << e.errorCode() << " " << e.what() << std::endl;
                std::cerr << "Now rebooting Pico" << std::endl;
                sleep_ms( 500 );

                watchdog_reboot(0, SRAM_END, 0); 
#if 0
                while ( 1 )
                {
                    __wfi();
                }
#endif 
            }
        }
    
        return 0;
    }

    catch( const std::exception& e )
    {
        std::cerr << "Error (non-CARRT) " << e.what() << std::endl;
    }
}
