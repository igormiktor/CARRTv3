#include <iostream>

#include "CarrtPigpio.h"

#include "DebugUtils.hpp"
#include "OutputUtils.hpp"


/*
        -DDEBUGUTILS_ON=${DEBUGUTILS_VAL} 
        -DUSE_CARRTRPI0_STDIO=${CARRT_RPI0_ENABLE_STDIO_OUTPUT_VAL}
        "$<$<CONFIG:DEBUG>:DEBUGRPI0=1>" 
        -DBUILDING_FOR_RPI0=1
*/

#if 0   // Use this to test that compile definitions inherit properly in cmake build

#if defined( USE_CARRTRPI0_STDIO ) && DEBUGUTILS_ON
    #pragma message "DEBUGUTILS_ON = ON"
#elif defined( USE_CARRTRPI0_STDIO )
    #pragma message "DEBUGUTILS_ON = OFF"
#else
    #pragma message "DEBUGUTILS_ON = undef"
#endif

#if defined( USE_CARRTRPI0_STDIO ) && USE_CARRTRPI0_STDIO
    #pragma message "USE_CARRTRPI0_STDIO = ON"
#elif defined( USE_CARRTRPI0_STDIO )
    #pragma message "USE_CARRTRPI0_STDIO = OFF"
#else
    #pragma message "USE_CARRTRPI0_STDIO = undef"
#endif

#if defined( DEBUGRPI0 ) && DEBUGRPI0
    #pragma message "DEBUGRPI0 = ON"
#elif defined( DEBUGRPI0 ) 
    #pragma message "DEBUGRPI0 = OFF"
#else
    #pragma message "DEBUGRPI0 = undef"
#endif

#if defined( BUILDING_FOR_RPI0 ) && BUILDING_FOR_RPI0
    #pragma message "BUILDING_FOR_RPI0 = ON"
#elif defined( BUILDING_FOR_RPI0 ) 
    #pragma message "BUILDING_FOR_RPI0 = OFF"
#else
    #pragma message "BUILDING_FOR_RPI0 = undef"
#endif

#if defined( USE_PIGPIOD ) && USE_PIGPIOD
    #pragma message "USE_PIGPIOD = ON"
#elif defined( USE_PIGPIOD ) 
    #pragma message "USE_PIGPIOD = OFF"
#else
    #pragma message "USE_PIGPIOD = undef"
#endif

#endif  // Checking compile definitions



int main(int, char**)
{
    gpioInitialise();

    std::cout << "Hello, from CarrtRpi!\n";

    debugM( "This is a debug message" );

    output2cout( "This is a conditional stdio message" );

    gpioTerminate();
}

