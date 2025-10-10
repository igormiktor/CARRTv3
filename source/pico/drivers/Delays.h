/*
    Delays.h - Delays for the CARRT based on Pico. 

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



#ifndef Delays_h
#define Delays_h

#include <chrono>
#include "pico/time.h"

// Required to use chrono-related literals
using namespace std::chrono_literals;

namespace CarrtPico
{

    inline void delay( const std::chrono::milliseconds& howLong )
    {
        sleep_ms( howLong.count() );
    }


    inline void sleep( const std::chrono::milliseconds& howLong )
    {
        sleep_ms( howLong.count() );
    }

};



#endif // Delays_h

