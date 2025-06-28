#include <iostream>

#include "CarrtPigpio.h"

#include "DebugUtils.hpp"

int main(int, char**)
{
    gpioInitialise();

    std::cout << "Hello, from CarrtRpi!\n";

    debugM( "This is a debug message" );

    gpioTerminate();
}

