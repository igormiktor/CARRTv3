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
#include <stdexcept>

#include <unistd.h>			//Needed for I2C port
#include <fcntl.h>			//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port

using namespace std;


namespace I2c
{

    const char* sI2cDeviceName = "/dev/i2c-1";

    // Utility class

    class I2cConnection
    {
    public:
        I2cConnection( uint8_t address );
        ~I2cConnection();
        int fd() const { return mI2cFd; }

    private:
        uint8_t mI2cFd;
    };


    I2cConnection::I2cConnection( uint8_t address )
    {
        mI2cFd = open( sI2cDeviceName, O_RDWR );

        if ( mI2cFd < 0 )
        {
            //ERROR HANDLING: you can check errno to see what went wrong
            printf( "Failed to open the i2c bus\n" );
            throw I2cError( "i2c open err" );;
        }

        int err = ioctl( mI2cFd, I2C_SLAVE, address );
        if ( err < 0 )
        {
            printf( "Failed to acquire bus access and/or talk to slave\n" );
            //ERROR HANDLING; you can check errno to see what went wrong
            throw I2cError( "i2c slave err" );
        }

    }


    I2cConnection::~I2cConnection()
    {
        close( mI2cFd );
    }


};






int I2c::write( uint8_t address, uint8_t registerAddress )
{
    I2cConnection i2c( address );

    return i2c_smbus_write_byte( i2c.fd(), registerAddress );
}



int I2c::write( uint8_t address, uint8_t registerAddress, uint8_t data )
{
    I2cConnection i2c( address );

    return i2c_smbus_write_byte_data( i2c.fd(), registerAddress, data );
}



int I2c::write( uint8_t address, uint8_t registerAddress, uint16_t data )
{
    I2cConnection i2c( address );

    return i2c_smbus_write_word_data( i2c.fd(), registerAddress, data );
}



int I2c::write( uint8_t address, uint8_t registerAddress, const char* data )
{
    I2cConnection i2c( address );

    int len = strlen( data );
    if ( len > 32 )
    {
        printf( "i2c message longer than 32 bytes\n" );
        throw I2cError( "i2c long msg" );
    }
    else
    {
        return i2c_smbus_write_i2c_block_data( i2c.fd(), registerAddress, len, reinterpret_cast<const uint8_t*>( data ) );
    }
}


int I2c::write( uint8_t address, uint8_t registerAddress, uint8_t* data, uint8_t numberBytes )
{
    I2cConnection i2c( address );

    if ( numberBytes > 32 )
    {
        printf( "i2c message longer than 32 bytes" );
        throw I2cError( "i2c long msg" );
    }
    else
    {
        return i2c_smbus_write_i2c_block_data( i2c.fd(), registerAddress, numberBytes, data );
    }
}


int I2c::read( uint8_t address, uint8_t registerAddress, uint8_t* value )
{
    I2cConnection i2c( address );

    int ret = i2c_smbus_read_byte_data( i2c.fd(), registerAddress );
    if ( ret < 0 )
    {
        return -1;
    }
    else
    {
        *value = ret;
        return 0;
    }
}



int I2c::read( uint8_t address, uint8_t registerAddress, uint16_t* value )
{
    I2cConnection i2c( address );

    int ret = i2c_smbus_read_word_data( i2c.fd(), registerAddress );
    if ( ret < 0 )
    {
        return -1;
    }
    else
    {
        *value = ret;
        return 0;
    }
}


int I2c::read( uint8_t address, uint8_t registerAddress, uint8_t numberBytes, uint8_t* destination )
{
    I2cConnection i2c( address );

    int ret = i2c_smbus_read_i2c_block_data( i2c.fd(), registerAddress, numberBytes, destination );
    return ( ret < 0 ? -1 : 0 );
}






