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
    // These all happen in Core0 so no atomics needed
    bool  sStartUpFinished{ false };
    bool  sSendTimerMsgs{ false };
    bool  sSendNavMsgs{ false };
    bool  sSendNavStatusMsgs{ false };
    bool  sSendEncoderMsgs{ false };
    bool  sSendCalibrationMsgs{ false };
    bool  sNavCalibrated{ false };
    bool  sAutoCalibrateMode{ false }; 

    // These are shared Core0 and Core1 and require atomics
    CoreAtomic::CAtomic<bool> sInCalibrationMode{ false };
}



void PicoState::initialize() noexcept
{
    sStartUpFinished        = false;
    sSendTimerMsgs          = false;
    sSendNavMsgs            = false;
    sSendNavStatusMsgs      = false;
    sSendEncoderMsgs        = false;
    sSendCalibrationMsgs    = false;
    sNavCalibrated          = false;
    sAutoCalibrateMode      = false;
    
    sInCalibrationMode      = false;
}


bool PicoState::startUpFinished( bool newVal ) noexcept
{
    bool oldVal = sStartUpFinished;
    sStartUpFinished = newVal;
    return oldVal;
}

bool PicoState::startUpFinished() noexcept
{
    return sStartUpFinished;
}


bool PicoState::sendTimerMsgs( bool newVal ) noexcept
{
    bool oldVal = sSendTimerMsgs;
    sSendTimerMsgs = newVal;
    return oldVal;
}

bool PicoState::wantTimerMsgs() noexcept
{
    return sSendTimerMsgs;
}


bool PicoState::sendNavMsgs( bool newVal ) noexcept
{
    bool oldVal = sSendNavMsgs;
    sSendNavMsgs = newVal;
    return oldVal;
}

bool PicoState::wantNavMsgs() noexcept
{
    return sSendNavMsgs;
}


bool PicoState::sendNavStatusMsgs( bool newVal ) noexcept
{
    bool oldVal = sSendNavStatusMsgs;
    sSendNavStatusMsgs = newVal;
    return oldVal;
}

bool PicoState::wantNavStatusMsgs() noexcept
{
    return sSendNavStatusMsgs;
}


bool PicoState::sendEncoderMsgs( bool newVal ) noexcept
{
    bool oldVal = sSendEncoderMsgs;
    sSendEncoderMsgs = newVal;
    return oldVal;
}

bool PicoState::wantEncoderMsgs() noexcept
{
    return sSendEncoderMsgs;
}


bool PicoState::sendCalibrationMsgs( bool newVal ) noexcept
{
    bool oldVal = sSendCalibrationMsgs;
    sSendCalibrationMsgs = newVal;
    return oldVal;
}

bool PicoState::wantCalibrationMsgs() noexcept
{
    return sSendEncoderMsgs;
}


void PicoState::allMsgsSendOn() noexcept
{
    sSendTimerMsgs          = true;
    sSendNavMsgs            = true;
    sSendEncoderMsgs        = true;
    sSendCalibrationMsgs    = true;
}

void PicoState::allMsgsSendOff() noexcept
{
    sSendTimerMsgs          = false;
    sSendNavMsgs            = false;
    sSendEncoderMsgs        = false;
    sSendCalibrationMsgs    = false;
}


bool PicoState::calibrationInProgress() noexcept
{
    return sInCalibrationMode;
}

bool PicoState::calibrationInProgress( bool newVal ) noexcept
{
    bool oldVal = sInCalibrationMode;
    sInCalibrationMode = newVal;
    return oldVal;
}


bool PicoState::navCalibrated() noexcept    
{
    return sNavCalibrated;
}  

bool PicoState::navCalibrated( bool newVal ) noexcept
{
    bool oldVal = sNavCalibrated;
    sNavCalibrated = newVal;
    return oldVal;
}


bool PicoState::wantAutoCalibrate() noexcept
{
    return sAutoCalibrateMode;
}

bool PicoState::wantAutoCalibrate( bool newVal ) noexcept
{
    bool oldVal = sAutoCalibrateMode;
    sAutoCalibrateMode = newVal;
    return oldVal;
}

