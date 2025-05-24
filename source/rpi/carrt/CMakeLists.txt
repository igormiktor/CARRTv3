add_executable( CarrtRpi CarrtMain.cpp )

target_include_directories( CarrtRpi PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)


target_include_directories( CarrtRpi PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CARRT_RPI_DIRECTORY}
)

# Add any user requested libraries
target_link_libraries( CarrtRpi  ${PIGPIO_LIB} -lpthread -lrt )


