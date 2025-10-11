/*
    CarrtPicoReset.cpp - CARRT-Pico's reset function.  Reinitializes the Pico.

    Copyright (c) 2024 Igor Mikolic-Torreira.  All right reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/




#include "CarrtPicoReset.h"

#include "PicoOutputUtils.hpp"
#include "SerialCommands.h"
#include "SerialLinkPico.h"
#include "Delays.h"

#include "pico/stdlib.h"
#include "hardware/sync.h"
#include "hardware/watchdog.h"


void PicoReset::reset( SerialLink& rpi0 )
{
    // This performs a "reboot" reset (so don't care about queues overflowing )
    // Let RPi0 know we are going to reset
    ResetCmd resetMsg;
    resetMsg.sendOut( rpi0 );

    output2cout( "Pico reseting via watchdog_reboot" );
    
    CarrtPico::sleep( 100ms );

    watchdog_reboot( 0, SRAM_END, 0 ); 

    // Wait for the reset to happen
    while ( 1 )
    {
        CarrtPico::sleep( 100ms );
    }
}


void PicoReset::fatalReset()
{
    // This performs a "reboot" reset after falling off the end of main
    // Only things we can rely on are low-level pico SDK functionality 
     
    watchdog_reboot( 0, SRAM_END, 0 ); 

    // Wait for the reset to happen
    while ( 1 )
    {
         tight_loop_contents(); 
    }
}

