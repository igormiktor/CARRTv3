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



#include <array>
#include <bit>
#include <cstdint>
#include <cstring>
#include <optional>



enum class MsgId : std::uint8_t;


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
    virtual std::optional<MsgId> getMsgType() = 0;
    virtual std::optional<std::uint8_t> getByte() = 0;
    virtual std::optional<std::uint32_t> get4Bytes() = 0;
    virtual bool get4Bytes( std::uint8_t c[4] ) = 0;

    // Foundational functions, writing 
    virtual void putByte( std::uint8_t c ) = 0;
    virtual void put4Bytes( const std::uint8_t c[4] ) = 0;


    // Reading functions
    std::optional<int> getInt();
    std::optional<std::uint32_t> getUInt32()            { return get4Bytes(); }
    std::optional<float> getFloat();

    // Reading overloaded functions
    std::optional<std::uint8_t> get( std::uint8_t )     { return getByte(); }
    std::optional<int> get( int )                       { return getInt(); }
    std::optional<std::uint32_t> get( std::uint32_t )   { return getUInt32(); }
    std::optional<float> get( float )                   { return getFloat(); }

    
    // Bulk functions
    virtual int getBytes( int nbr, std::uint8_t* buffer ) = 0;
    virtual int putBytes( int nbr, const std::uint8_t* buffer ) = 0;


    // Writing functions
    inline void putMsgType( char msg )
        { putByte( static_cast<std::uint8_t>( msg ) );}
    inline void putMsgType( std::uint8_t msg )  
        { putByte( msg ); }
    inline void put( char c )                   
        { putByte( c ); }
    inline void put( std::uint8_t c )           
        { putByte( c ); }
    inline void put( int i )                    
        { RawData r( i ); put4Bytes( r.c() ); }
    inline void put( std::uint32_t u )          
        { RawData r( u ); put4Bytes( r.c() ); }
    inline void put( float f )                 
        { RawData r( f ); put4Bytes( r.c() ); }


protected:

    // Only derived classes can create a SerialLink
    SerialLink() = default;     

#if 0  // Turn off, remove soonish
    union Transfer
    {
        std::uint8_t    c[4];
        int             i;
        std::uint32_t   u;
        float           f;
    };
#endif

    class RawData
    {
    public:
        using RawDataT = std::array<std::uint8_t, 4>;

        RawData() = default;
        RawData( std::uint8_t* cc )
            { c( cc ); }
        RawData( int ii )
            { i( ii ); }
        RawData( std::uint32_t uu )
            { u( uu ); }
        RawData( float ff )
            { f( ff ); }

        std::uint8_t* c()
            { return mRaw.data(); }

        void c( std::uint8_t* cc )
            { std::memcpy( mRaw.data(), cc, 4 ); }

        int i()
            { return std::bit_cast<int>( mRaw ); }
        void i( int ii )
            { mRaw = std::bit_cast<RawDataT>( ii ); }

        std::uint32_t u()
            { return std::bit_cast<std::uint32_t>( mRaw ); }
        void u( std::uint32_t uu )
            { mRaw = std::bit_cast<RawDataT>( uu ); }

        float f()
            { return std::bit_cast<float>( mRaw ); }
        void f( float ff )
            { mRaw = std::bit_cast<RawDataT>( ff ); }

    private:
        RawDataT    mRaw;
    };

private:

};



#endif // SerialLink_h
