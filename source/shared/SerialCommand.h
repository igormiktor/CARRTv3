/*
    SerialCommand.h - Serial Command tools for CARRT3 communications
    between the RPI and Pico.  This file is shared by both the
    RPI and Pico code bases.

    Copyright (c) 2023 Igor Mikolic-Torreira.  All right reserved.

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


#ifndef SerialCommand_h
#define SerialCommand_h



enum SerialCommand : uint8_t
{
    kNullCmd                = 0x00,

    // Timer events
    kTimerNavUpdate         = 0x10,             // Navigation timer (1/8 sec)
    kTimer1_4s              = 0x20,             // 1/4 sec timer event
    kTimer1s                = 0x21,             // 1 sec timer event
    kTimer8s                = 0x22,             // 8 sec timer event


    // Debugging events
    kIdentifyCodeRequest    = 0xF0,
    kIdentifyCore           = 0xF1,

    kExtendCmd              = 0xFF
};






#endif // SerialCommand_h
