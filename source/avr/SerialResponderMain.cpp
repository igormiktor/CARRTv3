
#include <ctype.h>

#include "AVRTools/InitSystem.h"
#include "AVRTools/SystemClock.h"
#include "AVRTools/ArduinoPins.h"
#include "AVRTools/USART0.h"

#include "EventManager.h"
#include "EventClock.h"




int main()
{
    initSystem();
    initSystemClock();

    delayMilliseconds( 500 );

    Serial0 rpi;
    rpi.start( 115200 );

    delayMilliseconds( 1000 );

    rpi.write( "Trinket started\n" );

    while ( 1 )
    {
        bool gotSomething = false;

        while ( rpi.available() )
        {
            gotSomething = true;
            // Echo everything until we got no more
            int got = rpi.read();
            rpi.print( got );
            rpi.print( ' ' );
            if ( got == '\n' )
            {
                rpi.print( "(\\n)" );
            }
            else if ( isprint( got ) )
            {
                rpi.print( '(' );
                rpi.print( static_cast<char>( got ) );
                rpi.print( ')' );
            }
            else
            {
                rpi.print( "()" );
            }
            rpi.print( ", " );
        }
        if ( gotSomething )
        {
            // End the line
            rpi.println( "***" );
            gotSomething = false;
        }
    }
}
