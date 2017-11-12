/*
    I2c.cpp - An I2C master library for CARRTv3.
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



#include "I2c.h"

#include <cstdint>
#include <cstring>
#include <iostream>
#include <cstring>

#include <unistd.h>                     //Needed for I2C port
#include <fcntl.h>                      //Needed for I2C port
#include <sys/ioctl.h>                  //Needed for I2C port
#include <linux/i2c-dev.h>              //Needed for I2C port

using namespace std;


namespace I2c
{

    const char* sI2cDeviceName = "/dev/i2c-1";

    int openI2cConnection( uint8_t address );

    void closeI2cConnection( int fd );

};




int I2c::openI2cConnection( uint8_t address )
{
    int fd = open( sI2cDeviceName, O_RDWR );

    if ( fd < 0 )
    {
        printf( "Failed to open the i2c bus\n" );
//        std::cerr << "Failed to open the i2c bus" << std::endl;
        return -1;
    }

    int err = ioctl( fd, I2C_SLAVE, address );
    if ( err < 0 )
    {
        printf( "Failed to acquire bus access and/or talk to slave\n" );
//        std::cerr << "Failed to acquire bus access and/or talk to slave" << std::endl;
        close ( fd );
        return -1;
    }

    return fd;
}



void I2c::closeI2cConnection( int fd )
{
    close( fd );
}





int I2c::write( uint8_t address, uint8_t registerAddress )
{
    int fd = openI2cConnection( address );
    if ( fd < 0 )
    {
        return -1;
    }

    int ret = i2c_smbus_write_byte( fd, registerAddress );
    closeI2cConnection( fd );

    return ( ret < 0 ? -1 : 0 );
}



int I2c::write( uint8_t address, uint8_t registerAddress, uint8_t data )
{
    int fd = openI2cConnection( address );
    if ( fd < 0 )
    {
        return -1;
    }

    int ret = i2c_smbus_write_byte_data( fd, registerAddress, data );
    closeI2cConnection( fd );

    return ( ret < 0 ? -1 : 0 );
}



int I2c::write( uint8_t address, uint8_t registerAddress, uint16_t data )
{
    int fd = openI2cConnection( address );
    if ( fd < 0 )
    {
        return -1;
    }

    int ret = i2c_smbus_write_word_data( fd, registerAddress, data );
    closeI2cConnection( fd );

    return ( ret < 0 ? -1 : 0 );
}



int I2c::write( uint8_t address, uint8_t registerAddress, const char* data )
{
    int fd = openI2cConnection( address );
    if ( fd < 0 )
    {
        return -1;
    }

    int len = strlen( data );
    if ( len > 32 )
    {
        // Intentionally fail (partially) silently
        len = 32;
    }

    int ret = i2c_smbus_write_i2c_block_data( fd, registerAddress, len, reinterpret_cast<const uint8_t*>( data ) );
    closeI2cConnection( fd );

    return ( ret < 0 ? -1 : 0 );
}


int I2c::write( uint8_t address, uint8_t registerAddress, uint8_t* data, uint8_t numberBytes )
{
    int fd = openI2cConnection( address );
    if ( fd < 0 )
    {
        return -1;
    }

    if ( numberBytes > 32 )
    {
        // Intentionally fail (partially) silently
        numberBytes = 32;
    }

    int ret = i2c_smbus_write_i2c_block_data( fd, registerAddress, numberBytes, data );
    closeI2cConnection( fd );

    return ( ret < 0 ? -1 : 0 );
}


int I2c::read( uint8_t address, uint8_t registerAddress, uint8_t* value )
{
    int fd = openI2cConnection( address );
    if ( fd < 0 )
    {
        return -1;
    }

    int ret = i2c_smbus_read_byte_data( fd, registerAddress );
    if ( ret >=0 )
    {
        *value = static_cast<uint8_t>( ret );
    }
    closeI2cConnection( fd );

    return ( ret < 0 ? -1 : 0 );
}



int I2c::read( uint8_t address, uint8_t registerAddress, uint16_t* value )
{
    int fd = openI2cConnection( address );
    if ( fd < 0 )
    {
        return -1;
    }

    int ret = i2c_smbus_read_word_data( fd, registerAddress );
    if ( ret >= 0 )
    {
        *value = static_cast<uint16_t>( ret );
    }
    closeI2cConnection( fd );

    return ( ret < 0 ? -1 : 0 );
}


int I2c::read( uint8_t address, uint8_t registerAddress, uint8_t numberBytes, uint8_t* destination )
{
    int fd = openI2cConnection( address );
    if ( fd < 0 )
    {
        return -1;
    }

    int ret = i2c_smbus_read_i2c_block_data( fd, registerAddress, numberBytes, destination );
    closeI2cConnection( fd );

    return ( ret < 0 ? -1 : 0 );
}






