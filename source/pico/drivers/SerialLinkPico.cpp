/*
    SerialLink.cpp - A serial link for CARRT-Pico. This is the Pico implementation.
    (Implementation for RPiO in the RPi0 source directory)

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


#include "SerialLinkPico.h"

#include "CarrtPicoDefines.h"

#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"




SerialLinkPico& serialLink()
{
    // Not constructed until first time this function is called
    static SerialLinkPico  sTheSerialLink;

    return sTheSerialLink;
}








SerialLinkPico::SerialLinkPico() noexcept
{
    // Initialise UART for the Serial-Link
    uart_init( CARRTPICO_SERIAL_LINK_UART, CARRTPICO_SERIAL_LINK_UART_BAUD_RATE );
    uart_set_translate_crlf( CARRTPICO_SERIAL_LINK_UART, false );

    // Set the GPIO pin mux to the UART
    gpio_set_function( CARRTPICO_SERIAL_LINK_UART_TX_PIN, GPIO_FUNC_UART );
    gpio_set_function( CARRTPICO_SERIAL_LINK_UART_RX_PIN, GPIO_FUNC_UART );
}



SerialLinkPico::~SerialLinkPico() noexcept
{
    // Shutdown the Serial-Link UART
    gpio_set_function( CARRTPICO_SERIAL_LINK_UART_TX_PIN, GPIO_FUNC_NULL );
    gpio_set_function( CARRTPICO_SERIAL_LINK_UART_RX_PIN, GPIO_FUNC_NULL );
    uart_deinit( CARRTPICO_SERIAL_LINK_UART );
}



bool SerialLinkPico::isReadable() noexcept
{
    return uart_is_readable( CARRTPICO_SERIAL_LINK_UART );
}



std::optional<std::uint8_t> SerialLinkPico::getMsgType()
{
    // Always blocks, so make semantics the same by first checking if 
    // there is data to read
    if ( isReadable() )
    {
        return static_cast<uint8_t>( uart_getc( CARRTPICO_SERIAL_LINK_UART ) );
    }
    else
    {
        return std::nullopt;
    }
}



std::optional<std::uint8_t> SerialLinkPico::getByte()
{
    // Always blocks, so make semantics the same by first checking if 
    // there is data to read
    if ( isReadable() )
    {
        return static_cast<uint8_t>( uart_getc( CARRTPICO_SERIAL_LINK_UART ) );
    }
    else
    {
        return std::nullopt;
    }
}



std::optional<std::uint32_t> SerialLinkPico::get4Bytes()
{
    // Always blocks, so make semantics the same by first checking if 
    // there is data to read
    if ( isReadable() )
    {   
        RawData r;
//        uart_read_blocking( CARRTPICO_SERIAL_LINK_UART, reinterpret_cast<std::uint8_t*>( &t.c ), 4 );
        uart_read_blocking( CARRTPICO_SERIAL_LINK_UART, r.c(), 4 );
        return r.u();
    }
    else
    {
        return std::nullopt;
    }
}



bool SerialLinkPico::get4Bytes( std::uint8_t c[4] )
{
    // Always blocks, so make semantics the same by first checking if 
    // there is data to read
    if ( isReadable() )
    {   
        uart_read_blocking( CARRTPICO_SERIAL_LINK_UART, c, 4 );
        return true;
    }
    else
    {
        return false;
    }
}



void SerialLinkPico::putByte( std::uint8_t c )
{
    uart_putc_raw( CARRTPICO_SERIAL_LINK_UART, static_cast<char>( c ) );
}



void SerialLinkPico::put4Bytes( const std::uint8_t c[4] )
{
    uart_write_blocking( CARRTPICO_SERIAL_LINK_UART, c, 4 );
}



int SerialLinkPico::getBytes( int nbr, std::uint8_t* buffer )
{
    uart_read_blocking( CARRTPICO_SERIAL_LINK_UART, buffer, nbr );
    return nbr;
}



int SerialLinkPico::putBytes( int nbr, const std::uint8_t* buffer )
{
    uart_write_blocking( CARRTPICO_SERIAL_LINK_UART, buffer, nbr );
    return nbr;
}

