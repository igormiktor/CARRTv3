
# Shared header list
set( SHARED_HDR_LIST
    CarrtError.h 
    DebugUtils.hpp
    ErrorCodes.h 
    SerialMessage.h 
    SerialMessageProcessor.h
    SerialLink.h 
)

# Shared source list
set( SHARED_SRC_LIST 
    SerialMessageProcessor.cpp
    SerialLink.cpp
)

add_library( shared_library STATIC ${SHARED_SRC_LIST} ${SHARED_HDR_LIST} )
target_compile_options( shared_library PUBLIC -Wall )
if( BULDING_FOR_PICO )
    target_compile_definitions( shared_library PUBLIC 
        -DDEBUGUTILS_ON=${DEBUGUTILS_VAL} 
        -DUSE_CARRTPICO_STDIO=${USE_CARRTPICO_STDIO_VAL}
        "$<$<CONFIG:DEBUG>:-DDEBUGPICO=1>" 
        -DDEBUGUTILS_ON=${DEBUGUTILS_VAL} -DBUILDING_FOR_PICO=1 
    )
endif()
if( BULDING_FOR_RPI0 )
    target_compile_definitions( shared_library PUBLIC -DDEBUGUTILS_ON=${DEBUGUTILS_VAL} -DBUILDING_FOR_RPI0=1 )
endif()
target_include_directories( shared_library PUBLIC "${PROJECT_SOURCE_DIR}/carrt")
target_include_directories( shared_library INTERFACE "${PROJECT_SOURCE_DIR}/shared" )
