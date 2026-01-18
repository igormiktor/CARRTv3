
#include <iostream>
#include <string>

#include "DebugUtils.hpp"
#include "SerialLinkRPi.h"
#include "SerialMessage.h"

union Transfer
{
    std::uint8_t c[ 4 ];
    int i;
    std::uint32_t u;
    float f;
};

int main()
{
    std::cout << "Serial link test" << std::endl;

    SerialLinkRPi pico;

    std::cout << "Start T test" << std::endl;

    {
        std::uint8_t c{ 'T' };
        pico.put( c );
        auto got = pico.getByte();
        if ( got )
        {
            if ( *got == c )
                std::cout << "Success";
            else
                std::cout << "Failure";
            std::cout << ": sent " << static_cast<char>( c ) << ", got "
                      << static_cast<char>( *got ) << std::endl;
        }
        else
        {
            std::cout << "Failure: nothing read" << std::endl;
        }
    }

    {
        std::uint8_t c{ 'i' };
        pico.putMsgType( c );
        auto got = pico.getMsgType();
        while ( !got )
        {
            got = pico.getMsgType();
        }
        // Pico always responds uppercase
        if ( std::to_underlying( *got ) == 'I' )
            std::cout << "Success";
        else
            std::cout << "Failure";
        std::cout << ": sent " << static_cast<char>( c ) << ", got " << static_cast<char>( *got )
                  << std::endl;

        int iVal = 123'456'789;
        auto got2 = pico.getInt();
        while ( !got2 )
        {
            got2 = pico.getInt();
        }
        if ( *got2 == iVal )
            std::cout << "Success";
        else
            std::cout << "Failure";
        std::cout << ": expected " << iVal << " got " << *got2 << std::endl;
    }

    {
        std::uint8_t c{ 'j' };
        pico.putMsgType( c );
        auto got = pico.getMsgType();
        while ( !got )
        {
            got = pico.getMsgType();
        }
        // Pico always responds uppercase
        if ( std::to_underlying( *got ) == 'J' )
            std::cout << "Success";
        else
            std::cout << "Failure";
        std::cout << ": sent " << static_cast<char>( c ) << ", got " << static_cast<char>( *got )
                  << std::endl;

        int iVal = -123'456'789;
        auto got2 = pico.getInt();
        while ( !got2 )
        {
            got2 = pico.getInt();
        }
        if ( *got2 == iVal )
            std::cout << "Success";
        else
            std::cout << "Failure";
        std::cout << ": expected " << iVal << " got " << *got2 << std::endl;
    }

    {
        std::uint8_t c{ 'k' };
        pico.putMsgType( c );
        auto got = pico.getMsgType();
        while ( !got )
        {
            got = pico.getMsgType();
        }
        // Pico always responds uppercase
        if ( std::to_underlying( *got ) == 'K' )
            std::cout << "Success";
        else
            std::cout << "Failure";
        std::cout << ": sent " << static_cast<char>( c ) << ", got " << static_cast<char>( *got )
                  << std::endl;

        int iVal = 660'327'733;
        auto got2 = pico.getInt();
        while ( !got2 )
        {
            got2 = pico.getInt();
        }
        if ( *got2 == iVal )
            std::cout << "Success";
        else
            std::cout << "Failure";
        std::cout << ": expected " << iVal << " got " << *got2 << std::endl;
    }

    {
        std::uint8_t c{ 'f' };
        pico.putMsgType( c );
        auto got = pico.getMsgType();
        while ( !got )
        {
            got = pico.getMsgType();
        }
        // Pico always responds uppercase
        if ( std::to_underlying( *got ) == 'F' )
            std::cout << "Success";
        else
            std::cout << "Failure";
        std::cout << ": sent " << static_cast<char>( c ) << ", got " << static_cast<char>( *got )
                  << std::endl;

        float fVal = 2.71828;
        auto got2 = pico.getFloat();
        while ( !got2 )
        {
            got2 = pico.getFloat();
        }
        if ( *got2 == fVal )
            std::cout << "Success";
        else
            std::cout << "Failure";
        std::cout << ": expected " << fVal << " got " << *got2 << std::endl;
    }

    return 0;    // success
};
