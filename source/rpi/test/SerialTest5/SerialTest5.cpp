#include <iostream>
#include <string>

#include "SerialLinkRPi.h"
#include "SerialMessages.h"
#include "SerialMessageProcessor.h"
#include "Clock.h"


#include "DebugUtils.hpp"


void doDebugLinkTest( int val1, int val2, SerialLink& link )
{
    DebugLinkMsg cmd0( val1, val2 );
    cmd0.sendOut( link ); 
    std::cout << "Sent debug link cmd: " << val1 << ", " << val2 << std::endl;
}


void doTest( int i, SerialLink& link )
{
    int j = i % 4;

    std::cout << "*********** Doing Test " << j << std::endl;
    switch ( j )
    {
        case 0:
            doDebugLinkTest( -100, 200, link );
            break;

        case 1:
            doDebugLinkTest( -1, 0, link );
            break;

        case 2:
            doDebugLinkTest( 18, 61, link );
            break;

        case 3:
            doDebugLinkTest( 111, 666, link );
            break;
    }
}


class EventManager
{

};


int main() 
{
    Clock::initSystemClock();
    SerialLinkRPi  pico;


    std::cout << "Serial link test" << std::endl;

    SerialMessageProcessor smp( 32, pico );
    smp.registerMessage<TimerEventMsg>( kTimerEvent );
    smp.registerMessage<TimerControlMsg>( kTimerControl );
    smp.registerMessage<ErrorReportMsg>( kErrorReportFromPico );
    smp.registerMessage<DebugLinkMsg>( kDebugSerialLink );

    std::cout << "Tests begin" << std::endl;

    try
    {
        bool keepGoing{ true };
        bool timerEventsOn{ false };
        int testNbr{ 0 };
        long start{ Clock::millis() };
        long lastTest{ start + 5000 };
        long lastToggle{ start - 15000 };

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
            auto timeForTest{ (now - lastTest) / 10000 };
            if ( timeForTest )
            {
                doTest( testNbr++, pico );
                lastTest = Clock::millis();
            }
#endif

#if 1
            auto timeForToggle{ (now - lastToggle) / 20000 };
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
            if ( (now - start) / 1000 > 4*60 )
            {
                keepGoing = false;
            }
            
            Clock::delayMilliseconds(10);
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

    catch (...)
    {
        std::cerr << "Error of unknown type." << std::endl;
    }


    std::cout << "Exiting test" << std::endl;
    

    return 0; 
};
