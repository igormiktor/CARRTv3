
# include_directories( ".." )

set( DriverSrcs
        ../../Drivers/Clock.cpp
        ../../Drivers/I2c.cpp
        ../../Drivers/Keypad.cpp
        ../../Drivers/Lcd.cpp
        ../../Drivers/PCA9685.cpp
        ../../Drivers/Motors.cpp
    )

set( UtilsSrcs
    )

set( CarrtSrcs
    )


add_executable( LcdOff LcdOff.cpp ${CarrtSrcs} ${DriverSrcs} ${UtilsSrcs}  )
target_link_libraries( LcdOff -lpigpio -lpthread )
target_include_directories( LcdOff PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../..
)
    
add_executable( LcdTest LcdTest.cpp ${CarrtSrcs} ${DriverSrcs} ${UtilsSrcs}  )
target_link_libraries( LcdTest -lpigpio -lpthread )
target_include_directories( LcdTest PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../..
)

add_executable( KeypadTest KeypadTest.cpp ${CarrtSrcs} ${DriverSrcs} ${UtilsSrcs} )
target_link_libraries( KeypadTest -lpigpio -lpthread )
target_include_directories( KeypadTest PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../..
)

add_executable( MotorTestIndividual MotorTestIndividual.cpp ${CarrtSrcs} ${DriverSrcs} ${UtilsSrcs} )
target_link_libraries( MotorTestIndividual -lpigpio -lpthread )
target_include_directories( MotorTestIndividual PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../..
)

add_executable( MotorTestCollective MotorTestCollective.cpp ${CarrtSrcs} ${DriverSrcs} ${UtilsSrcs} )
target_link_libraries( MotorTestCollective -lpigpio -lpthread )
target_include_directories( MotorTestCollective PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../..
)
