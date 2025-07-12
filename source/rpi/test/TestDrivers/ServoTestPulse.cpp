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
        Lcd::displayBottomRow( "Pulse version" );

        std::uint16_t pulseLen{ Servo::getPulseLen() };
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
                            pulseLen = 341;
                            Servo::setPulseLen( pulseLen );
                            loopAgain = false;
                            Clock::delayMilliseconds( 1000 );
                            break;

                        case Keypad::kButton_Right:
                            Lcd::displayTopRow( "Servo to R+10" );
                            pulseLen -= 10;
                            Servo::setPulseLen( pulseLen );
                            Lcd::setCursor( 1, 0 );
                            Lcd::print( Servo::getPulseLen() );
                            break;

                        case Keypad::kButton_Down:
                            Lcd::displayTopRow( "Servo to R+11" );
                            pulseLen -= 1;;
                            Servo::setPulseLen( pulseLen );
                            Lcd::setCursor( 1, 0 );
                            Lcd::print( Servo::getPulseLen() );
                            break;

                        case Keypad::kButton_Up:
                            Lcd::displayTopRow( "Servo to L+1" );
                            pulseLen += 1;
                            Servo::setPulseLen( pulseLen );
                            Lcd::setCursor( 1, 0 );
                            Lcd::print( Servo::getPulseLen() );
                            break;

                        case Keypad::kButton_Left:
                            Lcd::displayTopRow( "Servo to L+10" );
                            pulseLen += 10;
                            Servo::setPulseLen( pulseLen );
                            Lcd::setCursor( 1, 0 );
                            Lcd::print( Servo::getPulseLen() );
                            break;

                        case Keypad::kChord_Reset:
                        case Keypad::kChord_Reboot:
                        case Keypad::kChord_Shutdown:
                        case Keypad::kChord_Pico_OnOff:
                        case Keypad::kChord_Pause:
                        case Keypad::kChord_Continue:
                        case Keypad::kChord_A:
                        case Keypad::kChord_B:
                        case Keypad::kChord_C:
                        case Keypad::kChord_D:
                            Lcd::displayTopRow( "Servo to front" );
                            pulseLen = 341;
                            Servo::setPulseLen( pulseLen );
                            Lcd::setCursor( 1, 0 );
                            Lcd::print( Servo::getPulseLen() );
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
