# This builds both a library (that test code can link to) and the main application 

# This is the library

add_library( carrt_library STATIC )

target_sources( carrt_library  
    PRIVATE
        RPi0SerialMessages.cpp 
    PUBLIC FILE_SET HEADERS FILES
        CarrtRpi0Defines.h
)

# Compile definitions and options inhereted from link libs (shared_library is the "root")

target_include_directories( carrt_library PUBLIC "${PROJECT_SOURCE_DIR}/drivers" )

target_link_libraries( carrt_library PUBLIC shared_library )



# And this is the main Carrt RPi0 executable

add_executable( CarrtRpi 
    CarrtMain.cpp 
)

# Compile definitions and options inhereted from link libs (shared_library is the "root")

target_link_libraries( CarrtRpi PRIVATE carrt_library rpi_driver_library shared_library ${PIGPIO_LIB} -lpthread -lrt )


