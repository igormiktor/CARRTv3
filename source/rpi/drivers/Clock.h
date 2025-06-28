/*
    Clock.h - Functions to initialize and use a clock
    for CARRTv3 on Raspberry Pi Zero W.
    Copyright (c) 2019 Igor Mikolic-Torreira.  All right reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef Clock_h
#define Clock_h


namespace Clock
{

    void initSystemClock();


    void delayMicroseconds( long us );


    void delayMilliseconds( long ms );


    long micros();


    long millis();
}


#endif
