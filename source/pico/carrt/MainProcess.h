/*
    MainProcess.h - Functions that manage CARRT-Pico's main function.  These run the primary event loop
    and dispaches to other code as needed.

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


class EventManager;
class SerialLinkPico;
class EventProcessor;
class SerialMessageProcessor;


namespace MainProcess
{
    void runMainEventLoop( EventManager& events, EventProcessor& ep, SerialMessageProcessor& smp, SerialLinkPico& rpi0 );
}

