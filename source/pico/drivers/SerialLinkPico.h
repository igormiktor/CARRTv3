/*
    SerialLinkPico.h - Tools for CARRT3 communications over serial (UART) link
    between the RPI and Pico.  This is the Pico implementation of the SerialLink.  

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


#ifndef SerialLinkPico_h
#define SerialLinkPico_h




#include <cstdint>
#include <optional>

#include <SerialLink.h>


class SerialLinkPico : public SerialLink
{

public:

    SerialLinkPico();
    virtual ~SerialLinkPico();

    
    // Disable undesired defaults (create, delete, but no move, copy, or assignment)
    SerialLinkPico( const SerialLinkPico& ) = delete;
    SerialLinkPico( SerialLinkPico&& ) = delete;
    SerialLinkPico& operator=( const SerialLinkPico& ) = delete;
    SerialLinkPico& operator=( SerialLinkPico&& ) = delete;


    // Fundamental read functions
    std::optional<std::uint8_t> getByte() override;
    bool get4Bytes( std::uint8_t c[4] ) override;
    std::optional<std::uint32_t> get4Bytes() override;


    // Fundamental write functions
    void putByte( std::uint8_t c ) override;
    void put4Bytes( std::uint8_t c[4] ) override;


    // Bulk functions
    virtual int getBytes( int nbr, std::uint8_t* buffer ) override;
    virtual int putBytes( int nbr, std::uint8_t* buffer ) override;


private:

    int mSerialPort;

};



#endif // SerialLink_h
