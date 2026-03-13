# Generated Cmake Pico project file

cmake_minimum_required( VERSION 3.25 )

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set( PICO_SDK_PATH $ENV{PICO_SDK_PATH} )

set( PICO_BOARD pico CACHE STRING "Board type" )

# Pull in Raspberry Pi Pico SDK (must be before project)
include( pico_sdk_import.cmake )

if ( PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0" )
  message( FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}" )
endif()

project( carrt_pico 
    VERSION 0.1.0 
    DESCRIPTION "Executable for the CARRTv3 Pico"
    LANGUAGES ASM C CXX 
)


set( CMAKE_C_STANDARD 11 CACHE STRING "The C standard to use" )
set( CMAKE_C_STANDARD_REQUIRED ON )
set( CMAKE_CXX_STANDARD 23 CACHE STRING "The C++ standard to use" )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# This one is a must
set( PICO_CXX_ENABLE_EXCEPTIONS 1 )


option( 
    CARRTPICO_ENABLE_UART_STDIO
    "Enable stdio over UART.  Default: ON. Values: { OFF, ON }."
    ON
)

option(
    CARRTPICO_BUILD_TESTS
    "Enable building test executables. Default: ON. Values: { OFF, ON }."
    ON
)

option( 
    CARRTPICO_ENABLE_DEBUGUTILS
    "Enable DebugUtils debugging outputs.  Default: ON. Values: { OFF, ON }."
    ON
)


# Add a special define "DEBUGCARRTPICO" when building debugging builds
# add_compile_options( "$<$<CONFIG:DEBUG>:DEBUGCARRTPICO=1>" )

set( CARRTPICO_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" )
# message( CARRTPICO_DIRECTORY="${CARRTPICO_DIRECTORY}" )

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()


#######################################


set( BUILDING_FOR_PICO 1 )

if ( CARRTPICO_ENABLE_UART_STDIO )
    set( USE_CARRTPICO_STDIO_VAL 1 )
else()
    set( USE_CARRTPICO_STDIO_VAL 0 )
endif()

if ( CARRTPICO_ENABLE_DEBUGUTILS )
    set( DEBUGUTILS_VAL 1 )
else()
    set( DEBUGUTILS_VAL 0 )
endif()

# Versioning
set( CARRTPICO_VERSION_MAJOR 0 )
set( CARRTPICO_VERSION_MINOR 9 )
set( CARRTPICO_VERSION_REVISION 0 )

add_subdirectory( shared )
add_subdirectory( utils )
add_subdirectory( drivers )
add_subdirectory( carrt )

if ( CARRTPICO_BUILD_TESTS )
    add_subdirectory( test )
#    include( CTest )
#    enable_testing()
endif()

message( "" )
message( "Configuration summary on host ${CMAKE_HOST_SYSTEM_NAME}:" )
message( " - Build backend is ${CMAKE_GENERATOR}." )
message( " - Build type is ${CMAKE_BUILD_TYPE}." )
message( " - Preset is ${CARRTPICO_PRESET_NAME}.")
message( " - Building of tests is ${CARRTPICO_BUILD_TESTS}." )
message( " - Use of stdio over UART is ${CARRTPICO_ENABLE_UART_STDIO}." )
message( " - Use of DebugUtils output is ${CARRTPICO_ENABLE_DEBUGUTILS}." )
message( "" )



