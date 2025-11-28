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
    explicit PingMsg( MsgId id ) noexcept;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;

private:

    bool mSender;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class PingReplyMsg : public NoContentMsg 
{
public:

    PingReplyMsg() noexcept; 
    explicit PingReplyMsg( MsgId id ) noexcept;

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
    explicit PicoReadyMsg( TheData t ) noexcept; 
    explicit PicoReadyMsg( std::uint32_t time ) noexcept;
    explicit PicoReadyMsg( MsgId id );

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
    explicit PicoNavStatusUpdateMsg( TheData t ) noexcept; 
    PicoNavStatusUpdateMsg( bool status, std::uint8_t m, std::uint8_t a, std::uint8_t g, std::uint8_t s ) noexcept;
    explicit PicoNavStatusUpdateMsg( MsgId id );

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
    explicit PicoSaysStopMsg( MsgId id ) noexcept;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class MsgControlMsg  : public SerialMessage
{
public:

    using TheData = std::tuple< std::uint8_t >;

    enum Masks : std::uint8_t
    {
        kQtrSecTimerMsgMask     = 0x01,
        k1SecTimerMsgMask       = 0x02,
        k8SecTimerMsgMask       = 0x04,
        kNavMsgMask             = 0x08,
        kNavStatusMask          = 0x10,
        kEncoderMsgMask         = 0x20,
        kCalibrationMsgMask     = 0x40,

        kAllMsgsOff             = 0x00,
        kAllMsgsOn              = 0xFF
    };


    MsgControlMsg() noexcept;
    explicit MsgControlMsg( TheData t ) noexcept; 
    explicit MsgControlMsg( std::uint8_t val ) noexcept;
    explicit MsgControlMsg( MsgId id );

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



class ResetPicoMsg : public NoContentMsg 
{
public:

    ResetPicoMsg() noexcept;
    explicit ResetPicoMsg( MsgId id ) noexcept;

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
    explicit TimerEventMsg( TheData t ) noexcept; 
    TimerEventMsg( std::uint8_t which, int count, uint32_t time ) noexcept;
    explicit TimerEventMsg( MsgId id );

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

    enum Masks : std::uint8_t
    {
        kQtrSecTimerMsgMask     = 0x01,
        k1SecTimerMsgMask       = 0x02,
        k8SecTimerMsgMask       = 0x04,

        kAllMsgsOff             = 0x00,
        kAllMsgsOn              = 0xFF
    };


    TimerControlMsg() noexcept;
    explicit TimerControlMsg( TheData t ) noexcept; 
    explicit TimerControlMsg( std::uint8_t val ) noexcept;
    explicit TimerControlMsg( MsgId id );

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
    explicit BeginCalibrationMsg( MsgId id ) noexcept;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class RequestCalibrationStatusMsg : public NoContentMsg 
{
public:

    RequestCalibrationStatusMsg() noexcept;
    explicit RequestCalibrationStatusMsg( MsgId id ) noexcept;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class CalibrationInfoUpdateMsg : public SerialMessage
{
public:

    using TheData = std::tuple< std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t >;

    CalibrationInfoUpdateMsg() noexcept;
    explicit CalibrationInfoUpdateMsg( TheData t ) noexcept; 
    CalibrationInfoUpdateMsg( std::uint8_t mag, std::uint8_t accel, std::uint8_t gyro, std::uint8_t sys ) noexcept;
    explicit CalibrationInfoUpdateMsg( MsgId id );

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
    explicit SetAutoCalibrateMsg( TheData t ) noexcept; 
    explicit SetAutoCalibrateMsg( bool val ) noexcept;
    explicit SetAutoCalibrateMsg( MsgId id );

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
    explicit ResetBNO055Msg( MsgId id ) noexcept;

    virtual void takeAction( EventManager& events, SerialLink& link ) override;
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////



class NavUpdateMsg : public SerialMessage
{
public:

    using TheData = std::tuple< float, std::uint32_t >;

    NavUpdateMsg() noexcept;
    explicit NavUpdateMsg( TheData t ) noexcept; 
    NavUpdateMsg( float heading, std::uint32_t time ) noexcept;
    explicit NavUpdateMsg( MsgId id );

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

    using TheData = std::tuple< std::uint8_t, std::uint8_t >;

    NavUpdateControlMsg() noexcept;
    explicit NavUpdateControlMsg( TheData t ) noexcept; 
    NavUpdateControlMsg( bool wantNavUpdate, bool wantNavStatusUpdate ) noexcept;
    explicit NavUpdateControlMsg( MsgId id );

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
    explicit DrivingStatusUpdateMsg( TheData t ) noexcept; 
    explicit DrivingStatusUpdateMsg( Drive driveStatus ) noexcept;
    explicit DrivingStatusUpdateMsg( MsgId id );

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
    explicit EncoderUpdateMsg( TheData t ) noexcept; 
    EncoderUpdateMsg( int left, int right, std::uint32_t time ) noexcept;
    explicit EncoderUpdateMsg( MsgId id );

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
    explicit EncoderUpdateControlMsg( TheData t ) noexcept; 
    explicit EncoderUpdateControlMsg( bool val ) noexcept;
    explicit EncoderUpdateControlMsg( MsgId id );

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
    explicit BatteryLevelRequestMsg( TheData t ) noexcept; 
    explicit BatteryLevelRequestMsg( Battery whichBattery ) noexcept;
    explicit BatteryLevelRequestMsg( MsgId id );

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
    explicit BatteryLevelUpdateMsg( TheData t ) noexcept; 
    BatteryLevelUpdateMsg( Battery whichBattery, float level ) noexcept;
    explicit BatteryLevelUpdateMsg( MsgId id );

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
    explicit BatteryLowAlertMsg( TheData t ) noexcept; 
    BatteryLowAlertMsg( Battery whichBattery, float level ) noexcept;
    explicit BatteryLowAlertMsg( MsgId id );

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
    explicit ErrorReportMsg( TheData t ) noexcept; 
    ErrorReportMsg( bool val, int errorCode ) noexcept;
    explicit ErrorReportMsg( MsgId id );

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
    explicit TestPicoErrorRptMsg( TheData t ) noexcept; 
    TestPicoErrorRptMsg( bool makeItFatal, int errorCodeToTest ) noexcept;
    explicit TestPicoErrorRptMsg( MsgId id );

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



class TestPicoMessagesMsg : public SerialMessage
{
public:

    using TheData = std::tuple< std::uint8_t >;

    TestPicoMessagesMsg() noexcept;
    explicit TestPicoMessagesMsg( TheData t ) noexcept; 
    explicit TestPicoMessagesMsg( std::uint8_t msgIdToSendBack ) noexcept;
    explicit TestPicoMessagesMsg( MsgId id );

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

    using TheData = std::tuple< int, std::uint8_t, float, std::uint32_t >;

    DebugLinkMsg() noexcept;
    explicit DebugLinkMsg( TheData t ) noexcept; 
    DebugLinkMsg( int val1_i, std::uint8_t val2_u8, float val3_f, std::uint32_t val4_u32 ) noexcept;
    explicit DebugLinkMsg( MsgId id );

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
