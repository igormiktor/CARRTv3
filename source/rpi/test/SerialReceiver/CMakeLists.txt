# Executable (command line) to test messaging with Pico

add_executable( SerialReceiver
    SerialReceiver.cpp
    RPi0DumpSerialMessages.cpp
)

target_compile_options( SerialReceiver PRIVATE -Wall -pthread )

target_compile_definitions( SerialReceiver PRIVATE 
    BUILDING_FOR_RPI0=$<BOOL:${BUILDING_FOR_RPI0}>        
    USE_CARRTRPI0_STDIO=$<BOOL:${CARRT_RPI0_ENABLE_STDIO_OUTPUT}>
    USE_PIGPIOD=$<BOOL:${CARRT_RPI_USE_PIGPIOD}>
    DEBUGUTILS_ON=$<BOOL:${CARRT_ENABLE_DEBUGUTILS}> 
    DEBUGRPI0=$<CONFIG:DEBUG> 
)

target_link_libraries( SerialReceiver PRIVATE 
    rpi_driver_library 
    shared_library 
    $<IF:$<BOOL:${CARRT_RPI_USE_PIGPIOD}>,pigpiod_if2,pigpio> 
    pthread 
    rt 
)

