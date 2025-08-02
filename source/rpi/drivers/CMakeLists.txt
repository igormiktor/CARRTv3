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


add_library( driver_library STATIC ${DRVR_SRC_LIST} ${DRVR_HDR_LIST} )
target_compile_options( driver_library PUBLIC -Wall -pthread )
target_compile_definitions( driver_library PUBLIC 
    -DUSE_PIGPIOD=${PIGPIOD_VAL} 
    -DDEBUGUTILS_ON=${DEBUGUTILS_VAL}
)
target_include_directories( driver_library PUBLIC "${PROJECT_SOURCE_DIR}/drivers" )
target_link_libraries( driver_library PUBLIC utils_library shared_library )

