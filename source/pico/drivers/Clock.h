/*
    Clock.h - Delay and clock related functions for the CARRT based on Pico.

    Copyright (c) 2026 Igor Mikolic-Torreira.  All right reserved.

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

#ifndef Clock_h
#define Clock_h

#include <pico/time.h>

#include <chrono>

// Required to use chrono-related literals
using namespace std::chrono_literals;

namespace Clock
{

    inline void delay( const std::chrono::microseconds& howLong )
    {
        sleep_us( howLong.count() );
    }

    inline void sleep( const std::chrono::microseconds& howLong )
    {
        sleep_us( howLong.count() );
    }

    inline std::uint32_t millis()
    {
        return to_ms_since_boot( get_absolute_time() );
    }

    inline std::chrono::milliseconds elapsedMilliseconds()
    {
        return std::chrono::milliseconds{ Clock::millis() };
    }

    inline std::chrono::microseconds elapsedMicroseconds()
    {
        return std::chrono::microseconds{
            to_us_since_boot( get_absolute_time() ) };
    }

};    // namespace Clock

#endif    // Clock_h
