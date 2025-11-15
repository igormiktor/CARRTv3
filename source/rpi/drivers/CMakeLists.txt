# Drivers for RPi0

add_library( rpi_driver_library STATIC )

target_sources( rpi_driver_library
    PRIVATE
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
    PUBLIC FILE_SET HEADERS FILES
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

target_compile_options( rpi_driver_library PUBLIC -Wall -pthread )
target_compile_definitions( rpi_driver_library PUBLIC 
    -DUSE_PIGPIOD=${PIGPIOD_VAL} 
    -DDEBUGUTILS_ON=${DEBUGUTILS_VAL}
    "$<$<CONFIG:DEBUG>:-DDEBUGRPI0=1>" 
)
target_include_directories( rpi_driver_library PUBLIC "${PROJECT_SOURCE_DIR}/drivers" )
target_link_libraries( rpi_driver_library PUBLIC shared_library )

