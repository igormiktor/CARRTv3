
add_executable( SerialTest2 SerialTest2.cpp )
target_link_libraries( SerialTest2 -lpigpio -lpthread )
target_include_directories( SerialTest2 PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}
    ${CARRT_PICO_DIRECTORY}
)
