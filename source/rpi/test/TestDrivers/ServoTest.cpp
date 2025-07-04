/*
    ServoTest.cpp - Testing harness for the servo driver.

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


#include <iostream>

#include "CarrtPigpio.h"

#include "Clock.h"
#include "Keypad.h"
#include "Lcd.h"
#include "I2c.h"
#include "Servo.h"

#include "CarrtError.h"

#include "DebugUtils.hpp"

constexpr long kThreeMinutesInMillis = 3 * 60 *1000L;


int main()
{
    try
    {
        gpioInitialise();

        Clock::initSystemClock();

        Lcd::init();

        Lcd::displayOn();
        Lcd::setBacklight( Lcd::kBacklight_White );

        const int kMinTimeBetweenButtonChecks = 250;        // milliseconds
        long sNextTimeButtonClickAccepted = 0;

        Servo::init();

        Lcd::displayTopRow( "Button -> Servo" );

        int direction{ 0 };
        bool loopAgain{ true };
        while ( loopAgain )
        {
            try
            {
                uint8_t buttonHit = Keypad::readButtons();

                if ( buttonHit && Clock::millis() > sNextTimeButtonClickAccepted )
                {
                    // Accept the button click

                    // Rollover happens in about 50 days, so don't worry about it
                    sNextTimeButtonClickAccepted = Clock::millis() + kMinTimeBetweenButtonChecks;

                    Clock::delayMilliseconds( 25 );
                    Lcd::clear();

                    switch ( buttonHit )
                    {
                        case Keypad::kButton_Select:
                            Lcd::displayTopRow( "End..." );
                            Servo::slew( 0 );
                            loopAgain = false;
                            Clock::delayMilliseconds( 1000 );
                            Servo::disconnect();
                            break;

                        case Keypad::kButton_Right:
                            Lcd::displayTopRow( "Servo to R-10" );
                            direction += 10;
                            Servo::slew( direction );
                            Lcd::setCursor( 1, 0 );
                            Lcd::print( Servo::getCurrentAngle() );
                            break;

                        case Keypad::kButton_Down:
                            Lcd::displayTopRow( "Slew to 0" );
                            direction = 0;
                            Servo::slew( direction );
                            Lcd::setCursor( 1, 0 );
                            Lcd::print( Servo::getCurrentAngle() );
                            break;

                        case Keypad::kButton_Up:
                            Lcd::displayTopRow( "Servo to L-1" );
                            direction -= 1;
                            Servo::slew( direction );
                            Lcd::setCursor( 1, 0 );
                            Lcd::print( Servo::getCurrentAngle() );
                            break;

                        case Keypad::kButton_Left:
                            Lcd::displayTopRow( "Servo to L-10" );
                            direction -= 10;
                            Servo::slew( direction );
                            Lcd::setCursor( 1, 0 );
                            Lcd::print( Servo::getCurrentAngle() );
                            break;

                        case Keypad::kChord_Reset:
                            Lcd::displayTopRow( "Reset (chord)" );
                            break;

                        case Keypad::kChord_Pause:
                            Lcd::displayTopRow( "Pause (chord)" );
                            break;

                        case Keypad::kChord_Continue:
                            Lcd::displayBottomRow( "Continue (chord)" );
                            break;

                        case Keypad::kChord_A:
                            Lcd::displayTopRow( "A (chord)" );
                            break;

                        case Keypad::kChord_B:
                            Lcd::displayTopRow( "B (chord)" );
                            break;

                        case Keypad::kChord_C:
                            Lcd::displayTopRow( "C (chord)" );
                            break;

                        case Keypad::kChord_D:
                            Lcd::displayTopRow( "D (chord)" );
                            break;

                        default:
                            Lcd::displayTopRow( "???" );
                            break;
                    }
                }

                Clock::delayMilliseconds( 25 );
            }

            catch( const I2c::I2cError& err )
            {
                if ( err.errorCode() % 100 == -82 || err.errorCode() % 100 == -83 )
                {
                    // Just continue
                    std::cerr << "Continuing... Error: " << err.errorCode() << ", " << err.what() << std::endl;
                }
                else
                {
                    throw;
                }
            }
        }
    }

    catch ( const CarrtError& err )
    {
        std::cerr << "Error: " << err.errorCode() << ", " << err.what() << std::endl;
    }

    catch ( const std::exception& err )
    {
        std::cerr << "Error: " << err.what() << std::endl;
    }

    catch (...)
    {
        std::cerr << "Error of unknown type." << std::endl;
    }

    Lcd::setBacklight( Lcd::kBacklight_Off );
    Lcd::displayOff();
    Servo::reset();

    std::cout << "Test ended" << std::endl;

    gpioTerminate();
}
