/*
    SerialCommands.h - Serial Commands for CARRT3 communications
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




enum CommandId : std::uint8_t
{
    kNullMsg                    = 0x00,

    // Messages (to Pico); acknowledged from Pico with same Msg to ack.  Errors send by kErrorReportFromPico msg
    kStartCore1                 = 0x01,             // Pico to start Core1 
    kBeginCalibration           = 0x02,             // Pico to begin calibration of the BNO055 (end of calibration -> kPicoReadyNav msg)
    kPauseMsg                   = 0x07,             // Pico to pause event processing
    kResumeMsg                  = 0x08,             // Pico to resume event processing  
    kResetMsg                   = 0x09,             // Pico to reset itself (ack by sending kReset, then followed by kPicoReady)

    // Msgs from Pico
    kPicoReady                  = 0x10,             // Pico sends once ready to start receiving messages (Pico initiates serial comms with this message)
                                                    // If Pico fails to be ready, error report instead (via kErrorReportFromPico)
    kPicoReadyNav               = 0x11,              // Sent by Pico once Nav ready and ready to do stuff (bytes 2-5 -> uint32_t time hack for sync)
    KPicoSaysStop               = 0x1F,             // Pico tells RPi0 to stop CARRT (stop driving, stop slewing)       

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
    kMotorBattLowAlert          = 0xA3,             // Pico sends alert that motor battery is low

    // Error reports
    kErrorReportFromPico        = 0xE0,             // Pico sends a bool fatal flag (bool in a uint8_t) and error code (int) in following bytes (3-6)


    // Debugging events
    kIdentifyPicoCore           = 0xF0,             // RPi0 sends to Pico asking to identify core (0 or 1) running uart
                                                    // Pico responds with same and which core is running uart (2nd byte)
    kTestPicoReportError        = 0xF2,             // RPi0 sends to Pico asking to report an error (bytes 2-5 contain error code to send back)

    kDebugSerialLink            = 0xFE,             // RPi0 or Pico sends messages to debug/test the serial link

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



#if 0
union Transfer
{
    std::uint8_t    c[4];
    int             i;
    std::uint32_t   u;
    float           f;
};
#endif  // #if 0




class SerialLink;




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
    : mId{ id }, mData{} {}

    SerialMessage( CommandId id, TTuple data ) noexcept;

    void readIn( SerialLink& link )
    {
        // Don't read ID, we already have it if we call this function
        link.put( static_cast<std::uint8_t>( mId ) );
        for_each( mData, 
            []( SerialLink& lnk, auto& dataItem )
            {  
                auto got = lnk.get( dataItem );
#if 0   // See if we need this.  Think not
                int n{ 10 };
                while ( !got && n-- > 0 )
                {
                    got = lnk.get( dataItem );
                }
#endif  // if 0
                if ( got )
                {
                    dataItem = *got;
                }
                else
                {
                    throw CarrtError( makePicoErrorId( kSerialCmdReadError, 1, 1 ) );
                }
            }, 
            link 
        );
    }


    void sendOut( SerialLink& link )
    {
        // Send the ID, if we send the message
        link.put( static_cast<std::uint8_t>( mId ) );
        for_each( mData, []( SerialLink& lnk, auto& dataItem ){ lnk.put( dataItem ); }, link );
    }



    // Tuple compile-time iterator
    template <
        size_t Index = 0,                                                   // start iteration at 0 index
        size_t Size = std::tuple_size_v<std::remove_reference_t<TTuple>>,   // tuple size
        typename TCallable,                                                 // the callable to be invoked for each tuple item
        typename... TArgs                                                   // other arguments to be passed to the callable
    >
    void for_each( TTuple&& tuple, TCallable&& callable, TArgs&&... args )
    {
        if constexpr (Index < Size)
        {
            std::invoke(callable, args..., std::get<Index>(tuple));

            if constexpr (Index + 1 < Size)
                for_each<Index + 1>(
                    std::forward<TTuple>(tuple),
                    std::forward<TCallable>(callable),
                    std::forward<TArgs>(args)... );
        }
    }


    CommandId   mId;
    TTuple      mData;

};




#if 0

template<std::size_t I = 0, typename FuncT, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
  for_each(std::tuple<Tp...> &, FuncT) // Unused arguments are given no names.
  { }

template<std::size_t I = 0, typename FuncT, typename... Tp>
inline typename std::enable_if<I < sizeof...(Tp), void>::type
  for_each(std::tuple<Tp...>& t, FuncT f)
  {
    f(std::get<I>(t));
    for_each<I + 1, FuncT, Tp...>(t, f);
  }

#endif // if 0


class SerialCommand 
{
public:

    SerialCommand( CommandId id ) noexcept 
    : mId{ id } {}

    virtual ~SerialCommand() = default;

    virtual void readIn( SerialLink& link ) = 0;

    virtual void sendOut( SerialLink& link ) = 0;

    virtual void takeAction( SerialLink& link ) = 0;

    virtual bool needsAction() const noexcept = 0;

    virtual std::uint8_t getId() const noexcept = 0;


protected:

//    void setId( std::uint8_t id );


private:

    std::uint8_t    mId;

};


#endif // SerialMessage_h
