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

# Compile definitions and options inhereted from link libs (shared_library is the "root")

target_include_directories( rpi_driver_library PUBLIC "${PROJECT_SOURCE_DIR}/drivers" )

target_link_libraries( rpi_driver_library PUBLIC shared_library )

