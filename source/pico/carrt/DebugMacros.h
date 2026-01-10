/*
    DebugMacros.h - Macros for debugging on CARRT-Pico 

    Copyright (c) 2025 Igor Mikolic-Torreira.  All right reserved.

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


#ifndef DebugMacros_h
#define DebugMacros_h


#include "CarrtPicoDefines.h"

#include <iostream>


#if USE_CARRTPICO_STDIO

#define INFO_PICO_MSG1( x )            do { std::cout << x << std::endl; } while (0);
#define INFO_PICO_MSG2( x, y )         do { std::cout << x << y << std::endl; } while (0);
#define INFO_PICO_MSG3( x, y, z )      do { std::cout << x << y << z << std::endl; } while (0);
#define INFO_PICO_MSG4( x, y, z, t )   do { std::cout << x << y << z << t << std::endl; } while (0);

#else

#define INFO_PICO_MSG1( x )     
#define INFO_PICO_MSG2( x, y ) 
#define INFO_PICO_MSG3( x, y, z )      
#define INFO_PICO_MSG4( x, y, z, t )  

#endif



#if  DEBUGCARRTPICO

#define DEBUG_PICO_MSG1( x )            do { std::cout << x << std::endl; } while (0);
#define DEBUG_PICO_MSG2( x, y )         do { std::cout << x << y << std::endl; } while (0);

#else

#define DEBUG_PICO_MSG1( x )     
#define DEBUG_PICO_MSG2( x, y ) 

#endif





#endif  // DebugMacros_h