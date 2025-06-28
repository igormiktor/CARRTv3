
add_executable( CarrtRpi CarrtMain.cpp )
target_compile_options( CarrtRpi PRIVATE -Wall -pthread )
target_compile_definitions( CarrtRpi PRIVATE -DUSE_PIGPIOD=${CARRT_RPI_USE_PIGPIOD} -DDEBUGUTILS_ON=${CARRT_ENABLE_DEBUGUTILS} )
target_link_libraries( CarrtRpi PRIVATE driver_library utils_library shared_library ${PIGPIO_LIB} -lpthread -lrt )


