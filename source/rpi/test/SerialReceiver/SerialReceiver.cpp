#include <iostream>
#include <string>

#include "Clock.h"
#include "DebugUtils.hpp"
#include "OutputUtils.hpp"
#include "SerialLinkRPi.h"
#include "SerialMessageProcessor.h"
#include "SerialMessages.h"

class EventManager
{};

void setupMessageProcessor( SerialMessageProcessor& smp );

int main()
{
    Clock::initSystemClock();
    SerialLinkRPi pico;

    std::cout << "Serial link recevier -- report on every message received" << std::endl;

    SerialMessageProcessor smp( 32, pico );
    setupMessageProcessor( smp );

    EventManager events;

    try
    {
        // Turn on/off messages as desired
        /*
            kQtrSecTimerMsgMask = 0x01,
            k1SecTimerMsgMask = 0x02,
            k8SecTimerMsgMask = 0x04,
            kNavMsgMask = 0x08,
            kNavStatusMask = 0x10,
            kEncoderMsgMask = 0x20,
            kCalibrationMsgMask = 0x40,
            kBatteryMsgMask = 0x80,

            kAllMsgsOff = 0x00,
            kAllMsgsOn = 0xFF
        */
        
        [[maybe_unused]]
        std::uint8_t selectedMsgs = MsgControlMsg::k1SecTimerMsgMask
                                   | MsgControlMsg::k8SecTimerMsgMask
                                   | MsgControlMsg::kNavStatusMask 
                                   | MsgControlMsg::kEncoderMsgMask
                                   | MsgControlMsg::kCalibrationMsgMask
                                   | MsgControlMsg::kBatteryMsgMask;

        MsgControlMsg desiredMsgs( MsgControlMsg::kAllMsgsOn );
        desiredMsgs.sendOut( pico );

        SetAutoCalibrateMsg autoCalib( true );
        autoCalib.sendOut( pico );

        while ( true )
        {
            Clock::sleep( 10ms );

            smp.dispatchOneSerialMessage( events, pico );
        }
    }

    catch ( const CarrtError& err )
    {
        std::cerr << "Error: " << err.errorCode() << ", " << err.what() << std::endl;
    }

    catch ( const std::exception& err )
    {
        std::cerr << "Error: " << err.what() << std::endl;
    }

    catch ( ... )
    {
        std::cerr << "Error of unknown type." << std::endl;
    }

    std::cout << "Exiting test" << std::endl;

    return 0;
};

void setupMessageProcessor( SerialMessageProcessor& smp )
{
    // Only register those messages we actually can receive
    // Messages that are only outgoing don't need to be registered
    smp.registerMessage<PingMsg>( MsgId::kPingMsg );
    smp.registerMessage<PingReplyMsg>( MsgId::kPingReplyMsg );
    smp.registerMessage<PicoReadyMsg>( MsgId::kPicoReady );
    smp.registerMessage<PicoNavStatusUpdateMsg>( MsgId::kPicoNavStatusUpdate );
    smp.registerMessage<PicoSaysStopMsg>( MsgId::kPicoSaysStop );
    //  smp.registerMessage<MsgControlMsg>( MsgId::kMsgControlMsg );
    smp.registerMessage<ResetPicoMsg>( MsgId::kResetPicoMsg );
    smp.registerMessage<TimerEventMsg>( MsgId::kTimerEventMsg );
    //  smp.registerMessage<TimerControlMsg>( MsgId::kTimerControl );
    //  smp.registerMessage<BeginCalibrationMsg>( MsgId::kBeginCalibration );
    //  smp.registerMessage<RequestCalibrationStatusMsg>( MsgId::kRequestCalibStatus );
    smp.registerMessage<CalibrationInfoUpdateMsg>( MsgId::kCalibrationInfoUpdate );
    //  smp.registerMessage<SetAutoCalibrateMsg>( MsgId::kSetAutoCalibrate );
    //  smp.registerMessage<ResetBNO055Msg>( MsgId::kResetBNO055 );
    smp.registerMessage<NavUpdateMsg>( MsgId::kTimerNavUpdate );
    //  smp.registerMessage<NavUpdateControlMsg>( MsgId::kNavUpdateControl );
    //  smp.registerMessage<DrivingStatusUpdateMsg>( MsgId::kDrivingStatusUpdate );
    smp.registerMessage<EncoderUpdateMsg>( MsgId::kEncoderUpdate );
    //  smp.registerMessage<EncoderUpdateControlMsg>( MsgId::kEncoderUpdateControl );
    //  smp.registerMessage<BatteryLevelRequestMsg>( MsgId::kBatteryLevelRequest );
    smp.registerMessage<BatteryLevelUpdateMsg>( MsgId::kBatteryLevelUpdate );
    smp.registerMessage<ErrorReportMsg>( MsgId::kErrorReportFromPico );
    //  smp.registerMessage<TestPicoErrorRptMsg>( MsgId::kTestPicoReportError );
    //  smp.registerMessage<TestPicoMessagesMsg>( MsgId::kTestPicoMessages );
    smp.registerMessage<PicoReceivedTestMsg>( MsgId::kPicoReceivedTestMsg );
    smp.registerMessage<DebugLinkMsg>( MsgId::kDebugSerialLink );
}
