/*
    CriticalSection.h - Package RP2040 critical_section in an RAII way

    Usage:

    // non-critical code...
    {
        CriticalSection c;
        // critical code...
    }
    // non-critical code...

    Copyright (c) 2024 Igor Mikolic-Torreira.  All right reserved.

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


#ifndef CriticalSection_h
#define CriticalSection_h

#include "pico/critical_section.h"

#include "CarrtError.h"



class CriticalSection
{
    public:

        explicit CriticalSection( critical_section_t* initializeCritSection )
        : mCriticalSection( initializeCritSection )
        {
            if ( !mCriticalSection )
            {
                throw CarrtError( makePicoErrorId( kPicoCritSectionError, 1, 1 ), "Null critial_section ptr passed to constructor" );
            }
            critical_section_enter_blocking( mCriticalSection );
        }

        // Local objects that exist and self-destruct; nothing else
        CriticalSection( const CriticalSection& )               = delete;  
        CriticalSection( CriticalSection&& )                    = delete;    
        CriticalSection& operator=( const CriticalSection& )    = delete;
        CriticalSection& operator=( CriticalSection&& )          = delete;


        ~CriticalSection()
        {
            critical_section_exit( mCriticalSection );
        }


    private:

        critical_section_t*   mCriticalSection;

};



#endif // CriticalSection_h
