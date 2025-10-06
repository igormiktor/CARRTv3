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



namespace PicoState
{

    void initialize() noexcept;                             // Initialize stuff

    bool sendTimerEvents( bool newVal ) noexcept;           // Returns prior value
    bool wantTimerEvents() noexcept;                        // Returns value

    bool sendNavEvents( bool newVal  ) noexcept;            // Returns prior value
    bool wantNavEvents() noexcept;                          // Returns value

    bool sendEncoderEvents( bool newVal  ) noexcept;        // Returns prior value
    bool wantEncoderEvents() noexcept;                      // Returns value

    void allSendEventOn() noexcept;
    void allSendEventsOff() noexcept;

    bool navCalibrated() noexcept;                          // Return status of nav calibration
    bool navCalibrated( bool newVal ) noexcept;             // Set value of nav calibration; returns prior value

    bool calibrationInProgress() noexcept;                  // Are we in calibration mode
    bool calibrationInProgress( bool newVal ) noexcept;     // Set value of being in calibration mode; returns prior value

};




#endif  // PicoState_h
