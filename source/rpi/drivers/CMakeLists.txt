# Driver header list
set( DRVR_HDR_LIST
    Buzzer.h    
    CarrtPigpio.h 
    CarrtPinAssignments.h
    Clock.h 
    I2c.h 
    Keypad.h 
    Lcd.h 
    Lidar.h
    Motors.h 
    PCA9685.h
    SerialLinkRPi.h
    Servo.h 
)

# Driver sources list
set ( DRVR_SRC_LIST 
    Buzzer.cpp 
    CarrtPigpio.cpp 
    Clock.cpp 
    I2c.cpp 
    Keypad.cpp 
    Lcd.cpp 
    Lidar.cpp
    Motors.cpp 
    PCA9685.cpp 
    SerialLinkRPi.cpp
    Servo.cpp 
)


add_library( rpi_driver_library STATIC ${DRVR_SRC_LIST} ${DRVR_HDR_LIST} )
target_compile_options( rpi_driver_library PUBLIC -Wall -pthread )
target_compile_definitions( rpi_driver_library PUBLIC 
    -DUSE_PIGPIOD=${PIGPIOD_VAL} 
    -DDEBUGUTILS_ON=${DEBUGUTILS_VAL}
)
target_include_directories( rpi_driver_library PUBLIC "${PROJECT_SOURCE_DIR}/drivers" )
target_link_libraries( rpi_driver_library PUBLIC shared_library )

