add_executable( SerialTest3 SerialTest3.cpp )
target_compile_options( SerialTest3 PRIVATE -Wall -pthread )
target_compile_definitions( SerialTest3 PRIVATE 
    -DUSE_PIGPIOD=${PIGPIOD_VAL} 
    -DDEBUGUTILS_ON=${DEBUGUTILS_VAL}
)
target_link_libraries( SerialTest3 PRIVATE driver_library utils_library shared_library ${PIGPIO_LIB} -lpthread -lrt )
