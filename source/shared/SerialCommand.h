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

#include <stdint.h>


enum SerialCommand : uint8_t
{
    kNullCmd                    = 0x00,

    // Commands (to Pico); acknowledged from Pico with same Cmd & second byte (non-zero 2nd byte -> error code)
    kPauseCmd                   = 0x02,             // Pico to pause event processing
    kResumeCmd                  = 0x03,             // Pico to resume event processing  
    kResetCmd                   = 0x09,             // Pico to reset itself (ack by sending kReset + byte = 0, then kPicoReady)

    // Msgs from Pico
    kPicoReady                  = 0x10,             // Sent by Pico once ready (bytes 2-5 -> uint32_t time hack for sync)
                                                    // If Pico not ready, error report instead (via kErrorReportFrom Pico)
    KPicoSaysStop               = 0x11,             // Pico tells RPi0 to stop CARRT (stop driving, stop slewing)       

    // Timer events (from Pico)
    kTimerNavUpdate             = 0x20,             // Navigation timer (1/8 sec) (info in following bytes)
    kTimer1_4s                  = 0x21,             // 1/4 sec timer event (2nd byte -> count)
    kTimer1s                    = 0x22,             // 1 sec timer event (2nd byte -> count)
    kTimer8s                    = 0x23,             // 8 sec timer event (2nd byte -> count)

    // Navigation info
    kDrivingStatusUpdate        = 0x30,             // From RPi0 to Pico (2nd byte provides driving status)
    kLeftEncoderUpdate          = 0x31,             // Pico sends update containing left encoder count and time
    kRightEncoderUpdate         = 0x32,             // Pico sends update containing right encoder count and time
 
    // Battery info
    kRequestBatteryLevel        = 0x40,             // Pico to reply same with IC battery V in following bytes
    kBatteryLowAlert            = 0x41,             // Pico sends alert about IC battery low
    kRequestMotorBatteryLevel   = 0x42,             // Pico to reply same with motor battery V in following bytes
    kMotorBattLowAlert          = 0x43,             // Pico sends alert about motor battery low

    // Error reports
    kErrorReportFromPico        = 0xE0,             // Pico sends a bool fatal flag (bool in a uint8_t) and error code (int) in following bytes (3-6)


    // Debugging events
    kIdentifyPicoCore           = 0xF0,             // RPi0 sends to Pico asking to identify core (0 or 1) running uart
                                                    // Pico responds with same and which core is running uart (2nd byte)
    kTestPicoReportError        = 0xF2,             // RPi0 sends to Pico asking to report an error (bytes 2-5 contain error code to send back)

    kExtendedCmd                = 0xFF
};


// These are the commands in the second by of an extended command (kExtendCmd)
enum SerialExtendedCmd : uint8_t
{
    kNullExtCmd                 = 0x00,

    // Debugging extended cmds
    kTestExtendedCmd            = 0xF0,

    kTriggerError               = 0xFF
};




union Transfer
{
    uint8_t     c[4];
    int         i;
    uint32_t    u;
    float       f;
};



#endif // SerialCommand_h
