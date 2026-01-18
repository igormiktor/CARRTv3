/*
    MotorTestCollective.cpp - Testing harness for the motor driver.

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

#include <chrono>
#include <iostream>

#include "CarrtError.h"
#include "CarrtPigpio.h"
#include "Clock.h"
#include "DebugUtils.hpp"
#include "Motors.h"

int main()
{
    const std::chrono::milliseconds kThreeMinutesInMillis{ 3min };
    const std::chrono::milliseconds kRunTimeInMillis{ 5s };
    const std::chrono::milliseconds kPauseTimeInMillis{ 3s };

    try
    {
        gpioInitialise();
        std::cerr << "GPIO initialized" << std::endl;

        Clock::initSystemClock();
        std::cerr << "Clock initialized" << std::endl;

        Motors::init();
        std::cerr << "Motors initialized" << std::endl;

        Clock::sleep( 2 * kPauseTimeInMillis );

        long endTime = Clock::millis() + kThreeMinutesInMillis.count();
        while ( 1 && Clock::millis() < endTime )
        {
            std::cerr << "Motor loop start" << std::endl;

            Motors::goForward();
            Clock::sleep( kRunTimeInMillis );

            Motors::stop();
            Clock::sleep( kPauseTimeInMillis );

            Motors::goBackward();
            Clock::sleep( kRunTimeInMillis );

            Motors::stop();
            Clock::sleep( kPauseTimeInMillis );

            Motors::rotateLeft();
            Clock::sleep( kRunTimeInMillis );

            Motors::stop();
            Clock::sleep( kPauseTimeInMillis );

            Motors::rotateRight();
            Clock::sleep( kRunTimeInMillis );

            Motors::stop();
            Clock::sleep( kPauseTimeInMillis );

            std::cerr << "Motor loop end" << std::endl;
        }

        std::cerr << "Test complete" << std::endl;
    }

    catch ( const CarrtError& err )
    {
        std::cerr << "Error: " << err.errorCode() << ", " << err.what() << std::endl;
    }

    catch ( const std::exception& err )
    {
        std::cerr << "Error: " << err.what() << std::endl;
    }

    catch ( ... )
    {
        std::cerr << "Error of unknown type." << std::endl;
    }

    gpioTerminate();
}
