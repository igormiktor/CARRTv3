/*
    I2C.cpp - I2C functions for CARRT Pico.

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


#include "I2C.h"

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include <cstring>

#include "CarrtError.h"
#include "CarrtPicoDefines.h"







void I2C::initI2C() noexcept
{
    // I2C Initialisation
    i2c_init( CARRTPICO_I2C_PORT, CARRTPICO_I2C_SPEED );
    
    gpio_set_function( CARRTPICO_I2C_SDA, GPIO_FUNC_I2C ) ;
    gpio_set_function( CARRTPICO_I2C_SCL, GPIO_FUNC_I2C );
    gpio_pull_up( CARRTPICO_I2C_SDA );
    gpio_pull_up( CARRTPICO_I2C_SCL );
}



extern "C" signed char I2C::send( unsigned char address, unsigned char reg, unsigned char* data, unsigned char len ) noexcept
{
    constexpr int CARRT_BNO055_MAX_SENT_MSG_LEN = 18;   // Actual longest message sent to BNO055 is 18 bytes; need +1 for address

    if ( len > 18 )
    {
        return PICO_ERROR_GENERIC;
    }

    unsigned char array[ CARRT_BNO055_MAX_SENT_MSG_LEN + 1 ];
    array[0] = reg;
    memcpy( (array + 1), data, len );

    // Count the reg entry in array[0]
    ++len;

    int ret = i2c_write_blocking( CARRTPICO_I2C_PORT, address, array, len, false );

    if ( ret == len )
    {
        return 0;
    }
    else
    {
        return PICO_ERROR_GENERIC;
    }
}



extern "C" signed char I2C::receive( unsigned char address, unsigned char reg, unsigned char* data, unsigned char len ) noexcept
{
    // Write the register address without a STOP...
    int ret = i2c_write_blocking( CARRTPICO_I2C_PORT, address, &reg, 1, true );
    if ( ret == 1 )
    {
        // Then read the data...
        ret = i2c_read_blocking( CARRTPICO_I2C_PORT, address, data, len, false );
        if ( ret == len )
        {
            ret = 0;
        }
    }

    if ( ret )
    {
        return PICO_ERROR_GENERIC;
    }
    else
    {
        return 0;
    }
}


