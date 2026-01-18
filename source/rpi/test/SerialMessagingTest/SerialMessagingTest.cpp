#include <iostream>
#include <string>

#include "Clock.h"
#include "DebugUtils.hpp"
#include "OutputUtils.hpp"
#include "SerialLinkRPi.h"
#include "SerialMessageProcessor.h"
#include "SerialMessages.h"

void doDebugLinkTest( SerialLink& link )
{
    DebugLinkMsg cmd0( 1, 2, 3.0, 4 );
    cmd0.sendOut( link );
}

class EventManager
{};

void setupMessageProcessor( SerialMessageProcessor& smp );

int main()
{
    Clock::initSystemClock();
    SerialLinkRPi pico;

    std::cout << "Serial link test" << std::endl;

    SerialMessageProcessor smp( 32, pico );
    setupMessageProcessor( smp );

    EventManager events;

    std::cout << "Tests begin" << std::endl;

    try
    {
        auto start{ Clock::millis() };

        std::uint8_t which{ 0 };
        while ( true )
        {
            Clock::sleep( 10ms );

            smp.dispatchOneSerialMessage( events, pico );

            if ( ( Clock::millis() - start ) > 1'000 )
            {
                switch ( which )
                {
                    case 0:    // No msg with MsgId 0, ping instead
                        output2cout( "Top of message test loop" );
                        output2cout( "Sending Ping" );
                        {
                            PingMsg ping;
                            ping.sendOut( pico );
                        }
                        break;

                    case static_cast<int>( MsgId::kPicoReceivedTestMsg ):
                        // Ignore this one
                        break;

                    default:
                    {
                        // Need to use the std::uint8_t constructor
                        // (not the MsgId constructor)
                        TestPicoMessagesMsg reqestSendUsBack( which );
                        reqestSendUsBack.sendOut( pico );
                    }
                    break;
                }

                start = Clock::millis();

                ++which;
                which %= std::to_underlying( MsgId::kCountOfMsgIds );
            }
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
    smp.registerMessage<BatteryLowAlertMsg>( MsgId::kBatteryLowAlert );
    smp.registerMessage<ErrorReportMsg>( MsgId::kErrorReportFromPico );
    //  smp.registerMessage<TestPicoErrorRptMsg>( MsgId::kTestPicoReportError );
    //  smp.registerMessage<TestPicoMessagesMsg>( MsgId::kTestPicoMessages );
    smp.registerMessage<PicoReceivedTestMsg>( MsgId::kPicoReceivedTestMsg );
    smp.registerMessage<DebugLinkMsg>( MsgId::kDebugSerialLink );
}
