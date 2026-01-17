
#include <errno.h>    // Error integer and strerror() function
#include <fcntl.h>    // Contains file controls like O_RDWR
#include <stdio.h>
#include <string.h>
#include <termios.h>    // Contains POSIX terminal control definitions
#include <unistd.h>     // write(), read(), close()

union Transfer
{
    char c[ 4 ];
    int i;
    float f;
};

int main()
{
    printf( "Starting serial test\n" );
    // Open the serial port. Change device path as needed
    // (currently set to an standard FTDI USB-UART cable type device)
    int serial_port = open( "/dev/serial0", O_RDWR );
    if ( !serial_port )
    {
        printf( "Error %i opening serial port: %s/n", serial_port, strerror( serial_port ) );
    }
    else
    {
        printf( "Serial port opened with fd = %i\n", serial_port );
    }

    // Create new termios struct, we call it 'tty' for convention
    struct termios tty;

    // Read in existing settings, and handle any error
    if ( tcgetattr( serial_port, &tty ) != 0 )
    {
        printf( "Error %i from tcgetattr: %s\n", errno, strerror( errno ) );
        return 1;
    }

    // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~PARENB;
    // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSTOPB;
    // Clear all bits that set the data size
    tty.c_cflag &= ~CSIZE;
    // 8 bits per byte (most common)
    tty.c_cflag |= CS8;
    // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag &= ~CRTSCTS;
    // Turn on READ & ignore ctrl lines (CLOCAL = 1)
    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_lflag &= ~ICANON;
    // Disable echo
    tty.c_lflag &= ~ECHO;
    // Disable erasure
    tty.c_lflag &= ~ECHOE;
    // Disable new-line echo
    tty.c_lflag &= ~ECHONL;
    // Disable interpretation of INTR, QUIT and SUSP
    tty.c_lflag &= ~ISIG;
    // Turn off s/w flow ctrl
    tty.c_iflag &= ~( IXON | IXOFF | IXANY );
    // Disable any special handling of received bytes
    tty.c_iflag &= ~( IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL );

    // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~OPOST;
    // Prevent conversion of newline to carriage return/line feed
    tty.c_oflag &= ~ONLCR;
    // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~OXTABS;
    // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT;

    // Wait for up to 0.1s (1 deciseconds),
    // returning after 0.1s if no data is present.
    tty.c_cc[ VTIME ] = 10;
    tty.c_cc[ VMIN ] = 0;

    // Set in/out baud rate to be 115200
    cfsetispeed( &tty, B115200 );
    cfsetospeed( &tty, B115200 );

    // Save tty settings, also checking for error
    if ( tcsetattr( serial_port, TCSANOW, &tty ) != 0 )
    {
        printf( "Error %i from tcsetattr: %s\n", errno, strerror( errno ) );
        return 1;
    }

    printf( "Start T test\n" );
    char c = 'T';
    write( serial_port, &c, 1 );

    c = 0;
    int n = 0;
    n = read( serial_port, &c, 1 );

    if ( c == 'T' )
    {
        printf( "Sent t, got %c -- Success!\n", c );
    }
    else
    {
        printf( "Sent t, got %c -- Failure!\n", c );
    }

    Transfer xfer;

    c = 'i';
    write( serial_port, &c, 1 );
    c = 0;
    n = 0;
    n = read( serial_port, &c, 1 );
    if ( c != 'I' )
    {
        printf( "Error, sent i but didn't get I\n" );
    }
    xfer.i = 0;
    n = 0;
    while ( n == 0 )
    {
        n = read( serial_port, &( xfer.c ), 4 );
    }
    if ( xfer.i == 123'456'789 )
    {
        printf( "Sent 123456789, got %i -- Success!\n", xfer.i );
    }
    else
    {
        printf( "Sent 123456789, got %i -- Failure!\n", xfer.i );
    }

    c = 'j';
    write( serial_port, &c, 1 );
    c = 0;
    n = 0;
    while ( n == 0 )
    {
        n = read( serial_port, &c, 1 );
    }
    if ( c != 'J' )
    {
        printf( "Error, sent j but didn't get J\n" );
    }
    xfer.i = 0;
    n = 0;
    while ( n == 0 )
    {
        n = read( serial_port, &( xfer.c ), 4 );
    }

    if ( xfer.i == -123'456'789 )
    {
        printf( "Sent -123456789, got %i -- Success!\n", xfer.i );
    }
    else
    {
        printf( "Sent -123456789, got %i -- Failure!\n", xfer.i );
    }

    c = 'k';
    write( serial_port, &c, 1 );
    c = 0;
    n = 0;
    while ( n == 0 )
    {
        n = read( serial_port, &c, 1 );
    }
    if ( c != 'K' )
    {
        printf( "Error, sent k but didn't get K\n" );
    }
    xfer.i = 0;
    n = 0;
    while ( n == 0 )
    {
        n = read( serial_port, &( xfer.c ), 4 );
    }

    if ( xfer.i == 660'327'733 )
    {
        printf( "Sent 660327733, got %i -- Success!\n", xfer.i );
    }
    else
    {
        printf( "Sent 660327733, got %i -- Failure!\n", xfer.i );
    }

    c = 'f';
    write( serial_port, &c, 1 );
    c = 0;
    n = 0;
    while ( n == 0 )
    {
        n = read( serial_port, &c, 1 );
    }
    if ( c != 'F' )
    {
        printf( "Error, sent f but didn't get F\n" );
    }
    xfer.f = 0.0;
    n = 0;
    while ( n == 0 )
    {
        n = read( serial_port, &( xfer.c ), 4 );
    }

    float f2 = 2.71828;
    if ( xfer.f == f2 )
    {
        printf( "Sent 2.71828, got %f -- Success!\n", xfer.f );
    }
    else
    {
        printf( "Sent 2.71828, got %f -- Failure!\n", xfer.f );
    }

    close( serial_port );
    return 0;    // success
};
