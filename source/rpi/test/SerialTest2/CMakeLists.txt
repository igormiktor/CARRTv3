
add_executable( SerialTest2 SerialTest2.cpp )
target_link_libraries( SerialTest2 PRIVATE shared_library -lpthread -lrt )

