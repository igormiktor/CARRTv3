add_executable(helloCarrtPico 
        hello.cpp 
        )
        
pico_set_program_name(helloCarrtPico "hello")
pico_set_program_version(helloCarrtPico "0.1")

pico_enable_stdio_uart(helloCarrtPico 1)
pico_enable_stdio_usb(helloCarrtPico 0)

# Add the standard library to the build
target_link_libraries(helloCarrtPico
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(helloCarrtPico PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(helloCarrtPico 
        hardware_spi
        hardware_i2c
        hardware_timer
        hardware_clocks
        )

# create map/bin/hex file etc.
pico_add_extra_outputs(helloCarrtPico)

