/*
    Buzzer.h - Driver for CARRT's simple audio system (a PWM buzzer)

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


#ifndef Buzzer_h
#define Buzzer_h

#include <chrono>

using namespace std::chrono_literals;

namespace Buzzer
{

    constexpr unsigned int                  kBeepDefaultBeepTone{ 40 };
    constexpr std::chrono::milliseconds     kBeepDefaultBeepDuration{ 100ms };
    constexpr unsigned int                  kBeepDefaultChirpTone{ 100 };
    constexpr std::chrono::milliseconds     kBeepDefaultChirpDuration{ 20ms };

    constexpr unsigned int                  kBeepReadyTriTone1{ 0 };
    constexpr unsigned int                  kBeepReadyTriTone2{ 200 };
    constexpr unsigned int                  kBeepReadyTriTone3{ 100 };

    constexpr unsigned int                  kBeepCollisionTriTone1{ 100 };
    constexpr unsigned int                  kBeepCollisionTriTone2{ 10 };
    constexpr unsigned int                  kBeepCollisionTriTone3{ 200 };



    void initBuzzer();

    // Decide what to do
    void alert( const std::chrono::milliseconds& duration = kBeepDefaultBeepDuration, unsigned int tone = kBeepDefaultBeepTone );

    void beep( const std::chrono::milliseconds& duration = kBeepDefaultBeepDuration, unsigned int tone = kBeepDefaultBeepTone );

    void chirp();

    void errorChime();

    void triTone( unsigned int tone1, unsigned int tone2, unsigned int tone3 );

    inline void readyChime()   { triTone( kBeepReadyTriTone1, kBeepReadyTriTone2, kBeepReadyTriTone3 ); }

    inline void collisionChime()    { triTone( kBeepCollisionTriTone1, kBeepCollisionTriTone2, kBeepCollisionTriTone3 ); }

    void beepOn( unsigned int tone = kBeepDefaultBeepTone );

    void beepOff();

}


#endif  // Buzzer_h

