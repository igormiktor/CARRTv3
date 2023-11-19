add_executable(multicoreTest
        multicoreTest.c
        )

# Add the standard library to the build
target_link_libraries(multicoreTest
        pico_multicore
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(multicoreTest PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# create map/bin/hex file etc.
pico_add_extra_outputs(multicoreTest)

# add url via pico_set_program_url
# example_auto_set_url(multicoreTest)