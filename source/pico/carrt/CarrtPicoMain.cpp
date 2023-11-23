#include <iostream>
#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"



// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

bool timerCallback( repeating_timer_t *rt ) 
{
    static uint8_t eighthSecCount = 0;

    ++eighthSecCount;
    eighthSecCount %= 64;

    // Queue nav update events every 1/8 second
    // Event parameter counts eighth seconds ( 0, 1, 2, 3, 4, 5, 6, 7 )
    // EventManager::queueEvent( EventManager::kNavUpdateEvent, eighthSecCount % 8, EventManager::kHighPriority );

    if ( ( eighthSecCount % 2 ) == 0 )
    {
        // Event parameter counts quarter seconds ( 0, 1, 2, 3 )
        // EventManager::queueEvent( EventManager::kQuarterSecondTimerEvent, (eighthSecCount % 8) / 2 );
    }

    if ( ( eighthSecCount % 8 ) == 0 )
    {
        // Event parameter counts seconds to 8 ( 0, 1, 2, 3, 4, 5, 6, 7 )
        // EventManager::queueEvent( EventManager::kOneSecondTimerEvent, ( eighthSecCount / 8 ) );
    }

    if ( eighthSecCount == 0 )
    {
        // EventManager::queueEvent( EventManager::kEightSecondTimerEvent, 0 );
    }

    return true;
}



int main()

{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);


    repeating_timer_t timer;
    // Repeating 1/8 sec timer; negative timeout means exact delay between triggers
    if ( !add_repeating_timer_us( -125, timerCallback, NULL, &timer ) ) 
    {
        std::cout << "Failed to add timer\n" << std::endl;
        return 1;
    }

    // puts("This is CARRT Pico!");
    std::cout << "This is CARRT Pico" << std::endl;

    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init( LED_PIN );
    gpio_set_dir( LED_PIN, GPIO_OUT );

    while ( true ) 
    {
        gpio_put( LED_PIN, 1 );
        sleep_ms( 250 );
        gpio_put( LED_PIN, 0 );
        sleep_ms( 500 );
        std::cout << "Hello from CARRT Pico! LED is about to flash on." << std::endl;
        sleep_ms(500);
    }

    return 0;
}
