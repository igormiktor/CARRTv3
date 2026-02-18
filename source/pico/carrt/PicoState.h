/*
    PicoState.h - System-wide state information for CARRT-Pico
    Any atomics to be shared between cores are shared here.

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

#ifndef PicoState_h
#define PicoState_h

namespace PicoState
{

    void initialize() noexcept;    // Initialize stuff

    // Have we finished start up of Pico
    // Everything through BNO initialized, but not necessarily calibrated)
    bool startUpFinished() noexcept;                 // Returns value
    bool startUpFinished( bool newval ) noexcept;    // Returns prior value

    // Sets all three Timer msgs to "send" at once
    void sendAllTimerMsgs( bool newVal ) noexcept;

    bool wantQtrSecTimerMsgs() noexcept;                 // Returns value
    bool sendQtrSecTimerMsgs( bool newVal ) noexcept;    // Returns prior value

    bool want1SecTimerMsgs() noexcept;                 // Returns value
    bool send1SecTimerMsgs( bool newVal ) noexcept;    // Returns prior value

    bool want8SecTimerMsgs() noexcept;                 // Returns value
    bool send8SecTimerMsgs( bool newVal ) noexcept;    // Returns prior value

    bool wantNavMsgs() noexcept;                 // Returns value
    bool sendNavMsgs( bool newVal ) noexcept;    // Returns prior value

    bool wantNavStatusMsgs() noexcept;                 // Returns value
    bool sendNavStatusMsgs( bool newVal ) noexcept;    // Returns prior value

    bool wantEncoderMsgs() noexcept;                 // Returns value
    bool sendEncoderMsgs( bool newVal ) noexcept;    // Returns prior value

    bool wantCalibrationMsgs() noexcept;                 // Returns value
    bool sendCalibrationMsgs( bool newVal ) noexcept;    // Returns prior value

    bool wantBatteryMsgs() noexcept;                 // Returns value
    bool sendBatteryMsgs( bool newVal ) noexcept;    // Returns prior value

    void allMsgsSendOn() noexcept;
    void allMsgsSendOff() noexcept;

    // Return status of nav calibration
    bool navCalibrated() noexcept;
    bool navCalibrated( bool newVal ) noexcept;    // Returns prior value

    // Are we in calibration mode?
    bool calibrationInProgress() noexcept;
    bool calibrationInProgress( bool newVal ) noexcept;    // Rtn prior value

    // Do we auto enter calibration mode whenever needed?
    bool wantAutoCalibrate() noexcept;
    bool setAutoCalibrate( bool newVal ) noexcept;    // Returns prior value

};    // namespace PicoState

#endif    // PicoState_h
