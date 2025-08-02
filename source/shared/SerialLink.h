/*
    SerialLink.h - Tools for CARRT3 communications over serial (UART) link
    between the RPI and Pico.  This file is shared by both the
    RPI and Pico code bases.  

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


#ifndef SerialLink_h
#define SerialLink_h




#include <cstdint>
#include <optional>

class SerialLink
{

public:

    // Abstract class that needs virtual destructor
    virtual ~SerialLink() = default;

    // Prevent copy, move, or assignment
    SerialLink( const SerialLink& ) = delete;
    SerialLink( SerialLink&& ) = delete;
    SerialLink& operator=( const SerialLink& ) = delete;
    SerialLink& operator=( SerialLink&& ) = delete;

    // Foundational functions, reading
    virtual std::optional<std::uint8_t> getByte() = 0;
    virtual std::optional<std::uint32_t> get4Bytes() = 0;
    virtual bool get4Bytes( std::uint8_t c[4] ) = 0;

    // Foundational functions, writing 
    virtual void putByte( std::uint8_t c ) = 0;
    virtual void put4Bytes( std::uint8_t c[4] ) = 0;


    // Reading functions
    std::optional<std::uint8_t> getMsgType()            { return getByte(); }
    std::optional<int> getInt();
    std::optional<std::uint32_t> getUInt32()            { return get4Bytes(); }
    std::optional<float> getFloat();


    // Writing functions
    inline void putMsgType( std::uint8_t cmd )  
        { putByte( cmd ); }
    inline void put( char c )                   
        { putByte( c ); }
    inline void put( std::uint8_t c )           
        { putByte( c ); }
    inline void put( int i )                    
        { Transfer t{}; t.i = i; put4Bytes( t.c ); }
    inline void put( std::uint32_t u )          
        { Transfer t{}; t.u = u; put4Bytes( t.c ); }
    inline void put( float f )                 
        { Transfer t{}; t.f = f; put4Bytes( t.c ); }


protected:

    // Only derived classes can create a SerialLink
    SerialLink() = default;     
    
    union Transfer
    {
        std::uint8_t    c[4];
        int             i;
        std::uint32_t   u;
        float           f;
    };


private:

};



#endif // SerialLink_h
