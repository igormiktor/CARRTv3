
# Utils header list
set( UTILS_HDR_LIST
    DebugUtils.hpp
)

# Utils source list
set( UTILS_SRC_LIST 
#    DebugUtils.cpp
)

add_library( utils_library INTERFACE ${UTILS_SRC_LIST} ${UTILS_HDR_LIST} )
target_include_directories( utils_library INTERFACE "${PROJECT_SOURCE_DIR}/utils" )

