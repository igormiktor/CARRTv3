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

# This one is a must
set( PICO_CXX_ENABLE_EXCEPTIONS 1 )


option( 
    CARRT_PICO_ENABLE_UART_STDIO
    "Enable stdio over UART.  Default: ON. Values: { OFF, ON }."
    ON
)

option(
    CARRT_PICO_BUILD_TESTS
    "Enable building test executables. Default: ON. Values: { OFF, ON }."
    ON
)

option( 
    CARRT_PICO_ENABLE_DEBUGUTILS
    "Enable DebugUtils debugging outputs.  Default: ON. Values: { OFF, ON }."
    ON
)


# Add a special define "DEBUGPICO" when building debugging builds
add_compile_options( "$<$<CONFIG:DEBUG>:-DDEBUGPICO=1>" )

set( CARRT_PICO_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" )
# message( CARRT_PICO_DIRECTORY="${CARRT_PICO_DIRECTORY}" )

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()


#######################################


set( BUILDING_FOR_PICO 1 )

if ( CARRT_PICO_ENABLE_UART_STDIO )
    set( USE_CARRTPICO_STDIO_VAL 1 )
else()
    set( USE_CARRTPICO_STDIO_VAL 0 )
endif()

if ( CARRT_PICO_ENABLE_DEBUGUTILS )
    set( DEBUGUTILS_VAL 1 )
else()
    set( DEBUGUTILS_VAL 0 )
endif()


add_subdirectory( shared )
add_subdirectory( utils )
add_subdirectory( drivers )
add_subdirectory( carrt )

if ( CARRT_PICO_BUILD_TESTS )
    add_subdirectory( test )
#    include( CTest )
#    enable_testing()
endif()

message( "" )
message( "Configuration summary on host ${CMAKE_HOST_SYSTEM_NAME}:" )
message( " - Build type is ${CMAKE_BUILD_TYPE}." )
message( " - Building of tests is ${CARRT_PICO_BUILD_TESTS}." )
message( " - Use of stdio over UART is ${CARRT_PICO_ENABLE_UART_STDIO}." )
message( " - Use of DebugUtils output is ${CARRT_PICO_ENABLE_DEBUGUTILS}." )
message( "" )



