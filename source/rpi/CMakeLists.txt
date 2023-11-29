cmake_minimum_required(VERSION 3.0.0)


project(rpiCarrt VERSION 0.1.0 LANGUAGES ASM C CXX)

include(CTest)
enable_testing()

add_executable(rpiCarrt main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set( CARRT_PICO_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" )
message( CARRT_PICO_DIRECTORY="${CARRT_PICO_DIRECTORY}")

add_subdirectory(Test)

