/*
    SerialCommandProcessor.h - Master processor for Serial Commands for both 
    the RPI and Pico.  This file is shared by both the RPI and Pico code bases.

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


#ifndef SerialCommandProcessor_h
#define SerialCommandProcessor_h



#include <map>
#include <string>
#include <memory>

#include "SerialCommand.h"



class CommandFactory
{
public:

    using CmdPtr = typename std::unique_ptr<SerialCommand>;

    CommandFactory() = default;
    ~CommandFactory() = default;
    
    CommandFactory( const CommandFactory& ) = delete;
    CommandFactory( CommandFactory&& ) = delete;
    CommandFactory& operator=( const CommandFactory& ) = delete;
    CommandFactory& operator=( CommandFactory&& ) = delete;


    template <typename T>
    void registerCommand( CommandId id )
    {
        static_assert( std::is_base_of<SerialCommand, T>::value, "CommandFactory::registerCmd: commands must derive from SerialCommand" );
        mCreators[id] = &creator<T>;
    }

    CmdPtr createCommand( CommandId id ) 
    {
        auto it = mCreators.find( id );
        if ( it != mCreators.end() )  
        {
//            return std::make_unique<SerialCommand>( it->second( id ) );
            return it->second( id );
        }
        return nullptr;
    }


private:

    template <typename T>
    static CmdPtr creator( CommandId id )
    {
        // All SerialCommands must have a constructor that takes a uint8_t parameter (the ID)
        return CmdPtr( new T( id ) );
    }

    typedef CmdPtr (*PCreator)( CommandId );
    std::map<std::uint8_t, PCreator> mCreators;
};




class SerialCommandProcessor
{
public:

    using CmdPtr = typename CommandFactory::CmdPtr;

    SerialCommandProcessor( SerialLink& link );

    ~SerialCommandProcessor() = default;
    
    SerialCommandProcessor( const SerialCommandProcessor& ) = delete;
    SerialCommandProcessor( SerialCommandProcessor&& ) = delete;
    SerialCommandProcessor& operator=( const SerialCommandProcessor& ) = delete;
    SerialCommandProcessor& operator=( SerialCommandProcessor&& ) = delete;


    template <typename T> 
    void registerCommand( CommandId id )
    {
        mFactory.registerCommand<T>( id );
    }





private:

    CmdPtr createCommandFromSerialLink( CommandId id );


    CommandFactory      mFactory;
    SerialLink&         mLink;


};



#endif      // SerialCommandProcessor_h