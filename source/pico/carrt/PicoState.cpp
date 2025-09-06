/*
    PicoState.cpp - System-wide state information for CARRT-Pico

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


#include "PicoState.h"

#include "CoreAtomic.hpp"



namespace 
{
    // These all happen in Core1 so no atomics needed
    bool  sSendTimerEvents;
    bool  sSendNavEvents;
    bool  sSendEncoderEvents;

}





bool PicoState::sendTimerEvents( bool newVal ) noexcept
{
    bool oldVal = sSendTimerEvents;
    sSendTimerEvents = newVal;
    return oldVal;
}

bool PicoState::wantTimerEvents() noexcept
{
    return sSendTimerEvents;
}

bool PicoState::sendNavEvents( bool newVal ) noexcept
{
    bool oldVal = sSendNavEvents;
    sSendNavEvents = newVal;
    return oldVal;
}

bool PicoState::wantNavEvents() noexcept
{
    return sSendNavEvents;
}

bool PicoState::sendEncoderEvents( bool newVal ) noexcept
{
    bool oldVal = sSendEncoderEvents;
    sSendEncoderEvents = newVal;
    return oldVal;
}

bool PicoState::wantEncoderEvents() noexcept
{
    return sSendEncoderEvents;
}

void PicoState::allSendEventOn() noexcept
{
    sSendTimerEvents = true;
    sSendNavEvents = true;
    sSendEncoderEvents = true;
}

void PicoState::allSendEventsOff() noexcept
{
    sSendTimerEvents = false;
    sSendNavEvents = false;
    sSendEncoderEvents = false;
}



