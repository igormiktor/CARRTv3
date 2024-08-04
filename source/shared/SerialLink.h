/*
    SerialLink.h - Tools for CARRT3 communications over serial (UART) link
    between the RPI and Pico.  This file is shared by both the
    RPI and Pico code bases.

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


#ifndef SerialLink_h
#define SerialLink_h




#include <stdint.h>



namespace SerialLink
{

    union Transfer
    {
        uint8_t     c[4];
        int         i;
        uint32_t    u;
        float       f;
    };

    void openSerialLink();
    void closeSerialLink();

    bool isReadable();

    uint8_t getByte();
    void get4Bytes( uint8_t* c );

    inline uint8_t getMsg()  { return getByte(); }

    inline int getInt()  { SerialLink::Transfer t; SerialLink::get4Bytes( t.c ); return t.i; }

    inline uint32_t getUInt32()  { SerialLink::Transfer t; SerialLink::get4Bytes( t.c ); return t.u; }

    inline float getFloat()  { SerialLink::Transfer t; SerialLink::get4Bytes( t.c ); return t.f; }


    void putByte( uint8_t c );
    void put4Bytes( uint8_t* c );

    inline void putMsg( uint8_t cmd )  { putByte( cmd ); }
    inline void put( char c )   { SerialLink::putByte( c ); }
    inline void put( uint8_t c )   { SerialLink::putByte( c ); }
    inline void put( int i )  { SerialLink::Transfer t; t.i = i; SerialLink::put4Bytes( t.c ); }
    inline void put( uint32_t u )  { SerialLink::Transfer t; t.u = u; SerialLink::put4Bytes( t.c ); }
    inline void put( float f )  { SerialLink::Transfer t; t.f = f; SerialLink::put4Bytes( t.c ); }

};



#endif // SerialLink_h
