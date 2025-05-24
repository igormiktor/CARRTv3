cmake_minimum_required( VERSION 3.25.0 )


project( carrt_rpi VERSION 0.1.0 LANGUAGES ASM C CXX )

set( CMAKE_C_STANDARD 11 )
set( CMAKE_CXX_STANDARD 23 )

include( CTest )
enable_testing()


set( CARRT_RPI_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" )
message( CARRT_RPI_DIRECTORY="${CARRT_RPI_DIRECTORY}")

set( CARRT_RPI_USE_PIGPIOD 0 )

if ( CARRT_RPI_USE_PIGPIOD )
    message ( "Using PIGPIOD daemon" )
    set( PIGPIO_LIB "-lpigpiod_if2" )
else()
    message( "Using plain PIGPIO" )
    set( PIGPIO_LIB "-lpigpio" )
endif()


add_subdirectory( carrt )
add_subdirectory( Test )

