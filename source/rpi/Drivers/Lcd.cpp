/*
    Lcd.cpp - Driver for  an I2C-based LCD for CARRTv3.
    Copyright (c) 2019 Igor Mikolic-Torreira.  All right reserved.

    Inspired by the Adafruit library written by Limor Fried/Ladyada
    for Adafruit Industries, but this is a very different implementation.

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


#include "Lcd.h"

#include <iostream>
#include <cstring>
#include <string>
#include <cinttypes>

#include <unistd.h>			//Needed for I2C port
#include <fcntl.h>			//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port

#include "Clock.h"
#include "I2c.h"

using namespace Clock;




// We talk to the LCD's HD44780U controller via an MCP23017

// MCP23017 I2C address

#define MCP23017_ADDRESS            0x20

// MCP23017 registers (assumes IOCON.BANK = 0 [default])

#define MCP23017_DIGITAL_HIGH       1
#define MCP23017_DIGITAL_LOW        0

#define MCP23017_INPUT_MODE         1
#define MCP23017_OUTPUT_MODE        0

#define MCP23017_PULLUP_ON          1
#define MCP23017_PULLUP_OFF         0

#define MCP23017_IODIRA             0x00
#define MCP23017_IPOLA              0x02
#define MCP23017_GPINTENA           0x04
#define MCP23017_DEFVALA            0x06
#define MCP23017_INTCONA            0x08
#define MCP23017_IOCONA             0x0A
#define MCP23017_GPPUA              0x0C
#define MCP23017_INTFA              0x0E
#define MCP23017_INTCAPA            0x10
#define MCP23017_GPIOA              0x12
#define MCP23017_OLATA              0x14

#define MCP23017_IODIRB             0x01
#define MCP23017_IPOLB              0x03
#define MCP23017_GPINTENB           0x05
#define MCP23017_DEFVALB            0x07
#define MCP23017_INTCONB            0x09
#define MCP23017_IOCONB             0x0B
#define MCP23017_GPPUB              0x0D
#define MCP23017_INTFB              0x0F
#define MCP23017_INTCAPB            0x11
#define MCP23017_GPIOB              0x13
#define MCP23017_OLATB              0x15





// Commands
#define LCD_CLEARDISPLAY            0x01
#define LCD_RETURNHOME              0x02
#define LCD_ENTRYMODESET            0x04
#define LCD_DISPLAYCONTROL          0x08
#define LCD_CURSORSHIFT             0x10
#define LCD_FUNCTIONSET             0x20
#define LCD_SETCGRAMADDR            0x40
#define LCD_SETDDRAMADDR            0x80

// Flags for display entry mode
#define LCD_ENTRYRIGHT              0x00
#define LCD_ENTRYLEFT               0x02
#define LCD_ENTRYSHIFTINCREMENT     0x01
#define LCD_ENTRYSHIFTDECREMENT     0x00

// Flags for display on/off control
#define LCD_DISPLAYON               0x04
#define LCD_DISPLAYOFF              0x00
#define LCD_CURSORON                0x02
#define LCD_CURSOROFF               0x00
#define LCD_BLINKON                 0x01
#define LCD_BLINKOFF                0x00

// Flags for display/cursor shift
#define LCD_DISPLAYMOVE             0x08
#define LCD_CURSORMOVE              0x00
#define LCD_MOVERIGHT               0x04
#define LCD_MOVELEFT                0x00

// Flags for function set
#define LCD_8BITMODE                0x10
#define LCD_4BITMODE                0x00
#define LCD_2LINE                   0x08
#define LCD_1LINE                   0x00
#define LCD_5x10DOTS                0x04
#define LCD_5x8DOTS                 0x00



/*
    This numbering of MCP23017 pins (with GPIO B pins numbered lower
    than GPIO A pins) is non-standard, but allows reading 16-bits as
    a 16-bit int without having to flip high and low bytes.

    Pin numbers <-> MCP23017 <-> HD44780U mapping

    00          GPIOB 0     blue        blue backlight
    01          GPIOB 1     d7          4-bit data transfer
    02          GPIOB 2     d6          4-bit data transfer
    03          GPIOB 3     d5          4-bit data transfer
    04          GPIOB 4     d4          4-bit data transfer
    05          GPIOB 5     Enable      pulse high to enable data transfer
    06          GPIOB 6     R/W         0 = write, 1 = read
    07          GPIOB 7     RS          0 = cmd, 1 = data
    08          GPIOA 0     button 0
    09          GPIOA 1     button 1
    10          GPIOA 2     button 2
    11          GPIOA 3     button 3
    12          GPIOA 4     button 4
    13          GPIOA 5     <not used>
    14          GPIOA 6     red         red backlight
    15          GPIOA 7     green       green backlight
*/



