# Driver header list
set( DRVR_HDR_LIST
    BNO055/bno055.h   
    BNO055.h
    Encoders.h
    HeartBeatLed.h 
    I2C.h 
    SerialLinkPico.h
)

# Driver sources list
set ( DRVR_SRC_LIST 
    BNO055/bno055.c
    BNO055.cpp
    Encoders.cpp
    HeartBeatLed.cpp
    I2C.cpp 
    SerialLinkPico.cpp
)


add_library( pico_driver_library STATIC ${DRVR_SRC_LIST} ${DRVR_HDR_LIST} )
target_compile_options( pico_driver_library PUBLIC -Wall )
target_compile_definitions( pico_driver_library PUBLIC -DDEBUGUTILS_ON=${DEBUGUTILS_VAL} )
target_include_directories( pico_driver_library PUBLIC "${PROJECT_SOURCE_DIR}/drivers" "${PROJECT_SOURCE_DIR}/carrt" )
target_link_libraries( pico_driver_library PUBLIC shared_library )
target_link_libraries( pico_driver_library PUBLIC 
    pico_stdlib
    hardware_i2c
    hardware_timer
    hardware_clocks
)

