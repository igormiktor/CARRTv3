# Basic tests of all the various drivers


add_executable( LcdOff LcdOff.cpp )
target_compile_options( LcdOff PRIVATE -Wall -pthread )
target_compile_definitions( LcdOff PRIVATE -DUSE_PIGPIOD=${CARRT_RPI_USE_PIGPIOD} -DDEBUGUTILS_ON=${CARRT_ENABLE_DEBUGUTILS} )
target_link_libraries( LcdOff PRIVATE driver_library utils_library shared_library ${PIGPIO_LIB} -lpthread -lrt )

    
add_executable( LcdTest LcdTest.cpp  )
target_compile_options( LcdTest PRIVATE -Wall -pthread )
target_compile_definitions( LcdTest PRIVATE -DUSE_PIGPIOD=${CARRT_RPI_USE_PIGPIOD} -DDEBUGUTILS_ON=${CARRT_ENABLE_DEBUGUTILS} )
target_link_libraries( LcdTest PRIVATE driver_library utils_library shared_library ${PIGPIO_LIB} -lpthread -lrt )


add_executable( KeypadTest KeypadTest.cpp )
target_compile_options( KeypadTest PRIVATE -Wall -pthread )
target_compile_definitions( KeypadTest PRIVATE -DUSE_PIGPIOD=${CARRT_RPI_USE_PIGPIOD} -DDEBUGUTILS_ON=${CARRT_ENABLE_DEBUGUTILS} )
target_link_libraries( KeypadTest PRIVATE driver_library utils_library shared_library ${PIGPIO_LIB} -lpthread -lrt )


add_executable( MotorTestIndividual MotorTestIndividual.cpp )
target_compile_options( MotorTestIndividual PRIVATE -Wall -pthread )
target_compile_definitions( MotorTestIndividual PRIVATE -DUSE_PIGPIOD=${CARRT_RPI_USE_PIGPIOD} -DDEBUGUTILS_ON=${CARRT_ENABLE_DEBUGUTILS} )
target_link_libraries( MotorTestIndividual PRIVATE driver_library utils_library shared_library ${PIGPIO_LIB} -lpthread -lrt )


add_executable( MotorTestCollective MotorTestCollective.cpp )
target_compile_options( MotorTestCollective PRIVATE -Wall -pthread )
target_compile_definitions( MotorTestCollective PRIVATE -DUSE_PIGPIOD=${CARRT_RPI_USE_PIGPIOD} -DDEBUGUTILS_ON=${CARRT_ENABLE_DEBUGUTILS} )
target_link_libraries( MotorTestCollective PRIVATE driver_library utils_library shared_library ${PIGPIO_LIB} -lpthread -lrt )

