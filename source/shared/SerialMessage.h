/*
    SerialMessage.h - Serial Message tools for CARRT3 communications
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


#ifndef SerialMessage_h
#define SerialMessage_h

#include <cstdint>


enum SerialMessage : std::uint8_t
{
    kNullMsg                    = 0x00,

    // Messages (to Pico); acknowledged from Pico with same Msg to ack.  Errors send by kErrorReportFromPico msg
    kStartCore1                 = 0x01,             // Pico to start Core1 
    kBeginCalibration           = 0x02,             // Pico to begin calibration of the BNO055  (end of calibration -> kPicoReady msg)
    kPauseMsg                   = 0x07,             // Pico to pause event processing
    kResumeMsg                  = 0x08,             // Pico to resume event processing  
    kResetMsg                   = 0x09,             // Pico to reset itself (ack by sending kReset, then followed by kPicoReady)

    // Msgs from Pico
    kPicoReady                  = 0x10,             // Sent by Pico once ready (bytes 2-5 -> uint32_t time hack for sync)
                                                    // If Pico not ready, error report instead (via kErrorReportFromPico)
    KPicoSaysStop               = 0x11,             // Pico tells RPi0 to stop CARRT (stop driving, stop slewing)       

    // Timer events (from Pico)
    kTimerNavUpdate             = 0x20,             // Navigation timer (1/8 sec) (info in following bytes)
    kTimer1_4s                  = 0x21,             // 1/4 sec timer event (2nd byte -> count)
    kTimer1s                    = 0x22,             // 1 sec timer event (2nd byte -> count)
    kTimer8s                    = 0x23,             // 8 sec timer event (2nd byte -> count)

    // Calibration cmds       
    kRequestCalibStatus         = 0x30,             // Request status of BNO055 calibration (return with one-byte status)
    kSendCalibProfileToPico     = 0x31,             // Sending a calibration profile to Pico (follow by calibration data)
    kRequestCalibProfileFmPico  = 0x32,             // Request a calibration profile from Pico (reply followed by calibration data)

    // Navigation info
    kDrivingStatusUpdate        = 0x90,             // From RPi0 to Pico (2nd byte provides driving status)
    kLeftEncoderUpdate          = 0x91,             // Pico sends update containing left encoder count and time
    kRightEncoderUpdate         = 0x92,             // Pico sends update containing right encoder count and time
 
    // Battery info
    kRequestBatteryLevel        = 0xA0,             // Pico to reply same with IC battery V in following bytes
    kBatteryLowAlert            = 0xA1,             // Pico sends alert that IC battery is low
    kRequestMotorBatteryLevel   = 0xA2,             // Pico to reply same with motor battery V in following bytes
    kMotorBattLowAlert          = 0x53,             // Pico sends alert that motor battery is low

    // Error reports
    kErrorReportFromPico        = 0xE0,             // Pico sends a bool fatal flag (bool in a uint8_t) and error code (int) in following bytes (3-6)


    // Debugging events
    kIdentifyPicoCore           = 0xF0,             // RPi0 sends to Pico asking to identify core (0 or 1) running uart
                                                    // Pico responds with same and which core is running uart (2nd byte)
    kTestPicoReportError        = 0xF2,             // RPi0 sends to Pico asking to report an error (bytes 2-5 contain error code to send back)

    kExtendedMsg                = 0xFF
};


// These are the messages in the second byte of an extended message (kExtendMsg)
enum SerialExtendedMsg : std::uint8_t
{
    kNullExtMsg                 = 0x00,

    // Debugging extended cmds
    kTestExtendedMsg            = 0xF0,

    kTriggerError               = 0xFF
};




union Transfer
{
    std::uint8_t    c[4];
    int             i;
    std::uint32_t   u;
    float           f;
};



#endif // SerialMessage_h
