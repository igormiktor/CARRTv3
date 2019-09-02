/*
    SerialCommsToTrinketTest.cpp.cpp - Test serial communications without
    the Trinket.

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


#include <iostream>
#include <cstring>

#include <ctype.h>

#include <fcntl.h>  /* File Control Definitions          */
#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */
#include <errno.h>  /* ERROR Number Definitions          */

#include <sys/time.h>

#include <pigpio.h>


class SerialData
{
public:
    uint8_t data[255];
    uint8_t len;

    SerialData() : len( 0 ) {}
};



class SerialConnection
{
public:
    SerialConnection();
    ~SerialConnection();

    uint8_t readOneByte( uint8_t* byte );
    uint8_t readUntilPause( SerialData* buffer );
    uint8_t readUntilPause( uint8_t* buffer, int n );
    uint8_t readUntilPause( char* buffer, int n );

    uint8_t send( int n, const uint8_t* buffer );
    uint8_t send( const char* buffer );

private:

    int mSerialConnectionFd;
};


SerialConnection::SerialConnection() :
mSerialConnectionFd( -1 )
{
    const char* kSerialDeviceName = "/dev/serial0";

    int fd = open( kSerialDeviceName, O_RDWR | O_NOCTTY );
    if ( fd == -1 )
    {
        std::cout << "open " << kSerialDeviceName << " failed." << std::endl;
        throw 666;
    }

    mSerialConnectionFd = fd;

    // Configure serial to SerialConnection
    struct termios serialPortSettings;
    tcgetattr( mSerialConnectionFd, &serialPortSettings );
    cfmakeraw( &serialPortSettings );

    // cfsetispeed( &serialPortSettings, B115200 );                        // Input baud rate
    // cfsetospeed( &serialPortSettings, B115200 );                        // Output baud rate
    cfsetspeed( &serialPortSettings, B115200 );
    // serialPortSettings.c_cflag &= ~PARENB;                              // No Parity
    // serialPortSettings.c_cflag &= ~CSTOPB;                              // Stop bits = 1
    // serialPortSettings.c_cflag &= ~CSIZE;                               // Clears the bits portion of the flags
    // serialPortSettings.c_cflag |=  CS8;                                 // Set the data bits = 8
    serialPortSettings.c_cflag &= ~CRTSCTS;                             // Turn off hardware based flow control (RTS/CTS)
    serialPortSettings.c_cflag |= CREAD | CLOCAL;                       // Turn on the receiver of the serial port (CREAD) otherwise reading from the serial port will not work
    // serialPortSettings.c_iflag &= ~( IXON | IXOFF | IXANY );              // Turn off software based flow control (XON/XOFF)
    // serialPortSettings.c_iflag &= ~( ICANON | ECHO | ECHOE | ISIG );      // Ensure non-cannonical mode operations

    // Set time outs
    serialPortSettings.c_cc[VMIN]  = 0;                                     // Read 1 character
    serialPortSettings.c_cc[VTIME] = 5;                                     // Wait up to 5 seconds

    tcsetattr( mSerialConnectionFd, TCSANOW, &serialPortSettings );
};


SerialConnection::~SerialConnection()
{
    if ( mSerialConnectionFd > 0 )
    {
        close( mSerialConnectionFd );
    }
}


uint8_t SerialConnection::readUntilPause( SerialData* buffer )
{
    int max = 255;
    buffer->len = 0;
    int len = 0;
    char* here = reinterpret_cast<char*>( buffer->data );

    char c;
    uint8_t bytesRead = read( mSerialConnectionFd, &c, 1 );

    while ( !bytesRead )
    {
        bytesRead = read( mSerialConnectionFd, &c, 1 );
    }
    *here++ = c;
    len++;

    // now see if we get more...
    // Start timer...
    struct timeval t1, t2;
    double elapsedTime;

    // start timer
    gettimeofday( &t1, NULL );
    gettimeofday( &t2, NULL );
    while ( (t2.tv_sec - t1.tv_sec) < 3.0 )
    {
        uint8_t bytesRead = read( mSerialConnectionFd, &c, 1 );
        while ( !bytesRead  && (t2.tv_sec - t1.tv_sec) < 3.0 )
        {
            bytesRead = read( mSerialConnectionFd, &c, 1 );
            gettimeofday( &t2, NULL );
        }
        if ( bytesRead )
        {
            *here++ = c;
            len++;
            gettimeofday( &t1, NULL );
            gettimeofday( &t2, NULL );
        }
    }
    // If I get here, we timed out
    buffer->len = len;
    return buffer->len;
}


uint8_t SerialConnection::send( const char* buffer )
{
    int len = std::strlen( buffer );

    return write( mSerialConnectionFd, reinterpret_cast<const void*>( buffer ), static_cast<size_t>( len ) );
}




void doOutput( const SerialData& serialData )
{
    std::cout << "Rcvd: ";
    for ( int i = 0; i < serialData.len; ++i )
    {
        std::cout << serialData.data[i];
    }
    std::cout << std::endl;
}


int main()
{
    // Turn on Trinket
    std::cout << "Turning on Trinket" << std::endl;
    if ( gpioInitialise() < 0 )
    {
        std::cout << "pigpio init failed" << std::endl;
        return 1;
    }

    if ( gpioSetMode( 27, PI_OUTPUT ) )
    {
        std::cout << "gpioSetMode failed" << std::endl;
        gpioTerminate();
        return 1;
    }

    if ( gpioWrite( 27, 1 ) )
    {
        std::cout << "gpioWrite failed" << std::endl;
        gpioSetMode( 27, PI_INPUT );
        gpioTerminate();
        return 1;
    }

    if ( 1 )
    {
        // Set up serial to Trinket
        SerialConnection trinket;
        SerialData dataFromTrinket;

        int bytes = trinket.readUntilPause( &dataFromTrinket );
        if ( bytes )
        {
            doOutput( dataFromTrinket );
        }
        else
        {
            std::cout << "No start banner received" << std::endl;
        }

        const char* stuffToSend[] =
        {
            "Hello World!",
            "Test 1",
            "Test 2\n",
            "Test\n 3"
        };

        for ( int i = 0; i < sizeof(stuffToSend)/sizeof(char*); ++i )
        {
            trinket.send( stuffToSend[i] );
            std::cout << "Sent: " << stuffToSend[i] << std::endl;
            int bytes = trinket.readUntilPause( &dataFromTrinket );
            if ( bytes )
            {
                doOutput( dataFromTrinket );
            }
            else
            {
                std::cout << "No data received" << std::endl;
            }
        }
    }

    std::cout << "Turn off Trinket" << std::endl;

    gpioWrite( 27, 0 );

    std::cout << "Restore gpio settings" << std::endl;

    gpioSetMode( 27, PI_INPUT );
    gpioTerminate();

    std::cout << "Test finished" << std::endl;

    return 0;
}
