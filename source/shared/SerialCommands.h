/*
    SerialCommands.h - The actual Serial Commands for CARRT3 communications
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


#ifndef SerialCommands_h
#define SerialCommands_h



#include "SerialCommand.h"

#include "SerialLink.h"
#include "CarrtError.h"




//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    All the actual Serial Commands follow (except for the UnknownCmd, which is in SerialCommand.h)
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////




class NullCmd : public NoContentCmd 
{
public:

    NullCmd() noexcept;
    NullCmd( CommandId id ) noexcept;

    virtual ~NullCmd() = default;


    virtual void takeAction( EventManager& events, SerialLink& link ) override;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class PicoReadyCmd : public SerialCommand 
{
public:

    using TheData = std::tuple< std::uint32_t >;

    PicoReadyCmd() noexcept;
    PicoReadyCmd( TheData t ) noexcept; 
    PicoReadyCmd( std::uint32_t time ) noexcept;
    PicoReadyCmd( CommandId id );

    virtual ~PicoReadyCmd() = default;


    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual std::uint8_t getId() const noexcept override { return mContent.mId; }


private:

    struct SerialMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class PicoNavStatusUpdateCmd : public SerialCommand 
{
public:

    using TheData = std::tuple< bool, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t >;

    PicoNavStatusUpdateCmd() noexcept;
    PicoNavStatusUpdateCmd( TheData t ) noexcept; 
    PicoNavStatusUpdateCmd( bool status, std::uint8_t m, std::uint8_t a, std::uint8_t g, std::uint8_t s ) noexcept;
    PicoNavStatusUpdateCmd( CommandId id );

    virtual ~PicoNavStatusUpdateCmd() = default;


    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual std::uint8_t getId() const noexcept override { return mContent.mId; }


private:

    struct SerialMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class PicoSaysStopCmd : public NoContentCmd 
{
public:

    PicoSaysStopCmd() noexcept;
    PicoSaysStopCmd( CommandId id ) noexcept;

    virtual ~PicoSaysStopCmd() = default;


    virtual void takeAction( EventManager& events, SerialLink& link ) override;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class MsgControlCmd  : public SerialCommand
{
public:

    using TheData = std::tuple< std::uint8_t >;

    enum Masks : std::uint8_t
    {
        kNavMsgMask             = 0x01,
        kNavStatusMask          = 0x02,
        kTimerMsgMask           = 0x04,
        kEncoderMsgMask         = 0x08,
        kCalibrationMsgMask     = 0x10,

        kAllMsgsOff             = 0x00,
        kAllMsgsOn              = 0xFF
    };

    MsgControlCmd() noexcept;
    MsgControlCmd( TheData t ) noexcept; 
    MsgControlCmd( bool val ) noexcept;
    MsgControlCmd( CommandId id );

    virtual ~MsgControlCmd() = default;


    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual std::uint8_t getId() const noexcept override { return mContent.mId; }


private:

    struct SerialMessage<TheData>  mContent;

    bool    mNeedsAction;
};;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class ResetCmd : public NoContentCmd 
{
public:

    ResetCmd() noexcept;
    ResetCmd( CommandId id ) noexcept;

    virtual ~ResetCmd() = default;


    virtual void takeAction( EventManager& events, SerialLink& link ) override;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class TimerEventCmd : public SerialCommand
{
public:

    using TheData = std::tuple< std::uint8_t, int, uint32_t >;

    enum : std::uint8_t
    {
        k1QuarterSecondEvent = 1,
        k1SecondEvent = 4,
        k8SecondEvent = 32
    };

    TimerEventCmd() noexcept;
    TimerEventCmd( TheData t ) noexcept; 
    TimerEventCmd( std::uint8_t which, int count, uint32_t time ) noexcept;
    TimerEventCmd( CommandId id );

    virtual ~TimerEventCmd() = default;


    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual std::uint8_t getId() const noexcept override { return mContent.mId; }


private:

    struct SerialMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class TimerControlCmd : public SerialCommand
{
public:

    using TheData = std::tuple< std::uint8_t >;

    TimerControlCmd() noexcept;
    TimerControlCmd( TheData t ) noexcept; 
    TimerControlCmd( bool val ) noexcept;
    TimerControlCmd( CommandId id );

    virtual ~TimerControlCmd() = default;


    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual std::uint8_t getId() const noexcept override { return mContent.mId; }


private:

    struct SerialMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class BeginCalibrationCmd : public NoContentCmd 
{
public:

    BeginCalibrationCmd() noexcept;
    BeginCalibrationCmd( CommandId id ) noexcept;

    virtual ~BeginCalibrationCmd() = default;


    virtual void takeAction( EventManager& events, SerialLink& link ) override;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class RequestCalibrationStatusCmd : public NoContentCmd 
{
public:

    RequestCalibrationStatusCmd() noexcept;
    RequestCalibrationStatusCmd( CommandId id ) noexcept;

    virtual ~RequestCalibrationStatusCmd() = default;


    virtual void takeAction( EventManager& events, SerialLink& link ) override;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class SendCalibrationInfoCmd : public SerialCommand
{
public:

    using TheData = std::tuple< std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t >;

    SendCalibrationInfoCmd() noexcept;
    SendCalibrationInfoCmd( TheData t ) noexcept; 
    SendCalibrationInfoCmd( std::uint8_t mag, std::uint8_t accel, std::uint8_t gyro, std::uint8_t sys ) noexcept;
    SendCalibrationInfoCmd( CommandId id );

    virtual ~SendCalibrationInfoCmd() = default;


    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual std::uint8_t getId() const noexcept override { return mContent.mId; }


private:

    struct SerialMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class ResetBNO055Cmd : public NoContentCmd 
{
public:

    ResetBNO055Cmd() noexcept;
    ResetBNO055Cmd( CommandId id ) noexcept;

    virtual ~ResetBNO055Cmd() = default;


    virtual void takeAction( EventManager& events, SerialLink& link ) override;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class NavUpdateCmd : public SerialCommand
{
public:

    using TheData = std::tuple< float, std::uint32_t >;

    NavUpdateCmd() noexcept;
    NavUpdateCmd( TheData t ) noexcept; 
    NavUpdateCmd( float heading, std::uint32_t time ) noexcept;
    NavUpdateCmd( CommandId id );

    virtual ~NavUpdateCmd() = default;


    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual std::uint8_t getId() const noexcept override { return mContent.mId; }


private:

    struct SerialMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class NavUpdateControlCmd : public SerialCommand
{
public:

    using TheData = std::tuple< std::uint8_t >;

    NavUpdateControlCmd() noexcept;
    NavUpdateControlCmd( TheData t ) noexcept; 
    NavUpdateControlCmd( bool val ) noexcept;
    NavUpdateControlCmd( CommandId id );

    virtual ~NavUpdateControlCmd() = default;


    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual std::uint8_t getId() const noexcept override { return mContent.mId; }


private:

    struct SerialMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class ErrorReportCmd : public SerialCommand
{
public:

    using TheData = std::tuple< std::uint8_t, int >;

    ErrorReportCmd() noexcept;
    ErrorReportCmd( TheData t ) noexcept; 
    ErrorReportCmd( bool val, int errorCode ) noexcept;
    ErrorReportCmd( CommandId id );

    virtual ~ErrorReportCmd() = default;


    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual std::uint8_t getId() const noexcept override { return mContent.mId; }


private:

    struct SerialMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class DebugLinkCmd : public SerialCommand
{
public:

    using TheData = std::tuple< int, int >;

    DebugLinkCmd() noexcept;
    DebugLinkCmd( TheData t ) noexcept; 
    DebugLinkCmd( int val1, int val2 ) noexcept;
    DebugLinkCmd( CommandId id );

    virtual ~DebugLinkCmd() = default;


    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual std::uint8_t getId() const noexcept override { return mContent.mId; }


private:

    struct SerialMessage<TheData>   mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////








#endif // SerialCommands_h
