/*
    Event.h - CARRTv3 events for Pico.  
    This file is shared by both the RPI and Pico code bases.

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


#ifndef Event_h
#define Event_h



enum Event : int
{
    kNullEvent = 0,

    // Timer events
    kQuarterSecondTimerEvent,
    kOneSecondTimerEvent,
    kEightSecondTimerEvent,

    // Nav update events
    kNavUpdateEvent,
    kNavDriftCorrectionEvent,

    // BNO055 events
    kBNO055InitializeEvent,
    kBNO055BeginCalibrationEvent,
    kSendCalibrationInfoEvent,

    // Encoder events
    kEncoderLeftEvent,
    kEncoderRightEvent,

    // Pulse LEDs events
    kPulsePicoLedEvent,

    // Battery events
    kBatteryLowEvent,

    // Keypad events
    kKeypadButtonHitEvent,

    // Reset
    kPicoResetEvent,

    // Core0 -> Core1 -> COre0 events
    kBNO055InitDelay,

    // Test events
    kGpioInterruptTestFallingEvent,
    kGpioInterruptTestRisingEvent,
    kGpioInterruptTestFailureEvent,
    kGpioInterruptWrongPinEvent,

    // Error event
    kErrorEvent,

    kLastEvent
};






#endif // Event_h
