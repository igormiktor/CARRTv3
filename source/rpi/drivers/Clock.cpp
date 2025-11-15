/*
    Clock.cpp - Functions to initialize and use a clock
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



#include "Clock.h"


#include <time.h>



namespace Clock
{

    const long kMilliSecsPerSec = 1000L;
    const long kNanoSecsPerMilliSec = 1000000L;

    const long kMicroSecsPerSec = 1000000L;
    const long kNanoSecsPerMicroSec = 1000L;


    struct timespec sProgramStartTime;


    long elaspedMicroSeconds( struct timespec t1, struct timespec t0 );
    long elapsedMilliSeconds( struct timespec t1, struct timespec t0 );

}




long Clock::elaspedMicroSeconds( struct timespec t1, struct timespec t0 )
{
    long secDiff = t1.tv_sec - t0.tv_sec;
    long nanoSecDiff = t1.tv_nsec - t0.tv_sec;

    return secDiff * kMicroSecsPerSec  +  nanoSecDiff / kNanoSecsPerMicroSec;
}



long Clock::elapsedMilliSeconds( struct timespec t1, struct timespec t0 )
{
    long secDiff = t1.tv_sec - t0.tv_sec;
    long nanoSecDiff = t1.tv_nsec - t0.tv_sec;

    return secDiff * kMilliSecsPerSec  +  nanoSecDiff / kNanoSecsPerMilliSec;
}



void Clock::initSystemClock()
{
    clock_gettime( CLOCK_MONOTONIC, &sProgramStartTime );
}



void Clock::delay( const std::chrono::nanoseconds& howLong )
{
    std::chrono::seconds delaySeconds{ std::chrono::duration_cast<std::chrono::seconds>( howLong ) };
    std::chrono::nanoseconds delayNanoseconds{ howLong - std::chrono::nanoseconds( delaySeconds ) };
    struct timespec req; 
    struct timespec rem;

    req.tv_sec = delaySeconds.count();
    req.tv_nsec = delayNanoseconds.count();

    nanosleep( &req, &rem );
}


#if 0  // These are deprecated functions

void Clock::delayMicroseconds( long us )
{
    struct timespec req;
    struct timespec rem;

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

    nanosleep( &req, &rem );
}



void Clock::delayMilliseconds( long ms )
{
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

#endif  // Deprecated code


long Clock::micros()
{
    struct timespec now;

    clock_gettime( CLOCK_MONOTONIC, &now );
    return elaspedMicroSeconds( now, sProgramStartTime );
}



long Clock::millis()
{
    struct timespec now;

    clock_gettime( CLOCK_MONOTONIC, &now );
    return elapsedMilliSeconds( now, sProgramStartTime );
}
