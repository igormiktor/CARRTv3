
# Shared header list
set( SHARED_HDR_LIST
    CarrtError.h 
    ErrorCodes.h 
    Event.h 
    SerialLink.h 
    SerialMessage.h 
)

add_library( shared_library INTERFACE )
target_include_directories( shared_library INTERFACE "${PROJECT_SOURCE_DIR}/shared" )
