/*
    Encoders.cpp - Track interrupts from the wheel encoders.

    Copyright (c) 2025 Igor Mikolic-Torreira.  All right reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "Encoders.h"

#include "hardware/gpio.h"

#include <cstdint>

#include "CarrtPicoDefines.h"
#include "Clock.h"
#include "EventManager.h"





/***************************************************/

// All this code intended to run on Core 1

/***************************************************/




namespace Encoders
{
    void configureEncoderGpio( uint gpio, gpio_irq_callback_t callback ) noexcept;
};




void callbackLeftEncoder( uint, std::uint32_t events )
{
    // This function designed/configured to be called and run on Core1
    static std::uint32_t lastInterrupt = 0;

    std::uint32_t tick = Clock::millis();
    if ( tick - lastInterrupt < PICO_GPIO_DEBOUNCE_TIME )
    {
        // Debounce
        return;
    }

    lastInterrupt = tick;

    int direction = 0;
    if ( events & GPIO_IRQ_EDGE_FALL )
    {
        direction = -1;
    }
    if ( events & GPIO_IRQ_EDGE_RISE )
    {
        direction = 1;;
    }

    Events().queueEvent( EvtId::kEncoderLeftEvent, direction, tick );

    return;
}



void callbackRightEncoder( uint, std::uint32_t events )
{
    // This function designed/configured to be called and run on Core1
    static std::uint32_t lastInterrupt = 0;

    std::uint32_t tick = Clock::millis();
    if ( tick - lastInterrupt < PICO_GPIO_DEBOUNCE_TIME )
    {
        // Debounce
        return;
    }

    lastInterrupt = tick;

    int direction = 0;
    if ( events & GPIO_IRQ_EDGE_FALL )
    {
        direction = -1;
    }
    if ( events & GPIO_IRQ_EDGE_RISE )
    {
        direction = 1;
    }

    Events().queueEvent( EvtId::kEncoderRightEvent, direction, tick );
    
    return;
}



void Encoders::initEncoders() noexcept
{
    // Call this from the core that will receive the interrupts
    configureEncoderGpio( CARRTPICO_ENCODER_LEFT_GPIO, callbackLeftEncoder ); 
    configureEncoderGpio( CARRTPICO_ENCODER_RIGHT_GPIO, callbackRightEncoder ); 
}



void Encoders::configureEncoderGpio( uint pin, gpio_irq_callback_t callBack ) noexcept
{
    // Configure interrupt on "pin" with "callback"
    gpio_init( pin );
    gpio_set_dir( pin, GPIO_IN );
    gpio_pull_down( pin );
    gpio_set_irq_enabled_with_callback( pin, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, 
        true, callBack );
}
