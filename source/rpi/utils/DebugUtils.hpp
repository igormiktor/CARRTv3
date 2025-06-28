/*
    DebuggingUtils.hpp   
    
    Debugging code using policy types that drive template specialization and function overloading 
    that generate debugging code when requested and no code when not requested.  C++20 is required.
    
    Debug code generation is selected by #define DEBUGUTILS_ON to be non-zero at compile time.  

    There is no preprocessor selected code generation.  The selection of what code is generated 
    (or not generated) is entirely governed by template specialization (helped along by type inference 
    by the compiler) and function overloading.  Certain template specializations and function overloads 
    generate debugging code; other template specializations and function overloads generate no code.

    When compiled on GCC v13 with -O2 and DEBUGUTILS_ON=0, code size is exactly the same as when all 
    DebugUtils debugging calls are manual editted out from the source code.

    Varidic template functions allow as many variables to be output as desired.  Template recursion
    enables the debug printing of complex nested data types and data structures (e.g., vectors of pairs, 
    lists of vectors, maps of strings, queues of tuples).

    A small number of preprocessor macros are provided to provide a way to conveniently capture __FILE__ and 
    __LINE__ at the point of calling the debugging functions.  If someone knows a way to do this without using 
    a macro function, let me know!  Note that none of these macros are conditional.  Another macro simply hides
    a simple but rote object instantiation and is absolutely not necessary.

    This code was an experiment to see how far modern C++20 tools can replace the preprocessor to conditionally 
    generate code.  Turns out I was able to entirely replace it.

    This code is adapted from code by Anshul Johri.  They did not provide any license or copyright info.
    You can access their version at https://github.com/Anshul-Johri-1/Debug-Template

    This version of the code is open source shared under an MIT license.

    Copyright (c) 2025 Anshul Johri and Igor Mikolic-Torreira 

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/






#ifndef DebugUtils_hpp
#define DebugUitls_hpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <type_traits>
#include <string>
#include <iomanip>
#include <ctime>
#include <bits/stdc++.h>



#ifndef DEBUGUTILS_ON
    #define DEBUGUTILS_ON 0
#endif





namespace DebugUtils
{

    // Utility structure used to type switch on DEBUGUTILS_ON 
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



    // DebugUtilsPolicy is a type used to manage template instantiation and specialization,
    // as well as overload resolution.
    
    // When DebugUtilsPolicy == std::true_type, code for debugging is compiled.
    // When DebugUtilsPolicy == std::false_type, it is not.

    using DebugUtilsPolicy = typename TypeSelect<DEBUGUTILS_ON>::type;



    // This generic type is an intentionally trivial class
    template <typename T>
    class DebugFileOnBase
    {
        public:
            constexpr DebugFileOnBase( T, const char* ) {}
    };

    // Specialization applies when debug mode is on (DebugUtilsPolicy == std::true_type)
    // It is the only template instantiation that does anything
    template <>
    class DebugFileOnBase<std::true_type>
    {
        public:
            DebugFileOnBase( std::true_type, const char* filename ) 
                : mDebugLogFile{}, mOriginalCerrBuff{ nullptr }
            {
                auto t = std::time( nullptr );
                auto tm = *std::localtime( &t );
                std::ostringstream oss;
                oss << std::put_time( &tm, "%Y%m%d_%H%M%S" );
                
                std::string fn{ filename };
                fn += '_' + oss.str() + ".log";
                mDebugLogFile.open( fn );
                if ( mDebugLogFile )
                {
                    mOriginalCerrBuff = std::cerr.rdbuf();          // Save cerr buffer to restore in destructor
                    std::cerr.rdbuf( mDebugLogFile.rdbuf() );
                    std::cerr << "Debug logging redirected to file " << fn << std::endl;
                }
                else
                {
                    // In case of error, std:cerr remains as it was
                    std::cerr << "Unable to open debug logging file " << fn << std::endl;
                }
            }

            ~DebugFileOnBase()
            {
                std::cerr << "Closing the debug logging file" << std::endl;
                if ( mOriginalCerrBuff )
                {
                    std::cerr.rdbuf( mOriginalCerrBuff );       // Restore cerr
                }
                // ofstream destructor closes the file
            }


        private:
            std::ofstream       mDebugLogFile;
            std::streambuf*     mOriginalCerrBuff;
    };


    class DebugFileOn : public DebugFileOnBase<DebugUtilsPolicy>
    {
        public:
            DebugFileOn( const char* filename ) : DebugFileOnBase( DebugUtilsPolicy{}, filename ) {}
    };



    // Following code uses template recursion on variadic function templates. 
    // The various functions print(...) provide the base case for template recursion.

    // The functions below handle single arguments, which provide the "base cases" for 
    // template recursion. Base case in the context means the single argument case
    
    // These handle specific types of single arguments base cases
    inline void print( const char* x )  { std::cerr << x; }

    inline void print( char x )  { std::cerr << "\'" << x << "\'"; }

    inline void print( bool x )  { std::cerr << (x ? "T" : "F"); }

    inline void print( std::string x )  { std::cerr << "\"" << x << "\""; }

    inline void print( std::vector<bool>& v )
    { 
        // This overload because stl optimizes vector<bool> by using
        // _Bit_reference instead of bool to conserve space.
        int f{ 0 };
        std::cerr << '{';
        for ( auto &&i : v )
        {
            std::cerr << (f++ ? "," : "") << ( i ? "T" : "F" );
        }
        std::cerr << "}";
    }

    // Helper concept/requirement processing the generic base case
    template <typename T>
    concept is_iterable = requires( T &&x ) { begin(x); } &&
                            !std::is_same_v<std::remove_cvref_t<T>, std::string>;

    // This template handles all other single argument cases
    template <typename T>
    void print( T&& x )
    {
        if constexpr ( is_iterable<T> )                                 // Various iterables...
        {
            if ( size(x) && is_iterable<decltype( *(begin(x)) )> )      // Iterable inside Iterable
            {
                int f{ 0 };
                std::cerr << "\n~~~~~\n";
                int w = std::max( 0, (int) std::log10( size(x) - 1 ) ) + 2;
                for ( auto&& i : x )
                {
                    std::cerr << std::setw(w) << std::left << f++, print(i), std::cerr << "\n";
                }
                std::cerr << "~~~~~\n";
            }
            else                                                        // A plain, normal Iterable
            {
                int f{ 0 };
                std::cerr << "{";
                for ( auto&& i : x )
                {
                    std::cerr << ( f++ ? "," : "" ), print( i );
                }
                std::cerr << "}";
            }
        }
        else if constexpr ( requires { x.pop(); } )                     // Stacks, Priority Queues, Queues 
        {
            auto temp{ x };
            int f{ 0 };
            std::cerr << "{";
            if constexpr ( requires { x.top(); } )
            {            
                while ( !temp.empty() )
                    std::cerr << ( f++ ? "," : "" ), print( temp.top() ), temp.pop();
            }
            else
            {
                while ( !temp.empty() )
                    std::cerr << ( f++ ? "," : "" ), print( temp.front() ), temp.pop();
            }
            std::cerr << "}";
        }
        else if constexpr ( requires { x.first; x.second; } )           // Pair 
        {
            std::cerr << '(', print( x.first ), std::cerr << ',', print( x.second ), std::cerr << ')';
        }
        else if constexpr ( requires { get<0>(x); } )                   // Tuple 
        {
            int f{ 0 };
            std::cerr << '(', apply( [&f](auto... args) { (( std::cerr << (f++ ? "," : ""), print(args) ), ...); }, x );
            std::cerr << ')';
        }
        else
        {
            std::cerr << x;                                             // Anything else
        }
    }


    // This is the variadic function that drives the template recursion
    // Single arguments are passed to one of the print() functions
    // The "tail" argument(s) is/are passed back to the printer() 
    // function (but with one less argument to trigger the template recursion)
    template <typename T, typename... V>
    void printerV( const char* names, T&& head, V&&... tail )
    {
        int i{ 0 };
        for ( int bracket = 0; names[i] != '\0' and ( names[i] != ',' or bracket > 0 ); i++ )
        {
            if ( names[i] == '(' or names[i] == '<' or names[i] == '{' )
                bracket++;
            else if ( names[i] == ')' or names[i] == '>' or names[i] == '}' )
                bracket--;
        }
        std::cerr.write( names, i ) << " = ";
        print( std::forward<T>( head ) );
        if constexpr ( sizeof...(tail) )
        {
            std::cerr << " ||", printerV( names + i + 1, std::forward<V>( tail )... );
        }
        else
        {    
            std::cerr << " ]\n";
        }
    }


    // This a version of the variadic function for plain arrays 
    // Pass using macros as debugArr( array1Ptr, N1, array2Ptr, N2, array3Ptr, N3 )
    // It works the same way as printer()
    template <typename T, typename... V>
    void printerArr( const char* names, T arr[], size_t n, V... tail )
    {
        size_t i = 0;
        for ( ; names[i] and names[i] != ','; i++ )
            std::cerr << names[i];
        for ( i++; names[i] and names[i] != ','; i++ )
            ;
        std::cerr << " = {";
        for ( size_t ind = 0; ind < n; ind++ )
            std::cerr << ( ind ? "," : "" ), print( arr[ind] );
        std::cerr << "}";
        if constexpr ( sizeof...( tail ) )
            std::cerr << " ||", printerArr( names + i + 1, tail... );
        else
            std::cerr << " ]\n";
    }



    // Following sets up the functions actually called by user code.  They are overloaded on the first parameter.
    // When the first parameter is of type convertible to std::true_type, actual debug code is generated.
    // When the first parameter is convertible to std::false_type, empty functions (compiler eliminated) are generated.


    //*** debugPrinterV() variants

    // Debugging version overload
    template <typename T, typename... V>
    void debugPrinterV( std::true_type, const char* filename, int lineNbr, const char* names, T&& head, V&&... tail )
    {
        std::cerr << std::filesystem::path{ filename }.filename().string() << "(" << lineNbr << ") [ ", 
            printerV( names, std::forward<T>( head ), std::forward<V>( tail )... );
    }

    // Non-debugging version overload, an empty function
    template <typename T, typename... V>
    constexpr void debugPrinterV( std::false_type, const char* filename, int lineNbr, const char* names, T&& head, V&&... tail ) {}
 
    // Function overload actually called in user code that triggers selection of debug/non-debug versions
    template <typename T, typename... V>
    void debugPrinterV( const char* filename, int lineNbr, const char* names, T&& head, V&&... tail )
    {
        debugPrinterV( DebugUtilsPolicy{}, filename, lineNbr, names, std::forward<T>( head ), std::forward<V>( tail )... );
    }
 
    // Function overload actually called in user code that triggers selection of debug/non-debug versions, conditional version
    template <typename T, typename... V>
    void debugPrinterV( bool active, const char* filename, int lineNbr, const char* names, T&& head, V&&... tail )
    {
        if constexpr ( std::is_convertible<DebugUtilsPolicy, std::true_type>::value )
        {
            if ( active )
            {
                debugPrinterV( DebugUtilsPolicy{}, filename, lineNbr, names, std::forward<T>( head ), std::forward<V>( tail )... );
            }
        }
    }


    //*** debugPrinterArr() variants

    // Debugging version overload
    template <typename T, typename... V>
    void debugPrinterArr( std::true_type, const char* filename, int lineNbr, const char* names, T arr[], size_t n, V... tail )
    {
        std::cerr << std::filesystem::path{ filename }.filename().string() << "(" << lineNbr << ") [ ", printerArr( names, arr, n, tail... );
    }

    // Non-debugging version overload
    template <typename T, typename... V>
    constexpr void debugPrinterArr( std::false_type, const char* filename, int lineNbr, const char* names, T arr[], size_t n, V... tail ) {}

    // Function overload actually called in user code that triggers selection of debug/non-debug versions
    template <typename T, typename... V>
    void debugPrinterArr( const char* filename, int lineNbr, const char* names, T arr[], size_t n, V... tail )
    { 
        debugPrinterArr( DebugUtilsPolicy{}, filename, lineNbr, names, arr, n, tail... );
    }

    // Function overload actually called in user code that triggers selection of debug/non-debug versions, conditional version
    template <typename T, typename... V>
    void debugPrinterArr( bool active, const char* filename, int lineNbr, const char* names, T arr[], size_t n, V... tail )
    { 
        if constexpr ( std::is_convertible<DebugUtilsPolicy, std::true_type>::value )
        {
            if ( active )
            {
                debugPrinterArr( DebugUtilsPolicy{}, filename, lineNbr, names, arr, n, tail... );
            }
        }
    }


    //*** debugMsg() variants ***

    // This is a function to simply print a simple message to debug (no variables dumped)

    // Debugging version overload
    template<typename T>
    void debugMsg( std::true_type, const char* filename, int lineNbr, T&& output )
    {
        std::cerr << std::filesystem::path{ filename }.filename().string() << "(" << lineNbr << "): " << output << std::endl;
    }

    // Non-debuging version overload
    template<typename T>
    constexpr void debugMsg( std::false_type, const char*,  int, T&& ) {}

    // Function overload actually called in user code that triggers selection of debug/non-debug versions
    template <typename T>
    void debugMsg( const char* filename, int lineNbr, T&& output )
    {
        debugMsg( DebugUtilsPolicy{}, filename, lineNbr, std::forward<T>( output ) );
    }

    // Function overload actually called in user code that triggers selection of debug/non-debug versions, conditional version
    template <typename T>
    void debugMsg( bool active, const char* filename, int lineNbr, T&& output )
    {
        if constexpr ( std::is_convertible<DebugUtilsPolicy, std::true_type>::value )
        {
            if ( active )
            {
                debugMsg( DebugUtilsPolicy{}, filename, lineNbr, std::forward<T>( output ) );
            }
        }
    }


}   // namespace DebugUtils


// Convenience macros to provide __FILE__, __LINE__ and the catenation of variable names
#define debugV(...)         DebugUtils::debugPrinterV( __FILE__,  __LINE__, #__VA_ARGS__, __VA_ARGS__ )
#define debugArr(...)       DebugUtils::debugPrinterArr( __FILE__,  __LINE__, #__VA_ARGS__, __VA_ARGS__ )
#define debugM( msg )       DebugUtils::debugMsg( __FILE__,  __LINE__, msg )

// Convenience macros for conditional debugging
#define debugCondV( active, ...)    DebugUtils::debugPrinterV( active, __FILE__,  __LINE__, #__VA_ARGS__, __VA_ARGS__ )
#define debugCondArr( active, ...)  DebugUtils::debugPrinterArr( active, __FILE__,  __LINE__, #__VA_ARGS__, __VA_ARGS__ )
#define debugCondM( active, msg )   DebugUtils::debugMsg( active, __FILE__,  __LINE__, msg )

// Convenience macro to instantiate a file to log all the debug output
#define logDebugToFile( filename )      DebugUtils::DebugFileOn debugEnabled( filename )

#endif  // DebugUtils_h
