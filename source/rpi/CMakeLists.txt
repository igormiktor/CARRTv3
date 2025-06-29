# Build the executable for the CARRTv3 RPi Zero master computer
# Also builds (optionally) various test executables to run on the CARRT RPi Zero

# Version is limited by RPi cmake
cmake_minimum_required( VERSION 3.25 )


project( 
    carrt_rpi 
    VERSION 0.1.0 
    DESCRIPTION "Executable for the CARRTv3 RPi Zero"
    LANGUAGES ASM C CXX )

set( CMAKE_C_STANDARD 11 CACHE STRING "The C standard to use" )
set( CMAKE_C_STANDARD_REQUIRED ON )
set( CMAKE_CXX_STANDARD 23 CACHE STRING "The C++ standard to use" )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

option(
    
    "Enable building test executables. Default: 1. Values: { 0, 1 }."
    1
)

option( 
    CARRT_ENABLE_DEBUGUTILS
    "Enable DebugUtils debugging outputs.  Default: 1. Values: { 0, 1 }."
    1
)

option( 
    CARRT_RPI_USE_PIGPIOD
    "Enable use of PIGPIO daemon.  Default: 0. Values: { 0, 1 }."
    0
)



if ( CARRT_RPI_USE_PIGPIOD )
    message ( "Using PIGPIOD daemon" )
    set( PIGPIO_LIB "-lpigpiod_if2" )
else()
    message( "Using plain PIGPIO" )
    set( PIGPIO_LIB "-lpigpio" )
endif()



add_subdirectory( shared )
add_subdirectory( utils )
add_subdirectory( drivers )
add_subdirectory( carrt )

if ( CARRT_BUILD_TESTS )
    add_subdirectory( test )
    include( CTest )
    enable_testing()
endif()



if ( CARRT_BUILD_TESTS )
    message( "Building of tests is ON" )
else()
    message( "Building of tests is OFF" )
endif()

if ( CARRT_ENABLE_DEBUGUTILS )
    message( "Use of DebugUtils output is ON" )
else()
    message( "Use of DebugUtils output is OFF" )
endif()

if ( CARRT_RPI_USE_PIGPIOD )
    message( "Use of PIGPIO daemon is ON" )
else()
    message( "Use of PIGPIO daemon is OFF (Using plain PIGPIO)" )
endif()
