# This builds both a library (that test code can link to) and the main application 


add_library( carrt_library STATIC )

target_sources( carrt_library  
    PRIVATE
        RPi0SerialMessages.cpp 
    PUBLIC FILE_SET HEADERS FILES
        CarrtRpi0Defines.h
)

target_compile_options( carrt_library PUBLIC -Wall -pthread )
target_compile_definitions( carrt_library PUBLIC 
    -DUSE_PIGPIOD=${PIGPIOD_VAL} 
    -DDEBUGUTILS_ON=${DEBUGUTILS_VAL}
    "$<$<CONFIG:DEBUG>:-DDEBUGRPI0=1>" 
    -DBUILDING_FOR_RPI0=1
)
target_include_directories( carrt_library PUBLIC "${PROJECT_SOURCE_DIR}/drivers" )
target_link_libraries( carrt_library PUBLIC shared_library )




add_executable( CarrtRpi 
    CarrtMain.cpp 
)

target_compile_options( CarrtRpi PRIVATE -Wall -pthread )

target_compile_definitions( CarrtRpi PRIVATE 
    -DUSE_PIGPIOD=${PIGPIOD_VAL} 
    -DDEBUGUTILS_ON=${DEBUGUTILS_VAL}
    "$<$<CONFIG:DEBUG>:-DDEBUGRPI0=1>" 
    -DBUILDING_FOR_RPI0=1
)

target_link_libraries( CarrtRpi PRIVATE carrt_library rpi_driver_library shared_library ${PIGPIO_LIB} -lpthread -lrt )


