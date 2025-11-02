#include <iostream>

#include "CarrtPigpio.h"

#include "DebugUtils.hpp"
#include "OutputUtils.hpp"

int main(int, char**)
{
    gpioInitialise();

    std::cout << "Hello, from CarrtRpi!\n";

    debugM( "This is a debug message" );

    output2cout( "This is a conditional stdio message" );

    gpioTerminate();
}

