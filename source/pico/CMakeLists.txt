# Generated Cmake Pico project file

cmake_minimum_required( VERSION 3.25 )

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set( PICO_SDK_PATH "/home/igor/Code/pico/pico-sdk" )

set( PICO_BOARD pico CACHE STRING "Board type" )

# Pull in Raspberry Pi Pico SDK (must be before project)
include( pico_sdk_import.cmake )

if ( PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0" )
  message( FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}" )
endif()

project( carrt_pico C CXX ASM )

set( CMAKE_C_STANDARD 11 )
set( CMAKE_CXX_STANDARD 17 )
set( PICO_CXX_ENABLE_EXCEPTIONS 1 )


# Add a special define "DEBUGPICO" when building debugging builds
add_compile_options( "$<$<CONFIG:DEBUG>:-DDEBUGPICO=1>" )

set( CARRT_PICO_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" )
# message( CARRT_PICO_DIRECTORY="${CARRT_PICO_DIRECTORY}" )

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory( carrt )
add_subdirectory( test )


