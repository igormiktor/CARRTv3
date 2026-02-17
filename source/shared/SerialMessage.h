/*
    RawMessage.h - Serial Message structure for CARRT3 communications
    between the RPI and Pico.  This file is shared by both the
    RPI and Pico code bases.

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

#ifndef SerialMessage_h
#define SerialMessage_h

#include <cstdint>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#include "CarrtError.h"
#include "SerialLink.h"

enum class MsgId : std::uint8_t
{
    /////// Never use -- don't want a message with ID=0
    kNull_NeverUse,

    // Exactly what they say they are
    kPingMsg,
    kPingReplyMsg,

    /////// Msgs from Pico

    // Pico sends once ready to start receiving messages (Pico initiates serial
    // comms with this message) If Pico fails to be ready, error report instead
    // (via kErrorReportFromPico)
    kPicoReady,

    // Sent by Pico first time Nav calibrated and whenever calibration status
    // changes (byte 2 -> status boolean)
    kPicoNavStatusUpdate,

    // Pico tells RPi0 to stop CARRT (stop driving, stop slewing)
    kPicoSaysStop,

    /////// Messages (to Pico). Errors sent by kErrorReportFromPico msg

    // Pico to turn on or off various types of messages sent over Serial Link
    kMsgControlMsg,

    // Pico to reset itself (ack by sending kResetPicoMsg back, then followed by
    // kPicoReady)
    kResetPicoMsg,

    /////// Timer events

    // Timer event (2nd byte -> 1 = 1/4s, 4 = 1s, 32 = 8s;
    // 3rd byte -> count by type; 4th byte time hack)
    kTimerEventMsg,

    // To pico to start/stop sending of timer msgs
    // (2nd byte -> 0/1 = stop/start)
    kTimerControl,

    /////// BNO005 msgs

    // Pico to begin calibration of the BNO055 (end of calibration -> Pico
    // sends kPicoReadyNav msg)
    kBeginCalibration,

    // Request status of BNO055 calibration (return with kPicoNavStatusUpdate)
    kRequestCalibStatus,

    // Send status of BNO055 calibration (contains 4 x one-byte status values
    // in order M-A-G-S)
    kCalibrationInfoUpdate,

    // Set Pico auto-calibration mode (2nd byte is bool on/off (0/1) value)
    kSetAutoCalibrate,

    // RPi to Pico message to reset BNO055
    kResetBNO055,

    /////// Navigation events

    // Navigation timer (1/8 sec) from Pico to RPi0 (info in following bytes)
    kTimerNavUpdate,

    // From RPi0 to Pico to start/stop sending of NavUpdates
    // (2nd byte -> 0/1 = stop/start)
    kNavUpdateControl,

    // From RPi0 to Pico (2nd byte provides driving status)
    kDrivingStatusUpdate,

    // From Pico to RPi0, 2nd byte = L count; 3rd byte = R count; followed by
    // time hack
    kEncoderUpdate,

    // From RPi0 to Pico to start/stop sending of encoder udpates
    // (2nd byte -> 0/1 = stop/start)
    kEncoderUpdateControl,

    ///////  Battery info

    // From RPi0 to Pico requesting battery level (2nd byte = which battery:
    // 0 = IC, 1 = Motor)
    kBatteryLevelRequest,

    // Pico to RPi0 battery V; 2nd byte = which battery; following 4 bytes
    // voltage (float)
    kBatteryLevelUpdate,

    /////// Error reports

    // Pico sends a bool fatal flag (bool in a std::uint8_t) and error code
    // (int) in following bytes (3-6)
    kErrorReportFromPico,

    /////// Unknown message

    // Never transmitted; used to designate an unknown message received;
    // contains error code (int)
    kUnknownMessage,

    /////// Debugging events

    // RPi0 sends to Pico asking Pico to report an error (bytes 2-5 contain
    // error code to send back)
    kTestPicoReportError,

    // RPi0 sends to Pico asking Pico to send a specific Msg (byte 2 contains
    // the MsgId desired)
    kTestPicoMessages,

    // Pico returns (during testing) when Pico asked to send a test message it
    // doesn't normally send
    kPicoReceivedTestMsg,

    // RPi0 or Pico sends messages to debug/test the serial link; data
    // consists of two int values
    kDebugSerialLink,

    // Count of number of MsgsIds (helpful to generate testing code)
    // Not actually used as a message
    kCountOfMsgIds
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline constexpr bool is_tuple_v = false;

template<typename... Elems>
inline constexpr bool is_tuple_v<std::tuple<Elems...>> = true;

template<typename T>
concept IsTuple = is_tuple_v<std::remove_cvref_t<T>>;

template<IsTuple TTuple>
struct RawMessage
{
public:
    explicit RawMessage( MsgId id ) noexcept
        : mId{ id }, mMsg{}
    {}

    RawMessage( MsgId id, TTuple t ) noexcept
        : mId{ id }, mMsg{ t }
    {}

    void readIn( SerialLink& link )
    {
        // Don't read ID, we already have it if we call this function
        forEach(
            mMsg,
            []( SerialLink& lnk, auto& dataItem )
            {
                auto got = lnk.get( dataItem );
                if ( got )
                {
                    dataItem = *got;
                }
                else
                {
                    throw CarrtError(
                        makeSharedErrorId( kSerialMsgReadError, 1, 1 ),
                        "Couldn't read serial message in lamba" );
                }
            },
            link );
    }

    void sendOut( SerialLink& link )
    {
        // Send the ID, if we send the message
        link.put( static_cast<std::uint8_t>( mId ) );
        forEach(
            mMsg, []( SerialLink& lnk, auto& dataItem )
            { lnk.put( dataItem ); }, link );
    }

    // Tuple compile-time iterator over elements
    template<
        size_t Index = 0,    // start iteration at 0 index
        typename TupleT,     // the tuple type
        size_t Size =
            std::tuple_size_v<std::remove_reference_t<TupleT>>,    // tuple size
        typename TCallable,    // the callable to be invoked for each tuple item
        typename... TArgs      // other arguments to be passed to the callable
        >
    void forEach( TupleT&& tuple, TCallable&& callable, TArgs&&... args )
    {
        if constexpr ( Index < Size )
        {
            std::invoke( callable, args..., std::get<Index>( tuple ) );

            if constexpr ( Index + 1 < Size )
            {
                forEach<Index + 1>( std::forward<TupleT>( tuple ),
                                    std::forward<TCallable>( callable ),
                                    std::forward<TArgs>( args )... );
            }
        }
    }

    MsgId mId;
    TTuple mMsg;
};

///////////////////////////////////////////////////////////////////////////////

class EventManager;

class SerialMessage
{
public:
    explicit SerialMessage( MsgId id ) noexcept {}

    virtual ~SerialMessage() = default;

    // Prohibit copying assignment
    SerialMessage( const SerialMessage& ) = delete;
    SerialMessage& operator=( const SerialMessage& ) = delete;

    virtual void readIn( SerialLink& link ) = 0;

    virtual void sendOut( SerialLink& link ) = 0;

    virtual void takeAction( EventManager& events, SerialLink& link ) = 0;

    [[nodiscard]] virtual bool needsAction() const noexcept = 0;

    virtual MsgId getId() const noexcept = 0;

    int getIdNum() const noexcept { return static_cast<int>( getId() ); }
};

////////////////////////////////////////////////////////////////////////////////

// Common to both RPi0 and Pico, but each has its own implementation
// This lets us handle unknown messages gracefully
//
// But can't really continue operation because serial pipe is now potentially
// corrupt after unknown message

class UnknownMsg : public SerialMessage
{
public:
    using TheData = std::tuple<std::uint8_t, int>;

    UnknownMsg() noexcept;
    explicit UnknownMsg( TheData t ) noexcept;
    UnknownMsg( std::uint8_t, int errorCode ) noexcept;
    explicit UnknownMsg( MsgId id );

    virtual ~UnknownMsg() = default;

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override
    {
        return mNeedsAction;
    }

    virtual MsgId getId() const noexcept override { return mContent.mId; }

private:
    struct RawMessage<TheData> mContent;

    bool mNeedsAction;
};

// Common to both RPi0 and Pico, but each has its own implementation
// This lets us handle just dump bytes whenever we need
// It is the default when there are no handlers in the SerialMessageProcessor
// It essentially reads a single byte (the "ID") and, via takeAction(),
// dumps it to output

class DumpByteMsg : public SerialMessage
{
public:
    DumpByteMsg() noexcept
        : SerialMessage( static_cast<MsgId>( 0 ) ),
          mByte{ static_cast<MsgId>( 0 ) }
    {}

    explicit DumpByteMsg( MsgId id ) noexcept
        : SerialMessage( id ), mByte{ id }
    {}

    virtual ~DumpByteMsg() = default;

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override
    {
        return false;
    }

    virtual MsgId getId() const noexcept override { return mByte; }

private:
    MsgId mByte;
};

// Messages with no content (just commmand ID) are common. This is a base class
// for those common to both RPi0 and Pico, but each has its own implementation

class NoContentMsg : public SerialMessage
{
public:
    explicit NoContentMsg( std::uint8_t id ) noexcept;
    explicit NoContentMsg( MsgId id ) noexcept;

    virtual ~NoContentMsg() = default;

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override;

    virtual MsgId getId() const noexcept override;

protected:
    MsgId mId;
    bool mNeedsAction;
};

#endif    // SerialMessage_h
