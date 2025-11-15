# Drivers for Pico-related hardware systems

add_library( pico_driver_library STATIC )

target_sources( pico_driver_library
    PRIVATE
        BNO055/bno055.c
        BNO055.cpp
        Encoders.cpp
        HeartBeatLed.cpp
        I2C.cpp 
        SerialLinkPico.cpp
    PUBLIC FILE_SET HEADERS FILES
        BNO055/bno055.h   
        BNO055.h
        Delays.h
        Encoders.h
        HeartBeatLed.h 
        I2C.h 
        SerialLinkPico.h
)

# Compile definitions and options inhereted from link libs (shared_library is the "root")

target_include_directories( pico_driver_library PUBLIC "${PROJECT_SOURCE_DIR}/drivers" "${PROJECT_SOURCE_DIR}/carrt" )

target_link_libraries( pico_driver_library PUBLIC 
    shared_library 
    pico_stdlib
    hardware_i2c
    hardware_timer
    hardware_clocks
)

