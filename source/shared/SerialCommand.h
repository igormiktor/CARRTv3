/*
    SerialCommand.h - Serial Command structure for CARRT3 communications
    between the RPI and Pico.  This file is shared by both the
    RPI and Pico code bases.

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


#ifndef SerialCommand_h
#define SerialCommand_h

#include <cstdint>
#include <tuple>
#include <utility>
#include <functional> 
#include <type_traits>


#include "SerialLink.h"
#include "CarrtError.h"




enum CommandId : std::uint8_t
{
    kNullMsg                    = 0x00,

    // Messages (to Pico). Errors send by kErrorReportFromPico msg
    kBeginCalibration           = 0x02,             // Pico to begin calibration of the BNO055 (end of calibration -> kPicoReadyNav msg)
    kPauseMsg                   = 0x07,             // Pico to pause event processing
    kResumeMsg                  = 0x08,             // Pico to resume event processing  
    kResetMsg                   = 0x09,             // Pico to reset itself (ack by sending kReset, then followed by kPicoReady)

    // Msgs from Pico
    kPicoReady                  = 0x10,             // Pico sends once ready to start receiving messages (Pico initiates serial comms with this message)
                                                    // If Pico fails to be ready, error report instead (via kErrorReportFromPico)
    kPicoReadyNav               = 0x11,             // Sent by Pico once Nav ready and ready to do stuff (bytes 2-5 -> uint32_t time hack for sync)
    kPicoSaysStop               = 0x1F,             // Pico tells RPi0 to stop CARRT (stop driving, stop slewing)       

    // Timer events
    kTimerEvent                 = 0x21,             // Timer event (2nd byte -> 1 = 1/4s, 4 = 1s, 32 = 8s; 3rd byte -> count by type; 4th byte time hack)
    kTimerControl               = 0x22,             // To pico to start/stop sending of timer msgs (2nd byte -> 0/1 = stop/start)

    // Calibration cmds       
    kRequestCalibStatus         = 0x30,             // Request status of BNO055 calibration (return with one-byte status)
    kSendCalibProfileToPico     = 0x31,             // Sending a calibration profile to Pico (follow by calibration data)
    kRequestCalibProfileFmPico  = 0x32,             // Request a calibration profile from Pico (reply followed by calibration data)

    // Navigation events
    kTimerNavUpdate             = 0x40,             // Navigation timer (1/8 sec) from Pico to RPi0 (info in following bytes)
    kDrivingStatusUpdate        = 0x41,             // From RPi0 to Pico (2nd byte provides driving status)
    kEncoderUpdate              = 0x42,             // From Pico to RPi0, 2nd byte = L count; 3rd byte = R count; followed by time hack
 
    // Battery info
    kRequestBatteryLevel        = 0x50,             // From RPi0 to Pico requesting battery level (2nd byte = which battery: 0 = IC, 1 = Motor)
    kBatteryLevelUpdate         = 0x51,             // Pico to RPi0 battery V; 2nd byte = which battery; following 4 bytes V (float)
    kBatteryLowAlert            = 0x52,             // Pico to RPi0 alert that battery is low; 2nd byte = which battery; following 4 bytes V (float)

    // Error reports
    kErrorReportFromPico        = 0xE0,             // Pico sends a bool fatal flag (bool in a uint8_t) and error code (int) in following bytes (3-6)

    // Unknown command
    kUnknownCommand             = 0xEE,             // Never transmitted; used to designate an unknown command received; contains error code (int)

    // Debugging events
    kIdentifyPicoCore           = 0xF0,             // RPi0 sends to Pico asking to identify core (0 or 1) running uart
                                                    // Pico responds with same and which core is running uart (2nd byte)
    kTestPicoReportError        = 0xF2,             // RPi0 sends to Pico asking to report an error (bytes 2-5 contain error code to send back)

    kDebugSerialLink            = 0xFE,             // RPi0 or Pico sends messages to debug/test the serial link; data is two int values

    kExtendedMsg                = 0xFF              // Indicates an extended msg; read next byte for extended command
};


// These are the messages in the second byte of an extended message (kExtendMsg)
enum SerialExtendedMsg : std::uint8_t
{
    kNullExtMsg                 = 0x00,

    // Debugging extended cmds
    kTestExtendedMsg            = 0xF0,

    kTriggerError               = 0xFF
};





//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////





template<typename T>
inline constexpr bool is_tuple_v = false;

template<typename ...Elems>
inline constexpr bool is_tuple_v<std::tuple<Elems...>> = true;

template<typename T>
concept IsTuple = is_tuple_v<std::remove_cvref_t<T>>;


template<IsTuple TTuple>
struct SerialMessage
{
public:

    SerialMessage( CommandId id ) noexcept
    : mId{ id }, mMsg{} {}

    SerialMessage( CommandId id, TTuple t ) noexcept
    : mId{ id }, mMsg{ t } {}


    void readIn( SerialLink& link )
    {
        // Don't read ID, we already have it if we call this function
        for_each( mMsg, 
            []( SerialLink& lnk, auto& dataItem )
            {  
                auto got = lnk.get( dataItem );

                int n{ 4 };
                while ( !got && n-- > 0 )
                {
                    got = lnk.get( dataItem );
                }

                if ( got )
                {
                    dataItem = *got;
                }
                else
                {
                    throw CarrtError( makePicoErrorId( kSerialCmdReadError, 1, 1 ), "Couldn't read serial message in lamba" );
                }
            }, 
            link 
        );
    }


    void sendOut( SerialLink& link )
    {
        // Send the ID, if we send the message
        link.put( static_cast<std::uint8_t>( mId ) );
        for_each( mMsg, []( SerialLink& lnk, auto& dataItem ){ lnk.put( dataItem ); }, link );
    }


    // Tuple compile-time iterator over elements
    template <
        size_t Index = 0,                                                   // start iteration at 0 index
        typename TupleT,                                                    // the tuple type
        size_t Size = std::tuple_size_v<std::remove_reference_t<TupleT>>,   // tuple size
        typename TCallable,                                                 // the callable to be invoked for each tuple item
        typename ...TArgs                                                   // other arguments to be passed to the callable
    >
    void for_each( TupleT&& tuple, TCallable&& callable, TArgs&& ...args )
    {
        if constexpr ( Index < Size )
        {
            std::invoke( callable, args..., std::get<Index>( tuple ) );

            if constexpr ( Index + 1 < Size )
            {
                for_each<Index + 1>( 
                    std::forward<TupleT>( tuple ), 
                    std::forward<TCallable>( callable ),
                    std::forward<TArgs>( args )... 
                );
            }
        }
    }


    CommandId   mId;
    TTuple      mMsg;

};




//////////////////////////////////////////////////////////////////////////////////////////////////////////////


class EventManager;


class SerialCommand 
{
public:

    SerialCommand( CommandId id ) noexcept {}

    virtual ~SerialCommand() = default;

    virtual void readIn( SerialLink& link ) = 0;

    virtual void sendOut( SerialLink& link ) = 0;

    virtual void takeAction( EventManager& events, SerialLink& link ) = 0;

    virtual bool needsAction() const noexcept = 0;

    virtual std::uint8_t getId() const noexcept = 0;

};




//////////////////////////////////////////////////////////////////////////////////////////////////////////////




// Common to both RPi0 and Pico, but each has its own implementation
// This lets us handle unknown commands gracefully 
// But can't really continue operation becuase serial pipe is now potentially corrupt after unknown command
class UnknownCmd : public SerialCommand 
{
public:

    using TheData = std::tuple< std::uint8_t, int >;

    UnknownCmd() noexcept;
    UnknownCmd( TheData t ) noexcept; 
    UnknownCmd( std::uint8_t, int errorCode ) noexcept;
    UnknownCmd( CommandId id );

    virtual ~UnknownCmd() = default;


    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual std::uint8_t getId() const noexcept override { return mContent.mId; }


private:

    struct SerialMessage<TheData>  mContent;

    bool    mNeedsAction;
};





#endif // SerialCommand_h
