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
        BUILDING_FOR_PICO=$<BOOL:${BUILDING_FOR_PICO}>
        USE_CARRTPICO_STDIO=$<BOOL:${CARRTPICO_ENABLE_UART_STDIO}>
        DEBUGUTILS_ON=$<BOOL:${CARRTPICO_ENABLE_DEBUGUTILS}>
        DEBUGCARRTPICO=$<CONFIG:DEBUG>
    )

    target_link_libraries( shared_library PUBLIC 
        pico_stdlib  
        pico_driver_library 
    )
endif()

if( BUILDING_FOR_RPI0 )
    target_compile_options( shared_library PUBLIC -Wall -pthread )

    target_compile_definitions( shared_library PUBLIC 
        BUILDING_FOR_RPI0=$<BOOL:${BUILDING_FOR_RPI0}>        
        USE_CARRTRPI0_STDIO=$<BOOL:${CARRT_RPI0_ENABLE_STDIO_OUTPUT}>
        USE_PIGPIOD=$<BOOL:${CARRT_RPI_USE_PIGPIOD}>
        DEBUGUTILS_ON=$<BOOL:${CARRT_ENABLE_DEBUGUTILS}> 
        DEBUGRPI0=$<CONFIG:DEBUG> 
    )
endif()

target_include_directories( shared_library PUBLIC "${PROJECT_SOURCE_DIR}/carrt")
target_include_directories( shared_library PUBLIC "${PROJECT_SOURCE_DIR}/shared" )
