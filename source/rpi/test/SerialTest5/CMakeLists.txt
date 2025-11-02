add_executable( SerialTest5 
    SerialTest5.cpp 
    RPi0SerialCommands.cpp
)

target_compile_options( SerialTest5 PRIVATE -Wall -pthread )
target_compile_definitions( SerialTest5 PRIVATE 
    -DUSE_PIGPIOD=${PIGPIOD_VAL} 
    -DDEBUGUTILS_ON=${DEBUGUTILS_VAL}
    "$<$<CONFIG:DEBUG>:-DDEBUGRPI0=1>" 
    -DBUILDING_FOR_RPI0=1
)
target_link_libraries( SerialTest5 PRIVATE rpi_driver_library shared_library ${PIGPIO_LIB} -lpthread -lrt )
