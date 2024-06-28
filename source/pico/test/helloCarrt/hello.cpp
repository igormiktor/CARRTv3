#include <iostream>
#include <stdexcept>
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"

#include "shared/CarrtError.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

bi_decl( bi_1pin_with_name( PICO_DEFAULT_LED_PIN, "On-board LED for blinking" ) );
bi_decl( bi_2pins_with_names( 0, "uart0 TX", 1, "uart0 RX" ) );
bi_decl( bi_2pins_with_names( I2C_SDA, "i2c0 SDA", I2C_SCL, "i2c0 SCL" ) );




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
        // SPI initialisation. This example will use SPI at 1MHz.
        spi_init( SPI_PORT, 1000*1000 );
        gpio_set_function( PIN_MISO, GPIO_FUNC_SPI );
        gpio_set_function( PIN_CS,   GPIO_FUNC_SIO );
        gpio_set_function( PIN_SCK,  GPIO_FUNC_SPI );
        gpio_set_function( PIN_MOSI, GPIO_FUNC_SPI );
        
        // Chip select is active-low, so we'll initialise it to a driven-high state
        gpio_set_dir( PIN_CS, GPIO_OUT );
        gpio_put( PIN_CS, 1 );
        

        // I2C Initialisation. Using it at 400Khz.
        i2c_init( I2C_PORT, 400*1000 );
        
        gpio_set_function( I2C_SDA, GPIO_FUNC_I2C );
        gpio_set_function( I2C_SCL, GPIO_FUNC_I2C );
        gpio_pull_up( I2C_SDA );
        gpio_pull_up( I2C_SCL );

        // Timer example code - This example fires off the callback after 2000ms
        add_alarm_in_ms( 2000, alarm_callback, NULL, false );

        std::cout << "Hello, this is CARRT Pico!" << std::endl;

        const uint LED_PIN = PICO_DEFAULT_LED_PIN;
        gpio_init( LED_PIN );
        gpio_set_dir( LED_PIN, GPIO_OUT );

        while ( true ) 
        {
            try 
            {
                x = 0;
                {
                    ++x;
                    gpio_put( LED_PIN, 1 );
                    sleep_ms( 250 );
                    gpio_put( LED_PIN, 0 );
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
