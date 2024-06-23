/*
    Events.h - CARRTv3 Events shared by RPI and Pico.  
    This file is shared by both the RPI and Pico code bases.

    Copyright (c) 2024 Igor Mikolic-Torreira.  All right reserved.

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


#ifndef Events_h
#define Events_h



enum Events : int
{
    kNullEvent = 0,

    // Timer events
    kQuarterSecondTimerEvent,
    kOneSecondTimerEvent,
    kEightSecondTimerEvent,

    // Nav update events
    kNavUpdateEvent,
    kNavDriftCorrectionEvent,

    // Encoder events
    kEncoderRightEvent,
    kEncoderLeftEvent,

    // Battery events
    kBatteryLowEvent,

    // Keypad events
    kKeypadButtonHitEvent,

    // Debugging events
    kIdentifyCoreEvent,

    // Error event
    kErrorEvent,

    kLastEvent
};






#endif // Events_h
