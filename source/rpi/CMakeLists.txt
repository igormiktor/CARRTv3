cmake_minimum_required( VERSION 3.7 )


execute_process(COMMAND "uname" "-n" OUTPUT_VARIABLE SYS_NAME )

message( STATUS "The system is ${SYS_NAME}" )

if( ${SYS_NAME} MATCHES "carrt" )
    message( STATUS "Compiling on carrt, no cross-compiler required" )
else( ${SYS_NAME} MATCHES "carrt" )
    message( STATUS "Not compiling on carrt, assume cross-compiler required" )
    set( CMAKE_TOOLCHAIN_FILE $ENV{HOME}/.cmake/rpiw-toolchain.cmake )
endif( ${SYS_NAME} MATCHES "carrt" )


project( CARRT3 )

set( CMAKE_BUILD_TYPE Release )

include_directories( ".." )


set( DriverSrcs
        Drivers/Clock.cpp
        Drivers/I2c.cpp
        Drivers/Keypad.cpp
        Drivers/Lcd.cpp
        Drivers/PCA9685.cpp
    )


set( CarrtSrcs
    )


add_executable( Carrt3 ${CarrtSrcs} ${DriverSrcs}  )
set_target_properties( Carrt3 PROPERTIES COMPILE_FLAGS "-std=c++11" )
