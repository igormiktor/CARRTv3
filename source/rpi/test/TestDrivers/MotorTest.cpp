/*
    MotorTestIndividual.cpp - Testing harness for the CARRT3 motor driver.

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
#include "CarrtError.h"

#include "Motors.h"

#include "DebugUtils.hpp"



int main()
{
    const long kThreeMinutesInMillis = 3 * 60 *1000L;

    try
    {
        gpioInitialise();
        std::cerr << "GPIO initialized" << std::endl;

        Clock::initSystemClock();
        std::cerr << "GPIO initialized" << std::endl;

        Motors::init();
        std::cerr << "GPIO initialized" << std::endl;

        Clock::sleep( 3ms );

        std::cerr<< "Starting motor loop" << std::endl;
        long endTime = Clock::millis() + kThreeMinutesInMillis;
        while ( 1 && Clock::millis() < endTime )
        {
            Clock::sleep( 1s );
            Motors::runRearRightMotor( Motors::kCmdForward );
            Clock::sleep( 5s );
            Motors::runRearRightMotor( Motors::kCmdRelease );

            Clock::sleep( 1s );
            Motors::runRearLeftMotor( Motors::kCmdForward );
            Clock::sleep( 5s );
            Motors::runRearLeftMotor( Motors::kCmdRelease );

            Clock::sleep( 1s );
            Motors::runFrontRightMotor( Motors::kCmdForward );
            Clock::sleep( 5s );
            Motors::runFrontRightMotor( Motors::kCmdRelease );

            Clock::sleep( 1s );
            Motors::runFrontLeftMotor( Motors::kCmdForward );
            Clock::sleep( 5s );
            Motors::runFrontLeftMotor( Motors::kCmdRelease );

            Clock::sleep( 5s );

            Clock::sleep( 1s );
            Motors::runRearRightMotor( Motors::kCmdBackward );
            Clock::sleep( 5s );
            Motors::runRearRightMotor( Motors::kCmdRelease );

            Clock::sleep( 1s );
            Motors::runRearLeftMotor( Motors::kCmdBackward );
            Clock::sleep( 5s );
            Motors::runRearLeftMotor( Motors::kCmdRelease );

            Clock::sleep( 1s );
            Motors::runFrontRightMotor( Motors::kCmdBackward );
            Clock::sleep( 5s );
            Motors::runFrontRightMotor( Motors::kCmdRelease );

            Clock::sleep( 1s );
            Motors::runFrontLeftMotor( Motors::kCmdBackward );
            Clock::sleep( 5s );
            Motors::runFrontLeftMotor( Motors::kCmdRelease );

            Clock::sleep( 5s );
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

    gpioTerminate();
}
