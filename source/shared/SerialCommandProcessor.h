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



#include <unordered_map>
#include <string>
#include <memory>

#include "SerialCommand.h"
#include "CarrtError.h"




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
        if ( mCreators.find( id ) != mCreators.end() )
        {
            throw CarrtError( makeSharedErrorId( kSerialCmdDupeError, 1, id ), "Id dupe at registation" );
        }
        mCreators[id] = &creator<T>;
    }

    CmdPtr createCommand( CommandId id ) 
    {
        auto it = mCreators.find( id );
        if ( it != mCreators.end() )  
        {
            return it->second( id );
        }
        // If we cannot find the id, return a special command, UnknownCmd.
        int err = makeSharedErrorId( kSerialCmdUnknownError, 1, id );
        return std::unique_ptr<SerialCommand>( new UnknownCmd(  id, err ) );
    }


private:

    template <typename T>
    static CmdPtr creator( CommandId id )
    {
        // All SerialCommands must have a constructor that takes a uint8_t parameter (the ID)
        return CmdPtr( new T( id ) );
    }

    typedef CmdPtr (*PCreator)( CommandId );
    std::unordered_map<std::uint8_t, PCreator> mCreators;
};



class EventManager;

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

    void dispatchOneSerialCommand( EventManager& events, SerialLink& link );

    std::optional<CmdPtr> receiveCommandIfAvailable();

    template <typename T> 
    void registerCommand( CommandId id )
    {
        mFactory.registerCommand<T>( id );
    }



private:

    inline CmdPtr createCommandFromSerialLink( std::uint8_t id ) { return createCommandFromSerialLink( static_cast<CommandId>( id ) ); }
    CmdPtr createCommandFromSerialLink( CommandId id );


    CommandFactory      mFactory;
    SerialLink&         mLink;


};






#endif      // SerialCommandProcessor_h