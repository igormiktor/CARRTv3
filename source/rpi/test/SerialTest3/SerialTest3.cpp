#include "SerialMessage.h"
#include "SerialLinkRPi.h"

#include "DebugUtils.hpp"

// library headers
#include <iostream>
#include <string>

int main() 
{
    std::cout << "Serial link test" << std::endl;

    SerialLinkRPi  pico;

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
            std::cout << ": sent " << static_cast<char>( c ) << ", got " << static_cast<char>( *got ) << std::endl;
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
        if ( *got == 'I' )      // Pico always responds uppercase
            std::cout << "Success";
        else
            std::cout << "Failure";
        std::cout << ": sent " << static_cast<char>( c ) << ", got " << static_cast<char>( *got ) << std::endl;

        int iVal = 123456789;
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
        if ( *got == 'J' )      // Pico always responds uppercase
            std::cout << "Success";
        else
            std::cout << "Failure";
        std::cout << ": sent " << static_cast<char>( c ) << ", got " << static_cast<char>( *got ) << std::endl;

        int iVal = -123456789;
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
        if ( *got == 'K' )      // Pico always responds uppercase
            std::cout << "Success";
        else
            std::cout << "Failure";
        std::cout << ": sent " << static_cast<char>( c ) << ", got " << static_cast<char>( *got ) << std::endl;

        int iVal = 660327733;
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
        if ( *got == 'F' )      // Pico always responds uppercase
            std::cout << "Success";
        else
            std::cout << "Failure";
        std::cout << ": sent " << static_cast<char>( c ) << ", got " << static_cast<char>( *got ) << std::endl;

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

    return 0; // success
};
