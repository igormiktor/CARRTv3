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

#include <hardware/gpio.h>

#include <cstdint>

#include "CarrtPicoDefines.h"
#include "Clock.h"
#include "EventManager.h"

/***************************************************/

// All this code intended to run on Core 1

/***************************************************/

namespace Encoders
{
    void configureEncoderGpio( uint pin ) noexcept;
    void encoderCallBack( uint, std::uint32_t events );
}    // namespace Encoders

/*!
 * \brief Initializes the encoder GPIO with interrupts for count
 *
 * This function is designed/configured to be called from and run on Core1
 *
 */
void Encoders::initEncoders() noexcept
{
    // Set the call back function (it's a generic/universal callback on Pico)
    gpio_set_irq_callback( Encoders::encoderCallBack );

    // Configure each of relevant pins
    configureEncoderGpio( CARRTPICO_ENCODER_LEFT_GPIO );
    configureEncoderGpio( CARRTPICO_ENCODER_RIGHT_GPIO );

    irq_set_enabled( IO_IRQ_BANK0, true );
}

/*!
 * \brief Configure encoder pins as inputs with pull down and enables interrupts
 *
 * This function is designed/configured to be called from and run on Core1
 *
 * \param pin (GPIO pin number)
 */
void Encoders::configureEncoderGpio( uint pin ) noexcept
{
    gpio_init( pin );
    gpio_set_dir( pin, GPIO_IN );
    gpio_pull_down( pin );
    gpio_set_irq_enabled( pin, ( GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE ), true );
}

/*!
 * \brief Called when either encoder pin triggers
 *
 * \param gpio (the GPIO pin that triggered the interrupt)
 * \param events (the interrupt event type)
 */
void Encoders::encoderCallBack( uint gpio, std::uint32_t events )
{
    constexpr int kLeft{ 0 };
    constexpr int kRight{ 1 };

    static std::uint32_t lastInterrupt[ 2 ]{ 0, 0 };
    static EvtId encoderEvt[ 2 ]{ EvtId::kEncoderLeftEvent, EvtId::kEncoderRightEvent };

    int side;
    if ( gpio == CARRTPICO_ENCODER_LEFT_GPIO )
    {
        side = kLeft;
    }
    else if ( gpio == CARRTPICO_ENCODER_RIGHT_GPIO )
    {
        side = kRight;
    }
    else
    {
        // Paranoid error -- only the encoder pins are configured to interrupt
        Events().queueEvent( EvtId::kErrorEvent, gpio );    // TODO argument should be error code
        return;
    }

    std::uint32_t tick = Clock::millis();
    if ( tick - lastInterrupt[ side ] < PICO_GPIO_DEBOUNCE_TIME )
    {
        // Debounce
        return;
    }

    lastInterrupt[ side ] = tick;

    int direction = 0;
    if ( events & GPIO_IRQ_EDGE_FALL )
    {
        direction = -1;
    }
    else if ( events & GPIO_IRQ_EDGE_RISE )
    {
        direction = 1;
    }

    Events().queueEvent( encoderEvt[ side ], direction, tick );

    return;
}
