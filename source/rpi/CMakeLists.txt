cmake_minimum_required(VERSION 3.25.0)


project(rpiCarrt VERSION 0.1.0 LANGUAGES ASM C CXX)

include(CTest)
enable_testing()

add_executable(rpiCarrt main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set( CARRT_RPI_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" )
message( CARRT_RPI_DIRECTORY="${CARRT_RPI_DIRECTORY}")

set( CARRT_RPI_USE_PIGPIOD 1 )

if ( CARRT_RPI_USE_PIGPIOD )
    message ( "Using PIGPIOD daemon" )
    set( PIGPIO_LIB "-lpigpiod_if2" )
else()
    message( "Using plain PIGPIO" )
    set( PIGPIO_LIB "-lpigpio" )
endif()


add_subdirectory(Test)

