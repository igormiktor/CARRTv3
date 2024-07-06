/*
    SerialLink.cpp - A serial link for CARRT-Pico. This is the Pico implementation.
    (Implementation for RPiO in the RPi0 source directory)

    Copyright (c) 2024 Igor Mikolic-Torreira.  All right reserved.

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



#include "shared/SerialLink.h"
#include "CarrtPicoDefines.h"


#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"



void SerialLink::openSerialLink()
{
    // Initialise UART for the Serial-Link
    uart_init( CARRTPICO_SERIAL_LINK_UART, CARRTPICO_SERIAL_LINK_UART_BAUD_RATE );
    uart_set_translate_crlf( CARRTPICO_SERIAL_LINK_UART, false );

    // Set the GPIO pin mux to the UART
    gpio_set_function( CARRTPICO_SERIAL_LINK_UART_TX_PIN, GPIO_FUNC_UART );
    gpio_set_function( CARRTPICO_SERIAL_LINK_UART_RX_PIN, GPIO_FUNC_UART );
}



void SerialLink::closeSerialLink()
{
    // Shutdown the Serial-Link UART
    gpio_set_function( CARRTPICO_SERIAL_LINK_UART_TX_PIN, GPIO_FUNC_NULL );
    gpio_set_function( CARRTPICO_SERIAL_LINK_UART_RX_PIN, GPIO_FUNC_NULL );
    uart_deinit( CARRTPICO_SERIAL_LINK_UART );
}



bool SerialLink::isReadable()
{
    return uart_is_readable( CARRTPICO_SERIAL_LINK_UART );
}



uint8_t SerialLink::getByte()
{
    return static_cast<uint8_t>( uart_getc( CARRTPICO_SERIAL_LINK_UART ) );
}



void SerialLink::get4Bytes( uint8_t* c )
{
    for ( int i = 0; i < 4; ++i, ++c )
    {
        *c = static_cast<uint8_t>( uart_getc( CARRTPICO_SERIAL_LINK_UART ) );
    }
    return;
}



void SerialLink::putByte( uint8_t c )
{
    uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, c );
}



void SerialLink::put4Bytes( uint8_t* c )
{
    for ( int i = 0; i < 4; ++i, ++c )
    {
        uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, *c );
    }
}




