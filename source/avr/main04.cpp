
#include "AVRTools/InitSystem.h"
#include "AVRTools/SystemClock.h"
#include "AVRTools/ArduinoPins.h"
#include "AVRTools/USART0.h"

#include "EventManager.h"
#include "EventClock.h"




void eighthSecondHandler(  int eventCode, int eventParam )
{
    static int pin;

    switch ( pin )
    {
        case 0:
            setGpioPinHigh( pPin03 );
            setGpioPinLow( pPin08 );
            break;

        case 1:
            setGpioPinHigh( pPin05 );
            setGpioPinLow( pPin03 );
            break;

        case 2:
            setGpioPinHigh( pPin08 );
            setGpioPinLow( pPin05 );
            break;
    }
    ++pin;
    pin %= 3;
}



void quarterSecondHandler(  int eventCode, int eventParam )
{
    switch ( eventParam % 2 )
    {
        case 0:
            setGpioPinHigh( pPin09 );
            break;

        case 1:
           setGpioPinLow( pPin09 );
            break;
    }

    USART0::write( "1/4 sec evt\n" );
}




void oneSecondHandler(  int eventCode, int eventParam )
{
    switch ( eventParam % 2 )
    {
        case 0:
            setGpioPinHigh( pPin11 );
            break;

        case 1:
           setGpioPinLow( pPin11 );
            break;
    }

    USART0::write( "1 sec evt\n" );
}





void eightSecondHandler(  int eventCode, int eventParam )
{
    static bool pin;

    if ( pin )
    {
        setGpioPinLow( pPin12 );
    }
    else
    {
        setGpioPinHigh( pPin12 );
    }

    pin = !pin;

    USART0::write( "8 sec evt\n" );
}




void clearLeds()
{
    setGpioPinLow( pPin03 );
    setGpioPinLow( pPin05 );
    setGpioPinLow( pPin08 );

    setGpioPinLow( pPin09 );
    setGpioPinLow( pPin11 );
    setGpioPinLow( pPin12 );
}



int main()
{
    initSystem();
    initSystemClock();

    Serial0 out;

    out.start( 115200 );

    EventManager::addListener( EventManager::kEighthSecondTimerEvent, eighthSecondHandler );
    EventManager::addListener( EventManager::kQuarterSecondTimerEvent, quarterSecondHandler );
    EventManager::addListener( EventManager::kOneSecondTimerEvent, oneSecondHandler );
    EventManager::addListener( EventManager::kEightSecondTimerEvent, eightSecondHandler );


    setGpioPinModeOutput( pPin13 );
    setGpioPinLow( pPin13 );

    setGpioPinModeOutput( pPin03 );
    setGpioPinModeOutput( pPin05 );
    setGpioPinModeOutput( pPin08 );

    setGpioPinModeOutput( pPin09 );
    setGpioPinModeOutput( pPin11 );
    setGpioPinModeOutput( pPin12 );

    setGpioPinHigh( pPin03 );
    setGpioPinHigh( pPin05 );
    setGpioPinHigh( pPin08 );

    setGpioPinHigh( pPin09 );
    setGpioPinHigh( pPin11 );
    setGpioPinHigh( pPin12 );

    delayMilliseconds( 3000 );

    clearLeds();

    bool waitingForGo = true;
    bool toggle = false;

    while ( waitingForGo )
    {
        if ( toggle )
        {
            setGpioPinHigh( pPin03 );
            setGpioPinLow( pPin12 );
        }
        else
        {
            setGpioPinLow( pPin03 );
            setGpioPinHigh( pPin12 );
        }
        toggle = !toggle;

        delayMilliseconds( 200 );

        if ( out.available() )
        {
            int got = out.read();

            if ( got == 'G' )
            {
                waitingForGo = false;
            }
        }
    }

    setGpioPinLow( pPin03 );
    setGpioPinLow( pPin12 );

    out.write( "Trinket got the GO\n" );

    EventClock::init();

    while ( 1 )
    {
        EventManager::processEvent();

        if ( USART0::available() )
        {
            int got = USART0::read();
            out.write( "Rcvd: " );
            out.write( static_cast<char>( got ) );
            out.write( '\n' );

            switch ( got )
            {
                case 'S':
                case 's':
                    EventClock::stop();
                    break;

                case 'G':
                case 'g':
                    clearLeds();
                    EventClock::init();
                    break;
            }
        }
    }
}
