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



PicoState* PicoState::mTheState{ nullptr };


PicoState* PicoState::access()
{
    if ( !mTheState )
    {
        mTheState = new PicoState();
    }

    return mTheState;
}


PicoState::PicoState() noexcept
: mSendTimerEvents{ false },
  mSendNavEvents{ false },
  mSendEncoderEvents{ false }
{
    // Nothing else to do  
}




#if 0       // Comment out because inlined
bool PicoState::sendTimerEvents( bool newVal ) noexcept
{
    bool oldVal = mSendTimerEvents;
    mSendTimerEvents = newVal;
    return oldVal;
}

bool PicoState::wantTimerEvents() noexcept
{
    return mSendTimerEvents;
}

bool PicoState::sendNavEvents( bool newVal ) noexcept
{
    bool oldVal = mSendNavEvents;
    mSendNavEvents = newVal;
    return oldVal;
}

bool PicoState::wantNavEvents() noexcept
{
    return mSendNavEvents;
}

bool PicoState::sendEncoderEvents( bool newVal ) noexcept
{
    bool oldVal = mSendEncoderEvents;
    mSendEncoderEvents = newVal;
    return oldVal;
}

bool PicoState::wantEncoderEvents() noexcept
{
    return mSendEncoderEvents;
}

inline void PicoState::allSendEventOn() noexcept
{
    mSendTimerEvents = true;
    mSendNavEvents = true;
    mSendEncoderEvents = true;
}

inline void PicoState::allSendEventsOff() noexcept
{
    mSendTimerEvents = false;
    mSendNavEvents = false;
    mSendEncoderEvents = false;
}
#endif  // #if 0 becuase of inlining