namespace Lcd
{

    // *** ATTENTION ***
    // To achieve speed, the logic of the code depends heavily on the specific pin layout.
    // Specifically, the logic "knows" whaich functions to access via GPIO A or GPIO B.
    // The code will BREAK if you move pin assignments around using these constants
    // without corresponding changes in the code.

    enum PinsNumbers
    {
        // GPIO B
        kByteLcdRS          = 7,
        kByteLcdRW          = 6,
        kByteLcdEnable      = 5,
        kByteLcdD4          = 4,
        kByteLcdD5          = 3,
        kByteLcdD6          = 2,
        kByteLcdD7          = 1,
        kByteLcdBlue        = 0,

        kWordLcdRS          = 7,
        kWordLcdRW          = 6,
        kWordLcdEnable      = 5,
        kWordLcdD4          = 4,
        kWordLcdD5          = 3,
        kWordLcdD6          = 2,
        kWordLcdD7          = 1,
        kWordLcdBlue        = 0,

        // GPIO A
        kByteLcdGreen       = 7,
        kByteLcdRed         = 6,
        kByteNotUsed        = 5,
        kByteButtonLeft     = 4,
        kByteButtonUp       = 3,
        kByteButtonDown     = 2,
        kByteButtonRight    = 1,
        kByteButtonSelect   = 0,

        kWordLcdGreen       = 15,
        kWordLcdRed         = 14,
        kWordNotUsed        = 13,
        kWordButtonLeft     = 12,
        kWordButtonUp       = 11,
        kWordButtonDown     = 10,
        kWordButtonRight    = 9,
        kWordButtonSelect   = 8
    };

    const uint8_t kNumLines     = 2;

    const char* kBlankLine      = "                ";


    enum
    {
        kWriteFourBitsSendChar = 0,
        kWriteFourBitsSendCommand = 1
    };

    void initMCP23017();
    void initHD44780U();
    size_t write( uint8_t value );
    void writeFourBitsToLcd( uint8_t value, uint8_t gpioB );
    void sendCharOrCmdToLcd( uint8_t value, bool isCommand );

    void sendCommand( uint8_t cmd )
    {
        sendCharOrCmdToLcd( cmd, kWriteFourBitsSendCommand );
    }

    void sendCharToDisplay( uint8_t value )
    {
        sendCharOrCmdToLcd( value, kWriteFourBitsSendChar );
    }

    uint8_t             mDisplayControl;
    uint8_t             mDisplayMode;
    uint8_t             mCurrLine;
};





void Lcd::init()
{
    mDisplayControl = 0;
    mDisplayMode = 0;
    mCurrLine = 0;

    initMCP23017();

    setBacklight( 0x07 );

    initHD44780U();
}




void Lcd::initMCP23017()
{
    // Set output and input modes correctly
    // Pins 0, 1, 2, 3, 4, 5, 6, 7 (GPIO B):  output mode
    // Pins 8, 9, 10, 11, 12 (GPIO A):  input mode, pullup on
    // Pins 14, 15 (GPIO A):  output mode

    // Set input/output modes
    union
    {
        uint8_t modes[2];
        uint16_t word;
    } data;
    // Pins 8-15, GPIOA, 0b00111111 = 0x3F
    data.modes[0] = 0x3F;
    // Pins 0-7, GPIOB, 0b00000000 = 0x00;
    data.modes[1] = 0x00;

    // Experimentally observed a set-up timing issue: need to make sure this
    // next transmission to MCP23017 completes before proceeding to the next one.
    I2c::write( MCP23017_ADDRESS, MCP23017_IODIRA, data.word );

    // Set pullups
    union
    {
        uint8_t pullups[2];
        uint16_t word;
    } data2;
    // Pins 8-15, GPIOA, 0b00011111 = 0x1F
    data2.pullups[0] = 0x1F;
    // Pins 1-7, GPIOB, 0b00000000 = 0x00
    data2.pullups[1] = 0x00;

    I2c::write( MCP23017_ADDRESS, MCP23017_GPPUA, data2.word );
}




