/*
    OutputUtils.hpp - Output code using policy types that drive template specialization and 
    function overloading that generate output code when requested and no code when not requested.  
    
    C++20 is required.
    
    Output code generation is selected by #define USE_CARRTPICO_STDIO to be non-zero at compile time.  

    There is no preprocessor selected code generation.  The selection of what code is generated 
    (or not generated) is entirely governed by template specialization (helped along by type inference 
    by the compiler) and function overloading.  Certain template specializations and function overloads 
    generate debugging code; other template specializations and function overloads generate no code.

    When compiled on GCC v13 with -O2 and DEBUGUTILS_ON=0, code size is exactly the same as when all 
    OutputUtils debugging calls are manual editted out from the source code.

    Varidic template functions allow as many variables to be output as desired.  Template recursion
    enables the debug printing of complex nested data types and data structures (e.g., vectors of pairs, 
    lists of vectors, maps of strings, queues of tuples).

    This code was an experiment to see how far modern C++20 tools can replace the preprocessor to conditionally 
    generate code.  Turns out I was able to entirely replace it.

    Copyright (c) 2025 Igor Mikolic-Torreira.  All right reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more deotherss.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/






#ifndef OutputUtils_hpp
#define OutputUtils_hpp

#include <iostream>
#include <type_traits>
#include <string>
#include <iomanip>




#if BUILDING_FOR_PICO

    #include "CarrtPicoDefines.h"

    #ifndef USE_CARRTPICO_STDIO
        #define USE_CARRTPICO_STDIO     0
    #endif

    #ifndef DEBUGPICO
        #define DEBUGPICO   0
    #endif

    #define USE_CARRTPICO_DEBUG     ( DEBUGPICO && USE_CARRTPICO_STDIO )

#else

    #define USE_CARRTPICO_STDIO     0
    #define USE_CARRTPICO_DEBUG     0

#endif // BUILDING_FOR_PICO


#if BUILDING_FOR_RPI0

    #include "CarrtRpi0Defines.h"

    #ifndef USE_CARRTRPI0_STDIO
        #define USE_CARRTRPI0_STDIO     1
    #endif


    #ifndef DEBUGRPI0
        #define DEBUGRPI0   0
    #endif

    #define USE_CARRTRPI0_DEBUG     ( DEBUGRPI0 && USE_CARRTRPI0_STDIO )

#else

    #define USE_CARRTRPI0_STDIO     0
    #define USE_CARRTRPI0_DEBUG     0

#endif // BUILDING_FOR_RPI0


#define USE_STDIO_OUPUT     ( USE_CARRTPICO_STDIO || USE_CARRTRPI0_STDIO )  
#define USE_STDIO_DEBUG     ( USE_CARRTPICO_DEBUG || USE_CARRTRPI0_DEBUG )



namespace OutputUtils
{

    // Utility structure used to type switch on USE_CARRTPICO_STDIO
    template<bool B>
    struct TypeSelect {};

    template<>
    struct TypeSelect<true>
    {
        using type = typename std::true_type;
    };

    template<>
    struct TypeSelect<false>
    {
        using type = typename std::false_type;
    };


    // OutputUtilsPolicy is a type used to manage template instantiation and specialization,
    // as well as overload resolution the generate output only when Pico STDIO is enabled.

    // When OutputUtilsPolicy == std::true_type, code for output to stdio (std::cout) is compiled.
    // When OutputUtilsPolicy == std::false_type, it is not.

    using OutputUtilsPolicy = typename TypeSelect<USE_STDIO_OUPUT>::type;


    // OutputDebugPolicy is a type used to manage template instantiation and specialization,
    // as well as overload resolution for output only when Pico STDIO *and* DEBUG_UTILS are both enabled.

    // When OutputDebugPolicy == std::true_type, code for debug output to stdio (std::cout) is compiled.
    // When OutputDebugPolicy == std::false_type, it is not.

    using OutputDebugPolicy = typename TypeSelect<USE_STDIO_DEBUG>::type;




    // This is the working overload
    template<typename T, typename ...V>
    void output2cout_( std::true_type, T&& first, V&&... others )
    {
        std::cout << std::forward<T>( first );
        [[maybe_unused]] auto outputWithSpace = []( const auto& arg )
        {
            std::cout << ' ' << arg;
        };
        ( ... , outputWithSpace( others ) );
        std::cout << std::endl;
    }

    // This is the null overload
    template<typename T, typename ...V>
    void output2cout_( std::false_type, T&& first, V&&... others )
    {
        // Do nothing
    }


    // This is the public function actually called in user code for std::cout output
    // Converts the arguments to 
    //  std::cout << arg1 << arg2 << arg3 << ... << argN << std::endl;
    //
    // Use output2cout for code that should remain in the Pico executable (if Pico STDIO 
    // functionality is enabled) even in release/production builds.

    template<typename T, typename ...V>
    inline void output2cout( T&& first, V&&... others )
    {
        output2cout_( OutputUtilsPolicy{}, std::forward<T>( first ), std::forward<V>( others )... );
    }

    // This is the public function actually called in user code for std::cout output *only* when debugging enabled
    // If USE_CARRTPICO_STDIO and DEBUGPICO are both "ON", converts the arguments to 
    //  std::cout << arg1 << arg2 << arg3 << ... << argN << std::endl;
    //
    // Use debug2cout for code that should remain in the Pico executable (if Pico STDIO 
    // functionality is enabled) only in debugging builds. 

    template<typename T, typename ...V>
    inline void debug2cout( T&& first, V&&... others )
    {
        output2cout_( OutputDebugPolicy{}, std::forward<T>( first ), std::forward<V>( others )... );
    }

    // This is the public function actually called in user code for std::cout output *only* when debugging enabled
    // and the lead bool argument is true.
    // If USE_CARRTPICO_STDIO and DEBUGPICO are both "ON", converts the arguments to 
    //  std::cout << arg1 << arg2 << arg3 << ... << argN << std::endl;
    //
    // Use debugCond2cout for code that should remain in the Pico executable (if Pico STDIO 
    // functionality is enabled) only in debugging builds and if the first bool argument is true. 

    template<bool onOff, typename T, typename ...V>
    inline void debugCond2cout( T&& first, V&&... others )
    {
        if constexpr ( onOff )
        {
            output2cout_( OutputDebugPolicy{}, std::forward<T>( first ), std::forward<V>( others )... );
        }
    }

    // For more sophisticated debugging output (to std::cerr) use DebugUitls.hpp
}


// For convenience (likelihood of name clash is very low)
using OutputUtils::output2cout;
using OutputUtils::debug2cout;
using OutputUtils::debugCond2cout;


#endif  // OutputUtils_hpp

