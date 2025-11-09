/*
    SerialMessages.h - The actual Serial Messages for CARRT3 communications
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


#ifndef SerialMessages_h
#define SerialMessages_h



#include "SerialMessage.h"

#include "SerialLink.h"
#include "CarrtError.h"




//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    All the actual Serial Messages follow (except for the UnknownMsg, which is in SerialMessage.h)
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////




class PingMsg : public NoContentMsg 
{
public:

    PingMsg() noexcept;
    PingMsg( MsgId id ) noexcept;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

private:

    bool mSender;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class PingReplyMsg : public NoContentMsg 
{
public:

    PingReplyMsg() noexcept; 
    PingReplyMsg( MsgId id ) noexcept;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

private:

    bool mSender;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class PicoReadyMsg : public SerialMessage 
{
public:

    using TheData = std::tuple< std::uint32_t >;

    PicoReadyMsg() noexcept;
    PicoReadyMsg( TheData t ) noexcept; 
    PicoReadyMsg( std::uint32_t time ) noexcept;
    PicoReadyMsg( MsgId id );

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual MsgId getId() const noexcept override { return mContent.mId; }


private:

    struct RawMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class PicoNavStatusUpdateMsg : public SerialMessage 
{
public:

    using TheData = std::tuple< bool, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t >;

    PicoNavStatusUpdateMsg() noexcept;
    PicoNavStatusUpdateMsg( TheData t ) noexcept; 
    PicoNavStatusUpdateMsg( bool status, std::uint8_t m, std::uint8_t a, std::uint8_t g, std::uint8_t s ) noexcept;
    PicoNavStatusUpdateMsg( MsgId id );

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual MsgId getId() const noexcept override { return mContent.mId; }


private:

    struct RawMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class PicoSaysStopMsg : public NoContentMsg 
{
public:

    PicoSaysStopMsg() noexcept;
    PicoSaysStopMsg( MsgId id ) noexcept;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class MsgControlMsg  : public SerialMessage
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

    MsgControlMsg() noexcept;
    MsgControlMsg( TheData t ) noexcept; 
    MsgControlMsg( std::uint8_t val ) noexcept;
    MsgControlMsg( MsgId id );

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual MsgId getId() const noexcept override { return mContent.mId; }


private:

    struct RawMessage<TheData>  mContent;

    bool    mNeedsAction;
};;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class ResetMsg : public NoContentMsg 
{
public:

    ResetMsg() noexcept;
    ResetMsg( MsgId id ) noexcept;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class TimerEventMsg : public SerialMessage
{
public:

    using TheData = std::tuple< std::uint8_t, int, uint32_t >;

    enum : std::uint8_t
    {
        k1QuarterSecondEvent = 1,
        k1SecondEvent = 4,
        k8SecondEvent = 32
    };

    TimerEventMsg() noexcept;
    TimerEventMsg( TheData t ) noexcept; 
    TimerEventMsg( std::uint8_t which, int count, uint32_t time ) noexcept;
    TimerEventMsg( MsgId id );

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual MsgId getId() const noexcept override { return mContent.mId; }


private:

    struct RawMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class TimerControlMsg : public SerialMessage
{
public:

    using TheData = std::tuple< std::uint8_t >;

    TimerControlMsg() noexcept;
    TimerControlMsg( TheData t ) noexcept; 
    TimerControlMsg( bool val ) noexcept;
    TimerControlMsg( MsgId id );

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual MsgId getId() const noexcept override { return mContent.mId; }


private:

    struct RawMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class BeginCalibrationMsg : public NoContentMsg 
{
public:

    BeginCalibrationMsg() noexcept;
    BeginCalibrationMsg( MsgId id ) noexcept;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class RequestCalibrationStatusMsg : public NoContentMsg 
{
public:

    RequestCalibrationStatusMsg() noexcept;
    RequestCalibrationStatusMsg( MsgId id ) noexcept;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class CalibrationInfoUpdateMsg : public SerialMessage
{
public:

    using TheData = std::tuple< std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t >;

    CalibrationInfoUpdateMsg() noexcept;
    CalibrationInfoUpdateMsg( TheData t ) noexcept; 
    CalibrationInfoUpdateMsg( std::uint8_t mag, std::uint8_t accel, std::uint8_t gyro, std::uint8_t sys ) noexcept;
    CalibrationInfoUpdateMsg( MsgId id );

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual MsgId getId() const noexcept override { return mContent.mId; }


private:

    struct RawMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class SetAutoCalibrateMsg : public SerialMessage
{
public:

    using TheData = std::tuple< std::uint8_t >;

    SetAutoCalibrateMsg() noexcept;
    SetAutoCalibrateMsg( TheData t ) noexcept; 
    SetAutoCalibrateMsg( bool val ) noexcept;
    SetAutoCalibrateMsg( MsgId id );

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual MsgId getId() const noexcept override { return mContent.mId; }


private:

    struct RawMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class ResetBNO055Msg : public NoContentMsg 
{
public:

    ResetBNO055Msg() noexcept;
    ResetBNO055Msg( MsgId id ) noexcept;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class NavUpdateMsg : public SerialMessage
{
public:

    using TheData = std::tuple< float, std::uint32_t >;

    NavUpdateMsg() noexcept;
    NavUpdateMsg( TheData t ) noexcept; 
    NavUpdateMsg( float heading, std::uint32_t time ) noexcept;
    NavUpdateMsg( MsgId id );

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual MsgId getId() const noexcept override { return mContent.mId; }


private:

    struct RawMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class NavUpdateControlMsg : public SerialMessage
{
public:

    using TheData = std::tuple< std::uint8_t >;

    NavUpdateControlMsg() noexcept;
    NavUpdateControlMsg( TheData t ) noexcept; 
    NavUpdateControlMsg( bool val ) noexcept;
    NavUpdateControlMsg( MsgId id );

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual MsgId getId() const noexcept override { return mContent.mId; }


private:

    struct RawMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class DrivingStatusUpdateMsg : public SerialMessage
{
public:

    using TheData = std::tuple< std::uint8_t >;

    enum class Drive : std::uint8_t
    {
        kStopped,
        kDrivingFwd,
        kDrivingBkwd,
        kTurningLeft,
        kTurningRight, 
    };

    DrivingStatusUpdateMsg() noexcept;
    DrivingStatusUpdateMsg( TheData t ) noexcept; 
    DrivingStatusUpdateMsg( Drive driveStatus ) noexcept;
    DrivingStatusUpdateMsg( MsgId id );

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual MsgId getId() const noexcept override { return mContent.mId; }


private:

    struct RawMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class EncoderUpdateMsg : public SerialMessage
{
public:

    using TheData = std::tuple< int, int, std::uint32_t >;

    EncoderUpdateMsg() noexcept;
    EncoderUpdateMsg( TheData t ) noexcept; 
    EncoderUpdateMsg( int left, int right, std::uint32_t time ) noexcept;
    EncoderUpdateMsg( MsgId id );

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual MsgId getId() const noexcept override { return mContent.mId; }


private:

    struct RawMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class EncoderUpdateControlMsg : public SerialMessage
{
public:

    using TheData = std::tuple< std::uint8_t >;

    EncoderUpdateControlMsg() noexcept;
    EncoderUpdateControlMsg( TheData t ) noexcept; 
    EncoderUpdateControlMsg( bool val ) noexcept;
    EncoderUpdateControlMsg( MsgId id );

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual MsgId getId() const noexcept override { return mContent.mId; }


private:

    struct RawMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



// For the battery-related message classes
enum class Battery : std::uint8_t
{
    kIcBattery,
    kMotorBattery,
    kBothBatteries,
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class BatteryLevelRequestMsg : public SerialMessage
{
public:

    using TheData = std::tuple< std::uint8_t >;

    BatteryLevelRequestMsg() noexcept;
    BatteryLevelRequestMsg( TheData t ) noexcept; 
    BatteryLevelRequestMsg( Battery whichBattery ) noexcept;
    BatteryLevelRequestMsg( MsgId id );

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual MsgId getId() const noexcept override { return mContent.mId; }


private:

    struct RawMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class BatteryLevelUpdateMsg : public SerialMessage
{
public:

    using TheData = std::tuple< std::uint8_t, float >;

    BatteryLevelUpdateMsg() noexcept;
    BatteryLevelUpdateMsg( TheData t ) noexcept; 
    BatteryLevelUpdateMsg( Battery whichBattery, float level ) noexcept;
    BatteryLevelUpdateMsg( MsgId id );

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual MsgId getId() const noexcept override { return mContent.mId; }


private:

    struct RawMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class BatteryLowAlertMsg : public SerialMessage
{
public:

    using TheData = std::tuple< std::uint8_t, float >;

    BatteryLowAlertMsg() noexcept;
    BatteryLowAlertMsg( TheData t ) noexcept; 
    BatteryLowAlertMsg( Battery whichBattery, float level ) noexcept;
    BatteryLowAlertMsg( MsgId id );

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual MsgId getId() const noexcept override { return mContent.mId; }


private:

    struct RawMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class ErrorReportMsg : public SerialMessage
{
public:

    using TheData = std::tuple< std::uint8_t, int >;

    ErrorReportMsg() noexcept;
    ErrorReportMsg( TheData t ) noexcept; 
    ErrorReportMsg( bool val, int errorCode ) noexcept;
    ErrorReportMsg( MsgId id );

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual MsgId getId() const noexcept override { return mContent.mId; }


private:

    struct RawMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class TestPicoErrorRptMsg : public SerialMessage
{
public:

    using TheData = std::tuple< std::uint8_t, int >;

    TestPicoErrorRptMsg() noexcept;
    TestPicoErrorRptMsg( TheData t ) noexcept; 
    TestPicoErrorRptMsg( bool makeItFatal, int errorCodeToTest ) noexcept;
    TestPicoErrorRptMsg( MsgId id );

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual MsgId getId() const noexcept override { return mContent.mId; }


private:

    struct RawMessage<TheData>  mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class DebugLinkMsg : public SerialMessage
{
public:

    using TheData = std::tuple< int, int >;

    DebugLinkMsg() noexcept;
    DebugLinkMsg( TheData t ) noexcept; 
    DebugLinkMsg( int val1, int val2 ) noexcept;
    DebugLinkMsg( MsgId id );

    virtual void readIn( SerialLink& link ) override;

    virtual void sendOut( SerialLink& link ) override;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

    [[nodiscard]] virtual bool needsAction() const noexcept override { return mNeedsAction; }

    virtual MsgId getId() const noexcept override { return mContent.mId; }


private:

    struct RawMessage<TheData>   mContent;

    bool    mNeedsAction;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////








#endif // SerialMessages_h
