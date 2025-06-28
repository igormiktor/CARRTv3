
# include_directories( ".." )

set( DriverSrcs
        ../../Drivers/Clock.cpp
        ../../Drivers/I2c.cpp
        ../../Drivers/CarrtPigpio.cpp
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
target_compile_options( LcdOff PRIVATE -Wall -pthread )
target_link_libraries( LcdOff ${PIGPIO_LIB} -lpthread -lrt )
target_compile_definitions( LcdOff PRIVATE -DUSE_PIGPIOD=${CARRT_RPI_USE_PIGPIOD} )
target_include_directories( LcdOff PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../..
)
    
add_executable( LcdTest LcdTest.cpp ${CarrtSrcs} ${DriverSrcs} ${UtilsSrcs}  )
target_compile_options( LcdTest PRIVATE -Wall -pthread )
target_link_libraries( LcdTest ${PIGPIO_LIB} -lpthread -lrt )
target_compile_definitions( LcdTest PRIVATE -DUSE_PIGPIOD=${CARRT_RPI_USE_PIGPIOD} )
target_include_directories( LcdTest PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../..
)

add_executable( KeypadTest KeypadTest.cpp ${CarrtSrcs} ${DriverSrcs} ${UtilsSrcs} )
target_compile_options( KeypadTest PRIVATE -Wall -pthread )
target_link_libraries( KeypadTest ${PIGPIO_LIB} -lpthread -lrt )
target_compile_definitions( KeypadTest PRIVATE -DUSE_PIGPIOD=${CARRT_RPI_USE_PIGPIOD} )
target_include_directories( KeypadTest PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../..
)

add_executable( MotorTestIndividual MotorTestIndividual.cpp ${CarrtSrcs} ${DriverSrcs} ${UtilsSrcs} )
target_compile_options( MotorTestIndividual PRIVATE -Wall -pthread )
target_link_libraries( MotorTestIndividual ${PIGPIO_LIB} -lpthread -lrt )
target_compile_definitions( MotorTestIndividual PRIVATE -DUSE_PIGPIOD=${CARRT_RPI_USE_PIGPIOD} )
target_include_directories( MotorTestIndividual PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../..
)

add_executable( MotorTestCollective MotorTestCollective.cpp ${CarrtSrcs} ${DriverSrcs} ${UtilsSrcs} )
target_compile_options( MotorTestCollective PRIVATE -Wall -pthread )
target_link_libraries( MotorTestCollective ${PIGPIO_LIB} -lpthread -lrt )
target_compile_definitions( MotorTestCollective PRIVATE -DUSE_PIGPIOD=${CARRT_RPI_USE_PIGPIOD} )
target_include_directories( MotorTestCollective PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../..
)
