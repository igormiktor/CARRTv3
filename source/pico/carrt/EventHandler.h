/*
    EventHandler.h - A base class to handle events.

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


#ifndef EventHandler_h
#define EventHandler_h


#include <cstdint>

class EventManager;
class SerialLink;

enum class EvtId;

class EventHandler 
{
public:

    EventHandler() = default;
    virtual ~EventHandler() = default;

    virtual void handleEvent( EventManager& events, SerialLink& link, EvtId eventCode, int eventParam, std::uint32_t eventTime ) const = 0;
};




#endif  // EventHandler_h

