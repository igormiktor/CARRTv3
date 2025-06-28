
add_executable( SerialTest1 SerialTest1.cpp )
target_link_libraries( SerialTest1 -lpigpio -lpthread )
target_include_directories( SerialTest1 PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}
)
