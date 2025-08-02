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

    std::cout << "Start tests" << std::endl;

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
        // We send 'i' and an integer
        std::uint8_t c{ 'i' };
        pico.putMsgType( c );
        int i{ 999 };
        pico.put( i );

        // We should get back 'I' and our integer + 1
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

        int iVal = i + 1;
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
        // We send 'j' and a negative integer
        std::uint8_t c{ 'j' };
        pico.putMsgType( c );
        int j{ -123456789 };
        pico.put( j );

        // We should get back 'J' and our integer + 1
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

        int iVal = j + 1;
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
        // We send 'k' and a large unsigned integer 
        std::uint8_t c{ 'k' };
        pico.putMsgType( c );
        std::uint32_t k{ 660327733 };
        pico.put( k );

        // We should get back 'K' and our integer + 1
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

        int iVal = k + 1;
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
        // We send 'f' and a float
        std::uint8_t c{ 'f' };
        pico.putMsgType( c );
        float f{ 2.71828 };
        pico.put( f );
        
        // We should get back 'F' and our float + 1
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

        float fVal = f + 1;
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
