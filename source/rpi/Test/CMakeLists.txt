
cmake_minimum_required( VERSION 3.7 )


execute_process(COMMAND "uname" "-n" OUTPUT_VARIABLE SYS_NAME )

message( STATUS "The system is ${SYS_NAME}" )

if( ${SYS_NAME} MATCHES "carrt" )
    message( STATUS "Compiling on carrt, no cross-compiler required" )
else( ${SYS_NAME} MATCHES "carrt" )
    message( STATUS "Not compiling on carrt, assume cross-compiler required" )
    set( CMAKE_TOOLCHAIN_FILE $ENV{HOME}/.cmake/rpiw-toolchain.cmake )
endif( ${SYS_NAME} MATCHES "carrt" )


project( TestCARRT3 )

set( CMAKE_BUILD_TYPE Release )

include_directories( ".." )


set( DriverSrcs
        ../Drivers/Clock.cpp
        ../Drivers/I2c.cpp
        ../Drivers/Keypad.cpp
        ../Drivers/Lcd.cpp
        ../Drivers/PCA9685.cpp
        ../Drivers/Motors.cpp
    )

set( UtilsSrcs
    )

set( CarrtSrcs
    )


add_executable( LcdTest TestDrivers/LcdTest.cpp ${CarrtSrcs} ${DriverSrcs} ${UtilsSrcs}  )
target_link_libraries( LcdTest -lpigpio -lpthread )

add_executable( KeypadTest TestDrivers/KeypadTest.cpp ${CarrtSrcs} ${DriverSrcs} ${UtilsSrcs} )
target_link_libraries( KeypadTest -lpigpio -lpthread )

add_executable( MotorTestIndividual TestDrivers/MotorTestIndividual.cpp ${CarrtSrcs} ${DriverSrcs} ${UtilsSrcs} )
target_link_libraries( MotorTestIndividual -lpigpio -lpthread )

add_executable( MotorTestCollective TestDrivers/MotorTestCollective.cpp ${CarrtSrcs} ${DriverSrcs} ${UtilsSrcs} )
target_link_libraries( MotorTestCollective -lpigpio -lpthread )

add_executable( CommsToTrinketTest SerialToTrinketTest/SerialCommsToTrinketTest ${CarrtSrcs} ${DriverSrcs} ${UtilsSrcs} )
target_link_libraries( CommsToTrinketTest -lpigpio -lpthread )
