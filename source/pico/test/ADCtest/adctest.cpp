#include <iostream>
#include <stdexcept>

#include "../carrt/CarrtPicoDefines.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/sync.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "shared/CarrtError.h"

bi_decl( bi_1pin_with_name( CARRTPICO_HEARTBEAT_LED, "On-board LED for blinking" ) );
bi_decl( bi_2pins_with_names( 0, "uart0 TX", 1, "uart0 RX" ) );
bi_decl( bi_2pins_with_names( CARRTPICO_I2C_SDA, "i2c0 SDA", CARRTPICO_I2C_SCL, "i2c0 SCL" ) );

int main()
{
    stdio_init_all();

    std::cout << "This is CARRT Pico test of ADC!" << std::endl;
    std::cout << "Size of signed char: " << sizeof( signed char ) << std::endl;
    std::cout << "Size of signed short int: " << sizeof( signed short int ) << std::endl;
    std::cout << "Size of signed int: " << sizeof( signed int ) << std::endl;
    std::cout << "Size of signed long int: " << sizeof( signed long int ) << std::endl;
    std::cout << "Size of signed long long int: " << sizeof( signed long long int ) << std::endl;
    std::cout << "Size of float: " << sizeof( float ) << std::endl;
    std::cout << "Size of double: " << sizeof( double ) << std::endl;

    try
    {
        adc_init();
        adc_gpio_init( 26 );
        adc_select_input( 0 );

        gpio_init( CARRTPICO_HEARTBEAT_LED );
        gpio_set_dir( CARRTPICO_HEARTBEAT_LED, GPIO_OUT );

        while ( true )
        {
            try
            {
                while ( true )
                {
                    gpio_put( CARRTPICO_HEARTBEAT_LED, 1 );
                    sleep_ms( 500 );
                    gpio_put( CARRTPICO_HEARTBEAT_LED, 0 );
                    sleep_ms( 500 );
                    const float conversionFactor = 3.3f / ( 1 << 12 );
                    std::uint16_t result = adc_read();
                    float vout = result * conversionFactor;
                    const float voltDividerFactor = ( 38.64 + 67.4 ) / 67.4;
                    float vin = vout * voltDividerFactor;
                    std::cout << "Raw value: " << result << "  V read: " << vout << " V original "
                              << vin << std::endl;
                }
            }

            catch ( const CarrtError& e )
            {
                std::cerr << "Error " << e.errorCode() << " " << e.what() << std::endl;
                std::cerr << "Now rebooting Pico" << std::endl;
                sleep_ms( 500 );

                watchdog_reboot( 0, SRAM_END, 0 );
            }
        }

        return 0;
    }

    catch ( const std::exception& e )
    {
        std::cerr << "Error (non-CARRT) " << e.what() << std::endl;
    }
}
