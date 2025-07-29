
# Shared header list
set( SHARED_HDR_LIST
    CarrtError.h 
    ErrorCodes.h 
    Event.h 
    SerialLink.h 
    SerialMessage.h 
)

# Shared source list
set( SHARED_SRC_LIST 
    SerialLink.cpp
)

add_library( shared_library STATIC ${SHARED_SRC_LIST} ${SHARED_HDR_LIST} )
target_include_directories( shared_library INTERFACE "${PROJECT_SOURCE_DIR}/shared" )
