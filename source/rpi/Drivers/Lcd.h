/*
    Lcd.h - Driver for  an I2C-based LCD for CARRTv3.
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


#ifndef Lcd_h
#define Lcd_h

#include <cstdint>
#include <sstream>




namespace Lcd
{

    enum IntegerOutputBase
    {
        kOct    = 8,        //!< Produce an octal representation of integers (e.g, 11 is output as 013)
        kDec    = 10,       //!< Produce a decimal representation of integers (e.g., 11 is output as 11
        kHex    = 16        //!< Produce a hexadecimal representation of integers (e.g., 11 is output as 0x0b)
    };



    /*!
     *  These constants are used to identify the five buttons.
     */
    enum Buttons
    {
        kButton_Select   = 0x01,        //!< the Select button       \hideinitializer
        kButton_Right    = 0x02,        //!< the Right button        \hideinitializer
        kButton_Down     = 0x04,        //!< the Down button         \hideinitializer
        kButton_Up       = 0x08,        //!< the Up button           \hideinitializer
        kButton_Left     = 0x10         //!< the Left button         \hideinitializer
    };


    /*!
     * These constants are used to set the backlight color on the LCD.
     */
    enum Colors
    {
        kBacklight_Off      = 0x0,
        kBacklight_Red      = 0x1,        //!< Backlight red            \hideinitializer
        kBacklight_Yellow   = 0x3,        //!< Backlight yellow         \hideinitializer
        kBacklight_Green    = 0x2,        //!< Backlight green          \hideinitializer
        kBacklight_Teal     = 0x6,        //!< Backlight teal           \hideinitializer
        kBacklight_Blue     = 0x4,        //!< Backlight blue           \hideinitializer
        kBacklight_Violet   = 0x5,        //!< Backlight violet         \hideinitializer
        kBacklight_White    = 0x7         //!< Backlight white          \hideinitializer
    };


    /*!
     * \brief Initialize the Lcd object.  This must be called before using the Lcd, or
     * calling any of the other member functions.  The I2C system must be initialized
     * before calling this function (by calling I2cMaster::start() from I2cMaster.h).
     *
     * The LCD display is initialized in 16-column, 2-row mode.
     */
    void init();


    /*!
     * \brief Clear the display (all rows, all columns).
     */
    void clear();


    /*!
     * \brief Move the cursor home (the top row, left column).
     */
    void home();


    /*!
     * \brief Display a C-string on the top row.
     *
     * \arg \c str the C-string to display.
     */
    void displayTopRow( const char* str );


    /*!
     * \brief Display a C-string on the bottom row.
     *
     * \arg \c str the C-string to display.
     */
    void displayBottomRow( const char* str );


    /*!
     * \brief Clear the top row.
     */
    void clearTopRow();


    /*!
     * \brief Clear the bottom row.
     */
    void clearBottomRow();



    /*!
     * \brief Turn the display off.
     */
    void displayOff();


    /*!
     * \brief Turn the display on.
     */
    void displayOn();


    /*!
     * \brief Do not blink the cursor.
     */
    void blinkOff();


    /*!
     * \brief Blink the cursor.
     */
    void blinkOn();


    /*!
     * \brief Hide the cursor.
     */
    void cursorOff();


    /*!
     * \brief Display the cursor.
     */
    void cursorOn();


    /*!
     * \brief Scroll the display to the left.
     */
    void scrollDisplayLeft();


    /*!
     * \brief Scroll the display to the right.
     */
    void scrollDisplayRight();


    /*!
     * \brief Turn on automatic scrolling of the display.
     */
    void autoscrollOn();


    /*!
     * \brief Turn off automatic scrolling of the display.
     */
    void autoscrollOff();


    /*!
     * \brief Move the cursor the a particular row and column.
     *
     * \arg \c row the row to move the cursor to (numbering starts at 0).
     * \arg \c col the column to move the cursor to (numbering starts at 0).
     */
    void setCursor( std::uint8_t row, std::uint8_t col );


    /*!
     * \brief Set the backlight to a given color.  Set a black-and-white LCD display to White if
     * you want to have a backlight.
     *
     * \arg \c color the color to set the backlight.  Pass one of kBacklight_Red, kBacklight_Yellow,
     *  kBacklight_Green, kBacklight_Teal, kBacklight_Blue, kBacklight_Violet, or kBacklight_White.
     */
    void setBacklight( std::uint8_t color );


    /*!
     * \brief Pass a command to the LCD.
     *
     * \arg \c cmd a valid command to send to the HD44780U.
     */
    void command( std::uint8_t cmd );


    /*!
     * \brief Read the state of the buttons associated with the LCD display.
     *
     * \returns a byte with flags set corresponding to the buttons that are depressed.  You must
     * "and" the return value with kButton_Right, kButton_Left, kButton_Down, kButton_Up, or
     * kButton_Select to determine which buttons have been pressed.
     */
    int readButtons();



    /*!
     * \brief Write a single character to the LCD at the current cursor location.
     * This implements the pure virtual function Writer::write( char c ).
     *
     * \arg \c  the character to be written.
     *
     * \returns the number of bytes written.
     */
    std::size_t write( char c );


    /*!
     * \brief Write a C-string to the LCD at the current cursor location.
     * This implements the pure virtual function Writer::write( const char* str ).
     *
     * \arg \c  the C-string to be written.
     *
     * \returns the number of bytes written.
     */
    std::size_t write( const char* str );


    /*!
     * \brief Write a given number of characters from a buffer to the LCD at the current cursor location.
     * This implements the pure virtual function Writer::write( const char* buffer, size_t size ).
     *
     * \arg \c buffer the buffer of characters to write.
     * \arg \c size the number of characters to write
     *
     * \returns the number of bytes written.
     */
    std::size_t write( const char* buffer, std::size_t size );


    /*!
     * \brief Write a given number of bytes from a buffer to the LCD at the current cursor location.
     * This implements the pure virtual function Writer::write( const uint8_t* buffer, size_t size ).
     *
     * \arg \c buffer the buffer of bytes to write.
     * \arg \c size the number of bytes to write
     *
     * \returns the number of bytes written.
     */
    std::size_t write( const std::uint8_t* buffer, std::size_t size );



    template< typename T > void print( T x )
    {
        std::stringstream tmp;
        tmp << x;
        write( tmp.str().c_str() );
    }


    template< typename T > void print( T x, Lcd::IntegerOutputBase base )
    {
        std::stringstream tmp;
        switch ( base )
        {
             case kOct:
                tmp << std::oct << std::showbase;
                break;

            case kHex:
                tmp << std::hex << std::showbase;
                break;

            default:
                break;
        }
        tmp << x;
        write( tmp.str().c_str() );
    }

};

#endif
