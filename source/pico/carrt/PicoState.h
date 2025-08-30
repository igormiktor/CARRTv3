/*
    PicoState.h - System-wide state information for CARRT-Pico (singleton class)

    Put any atomics to be shared between cores here.

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


#ifndef PicoState_h
#define PicoState_h


#include "CoreAtomic.hpp"


class PicoState
{
public:

    static PicoState* access();

    bool sendTimerEvents( bool newVal ) noexcept;       // Returns prior value
    bool wantTimerEvents() noexcept;             // Returns value

    bool sendNavEvents( bool newVal  ) noexcept;        // Returns prior value
    bool wantNavEvents() noexcept;               // Returns value

    bool sendEncoderEvents( bool newVal  ) noexcept;    // Returns prior value
    bool wantEncoderEvents() noexcept;          // Returns value

    void allSendEventOn() noexcept;
    void allSendEventsOff() noexcept;

protected:

    PicoState() noexcept;


private:

    // These all happen in Core1 so no atomics needed
    bool  mSendTimerEvents;
    bool  mSendNavEvents;
    bool  mSendEncoderEvents;

    static PicoState* mTheState;
};



inline bool PicoState::sendTimerEvents( bool newVal ) noexcept
{
    bool oldVal = mSendTimerEvents;
    mSendTimerEvents = newVal;
    return oldVal;
}

inline bool PicoState::wantTimerEvents() noexcept
{
    return mSendTimerEvents;
}

inline bool PicoState::sendNavEvents( bool newVal ) noexcept
{
    bool oldVal = mSendNavEvents;
    mSendNavEvents = newVal;
    return oldVal;
}

inline bool PicoState::wantNavEvents() noexcept
{
    return mSendNavEvents;
}

inline bool PicoState::sendEncoderEvents( bool newVal ) noexcept
{
    bool oldVal = mSendEncoderEvents;
    mSendEncoderEvents = newVal;
    return oldVal;
}

inline bool PicoState::wantEncoderEvents() noexcept
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


#endif  // PicoState_h
