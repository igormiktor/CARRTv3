/*
    KeypadTest.cpp - Testing harness for the keypad driver.

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

#include "CarrtPigpio.h"

#include "Clock.h"
#include "Keypad.h"
#include "Lcd.h"
#include "I2c.h"

#include "CarrtError.h"

#include "DebugUtils.hpp"

const long kThreeMinutesInMillis = 3 * 60 *1000L;


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
        uint8_t sPreviousButtonHit{ 0 }; 

        Lcd::displayTopRow( "Button hit:" );

        long endTime = Clock::millis() + kThreeMinutesInMillis;
        while ( 1 && Clock::millis() < endTime )
        {
            try
            {
                uint8_t buttonHit = Keypad::readButtons();

                auto currentMillis{ Clock::millis() };
                bool mightBeChord{ buttonHit && buttonHit != sPreviousButtonHit && currentMillis <= sNextTimeButtonClickAccepted };
                if ( mightBeChord )
                {
                    // Fake a chord and fake the time so we accept it
                    debugM( "Composite chord" );
                    debugV( (int) buttonHit, (int) sPreviousButtonHit );
                    buttonHit |= sPreviousButtonHit;
                    currentMillis = sNextTimeButtonClickAccepted + 1;
                }
                if ( buttonHit && currentMillis > sNextTimeButtonClickAccepted )
                {
                    // Accept the button click
                    sPreviousButtonHit = buttonHit;

                    // Rollover happens in about 50 days, so don't worry about it
                    sNextTimeButtonClickAccepted = Clock::millis() + kMinTimeBetweenButtonChecks;

                    Clock::delayMilliseconds( 25 );
                    Lcd::clearBottomRow();

                    switch ( buttonHit )
                    {
                        case Keypad::kButton_Select:
                            Lcd::displayBottomRow( "Select" );
                            break;

                        case Keypad::kButton_Right:
                            Lcd::displayBottomRow( "Right" );
                            break;

                        case Keypad::kButton_Down:
                            Lcd::displayBottomRow( "Down" );
                            break;

                        case Keypad::kButton_Up:
                            Lcd::displayBottomRow( "Up" );
                            break;

                        case Keypad::kButton_Left:
                            Lcd::displayBottomRow( "Left" );
                            break;

                        case Keypad::kChord_Reset:
                            Lcd::displayBottomRow( "Reset (chord)" );
                            break;

                        case Keypad::kChord_Reboot:
                            Lcd::displayBottomRow( "Reboot (chord)" );
                            break;

                        case Keypad::kChord_Shutdown:
                            Lcd::displayBottomRow( "Shutdown (chord)" );
                            break;

                        case Keypad::kChord_Pico_OnOff:
                            Lcd::displayBottomRow( "Pico 0/1 (chord)" );
                            break;

                        case Keypad::kChord_Pause:
                            Lcd::displayBottomRow( "Pause (chord)" );
                            break;

                        case Keypad::kChord_Continue:
                            Lcd::displayBottomRow( "Continue (chord)" );
                            break;

                        case Keypad::kChord_A:
                            Lcd::displayBottomRow( "A (chord)" );
                            break;

                        case Keypad::kChord_B:
                            Lcd::displayBottomRow( "B (chord)" );
                            break;

                        case Keypad::kChord_C:
                            Lcd::displayBottomRow( "C (chord)" );
                            break;

                        case Keypad::kChord_D:
                            Lcd::displayBottomRow( "D (chord)" );
                            break;

                        default:
                            Lcd::displayBottomRow( "???" );
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

    std::cout << "Test ended" << std::endl;

    gpioTerminate();
}
