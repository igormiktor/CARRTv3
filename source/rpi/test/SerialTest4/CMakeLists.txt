add_executable( SerialTest4 SerialTest4.cpp )
target_compile_options( SerialTest4 PRIVATE -Wall -pthread )
target_compile_definitions( SerialTest4 PRIVATE 
    -DUSE_PIGPIOD=${PIGPIOD_VAL} 
    -DDEBUGUTILS_ON=${DEBUGUTILS_VAL}
)
target_link_libraries( SerialTest4 PRIVATE rpi_driver_library shared_library ${PIGPIO_LIB} -lpthread -lrt )
