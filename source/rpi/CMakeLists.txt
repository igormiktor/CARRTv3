# Build the executable for the CARRTv3 RPi Zero master computer
# Also builds (optionally) various test executables to run on the CARRT RPi Zero

# Version is limited by RPi cmake
cmake_minimum_required( VERSION 3.25 )

# If we have RPi Zero toolchain, use it; otherwise assume native
if( EXISTS "$ENV{HOME}/.cmake/rpi0w-toolchain.cmake" )
    set( CMAKE_TOOLCHAIN_FILE $ENV{HOME}/.cmake/rpi0w-toolchain.cmake )
    set( CARRT_COMPILER "CROSS" )
else()
    set( CARRT_COMPILER "NATIVE" )
endif()

project( 
    carrt_rpi 
    VERSION 0.1.0 
    DESCRIPTION "Executable for the CARRTv3 RPi Zero"
    LANGUAGES ASM C CXX 
)

set( CMAKE_C_STANDARD 11 CACHE STRING "The C standard to use" )
set( CMAKE_C_STANDARD_REQUIRED ON )
set( CMAKE_CXX_STANDARD 23 CACHE STRING "The C++ standard to use" )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

SET( BUILDING_FOR_RPI0 1 )


option(
    CARRT_BUILD_TESTS
    "Enable building test executables. Default: ON. Values: { OFF, ON }."
    ON
)

option( 
    CARRT_RPI0_ENABLE_STDIO_OUTPUT
    "Enable lots of stdio output.  Default: ON. Values: { OFF, ON }."
    ON
)

option( 
    CARRT_ENABLE_DEBUGUTILS
    "Enable DebugUtils debugging outputs.  Default: ON. Values: { OFF, ON }."
    ON
)

option( 
    CARRT_RPI_USE_PIGPIOD
    "Enable use of PIGPIO daemon.  Default: OFF. Values: { OFF, ON }."
    OFF
)


add_subdirectory( shared )
#add_subdirectory( utils )
add_subdirectory( drivers )
add_subdirectory( carrt )

if( CARRT_BUILD_TESTS )
    add_subdirectory( test )
    include( CTest )
    enable_testing()
endif()

message( "" )
message( "Configuration summary on host ${CMAKE_HOST_SYSTEM_NAME}:" )
message( " - Build backend is ${CMAKE_GENERATOR}." )
message( " - Compiler is ${CARRT_COMPILER}.")
message( " - Preset is ${CARRT_PRESET_NAME}.")
message( " - Build type is ${CMAKE_BUILD_TYPE}." )
message( " - Building of tests is ${CARRT_BUILD_TESTS}." )
message( " - Use of DebugUtils output is ${CARRT_ENABLE_DEBUGUTILS}." )
message( " - Use of PIGPIO daemon is ${CARRT_RPI_USE_PIGPIOD}." )
message( "" )

