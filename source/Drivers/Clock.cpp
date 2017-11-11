/*
    Clock.cpp - Functions to initialize and use a clock
    for CARRTv3 on Raspberry Pi Zero W.
    Copyright (c) 2017 Igor Mikolic-Torreira.  All right reserved.

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



#include "Clock.h"


#include <time.h>




void Clock::initSystemClock()
{
}



void Clock::delayMicroseconds( long us )
{
    const long kMicroSecsPerSec = 1000000L;
    const long kNanoSecsPerMicroSec = 1000L;

    struct timespec req, rem;

    if ( us >= kMicroSecsPerSec )
    {
        req.tv_sec = static_cast<int>( us / kMicroSecsPerSec );
        req.tv_nsec = (us - (req.tv_sec * kMicroSecsPerSec)) * kNanoSecsPerMicroSec;
    }
    else
    {
        req.tv_sec = 0;
        req.tv_nsec = us * kNanoSecsPerMicroSec;
    }
}



void Clock::delayMilliseconds( long ms )
{
    const long kMilliSecsPerSec = 1000L;
    const long kNanoSecsPerMilliSec = 1000000L;

    struct timespec req, rem;

    if ( ms >= kMilliSecsPerSec )
    {
        req.tv_sec = static_cast<int>( ms / kMilliSecsPerSec );
        req.tv_nsec = (ms - (req.tv_sec * kMilliSecsPerSec)) * kNanoSecsPerMilliSec;
    }
    else
    {
        req.tv_sec = 0;
        req.tv_nsec = ms * kNanoSecsPerMilliSec;
    }

    nanosleep( &req, &rem );
}



unsigned long Clock::micros()
{
}



unsigned long Clock::millis()
{
}




