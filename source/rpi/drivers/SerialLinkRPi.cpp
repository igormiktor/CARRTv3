/*
    SerialLink.cpp - Tools for CARRT3 communications over serial (UART) link
    between the RPI and Pico.  This is the RPi implementation of the SerialLink.

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


#include "SerialLinkRPi.h"

#include <iostream>
#include <sstream>
#include <string>

#include <cstring>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include <CarrtError.h>
#include <Clock.h>
#include <DebugUtils.hpp>




SerialLinkRPi::SerialLinkRPi()
{
    // Open the serial port. 
    // Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
    mSerialPort = open( "/dev/serial0", O_RDWR );
    if ( !mSerialPort )
    {
        debugM( "Serial port open error" );
        char *errMsg = std::strerror( mSerialPort );
        debugV( mSerialPort, errMsg );
        throw CarrtError( makeRpi0ErrorId( kRpi0SerialError, 1, mSerialPort ), std::string( errMsg ) );
    }
    else
    {
        debugM( "Serial port opened successfully" );
        debugV( mSerialPort );
    }


    // Create new termios struct, we call it 'tty' for convention
    struct termios tty;

    // Read in existing settings, and handle any error
    if( tcgetattr( mSerialPort, &tty ) != 0 ) 
    {
        char* errMsg = std::strerror( errno );
        debugM( "tcgetattr() failed" );
        debugV( errno, errMsg );
        throw  CarrtError( makeRpi0ErrorId( kRpi0SerialError, 2, errno ), std::string( errMsg ) );
    }

    tty.c_cflag &= ~PARENB;                     // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB;                     // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE;                      // Clear all bits that set the data size 
    tty.c_cflag |= CS8;                         // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS;                    // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL;              // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;                     // ?Operate in NON-CANONICAL mode (immediate access to each byte)
    tty.c_lflag &= ~ECHO;                       // Disable echo
    tty.c_lflag &= ~ECHOE;                      // Disable erasure
    tty.c_lflag &= ~ECHONL;                     // Disable new-line echo
    tty.c_lflag &= ~ISIG;                       // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);     // Turn off s/w flow ctrl

    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);    // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST;                      // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR;                      // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS;                  // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT;                  // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10;    // Wait for up to 0.1s (1 deciseconds), returning after 0.1s if no data is present.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 115200
    cfsetispeed( &tty, B115200 );
    cfsetospeed( &tty, B115200 );

    // Save tty settings, also checking for error
    if ( tcsetattr( mSerialPort, TCSANOW, &tty ) != 0 ) 
    {
        char* errMsg = std::strerror( errno );
        debugM( "tcsetattr() failed" );
        debugV( errno, errMsg );
        throw  CarrtError( makeRpi0ErrorId( kRpi0SerialError, 2, errno ), std::string( errMsg ) );
    }
}



SerialLinkRPi::~SerialLinkRPi()
{
    close( mSerialPort );
}



std::optional<std::uint8_t> SerialLinkRPi::getByte()
{
    uint8_t c{};
    auto numRead = read( mSerialPort, &c, 1 );
    if ( numRead == 0 )
    {
        // EOF == read buffer empty
        return std::nullopt;
    }
    if ( numRead == 1 )
    {
        return c;
    }
    else
    {
        // Throw error
        debugM( "gettByte() failed reading" );
        debugV( numRead, errno );

        std::stringstream errMsgStrm{};
        errMsgStrm <<  "getByte() failed reading with errno: " << errno
            << " and numRead: " << numRead;
        std::string errMsg{};
        errMsgStrm >> errMsg;
        throw CarrtError( makeRpi0ErrorId( kRpi0SerialError, 666, errno ), errMsg );
    }
}


std::optional<uint32_t> SerialLinkRPi::get4Bytes()
{
    RawData r( 0 );

    auto numRead = read( mSerialPort, r.c(), 4 );
    if ( numRead == 0 )
    {
        // EOF == buffer empty
        return std::nullopt;
    }

    if ( numRead == 4 )
    {
        // We are done
        return r.u();
    }
    
    // Handle partial reads...
    const int kMaxAttempts{ 4 };
    int attempts{ 0 };
    while ( numRead < 4 && errno == 0 && attempts++ < kMaxAttempts )
    {
        // Add little delay
        Clock::sleep( 100us );

        // Try reading the remainder
        std::uint8_t buf[4];
        auto more = read( mSerialPort, buf, 4 - numRead );
        if ( more )
        {
            for ( int j{ 0 }, i{ numRead }; i < 4; i++, j++ )
            {
                r.c()[i] = buf[j];
            }
            numRead += more;
        }
    }

    if ( numRead == 4 )
    {
        // We are done
        return r.u();
    }
    
    // If we haven't returned by this point, we're out of options; throw
    debugM( "getByte() failed reading" );
    debugV( numRead, errno );

    std::stringstream errMsgStrm{};
    errMsgStrm <<  "get4Byte() failed reading with errno: " << errno
        << " and numRead: " << numRead;
    std::string errMsg{};
    errMsgStrm >> errMsg;
    throw CarrtError( makeRpi0ErrorId( kRpi0SerialError, 666, errno ), errMsg );
}



bool SerialLinkRPi::get4Bytes( std::uint8_t* c )
{
    auto u = get4Bytes();
    if ( u )
    {
        RawData t( *u );
        memcpy( c, t.c(), 4 );
        return true;
    }
    else
    {
        return false;
    }
}



void SerialLinkRPi::putByte( std::uint8_t c )
{
    auto numWritten = write( mSerialPort, &c, 1 );
    if ( numWritten != 1 )
    {
        debugM( "putByte() failed writing value: " );
        debugV( c, numWritten );

        std::stringstream errMsgStrm;
        errMsgStrm << "putByte() failed writing value: " << c
            << " with numWritten: " << numWritten << " and errno: " << errno;
        std::string errMsg;
        errMsgStrm >> errMsg;
        throw CarrtError( makeRpi0ErrorId( kRpi0SerialError, 666, errno ), errMsg );
    }
}



void SerialLinkRPi::put4Bytes( const std::uint8_t* c )
{
    auto numWritten = write( mSerialPort, c, 4 );
    if ( numWritten != 4 )
    {
        debugM( "put4Byte() failed writing values: " );
        debugV( c[0], c[1], c[2], c[3] );
        debugV( numWritten );

        std::stringstream errMsgStrm;
        errMsgStrm << "putByte() failed writing values: " 
            << c[0] << ", " << c[1] << ", " << c[2] << ", " << c[3]
            << " with numWritten: " << numWritten << " and errno: " << errno;
        std::string errMsg;
        errMsgStrm >> errMsg;
        throw CarrtError( makeRpi0ErrorId( kRpi0SerialError, 666, errno ), errMsg );
    }
}




int SerialLinkRPi::getBytes( int nbr, std::uint8_t* buffer )
{
    return  read( mSerialPort, buffer, nbr );
}



int SerialLinkRPi::putBytes( int nbr, const std::uint8_t* buffer )
{
    return write( mSerialPort, buffer, nbr );
}
