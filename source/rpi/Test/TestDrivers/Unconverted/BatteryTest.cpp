/*
    BatteryTest.cpp - Testing harness for the battery level driver

    Copyright (c) 2016 Igor Mikolic-Torreira.  All right reserved.

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




#include "AVRTools/InitSystem.h"
#include "AVRTools/SystemClock.h"
#include "AVRTools/Analog2Digital.h"
#include "AVRTools/USART0.h"

#include "Drivers/Battery.h"



int main()
{
    initSystem();
    initSystemClock();
    initA2D( kA2dReference256V );

    Serial0 out;
    out.start( 115200 );

    delayMilliseconds( 1000 );

    out.println( "Battery test..." );

    Battery::initBatteryStatusDisplay();

    while ( 1 )
    {
        int cpuV = Battery::getCpuBatteryMilliVoltage();
        int motorV = Battery::getMotorBatteryMilliVoltage();

        out.print( "CPU V = " );
        out.print( cpuV );
        out.print( " mV;  Motor mV = " );
        out.print( motorV );
        out.println( " mV" );

        Battery::checkAndDisplayBatteryStatus();

        delayMilliseconds( 5000 );
    }
}

