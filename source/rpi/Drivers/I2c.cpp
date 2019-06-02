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

#include <unistd.h>                     //Needed for I2C port
#include <fcntl.h>                      //Needed for I2C port
#include <sys/ioctl.h>                  //Needed for I2C port
#include <linux/i2c-dev.h>              //Needed for I2C port

using namespace std;


namespace I2c
{

    const char* sI2cDeviceName = "/dev/i2c-1";

    class I2cConnection
    {
    public:
        I2cConnection(  uint8_t address );
        ~I2cConnection();

        int getFd() const
            { return mFd; }

    private:

        int mFd;
    };

};









I2c::I2cConnection::I2cConnection( uint8_t address )
{
    mFd = open( sI2cDeviceName, O_RDWR );

    if ( mFd < 0 )
    {
        throw I2cError( 100, "Failed to open the i2c bus" );
    }

    int err = ioctl( mFd, I2C_SLAVE, address );
    if ( err < 0 )
    {
        close ( mFd );
        mFd = -1;
        throw I2cError( 101, "Failed to acquire i2c bus access and/or talk to slave" );
    }
}



I2c::I2cConnection::~I2cConnection()
{
    if ( mFd >= 0 )
    {
        close( mFd );
    }
}





void I2c::write( uint8_t address, uint8_t registerAddress )
{
    I2cConnection i2c( address );

    int ret = i2c_smbus_write_byte( i2c.getFd(), registerAddress );

    if ( ret < 0 )
    {
        throw I2cError( 102, "Error writing to i2c bus" );
    }
}



void I2c::write( uint8_t address, uint8_t registerAddress, uint8_t data )
{
    I2cConnection i2c( address );

    int ret = i2c_smbus_write_byte_data( i2c.getFd(), registerAddress, data );

    if ( ret < 0 )
    {
        throw I2cError( 102, "Error writing to i2c bus" );
    }
}



void I2c::write( uint8_t address, uint8_t registerAddress, uint16_t data )
{
    I2cConnection i2c( address );

    int ret = i2c_smbus_write_word_data( i2c.getFd(), registerAddress, data );

    if ( ret < 0 )
    {
        throw I2cError( 102, "Error writing to i2c bus" );
    }
}



void I2c::write( uint8_t address, uint8_t registerAddress, const char* data )
{
    I2cConnection i2c( address );

    int len = strlen( data );
    if ( len > 32 )
    {
        // Intentionally fail (partially) silently
        len = 32;
    }

    int ret = i2c_smbus_write_i2c_block_data( i2c.getFd(), registerAddress, len, reinterpret_cast<const uint8_t*>( data ) );

    if ( ret < 0 )
    {
        throw I2cError( 102, "Error writing to i2c bus" );
    }
}


void I2c::write( uint8_t address, uint8_t registerAddress, uint8_t* data, uint8_t numberBytes )
{
    I2cConnection i2c( address );

    if ( numberBytes > 32 )
    {
        // Intentionally fail (partially) silently
        numberBytes = 32;
    }

    int ret = i2c_smbus_write_i2c_block_data( i2c.getFd(), registerAddress, numberBytes, data );

    if ( ret < 0 )
    {
        throw I2cError( 102, "Error writing to i2c bus" );
    }
}


void I2c::read( uint8_t address, uint8_t registerAddress, uint8_t* value )
{
    I2cConnection i2c( address );

    int ret = i2c_smbus_read_byte_data( i2c.getFd(), registerAddress );

    if ( ret < 0 )
    {
        throw I2cError( 102, "Error writing to i2c bus" );
    }

    *value = static_cast<uint8_t>( ret );
}



void I2c::read( uint8_t address, uint8_t registerAddress, uint16_t* value )
{
    I2cConnection i2c( address );

    int ret = i2c_smbus_read_word_data( i2c.getFd(), registerAddress );

    if ( ret < 0 )
    {
        throw I2cError( 102, "Error writing to i2c bus" );
    }

    *value = static_cast<uint16_t>( ret );
}


void I2c::read( uint8_t address, uint8_t registerAddress, uint8_t numberBytes, uint8_t* destination )
{
    I2cConnection i2c( address );

    int ret = i2c_smbus_read_i2c_block_data( i2c.getFd(), registerAddress, numberBytes, destination );

    if ( ret < 0 )
    {
        throw I2cError( 102, "Error writing to i2c bus" );
    }
}
