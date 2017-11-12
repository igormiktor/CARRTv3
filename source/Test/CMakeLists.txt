
cmake_minimum_required( VERSION 2.8 )

set( CMAKE_TOOLCHAIN_FILE $ENV{HOME}/.cmake/rpiw-toolchain.cmake )


project( TestCARRT3 )

set( CMAKE_BUILD_TYPE Release )

include_directories( ".." )


set( DriverSrcs
        ../Drivers/Clock.cpp
        ../Drivers/I2c.cpp
        ../Drivers/Lcd.cpp
    )


set( CarrtSrcs
    )


add_executable( LcdTest TestDrivers/LcdTest.cpp ${CarrtSrcs} ${DriverSrcs}  )

#add_executable( KeypadTest TestDrivers/KeypadTest.cpp ${CarrtSrcs} ${DriverSrcs}  )

