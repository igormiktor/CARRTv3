/*
    EventHandlers.h - Actual handlers for Pico events.

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


#ifndef EventHandlers_h
#define EventHandlers_h


#include "EventHandler.h"



class NullEventHandler : public EventHandler
{
public:

    virtual void handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const;
};


// ********************** Timer event handlers


class QuarterSecondTimerHandler : public EventHandler
{
public:

    virtual void handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const;
};


class OneSecondTimerHandler : public EventHandler
{
public:

    virtual void handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const;
};


class EightSecondTimerHandler : public EventHandler
{
public:

    virtual void handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const;
};


// ********************** BNO055/navigation event handlers


class NavUpdateHandler : public EventHandler
{
public:

    virtual void handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const;
};


class InitializeBNO055Handler : public EventHandler
{
public:

    virtual void handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const;
};


class BNO055ResetHandler : public EventHandler
{
public:

    virtual void handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const;
};


class BeginCalibrationHandler : public EventHandler
{
public:

    virtual void handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const;
};


class SendCalibrationInfoHandler : public EventHandler
{
public:

    virtual void handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const;
};


// ********************** Encoder event handlers


// Go here


// ********************** Pulse LED event handlers


class PulsePicoLedHandler : public EventHandler
{
public:

    virtual void handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const;
};


// ********************** Battery event handlers


// Go here


// ********************** Pico Reset handlers


class PicoResetHandler : public EventHandler
{
public:

    virtual void handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const;
};


// ********************** Error event handlers


class ErrorEventHandler : public EventHandler
{
public:

    virtual void handleEvent( EventManager& events, SerialLink& link, int eventCode, int eventParam, std::uint32_t eventTime ) const;
};



#endif  // EventHandlers_h

