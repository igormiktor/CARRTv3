# Code shared between RPi0 and Pico


add_library( shared_library STATIC )

target_sources( shared_library 
    PRIVATE
        SerialMessageProcessor.cpp
        SerialLink.cpp
    PUBLIC FILE_SET HEADERS FILES
        CarrtError.h 
        DebugUtils.hpp
        ErrorCodes.h 
        OutputUtils.hpp
        SerialMessage.h 
        SerialMessageProcessor.h
        SerialLink.h 
)

if( BUILDING_FOR_PICO )
    target_compile_options( shared_library PUBLIC -Wall )

    target_compile_definitions( shared_library PUBLIC 
        -DDEBUGUTILS_ON=${DEBUGUTILS_VAL} 
        -DUSE_CARRTPICO_STDIO=${USE_CARRTPICO_STDIO_VAL}
        "$<$<CONFIG:DEBUG>:DEBUGCARRTPICO=1>" 
        -DBUILDING_FOR_PICO=1 
    )

    target_link_libraries( shared_library PUBLIC 
        pico_stdlib  
        pico_driver_library 
    )
endif()

if( BUILDING_FOR_RPI0 )
    target_compile_options( shared_library PUBLIC -Wall -pthread )

    target_compile_definitions( shared_library PUBLIC 
        -DUSE_PIGPIOD=${PIGPIOD_VAL} 
        -DDEBUGUTILS_ON=${DEBUGUTILS_VAL} 
        -DUSE_CARRTRPI0_STDIO=${CARRT_RPI0_ENABLE_STDIO_OUTPUT_VAL}
        "$<$<CONFIG:DEBUG>:DEBUGRPI0=1>" 
        -DBUILDING_FOR_RPI0=1
    )
endif()

target_include_directories( shared_library PUBLIC "${PROJECT_SOURCE_DIR}/carrt")
target_include_directories( shared_library PUBLIC "${PROJECT_SOURCE_DIR}/shared" )
