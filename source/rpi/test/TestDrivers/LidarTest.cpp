/*
    LidarTest.cpp - Test of how the lidar ranges across various configurations.

    Copyright (c) 2025 Igor Mikolic-Torreira.  All right reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; withlaptop even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include <cmath>
#include <cstdlib>
#include <cstring>

#include "CarrtPigpio.h"

#include "Clock.h"
#include "Keypad.h"
#include "Lcd.h"
#include "I2c.h"
#include "Servo.h"

#include "CarrtError.h"

#include "DebugUtils.hpp"

#include "Lidar.h"




bool respondToInput( const std::string& inputLine );
void doPing();
void doUpdateLidarMode( int pingSize );
void doInstructions();
void doConfigScan();




Lidar::Configuration gLidarMode;




int main()
{
    try
    {
        gpioInitialise();
        Clock::initSystemClock();

        Lidar::init();
        gLidarMode = Lidar::kDefault;

        Clock::sleep( 3000ms );

        doInstructions();

        bool stop{ false };
        while ( !stop )
        {
            try
            {            
                std::string inputLine{};
                std::getline( std::cin, inputLine );

                if ( !inputLine.empty() )
                {
                    stop = respondToInput( inputLine );
                }
            }        

            catch( const I2c::I2cError& err )
            {
                if ( err.errorCode() % 100 == -82 || err.errorCode() % 100 == -83 )
                {
                    // Just print out message and continue
                    std::cerr << "Continuing... Error: " << err.errorCode() << ", " << err.what() << std::endl;
                }
                else
                {
                    // Rethrow
                    throw;
                }
            }
        }
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

}



void doInstructions()
{
    std::cout << "Lidar basic ranging test..." << std::endl;
    std::cout << "Enter p (or P) to ping the Lidar" << std::endl;
    std::cout << "Enter c (or C) followed by nbr set a lidar configuration" << std::endl;
    std::cout << "Enter s (or S) for a configuration scan" << std::endl;
}



void doPing()
{
    int rng;

    int err = Lidar::getDistanceInCm( &rng );
    if ( !err )
    {
        std::cout << "One ping Range:  " << rng << std::endl;
    }
    else
    {
        std::cout << "Range attempt produced an error: " << err << std::endl;
    }

    err = Lidar::getMedianDistanceInCm( &rng );
    if ( !err )
    {
        std::cout << "Five ping median Range:  " << rng << std::endl;
    }
    else
    {
        std::cout << "Median range attempt produced an error: " << err << std::endl;
    }
}



void doLidarModeChange( char token )
{
    // Quick and crappy conversion of single digit '0' - '9' to numeric value
    // Definitely error prone.  Works for now for a test.
    int mode = token - '0';

    if ( mode >= Lidar::kDefault && mode <= Lidar::kLowSensitivityButLowerError )
    {
        doUpdateLidarMode( mode );
    }
    else
    {
        std::cout << "Mode " << token << " (" << mode << ") is out of range." << std::endl;
    }
}



void doUpdateLidarMode( int mode )
{
    static const char* modeStr[] =
    {
        "1 - Default",
        "2 - Short range, fast",
        "3 - Shorter range, fastest",
        "4 - Default range, faster at short range",
        "5 - Maximum range",
        "6 - High sensitivity, high error",
        "7 - Low sensitivity, low error"
    };


    gLidarMode = static_cast<Lidar::Configuration>( mode );

    Lidar::setConfiguration( gLidarMode );

    std::cout << "Lidar mode:  " <<  modeStr[mode] << std::endl;
}


void doConfigScan()
{
    std::cout << "Config Mode Scan..." << std::endl;

    for ( int i = Lidar::kDefault; i <= Lidar::kLowSensitivityButLowerError; ++i )
    {
        std::cout << i <<  ",   "; 
    }
    std::cout << std::endl;

    for ( int i = Lidar::kDefault; i <= Lidar::kLowSensitivityButLowerError; ++i )
    {
        Lidar::setConfiguration( static_cast<Lidar::Configuration>( i ) );
        Clock::sleep( 10ms );
        int rng;
        Lidar::getMedianDistanceInCm( &rng );
        std::cout << rng <<  ",   ";
    }
    std::cout << std::endl;

    Lidar::setConfiguration( Lidar::kDefault );
}



bool respondToInput( const std::string& inputLine )
{
    std::stringstream ssInput( inputLine );
    std::string token{};

    std::getline( ssInput, token, ' ' );

    if ( !token.empty() )
    {
        switch ( token[0] )
        {
            case 'p':
            case 'P':
                doPing();
                break;

            case 'c':
            case 'C':
                std::getline( ssInput, token, ' ' );   
                if ( token.empty() ) 
                {
                    std::cout << "Missing mode number after 'C'" << std::endl;
                }
                else
                {
                    doLidarModeChange( token[0] );
                }
                break;

            case 's':
            case 'S':
                doConfigScan();
                break;
        
            case 'q':
            case 'Q': 
                return true;
        }
    }

    return false;
}

