#include <iostream>
#include <stdexcept>
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"

#include "../carrt/CarrtPicoDefines.h"
#include "shared/CarrtError.h"



bi_decl( bi_1pin_with_name( SIGNALING_LED, "On-board LED for blinking" ) );
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

        std::cout << "Hello, this is CARRT Pico!" << std::endl;

        gpio_init( SIGNALING_LED );
        gpio_set_dir( SIGNALING_LED, GPIO_OUT );

        while ( true ) 
        {
            try 
            {
                x = 0;
                {
                    ++x;
                    gpio_put( SIGNALING_LED, 1 );
                    sleep_ms( 250 );
                    gpio_put( SIGNALING_LED, 0 );
                    sleep_ms( 500 );
                    std::cout << "Hello, world! Light is about to flash on." << std::endl;
                    sleep_ms( 500 );

                    if ( x == 20 )
                    {
                        throw CarrtError( 666, "Test error" );
                    }
                }
            }

            catch( const CarrtError& e )
            {
                std::cerr << "Error " << e.errorCode() << " " << e.what() << std::endl;
            }
        }
    
        return 0;
    }

    catch( const std::exception& e )
    {
        std::cerr << "Error (non-CARRT) " << e.what() << std::endl;
    }
}
