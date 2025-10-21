/*
    SerialLink.cpp - Tools for CARRT3 communications over serial (UART) link
    between the RPI and Pico.  This file is shared by both the
    RPi and Pico code bases.

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


#include "SerialLink.h"



std::optional<int> SerialLink::getInt()                  
{ 
    auto got = get4Bytes(); 
    if ( got )
        return static_cast<int>( *got );
    else 
        return std::nullopt; 
}

    

std::optional<float> SerialLink::getFloat()              
{ 
    auto got = get4Bytes(); 
    if ( got )
    {
        RawData r( *got );
        return r.f();
    }
    else 
        return std::nullopt; 
}
