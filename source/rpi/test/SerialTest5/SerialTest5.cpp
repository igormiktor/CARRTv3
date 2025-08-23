#include <iostream>
#include <string>

#include "SerialLinkRPi.h"
#include "SerialCommands.h"
#include "SerialCommandProcessor.h"
#include "Clock.h"


#include "DebugUtils.hpp"


void doDebugLinkTest( int val1, int val2, SerialLink& link )
{
    DebugLinkCmd cmd0( 1, 0 );
    cmd0.sendOut( link ); 
    std::cout << "Sent debug link cmd: " << val1 << ", " << val2 << std::endl;
}


void doTest( int i, SerialLink& link )
{
    int j = i % 4;

    switch ( j )
    {
        case 0:
            doDebugLinkTest( -100, 200, link );
            break;

        case 1:
            doDebugLinkTest( -1, 0, link );
            break;

        case 3:
            doDebugLinkTest( 111, 666, link );
            break;

        case 4:
            doDebugLinkTest( 111, 666, link );
            break;
    }
}



int main() 
{
    SerialLinkRPi  pico;

    std::cout << "Serial link test" << std::endl;

    SerialCommandProcessor scp( pico );
    scp.registerCommand<TimerEventCmd>( kTimerEvent );
    scp.registerCommand<TimerControlCmd>( kTimerControl );
    scp.registerCommand<ErrorReportCmd>( kErrorReportFromPico );
    scp.registerCommand<DebugLinkCmd>( kDebugSerialLink );

    std::cout << "Tests begin" << std::endl;

    try
    {
        int toggle{ 0 };
        int i{ 1 }; 
        while ( i )
        {
            auto cmd{ scp.receiveCommandIfAvailable() };
            if ( cmd )
            {
                cmd.value()->takeAction( pico );
            }

            doTest( i++, pico );

            if ( 0 == i % 8 )
            {
                int onOff = (++toggle) % 2; 
                TimerControlCmd toggleTimerEvts( onOff );
                toggleTimerEvts.sendOut( pico );
            }

            Clock::delayMilliseconds( 1000 );

            i %= 32;
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
