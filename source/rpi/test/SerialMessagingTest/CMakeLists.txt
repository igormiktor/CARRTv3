# Executable (command line) to test messaging with Pico

add_executable( SerialMessagingTest 
    SerialMessagingTest.cpp
)

target_compile_options( SerialMessagingTest PRIVATE -Wall -pthread )

target_compile_definitions( SerialMessagingTest PRIVATE 
    -DUSE_PIGPIOD=${PIGPIOD_VAL} 
    -DDEBUGUTILS_ON=${DEBUGUTILS_VAL}
    "$<$<CONFIG:DEBUG>:DEBUGRPI0=1>" 
    -DBUILDING_FOR_RPI0=1
)

target_link_libraries( SerialMessagingTest PRIVATE carrt_library rpi_driver_library shared_library ${PIGPIO_LIB} -lpthread -lrt )

