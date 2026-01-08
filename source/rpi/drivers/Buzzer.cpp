/*
    Buzzer.cpp - Driver for CARRT's simple audio system

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



#include "Buzzer.h"


#include "CarrtError.h"
#include "CarrtPigpio.h"
#include "CarrtPinAssignments.h"
#include "Clock.h"

#include "DebugUtils.hpp"



namespace Buzzer
{
    constexpr unsigned int pBuzzerPin = GpioPins::kBuzzerPin;       

    constexpr unsigned int kDutyCycle = 2*500000UL;
}


// Only check for PWM errors on the first call in each function (assume if first call works, so will subsequent)



void Buzzer::initBuzzer()
{
    chirp();
}


void Buzzer::beep( const std::chrono::milliseconds& durationMs, unsigned int tone )
{
    auto err = gpioHardwarePWM( pBuzzerPin, tone, kDutyCycle );
    if ( err )
    {
        throw CarrtError( makeRpi0ErrorId( kBuzzerError, 1, err ), "Hardware PWM error in Buzzer module" );
    }
    Clock::sleep( durationMs );
    gpioHardwarePWM( pBuzzerPin, 0, 0 );
}


void Buzzer::chirp()
{
    auto err = gpioHardwarePWM( pBuzzerPin, kBeepDefaultChirpTone, kDutyCycle );
    if ( err )
    {
        throw CarrtError( makeRpi0ErrorId( kBuzzerError, 2, err ), "Hardware PWM error in Buzzer module" );
    }
    Clock::sleep( kBeepDefaultChirpDuration );
    gpioHardwarePWM( pBuzzerPin, 0, 0 );
}


void Buzzer::errorChime()
{
    beep( 50ms );
    Clock::sleep( 50ms );
    beep( 50ms );
    Clock::sleep( 50ms );
    beep( 50ms );
}


void Buzzer::triTone( unsigned int tone1, unsigned int tone2, unsigned int tone3 )
{
    auto err = gpioHardwarePWM( pBuzzerPin, tone1, kDutyCycle );
    if ( err )
    {
        throw CarrtError( makeRpi0ErrorId( kBuzzerError, 3, err ), "Hardware PWM error in Buzzer module" );
    }
    Clock::sleep( 50ms );
    gpioHardwarePWM( pBuzzerPin, 0, 0 );

    Clock::sleep( 75ms );

    gpioHardwarePWM( pBuzzerPin, tone2, kDutyCycle );
    Clock::sleep( 100ms );
    gpioHardwarePWM( pBuzzerPin, 0, 0 );

    Clock::sleep( 75ms );

    gpioHardwarePWM( pBuzzerPin, tone3, kDutyCycle );
    Clock::sleep( 150ms );
    gpioHardwarePWM( pBuzzerPin, 0, 0 );
}


void Buzzer::beepOn( unsigned int tone )
{
    auto err = gpioHardwarePWM( pBuzzerPin, tone, kDutyCycle );
    if ( err )
    {
        throw CarrtError( makeRpi0ErrorId( kBuzzerError, 4, err ), "Hardware PWM error in Buzzer module" );
    }
}


void Buzzer::beepOff()
{
    auto err = gpioHardwarePWM( pBuzzerPin, 0, 0 );
    if ( err )
    {
        throw CarrtError( makeRpi0ErrorId( kBuzzerError, 5, err ), "Hardware PWM error in Buzzer module" );
    }
}
