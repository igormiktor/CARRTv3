/*
    I2C.cpp - I2C functions for CARRT Pico.

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


#include "I2C.h"

#include "CarrtPicoDefines.h"

#include "shared/CarrtError.h"

#include "pico/stdlib.h"
#include "hardware/i2c.h"


namespace I2C
{

    void initI2C();
    
    signed char send( unsigned char address, unsigned char reg, unsigned char* data, unsigned char len );

    signed char receive( unsigned char address, unsigned char reg, unsigned char* data, unsigned char len );

};





void I2C::initI2C()
{
    // I2C Initialisation
    i2c_init( CARRTPICO_I2C_PORT, CARRTPICO_I2C_SPEED );
    
    gpio_set_function( CARRTPICO_I2C_SDA, GPIO_FUNC_I2C ) ;
    gpio_set_function( CARRTPICO_I2C_SCL, GPIO_FUNC_I2C );
    gpio_pull_up( CARRTPICO_I2C_SDA );
    gpio_pull_up( CARRTPICO_I2C_SCL );
}



signed char I2C::send( unsigned char address, unsigned char reg, unsigned char* data, unsigned char len )
{
    // Write the register address without STOP...
    int ret = i2c_write_blocking( CARRTPICO_I2C_PORT, CARRT_BNO0555_I2C_ADDR, &reg, 1, true );
    if ( ret == 1 )
    {
        // Then write the data
        ret = i2c_write_blocking( CARRTPICO_I2C_PORT, CARRT_BNO0555_I2C_ADDR, data, len, false );
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



signed char I2C::receive( unsigned char address, unsigned char reg, unsigned char* data, unsigned char len )
{
    // Write the register address without a STOP...
    int ret = i2c_write_blocking( CARRTPICO_I2C_PORT, CARRT_BNO0555_I2C_ADDR, &reg, 1, true );
    if ( ret == 1 )
    {
        // Then read the data...
        ret = i2c_read_blocking( CARRTPICO_I2C_PORT, CARRT_BNO0555_I2C_ADDR, data, len, false );
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


