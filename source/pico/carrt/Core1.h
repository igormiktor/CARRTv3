/*
    Core1.h - Code that runs on CARRT-Pico Core 1 and code that runs
    on Core0 to launch Core 1 and/or to send messages to Core 1.

    Copyright (c) 2026 Igor Mikolic-Torreira.  All right reserved.

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

#ifndef Core1_h
#define Core1_h

#include <cstdint>

enum class EvtId;

namespace Core1
{

    void launchCore1();

    void queueEventForCore1( EvtId event, int waitMs = 0 );

    bool isRunningCore1();

}    // namespace Core1

#endif    // Core1.h
