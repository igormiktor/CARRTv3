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




#include <iostream>

#include "Drivers/Clock.h"
#include "Utils/CarrtError.h"

#include "Drivers/Motors.h"


int main()
{
    const long kThreeMinutesInMillis = 3 * 60 *1000L;

    try
    {
        Clock::initSystemClock();

        Motors::init();

        Clock::delayMilliseconds(1000);

        long endTime = Clock::millis() + kThreeMinutesInMillis;
        while ( 1 && Clock::millis() < endTime )
        {
            Motors::goForward();
            Clock::delayMilliseconds( 5000 );

            Motors::stop();
            Clock::delayMilliseconds( 1000 );

            Motors::goBackward();
            Clock::delayMilliseconds( 5000 );

            Motors::stop();
            Clock::delayMilliseconds( 1000 );

            Motors::rotateLeft();
            Clock::delayMilliseconds( 5000 );

            Motors::stop();
            Clock::delayMilliseconds( 1000 );

            Motors::rotateRight();
            Clock::delayMilliseconds( 5000 );

            Motors::stop();
            Clock::delayMilliseconds( 1000 );
        }

        std::cout << "Test complete" << std::endl;
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
}
