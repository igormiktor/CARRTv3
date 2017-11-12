/*
    LcdTest.cpp - Testing harness for the LCD display driver.

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




#include "Drivers/Lcd.h"
#include "Drivers/Clock.h"


int main()
{
    Lcd::init();

    Lcd::displayOn();
    Lcd::setBacklight( Lcd::kBacklight_White );

    for ( int i = 0; i < 10; ++i )
    {
        Lcd::displayTopRow( "ABCDEFGHIJKLMNOP" );
        Lcd::displayBottomRow( "abcdefghijklmnop" );

        Clock::delayMilliseconds( 5000 );

        Lcd::clearTopRow();
        Clock::delayMilliseconds( 1000 );
        Lcd::clearBottomRow();
        Clock::delayMilliseconds( 3000 );

        Lcd::displayTopRow( "01234567890ABCDE" );
        Lcd::setCursor( 1, 0 );
        Lcd::print( "abcdefghijklmnop" );

        Clock::delayMilliseconds( 5000 );

        Lcd::clearTopRow();
        Lcd::setCursor( 0, 8 );
        Lcd::print( 'Z' );
        Lcd::displayBottomRow( "01234567890ABCDE" );

        Clock::delayMilliseconds( 5000 );

        Lcd::clear();
        Lcd::displayTopRow( "255 base 10" );
        Lcd::setCursor( 1, 0 );
        Lcd::print( 255 );

        Clock::delayMilliseconds( 5000 );

        Clock::delayMilliseconds( 5000 );

        Lcd::clear();
        Lcd::displayTopRow( "255 base 8" );
        Lcd::setCursor( 1, 0 );
        Lcd::print( 255, Lcd::kOct );

        Clock::delayMilliseconds( 5000 );

        Lcd::clear();
        Lcd::displayTopRow( "255 base 16" );
        Lcd::setCursor( 1, 0 );
        Lcd::print( 255, Lcd::kHex );
        Clock::delayMilliseconds( 5000 );

        Lcd::clear();
        Lcd::displayTopRow( "-123456789L" );
        Lcd::setCursor( 1, 0 );
        Lcd::print( -123456789L );

        Clock::delayMilliseconds( 5000 );

        Lcd::clear();
        Lcd::displayTopRow( "123456789UL" );
        Lcd::setCursor( 1, 0 );
        Lcd::print( 123456789UL );

        Clock::delayMilliseconds( 5000 );

        Lcd::clear();
        Lcd::displayTopRow( "2.7182818289" );
        Lcd::setCursor( 1, 0 );
        Lcd::print( 2.7182818289 );

        Clock::delayMilliseconds( 5000 );
    }

    Lcd::setBacklight( Lcd::kBacklight_Off );
    Lcd::displayOff();
}