void Lcd::initHD44780U()
{
/*
    ** Initialize and configure the HD44780U/LCD **

    When the HD44780U powers up, it is configured as follows:

    1. Display clear
    2. Function set:
        DL = 1; 8-bit interface data
        N = 0; 1-line display
        F = 0; 5x8 dot character font
    3. Display on/off control:
        D = 0; Display off
        C = 0; Cursor off
        B = 0; Blinking off
    4. Entry mode set:
        I/D = 1; Increment by 1
        S = 0; No shift

    But we don't its state at program start because Arduino reset doesn't reset
    the HD44780U.

    The following initialization incantation is magic as far as I'm concerned.  The
    datasheet is ambiguous at best, so I simply follow the same steps Limor Fried
    did in her Adafruit library...

    The trick is we have to put the HD44780U in 4-bit mode.  The appropriate start-up
    sequence to do this is described on page 45/46 of the HD44780U datasheet.
*/

    // Datasheet says to wait at least 40ms after power rises above 2.7V before sending commands.
    delayMilliseconds( 50 );

    // Pull RS, R/W, and Enable low to begin commands:  0b00000001 = 0x01
    uint8_t gpioB = 0x01;
    I2c::write( MCP23017_ADDRESS, MCP23017_GPIOB, gpioB );

    // Put the LCD into 4 bit mode; see Hitachi HD44780U datasheet p. 46, fig. 24
    // HD44780U starts in 8-bit mode

    // First write
    writeFourBitsToLcd( 0x03, gpioB );

    // Datasheet says to wait at least 4.1ms
    delayMicroseconds( 4500 );

    // Second write
    writeFourBitsToLcd( 0x03, gpioB );

    // Datasheet says to wait at least 4.1ms
    delayMicroseconds( 4500 );

    // Third write
    writeFourBitsToLcd( 0x03, gpioB );

    // Datasheet says to wait at least 100ms
    delayMicroseconds( 150 );

    // Now can enter 4-bit mode
    writeFourBitsToLcd( 0x02, gpioB );

    // Now configure the number of lines, font size, etc.
    sendCommand( LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS );

    // Turn the display on with no cursor or blinking default
    mDisplayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    displayOn();
    clear();

    mCurrLine = 0;

    // Initialize to default text direction
    mDisplayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // Set the entry mode
    sendCommand( LCD_ENTRYMODESET | mDisplayMode );
}





void Lcd::writeFourBitsToLcd( uint8_t value, uint8_t gpioB )
{
    // Use the lower 4 bits of value
    gpioB &= ~( ( 1 << kByteLcdD4 ) | ( 1 << kByteLcdD5 ) | ( 1 << kByteLcdD6 ) | ( 1 << kByteLcdD7 ) );
    gpioB |= ( value & 0x01 ) << kByteLcdD4;
    gpioB |= ( (value >> 1) & 0x01 ) << kByteLcdD5;
    gpioB |= ( (value >> 2) & 0x01 ) << kByteLcdD6;
    gpioB |= ( (value >> 3) & 0x01 ) << kByteLcdD7;

    // Set enable low
    gpioB &= ~( 1 << kByteLcdEnable );

    I2c::write( MCP23017_ADDRESS, MCP23017_GPIOB, gpioB );

    //  HD44780U requires us to pulse the enable pin for values to be read
    delayMicroseconds( 1 );
    // Set enable high
    gpioB |= ( 1 <<  kByteLcdEnable );
    I2c::write( MCP23017_ADDRESS, MCP23017_GPIOB, gpioB );

    delayMicroseconds( 1 );
    // Set enable low
    gpioB &= ~( 1 <<  kByteLcdEnable );
    I2c::write( MCP23017_ADDRESS, MCP23017_GPIOB, gpioB );
    delayMicroseconds( 100 );
}




void Lcd::sendCharOrCmdToLcd( uint8_t value, bool isCommand )
{
    // Sending characters or command to the LCD involves only GPIOB pins
    uint8_t gpioB;
    I2c::read( MCP23017_ADDRESS, MCP23017_OLATB, &gpioB );

    // Clear the RW bit to write to the HD44780U
    gpioB &= ~(1 << kByteLcdRW);
    if ( isCommand )
    {
        // Clear the RS bit to send a command
        gpioB &= ~(1 << kByteLcdRS);
    }
    else
    {
        // Set the RS bit to send a character
        gpioB |= (1 << kByteLcdRS);
    }

    // Send the high bits
    writeFourBitsToLcd( (value >> 4), gpioB );
    // Send the low bits
    writeFourBitsToLcd( (value & 0b00001111), gpioB );
}






void Lcd::clear()
{
    sendCommand( LCD_CLEARDISPLAY );    // Clear display, set cursor position to zero
    delayMilliseconds( 2 );             // Takes a while...
}



void Lcd::home()
{
    sendCommand( LCD_RETURNHOME );      // Set cursor position to zero
    delayMilliseconds( 2 );             // Takes a while...
}



void Lcd::setCursor(  uint8_t row, uint8_t col )
{
    int row_offsets[] = { 0x00, 0x40 };
    if ( row >= kNumLines )
    {
        // Count rows starting at 0 (in traditional C/C++ fashion)
        row %= kNumLines;
    }

    sendCommand( LCD_SETDDRAMADDR | ( col + row_offsets[row] ) );
}



