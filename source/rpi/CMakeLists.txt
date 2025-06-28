cmake_minimum_required( VERSION 3.28 )


project( 
    carrt_rpi 
    VERSION 0.1.0 
    DESCRIPTION "Executable for the CARRTv3 RPi Zero"
    LANGUAGES ASM C CXX )

set( CMAKE_C_STANDARD 11 CACHE STRING "The C standard to use" )
set( CMAKE_C_STANDARD_REQUIRED ON )
set( CMAKE_CXX_STANDARD 23 CACHE STRING "The C++ standard to use" )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

# Let's nicely support folders in IDEs
set_property( GLOBAL PROPERTY USE_FOLDERS ON )


include( CTest )
enable_testing()


set( CARRT_RPI_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" )
message( CARRT_RPI_DIRECTORY="${CARRT_RPI_DIRECTORY}" )

set( CARRT_RPI_USE_PIGPIOD 0 )

if ( CARRT_RPI_USE_PIGPIOD )
    message ( "Using PIGPIOD daemon" )
    set( PIGPIO_LIB "-lpigpiod_if2" )
else()
    message( "Using plain PIGPIO" )
    set( PIGPIO_LIB "-lpigpio" )
endif()

add_subdirectory( drivers )
add_subdirectory( shared )
add_subdirectory( carrt )
add_subdirectory( test )

