#include <iostream>
#include <string>

#include "Clock.h"
#include "DebugUtils.hpp"
#include "OutputUtils.hpp"
#include "SerialLinkRPi.h"
#include "SerialMessageProcessor.h"
#include "SerialMessages.h"

void doDebugLinkTest( int val1_i, std::uint8_t val2_u8, float val3_f, std::uint32_t val4_u32,
                      SerialLink& link )
{
    DebugLinkMsg cmd0( val1_i, val2_u8, val3_f, val4_u32 );
    cmd0.sendOut( link );
    output2cout( "Sent debug link cmd", val1_i, val2_u8, val3_f, val4_u32 );
}

void doTest( int i, SerialLink& link )
{
    int j = i % 4;

    std::cout << "*********** Doing Test " << j << std::endl;
    switch ( j )
    {
        case 0:
            doDebugLinkTest( -100, 200, -1.1f, 3'000, link );
            break;

        case 1:
            doDebugLinkTest( -1, 0, -2.71828f, 12'345, link );
            break;

        case 2:
            doDebugLinkTest( 18, 254, 3.14159f, 54'321, link );
            break;

        case 3:
            doDebugLinkTest( -111, 66, -6.66f, 66'666, link );
            break;
    }
}

class EventManager
{};

int main()
{
    Clock::initSystemClock();
    SerialLinkRPi pico;

    std::cout << "Serial link test" << std::endl;

    SerialMessageProcessor smp( 32, pico );
    smp.registerMessage<TimerEventMsg>( MsgId::kTimerEventMsg );
    smp.registerMessage<TimerControlMsg>( MsgId::kTimerControl );
    smp.registerMessage<ErrorReportMsg>( MsgId::kErrorReportFromPico );
    smp.registerMessage<DebugLinkMsg>( MsgId::kDebugSerialLink );

    std::cout << "Tests begin" << std::endl;

    try
    {
        bool keepGoing{ true };
        bool timerEventsOn{ false };
        int testNbr{ 0 };
        long start{ Clock::millis() };
        long lastTest{ start + 5'000 };
        long lastToggle{ start - 15'000 };

        while ( keepGoing )
        {
            auto cmd{ smp.receiveMessageIfAvailable() };
            if ( cmd )
            {
                EventManager events;
                cmd.value()->takeAction( events, pico );
            }

            long now{ Clock::millis() };

#if 1
            auto timeForTest{ ( now - lastTest ) / 10'000 };
            if ( timeForTest )
            {
                doTest( testNbr++, pico );
                lastTest = Clock::millis();
            }
#endif

#if 1
            auto timeForToggle{ ( now - lastToggle ) / 20'000 };
            if ( timeForToggle )
            {
                if ( timerEventsOn )
                {
                    timerEventsOn = false;
                }
                else
                {
                    timerEventsOn = true;
                }
                std::cout << "************ Timer Events turned on? " << timerEventsOn << std::endl;
                TimerControlMsg toggleTimerEvts( timerEventsOn );
                toggleTimerEvts.sendOut( pico );
                lastToggle = Clock::millis();
            }
#endif
            if ( ( now - start ) / 1'000 > 4 * 60 )
            {
                keepGoing = false;
            }

            Clock::sleep( 10ms );
        }

        std::cout << "Tests end" << std::endl;
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
