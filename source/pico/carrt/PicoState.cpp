/*
    PicoState.cpp - System-wide state information for CARRT-Pico

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

#include "PicoState.h"

#include "CoreAtomic.hpp"

namespace
{
    // These all happen in Core0 so no atomics needed
    bool sSendQtrSecTimerMsgs{ false };
    bool sSend1SecTimerMsgs{ false };
    bool sSend8SecTimerMsgs{ false };
    bool sSendNavMsgs{ false };
    bool sSendNavStatusMsgs{ false };
    bool sSendEncoderMsgs{ false };
    bool sSendCalibrationMsgs{ false };
    bool sSendBatteryMsgs{ false };

    bool sStartUpFinished{ false };
    bool sNavCalibrated{ false };
    bool sAutoCalibrateMode{ false };

    // These are shared Core0 and Core1 and require atomics
    CoreAtomic::CAtomic<bool> sInCalibrationMode{ false };
}    // namespace

// clang-format off
void PicoState::initialize() noexcept
{
    sSendQtrSecTimerMsgs    = false;
    sSend1SecTimerMsgs      = false;
    sSend8SecTimerMsgs      = false;
    sSendNavMsgs            = false;
    sSendNavStatusMsgs      = false;
    sSendEncoderMsgs        = false;
    sSendCalibrationMsgs    = false;
    sSendBatteryMsgs        = false;

    sStartUpFinished        = false;

    sNavCalibrated          = false;
    
    sAutoCalibrateMode      = false;
    sInCalibrationMode      = false;
}
// clang-format on

////////////////////////////////////////////////////////////////////////////////

bool PicoState::startUpFinished( bool newVal ) noexcept
{
    bool oldVal = sStartUpFinished;
    sStartUpFinished = newVal;
    return oldVal;
}

bool PicoState::startUpFinished() noexcept { return sStartUpFinished; }

void PicoState::sendAllTimerMsgs( bool newVal ) noexcept
{
    sSendQtrSecTimerMsgs = newVal;
    sSend1SecTimerMsgs = newVal;
    sSend8SecTimerMsgs = newVal;
}

bool PicoState::sendQtrSecTimerMsgs( bool newVal ) noexcept
{
    bool oldVal = sSendQtrSecTimerMsgs;
    sSendQtrSecTimerMsgs = newVal;
    return oldVal;
}

bool PicoState::wantQtrSecTimerMsgs() noexcept { return sSendQtrSecTimerMsgs; }

bool PicoState::send1SecTimerMsgs( bool newVal ) noexcept
{
    bool oldVal = sSend1SecTimerMsgs;
    sSend1SecTimerMsgs = newVal;
    return oldVal;
}

bool PicoState::want1SecTimerMsgs() noexcept { return sSend1SecTimerMsgs; }

bool PicoState::send8SecTimerMsgs( bool newVal ) noexcept
{
    bool oldVal = sSend8SecTimerMsgs;
    sSend8SecTimerMsgs = newVal;
    return oldVal;
}

bool PicoState::want8SecTimerMsgs() noexcept { return sSend8SecTimerMsgs; }

bool PicoState::sendNavMsgs( bool newVal ) noexcept
{
    bool oldVal = sSendNavMsgs;
    sSendNavMsgs = newVal;
    return oldVal;
}

bool PicoState::wantNavMsgs() noexcept { return sSendNavMsgs; }

bool PicoState::sendNavStatusMsgs( bool newVal ) noexcept
{
    bool oldVal = sSendNavStatusMsgs;
    sSendNavStatusMsgs = newVal;
    return oldVal;
}

bool PicoState::wantNavStatusMsgs() noexcept { return sSendNavStatusMsgs; }

bool PicoState::sendEncoderMsgs( bool newVal ) noexcept
{
    bool oldVal = sSendEncoderMsgs;
    sSendEncoderMsgs = newVal;
    return oldVal;
}

bool PicoState::wantEncoderMsgs() noexcept { return sSendEncoderMsgs; }

bool PicoState::sendCalibrationMsgs( bool newVal ) noexcept
{
    bool oldVal = sSendCalibrationMsgs;
    sSendCalibrationMsgs = newVal;
    return oldVal;
}

bool PicoState::wantCalibrationMsgs() noexcept { return sSendEncoderMsgs; }

bool PicoState::sendBatteryMsgs( bool newVal ) noexcept
{
    bool oldVal = sSendBatteryMsgs;
    sSendBatteryMsgs = newVal;
    return oldVal;
}

bool PicoState::wantBatteryMsgs() noexcept { return sSendBatteryMsgs; }

// clang-format off
void PicoState::allMsgsSendOn() noexcept
{
    sSendQtrSecTimerMsgs    = true;
    sSend1SecTimerMsgs      = true;
    sSend8SecTimerMsgs      = true;
    sSendNavMsgs            = true;
    sSendNavStatusMsgs      = true;
    sSendEncoderMsgs        = true;
    sSendCalibrationMsgs    = true;
    sSendBatteryMsgs        = true;
}
// clang-format on

// clang-format off
void PicoState::allMsgsSendOff() noexcept
{
    sSendQtrSecTimerMsgs    = false;
    sSend1SecTimerMsgs      = false;
    sSend8SecTimerMsgs      = false;
    sSendNavMsgs            = false;
    sSendNavStatusMsgs      = false;
    sSendEncoderMsgs        = false;
    sSendCalibrationMsgs    = false;
    sSendBatteryMsgs        = false;
}
// clang-format on

bool PicoState::calibrationInProgress() noexcept { return sInCalibrationMode; }

bool PicoState::calibrationInProgress( bool newVal ) noexcept
{
    bool oldVal = sInCalibrationMode;
    sInCalibrationMode = newVal;
    return oldVal;
}

bool PicoState::navCalibrated() noexcept { return sNavCalibrated; }

bool PicoState::navCalibrated( bool newVal ) noexcept
{
    bool oldVal = sNavCalibrated;
    sNavCalibrated = newVal;
    return oldVal;
}

bool PicoState::wantAutoCalibrate() noexcept { return sAutoCalibrateMode; }

bool PicoState::setAutoCalibrate( bool newVal ) noexcept
{
    bool oldVal = sAutoCalibrateMode;
    sAutoCalibrateMode = newVal;
    return oldVal;
}
