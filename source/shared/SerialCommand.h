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

    // Commands (to Pico); acknowledged from Pico with same Cmd & second byte (non-zero 2nd byte -> error code)
    kPicoReady              = 0x01,             // Sent by Pico once ready (non-zero 2nd byte -> error code)
    kPauseCmd               = 0x02,             // Pico to pause event processing
    kResumeCmd              = 0x03,             // Pico to resume event processing  
    kResetCmd               = 0x09,             // Pico to reses itself (ack by sending kPicoReady)

    // Timer events (from Pico)
    kTimerNavUpdate         = 0x10,             // Navigation timer (1/8 sec) (info in following bytes)
    kTimer1_4s              = 0x11,             // 1/4 sec timer event (2nd byte -> count)
    kTimer1s                = 0x12,             // 1 sec timer event (2nd byte -> count)
    kTimer8s                = 0x13,             // 8 sec timer event (2nd byte -> count)

    // Navigation info
    kDrivingStatusUpdate    = 0x20,             // From Pi-0 to Pico (2nd byte provides driving status)
    kLeftEncoderUpdate      = 0x21,             // Pico sends update containing left encoder count
    kRightEncoderUpdate     = 0x22,             // Pico sends update containing right encoder count

    // Battery info
    kRequestBatteryLevel    = 0x30,             // Pico to reply same with IC battery V in following bytes
    kBatteryLowAlert        = 0x31,             // Pico sends alert about IC battery low
    kRequestMotorBattLevel  = 0x32,             // Pico to reply same with motor battery V in following bytes
    kMotorBattLowAlert      = 0x33,             // Pico sends alert about motor battery low

    // Error reports
    kErrorReportFromPico    = 0xE0,             // Pico sends error code (int) in following bytes


    // Debugging events
    kIdentifyCodeRequest    = 0xF0,             // Pi-0 sends to Pico asking to identify core running uart
    kIdentifyCore           = 0xF1,             // Pico reports which core is running uart (2nd byte)

    kExtendCmd              = 0xFF
};


enum SerialExtendedCmd : uint8_t
{
    kNullExtCmd             = 0x00,

    // Debugging extended cmds
    kTestExtendedCmd        = 0xF0,

    kTriggerError           = 0xFF
};



#endif // SerialCommand_h
