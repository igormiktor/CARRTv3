/*
    I2c.h - An I2C master library
    For Raspberry Pi (secifically, Pi Zero W).

    Copyright (c) 2017 Igor Mikolic-Torreira.  All right reserved.

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


/*!
 * \file
 *
 * \brief This file provides functions that interface to the TWI (two-wire serial interface) hardware
 * of the Raspberry Pi, providing a high-level interface to
 * I2C protocol communications.  Include this file if you want your application will operate in Master mode
 * as defined in the I2C protocol.
 *
 */





#ifndef I2c_h
#define I2c_h


#include <cstdint>
#include <string>
#include <Utils/CarrtError.h>




namespace I2c
{

    class I2cError : public CarrtError
    {
    public:
        explicit I2cError( int errCode, const std::string& what )
            : CarrtError( errCode, what ) {}

        explicit I2cError( int errCode, const char* what )
            : CarrtError( errCode, what ) {}
    };


    void write( std::uint8_t address, std::uint8_t registerAddress );

    void write( std::uint8_t address, std::uint8_t registerAddress, std::uint8_t data );

    void write( std::uint8_t address, std::uint8_t registerAddress, std::uint16_t data );

    void write( std::uint8_t address, std::uint8_t registerAddress, const char* data );

    void write( std::uint8_t address, std::uint8_t registerAddress, std::uint8_t* data, std::uint8_t numberBytes );

    void read( std::uint8_t address, std::uint8_t registerAddress, std::uint8_t* destination );

    void read( std::uint8_t address, std::uint8_t registerAddress, std::uint16_t* destination );

    void read( std::uint8_t address, std::uint8_t registerAddress, std::uint8_t numberBytes, std::uint8_t* destination );

};


#endif
