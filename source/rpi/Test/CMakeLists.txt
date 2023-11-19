
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


add_executable( LcdOff TestDrivers/LcdOff.cpp ${CarrtSrcs} ${DriverSrcs} ${UtilsSrcs}  )
target_link_libraries( LcdOff -lpigpio -lpthread )
    
add_executable( LcdTest TestDrivers/LcdTest.cpp ${CarrtSrcs} ${DriverSrcs} ${UtilsSrcs}  )
target_link_libraries( LcdTest -lpigpio -lpthread )

add_executable( KeypadTest TestDrivers/KeypadTest.cpp ${CarrtSrcs} ${DriverSrcs} ${UtilsSrcs} )
target_link_libraries( KeypadTest -lpigpio -lpthread )

add_executable( MotorTestIndividual TestDrivers/MotorTestIndividual.cpp ${CarrtSrcs} ${DriverSrcs} ${UtilsSrcs} )
target_link_libraries( MotorTestIndividual -lpigpio -lpthread )

add_executable( MotorTestCollective TestDrivers/MotorTestCollective.cpp ${CarrtSrcs} ${DriverSrcs} ${UtilsSrcs} )
target_link_libraries( MotorTestCollective -lpigpio -lpthread )
