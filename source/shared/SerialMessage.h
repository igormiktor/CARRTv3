/*
    RawMessage.h - Serial Message structure for CARRT3 communications
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


#ifndef SerialMessage_h
#define SerialMessage_h

#include <cstdint>
#include <tuple>
#include <utility>
#include <functional> 
#include <type_traits>


#include "SerialLink.h"
#include "CarrtError.h"




enum class MsgId : std::uint8_t
{
    kPingMsg,
    kPingReplyMsg,

    // Msgs from Pico
    kPicoReady,                 // Pico sends once ready to start receiving messages (Pico initiates serial comms with this message)
                                // If Pico fails to be ready, error report instead (via kErrorReportFromPico)
    kPicoNavStatusUpdate,       // Sent by Pico first time Nav calibrated and whenever calibration status changes (byte 2 -> status boolean)
    kPicoSaysStop,              // Pico tells RPi0 to stop CARRT (stop driving, stop slewing)       

    // Messages (to Pico). Errors send by kErrorReportFromPico msg
    kMsgControlMsg,             // Pico to turn on or off various types of messages sent over Serial Link
    kResetMsg,                  // Pico to reset itself (ack by sending kResetMsg back, then followed by kPicoReady)

    // Timer events
    kTimerEventMsg,             // Timer event (2nd byte -> 1 = 1/4s, 4 = 1s, 32 = 8s; 3rd byte -> count by type; 4th byte time hack)
    kTimerControl,              // To pico to start/stop sending of timer msgs (2nd byte -> 0/1 = stop/start)

    // BNO005 msgs       
    kBeginCalibration,          // Pico to begin calibration of the BNO055 (end of calibration -> kPicoReadyNav msg)
    kRequestCalibStatus,        // Request status of BNO055 calibration (return with kPicoNavStatusUpdate)
    kCalibrationInfoUpdate,     // Send status of BNO055 calibration (contains 4 x one-byte status values M-A-G-S )
    kSetAutoCalibrate,          // Set Pico auto-calibration mode (2nd byte is bool on/off (0/1) value)
    kResetBNO055,               // RPi to Pico message to reset BNO055

    // Navigation events
    kTimerNavUpdate,            // Navigation timer (1/8 sec) from Pico to RPi0 (info in following bytes)
    kNavUpdateControl,          // From RPi0 to Pico to start/stop sending of NavUpdates (2nd byte -> 0/1 = stop/start)
    kDrivingStatusUpdate,       // From RPi0 to Pico (2nd byte provides driving status)
    kEncoderUpdate,             // From Pico to RPi0, 2nd byte = L count; 3rd byte = R count; followed by time hack
    kEncoderUpdateControl,      // From RPi0 to Pico to start/stop sending of encoder udpates (2nd byte -> 0/1 = stop/start)
 
    // Battery info
    kBatteryLevelRequest,       // From RPi0 to Pico requesting battery level (2nd byte = which battery: 0 = IC, 1 = Motor)
    kBatteryLevelUpdate,        // Pico to RPi0 battery V; 2nd byte = which battery; following 4 bytes V (float)
    kBatteryLowAlert,           // Pico to RPi0 alert that battery is low; 2nd byte = which battery; following 4 bytes V (float)

    // Error reports
    kErrorReportFromPico,       // Pico sends a bool fatal flag (bool in a uint8_t) and error code (int) in following bytes (3-6)

    // Unknown message
    kUnknownMessage,            // Never transmitted; used to designate an unknown message received; contains error code (int)

    // Debugging events
    kTestPicoReportError,       // RPi0 sends to Pico asking to report an error (bytes 2-5 contain error code to send back)

    kDebugSerialLink,           // RPi0 or Pico sends messages to debug/test the serial link; data is two int values
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
struct RawMessage
{
public:

    RawMessage( MsgId id ) noexcept
    : mId{ id }, mMsg{} {}

    RawMessage( MsgId id, TTuple t ) noexcept
    : mId{ id }, mMsg{ t } {}


    void readIn( SerialLink& link )
    {
        // Don't read ID, we already have it if we call this function
        forEach( mMsg, 
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
                    throw CarrtError( makeSharedErrorId( kSerialMsgReadError, 1, 1 ), "Couldn't read serial message in lamba" );
                }
            }, 
            link 
        );
    }


    void sendOut( SerialLink& link )
    {
        // Send the ID, if we send the message
        link.put( static_cast<std::uint8_t>( mId ) );
        forEach( mMsg, []( SerialLink& lnk, auto& dataItem ){ lnk.put( dataItem ); }, link );
    }


    // Tuple compile-time iterator over elements
    template <
        size_t Index = 0,                                                   // start iteration at 0 index
        typename TupleT,                                                    // the tuple type
        size_t Size = std::tuple_size_v<std::remove_reference_t<TupleT>>,   // tuple size
        typename TCallable,                                                 // the callable to be invoked for each tuple item
        typename ...TArgs                                                   // other arguments to be passed to the callable
    >
    void forEach( TupleT&& tuple, TCallable&& callable, TArgs&& ...args )
    {
        if constexpr ( Index < Size )
        {
            std::invoke( callable, args..., std::get<Index>( tuple ) );

            if constexpr ( Index + 1 < Size )
            {
                forEach<Index + 1>( 
                    std::forward<TupleT>( tuple ), 
                    std::forward<TCallable>( callable ),
                    std::forward<TArgs>( args )... 
                );
            }
        }
    }


    MsgId       mId;
    TTuple      mMsg;
};




//////////////////////////////////////////////////////////////////////////////////////////////////////////////


class EventManager;


class SerialMessage 
{
public:

    SerialMessage( MsgId id ) noexcept {}

    virtual ~SerialMessage() = default;

    // Prohibit copying assignment
    SerialMessage( const SerialMessage& ) = delete;
    SerialMessage& operator=( const SerialMessage& ) = delete;

    virtual void readIn( SerialLink& link ) = 0;

    virtual void sendOut( SerialLink& link ) = 0;

    virtual void takeAction( EventManager& events, SerialLink& link ) = 0;

    virtual bool needsAction() const noexcept = 0;

    virtual MsgId getId() const noexcept = 0;

    int getIdNum() const noexcept
        { return static_cast<int>( getId() ); }
};




//////////////////////////////////////////////////////////////////////////////////////////////////////////////




// Common to both RPi0 and Pico, but each has its own implementation
// This lets us handle unknown messages gracefully 
// But can't really continue operation because serial pipe is now potentially corrupt after unknown message
class UnknownMsg : public SerialMessage 
{
public:

    using TheData = std::tuple< std::uint8_t, int >;

    UnknownMsg() noexcept;
    UnknownMsg( TheData t ) noexcept; 
    UnknownMsg( std::uint8_t, int errorCode ) noexcept;
    UnknownMsg( MsgId id );

    virtual ~UnknownMsg() = default;


    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual MsgId getId() const noexcept override { return mContent.mId; }


private:

    struct RawMessage<TheData>  mContent;

    bool   mNeedsAction;
};




// Messages with no content (just commmand ID) are common. This is a base class for those
// Common to both RPi0 and Pico, but each has its own implementation

class NoContentMsg : public SerialMessage 
{
public:

    NoContentMsg( std::uint8_t id ) noexcept;
    NoContentMsg( MsgId id ) noexcept;

    virtual ~NoContentMsg() = default;


    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) = 0;

    virtual bool needsAction() const noexcept override;

    virtual MsgId getId() const noexcept override;


protected:

    MsgId   mId;

    bool    mNeedsAction;
};





#endif // SerialMessage_h