void Lcd::displayOff()
{
    mDisplayControl &= ~LCD_DISPLAYON;
    sendCommand( LCD_DISPLAYCONTROL | mDisplayControl );
}



void Lcd::displayOn()
{
    mDisplayControl |= LCD_DISPLAYON;
    sendCommand( LCD_DISPLAYCONTROL | mDisplayControl );
}



void Lcd::cursorOff()
{
    mDisplayControl &= ~LCD_CURSORON;
    sendCommand( LCD_DISPLAYCONTROL | mDisplayControl );
}



void Lcd::cursorOn()
{
    mDisplayControl |= LCD_CURSORON;
    sendCommand( LCD_DISPLAYCONTROL | mDisplayControl );
}



void Lcd::blinkOff()
{
    mDisplayControl &= ~LCD_BLINKON;
    sendCommand( LCD_DISPLAYCONTROL | mDisplayControl );
}



void Lcd::blinkOn()
{
    mDisplayControl |= LCD_BLINKON;
    sendCommand( LCD_DISPLAYCONTROL | mDisplayControl );
}




void Lcd::scrollDisplayLeft()
{
    sendCommand( LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT );
}



void Lcd::scrollDisplayRight()
{
    sendCommand( LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT );
}



void Lcd::autoscrollOn()
{
    mDisplayMode |= LCD_ENTRYSHIFTINCREMENT;
    sendCommand( LCD_ENTRYMODESET | mDisplayMode );
}



void Lcd::autoscrollOff()
{
    mDisplayMode &= ~LCD_ENTRYSHIFTINCREMENT;
    sendCommand( LCD_ENTRYMODESET | mDisplayMode );
}




void Lcd::displayTopRow( const char* str )
{
    clearTopRow();
    setCursor( 0, 0 );
    write( str );
}


void Lcd::displayBottomRow( const char* str )
{
    clearBottomRow();
    setCursor( 1, 0 );
    write( str );
}



void Lcd::clearTopRow()
{
    setCursor( 0, 0 );
    write( kBlankLine );
}


void Lcd::clearBottomRow()
{
    setCursor( 1, 0 );
    write( kBlankLine );
}





size_t Lcd::write( uint8_t value )
{
    sendCharToDisplay( value );
    return 1;
}




size_t Lcd::write( char value )
{
    sendCharToDisplay( static_cast<uint8_t>( value ) );
    return 1;
}




size_t Lcd::write( const char* str )
{
    size_t n = 0;
    if ( str )
    {
        while ( *str )
        {
            sendCharToDisplay( static_cast<uint8_t>( *str++ ) );
            ++n;
        }
    }
    return n;
}




size_t Lcd::write( const char* buffer, size_t size )
{
    return write( reinterpret_cast<const uint8_t*>( buffer), size );
}


size_t Lcd::write( const uint8_t* buffer, size_t size )
{
    size_t n = 0;
    if ( buffer )
    {
        while ( n < size )
        {
             sendCharToDisplay( *buffer++ );
            ++n;
        }
    }
    return n;
}




void Lcd::setBacklight( uint8_t status )
{
    // Use pins 0, 14, 15; means we have to touch both GPIO A & B

    // First read GPIO A & B (actually read the latches = what we set them)
    union
    {
        uint8_t gpio[2];
        uint16_t byte;
    } data;
    I2c::read( MCP23017_ADDRESS, MCP23017_OLATA, &data.byte );

    // Clear pins 14 & 15 (GPIOA 6 & 7)
    data.gpio[0] &= ~( ( 1 << kByteLcdGreen ) | ( 1 << kByteLcdRed ) );
    // Set pins 14 & 15 (GPIOA 6 & 7) as appropriate
    data.gpio[0] |= ( (~(status >> 1) & 0x01) << kByteLcdGreen ) | ( (~status & 0x01) << kByteLcdRed );

    // Clear pin 0 (GPIOB 0), set it as needed, and write the new GPIO
    data.gpio[1] &= ~0x01;
    data.gpio[1] |= ~(status >> 2) & 0x01;

    I2c::write( MCP23017_ADDRESS, MCP23017_GPIOA, data.byte );
}



int Lcd::readButtons()
{
    // Buttons on all on GPIOA; they are bits 0 thru 4
    uint8_t gpioA;
    I2c::read( MCP23017_ADDRESS, MCP23017_GPIOA, &gpioA );

    // Buttons have pullups, so gpio bits record the inverse of button state
    // 0b00011111 = 0x1F; 0b10000000 = 0x80; 0b11100000 = 0xE0
    return static_cast<uint8_t>(~gpioA) & 0x1F;
}
