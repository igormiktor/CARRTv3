/*
    LcdOff.cpp - Turn off the LCD display.

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

#include "CarrtError.h"
#include "CarrtPigpio.h"
#include "Clock.h"
#include "DebugUtils.hpp"
#include "Lcd.h"

int main()
{
    try
    {
        gpioInitialise();

        Clock::initSystemClock();

        Lcd::init();

        Lcd::displayOn();
        Lcd::setBacklight( Lcd::kBacklight_Off );
        Lcd::displayOff();

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

    catch ( ... )
    {
        std::cerr << "Error of unknown type." << std::endl;
    }

    gpioTerminate();
}
