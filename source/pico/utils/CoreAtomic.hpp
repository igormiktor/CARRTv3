/*
    CoreAtomic.hpp - A template for an Atomic class that is atomic across cores
    and uses only one critical_section spinlock shared across all instances.

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



#ifndef CoreAtomic_hpp
#define CoreAtomic_hpp

#include <cstdint>
#include <concepts>
#include <utility>

#include "pico/sync.h"

#include "CriticalSection.h"



namespace CoreAtomic 
{

    namespace Internal 
    {
        // A critical section shared by all atomic variable instances
        inline critical_section_t mCritSec{}; 

    }   // Internal


    // Call this function from main() to init the shared critical section
    inline void initCAtomic()
    {
        critical_section_init( &Internal::mCritSec );  
    }




    // Currently only defined for std::integral types
    template<std::integral T>
    struct CAtomic 
    {
    public:

        // value_type
        using value_type = T;


        // Constructors
        explicit constexpr CAtomic( T initialValue = 0 ) noexcept 
        : mValue( initialValue ) { }

        CAtomic( const CAtomic<T>& )    = delete;  
        CAtomic( CAtomic<T>&& )         = delete;       


        // Assignment
        CAtomic<T>& operator=( const CAtomic<T>& )  = delete;  
        CAtomic<T>& operator=( CAtomic<T>&& )       = delete;       

        CAtomic<T>& operator=( T value ) noexcept
        {
            store( value ); 
            return *this; 
        }

        volatile CAtomic<T>& operator=( T value ) volatile noexcept
        {
            store( value ); 
            return *this; 
        }


        // is_lock_free()
        bool is_lock_free() const noexcept
        {
            return false;
        }

        bool is_lock_free() const volatile noexcept
        {
            return false;
        }


        // store()
        void store( T value ) noexcept
        {
            CriticalSection block( mCritSec );
            mValue = value; 
        }

        void store( T value ) volatile noexcept
        {
            CriticalSection block( mCritSec );
            mValue = value; 
        }


        // load()
        T load() const noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                ret = mValue; 
            }
           return ret; 
        }

        T load() const volatile noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                ret = mValue; 
            }
            return ret; 
        }


        // operator T()
        operator T() const noexcept
        {
            return load();
        }

        operator T() const volatile noexcept
        {
            return load();
        }


        // exchange()
        T exchange( T in ) noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                ret = mValue; 
                mValue = in;
            }
            return ret;
        }

        T exchange( T in ) volatile noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                ret = mValue; 
                mValue = in;
            }
            return ret;
         }


        // compare_exchange_[weak/strong]()
        bool compare_exchange( T& expected, T newValue ) noexcept
        {
            bool ret{ false };
            {
                CriticalSection block( mCritSec );
                if ( mValue == expected )
                {
                    mValue = newValue;
                    ret = true;
                }
                else
                {
                    expected = mValue;
                }
            }
            return ret;
        }

        bool compare_exchange( T& expected, T newValue ) volatile noexcept
        {
            bool ret{ false };
            {
                CriticalSection block( mCritSec );
                if ( mValue == expected )
                {
                    mValue = newValue;
                    ret = true;
                }
                else
                {
                    expected = mValue;
                }
            }
            return ret;
        }

        bool compare_exchange_weak( T& expected, T newValue ) noexcept
        {
            return compare_exchange( expected, newValue );
        }

        bool compare_exchange_weak( T& expected, T newValue ) volatile noexcept
        {
            return compare_exchange( expected, newValue );
        }

        bool compare_exchange_strong( T& expected, T newValue ) noexcept
        {
            return compare_exchange( expected, newValue );
        }

        bool compare_exchange_strong( T& expected, T newValue ) volatile noexcept
        {
            return compare_exchange( expected, newValue );
        }


        // fetch_add()
        T fetch_add( T arg ) noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                ret = mValue; 
                mValue += arg;
            }
            return ret;
        }

        T fetch_add( T arg ) volatile noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                ret = mValue; 
                mValue += arg;
            }
            return ret;
        }


        // fetch_sub()
        T fetch_sub( T arg ) noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                ret = mValue; 
                mValue -= arg;
            }
            return ret;
        }

        T fetch_sub( T arg ) volatile noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                ret = mValue; 
                mValue -= arg;
            }
            return ret;
        }


        // operator+=()
        T operator+=( T arg ) noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                mValue += arg;
                ret = mValue;
            }
            return ret;
        }

        T operator+=( T arg ) volatile noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                mValue += arg;
                ret = mValue;
            }
            return ret;
        }


        // operator-=()
        T operator-=( T arg ) noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                mValue -= arg;
                ret = mValue;
            }
            return ret;
        }

        T operator-=( T arg ) volatile noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                mValue -= arg;
                ret = mValue;
            }
            return ret;
        }


        // operator++()
        T operator++() noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                mValue += 1;                            // ++mValue/mValue++ trigger compiler warnings on volatile mValue
                ret = mValue;
            }
            return ret;
        }

        T operator++() volatile noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                mValue += 1;                            // ++mValue/mValue++ trigger compiler warnings on volatile mValue
                ret = mValue;
            }
            return ret;
        }


        // operator++( int )
        T operator++( int ) noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                ret = mValue;
                mValue += 1;                            // ++mValue/mValue++ trigger compiler warnings on volatile mValue
            }
            return ret;
        }

        T operator++( int ) volatile noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                ret = mValue;
                mValue += 1;                            // ++mValue/mValue++ trigger compiler warnings on volatile mValue
            }
            return ret;
        }


        // operator--()
        T operator--() noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                mValue -= 1;                            // --mValue/mValue-- trigger compiler warnings on volatile mValue
                ret = mValue;
            }
            return ret;
        }

        T operator--() volatile noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                mValue -= 1;                            // --mValue/mValue-- trigger compiler warnings on volatile mValue
                ret = mValue;
            }
            return ret;
        }


        // operator--( int )
        T operator--( int ) noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                ret = mValue;
                mValue -= 1;                            // --mValue/mValue-- trigger compiler warnings on volatile mValue
            }
            return ret;
        }

        T operator--( int ) volatile noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                ret = mValue;
                mValue -= 1;                            // --mValue/mValue-- trigger compiler warnings on volatile mValue
            }
            return ret;
        }


        // fetch_and()
        T fetch_and( T arg ) noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                ret = mValue;
                mValue &= arg;  
            }
            return ret;
        }

        T fetch_and( T arg ) volatile noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                ret = mValue;
                mValue &= arg;  
            }
            return ret;
       }


        // fetch_or()
        T fetch_or( T arg ) noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                ret = mValue;
                mValue |= arg;  
            }
            return ret;
        }

        T fetch_or( T arg ) volatile noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                ret = mValue;
                mValue |= arg;  
            }
            return ret;
        }


        // fetch_xor()
        T fetch_xor( T arg ) noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                ret = mValue;
                mValue ^= arg;  
            }
            return ret;
        }

        T fetch_xor( T arg ) volatile noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                ret = mValue;
                mValue ^= arg;  
            }
            return ret;
        }


        // operator&=()
        T operator&=( T arg ) noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                mValue &= arg;
                ret = mValue; 
            }
            return ret;
        }   

        T operator&=( T arg ) volatile noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                mValue &= arg;
                ret = mValue; 
            }
            return ret;
        }   


        // operator|=()
        T operator|=( T arg ) noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                mValue |= arg;
                ret = mValue; 
            }
            return ret;
        }   

        T operator|=( T arg ) volatile noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                mValue |= arg;
                ret = mValue; 
            }
            return ret;
        }   


        // operator^=()
        T operator^=( T arg ) noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                mValue ^= arg;
                ret = mValue; 
            }
            return ret;
        }   

        T operator^=( T arg ) volatile noexcept
        {
            T ret{};
            {
                CriticalSection block( mCritSec );
                mValue ^= arg;
                ret = mValue; 
            }
            return ret;
        }   




        // is_always_lock_free
        static constexpr bool is_always_lock_free = false;


    private:
    
        inline static critical_section_t& mCritSec{ Internal::mCritSec };  
        volatile T mValue; 

    };  // struct CAtomic 




    namespace Types 
    {
        // Standard type definitions from std::atomic
        using CAtomic_bool              = CAtomic<bool>;
        using CAtomic_char              = CAtomic<char>;
        using CAtomic_schar             = CAtomic<signed char>;
        using CAtomic_uchar             = CAtomic<unsigned char>;
        using CAtomic_short             = CAtomic<short>;
        using CAtomic_ushort            = CAtomic<unsigned short>;
        using CAtomic_int               = CAtomic<int>;
        using CAtomic_uint              = CAtomic<unsigned int>;
        using CAtomic_long              = CAtomic<long>;
        using CAtomic_ulong             = CAtomic<unsigned long>;
        using CAtomic_llong             = CAtomic<long long>;
        using CAtomic_ullong            = CAtomic<unsigned long long>;
        using CAtomic_char8_t           = CAtomic<char8_t>;
        using CAtomic_char16_t          = CAtomic<char16_t>;
        using CAtomic_char32_t          = CAtomic<char32_t>;
        using CAtomic_wchar_t           = CAtomic<wchar_t>;
        using CAtomic_int8_t            = CAtomic<std::int8_t>;
        using CAtomic_uint8_t           = CAtomic<std::uint8_t>;
        using CAtomic_int16_t           = CAtomic<std::int16_t>;
        using CAtomic_uint16_t          = CAtomic<std::uint16_t>;
        using CAtomic_int32_t           = CAtomic<std::int32_t>;
        using CAtomic_uint32_t          = CAtomic<std::uint32_t>;
        using CAtomic_int64_t           = CAtomic<std::int64_t>;
        using CAtomic_uint64_t          = CAtomic<std::uint64_t>;
        using CAtomic_int_least8_t      = CAtomic<std::int_least8_t>;
        using CAtomic_uint_least8_t     = CAtomic<std::uint_least8_t>;
        using CAtomic_int_least16_t     = CAtomic<std::int_least16_t>;
        using CAtomic_uint_least16_t    = CAtomic<std::uint_least16_t>;
        using CAtomic_int_least32_t     = CAtomic<std::int_least32_t>;
        using CAtomic_uint_least32_t    = CAtomic<std::uint_least32_t>;
        using CAtomic_int_least64_t     = CAtomic<std::int_least64_t>;
        using CAtomic_uint_least64_t    = CAtomic<std::uint_least64_t>;
        using CAtomic_int_fast8_t       = CAtomic<std::int_fast8_t>;
        using CAtomic_uint_fast8_t      = CAtomic<std::uint_fast8_t>;
    }   // Types            


} // CoreAtomic

#endif  // CAtomic_hpp
