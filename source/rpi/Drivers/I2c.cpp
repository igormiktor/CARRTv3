/*
    I2c.cpp - An I2C master library for CARRTv3.
    Copyright (c) 2019 Igor Mikolic-Torreira.  All right reserved.

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

#include <pigpio.h>

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
    int fd = i2cOpen( 1, address, 0 );

    if ( fd < 0 )
    {
        throw I2cError( kI2cError + 1*kFunctionIdIncrement + fd, "Failed to open the i2c bus" );
    }

    mFd = static_cast<unsigned>( fd );
}



I2c::I2cConnection::~I2cConnection()
{
    if ( mFd >= 0 )
    {
        i2cClose( mFd );
    }
}





void I2c::write( uint8_t address, uint8_t registerAddress )
{
    I2cConnection i2c( address );

    int ret = i2cWriteByte( i2c.getFd(), registerAddress );

    if ( ret < 0 )
    {
        throw I2cError( kI2cError + 2*kFunctionIdIncrement + ret, "Error writing to i2c bus" );
    }
}



void I2c::write( uint8_t address, uint8_t registerAddress, uint8_t data )
{
    I2cConnection i2c( address );

    int ret = i2cWriteByteData( i2c.getFd(), registerAddress, data );

    if ( ret < 0 )
    {
        throw I2cError( kI2cError + 3*kFunctionIdIncrement + ret, "Error writing to i2c bus" );
    }
}



void I2c::write( uint8_t address, uint8_t registerAddress, uint16_t data )
{
    I2cConnection i2c( address );

    int ret = i2cWriteWordData( i2c.getFd(), registerAddress, data );

    if ( ret < 0 )
    {
        throw I2cError( kI2cError + 4*kFunctionIdIncrement + ret, "Error writing to i2c bus" );
    }
}



void I2c::write( uint8_t address, uint8_t registerAddress, const char* data )
{
    int len = strlen( data );
    if ( len > 32 )
    {
        // Intentionally fail (partially) silently
        len = 32;
    }

    I2cConnection i2c( address );

    int ret = i2cWriteBlockData( i2c.getFd(), registerAddress, const_cast<char*>( data ), len );

    if ( ret < 0 )
    {
        throw I2cError( kI2cError + 5*kFunctionIdIncrement + ret, "Error writing to i2c bus" );
    }
}


void I2c::write( uint8_t address, uint8_t registerAddress, uint8_t* data, uint8_t numberBytes )
{
    if ( numberBytes > 32 )
    {
        // Intentionally fail (partially) silently
        numberBytes = 32;
    }

    I2cConnection i2c( address );

    int ret = i2cWriteI2CBlockData( i2c.getFd(), registerAddress, reinterpret_cast<char*>( data ), numberBytes );

    if ( ret < 0 )
    {
        throw I2cError( kI2cError + 6*kFunctionIdIncrement + ret, "Error writing to i2c bus" );
    }
}


void I2c::read( uint8_t address, uint8_t registerAddress, uint8_t* value )
{
    I2cConnection i2c( address );

    int ret = i2cReadByteData( i2c.getFd(), registerAddress );

    if ( ret < 0 )
    {
        throw I2cError( kI2cError + 7*kFunctionIdIncrement + ret, "Error reading from i2c bus" );
    }

    *value = static_cast<uint8_t>( ret );
}



void I2c::read( uint8_t address, uint8_t registerAddress, uint16_t* value )
{
    I2cConnection i2c( address );

    int ret = i2cReadWordData( i2c.getFd(), registerAddress );

    if ( ret < 0 )
    {
        throw I2cError( kI2cError + 8*kFunctionIdIncrement + ret, "Error reading from i2c bus" );
    }

    *value = static_cast<uint16_t>( ret );
}


int I2c::read( uint8_t address, uint8_t registerAddress, uint8_t numberBytes, uint8_t* destination )
{
    I2cConnection i2c( address );

    int ret = i2cReadI2CBlockData( i2c.getFd(), registerAddress, reinterpret_cast<char*>( destination ), numberBytes );

    if ( ret < 0 || ret > numberBytes )
    {
        throw I2cError( kI2cError + 9*kFunctionIdIncrement + ret, "Error reading from i2c bus" );
    }

    return ret;
}
