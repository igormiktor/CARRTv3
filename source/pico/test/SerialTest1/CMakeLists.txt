add_executable( SerialTest1
        SerialTest1.cpp
        ../../carrt/EventManager.cpp 
        )
        
pico_set_program_name( SerialTest1 "SerialTest1" )
pico_set_program_version( SerialTest1 "0.1")
pico_set_program_description( SerialTest1 "Minimal proof of concept for RPi0 to Pico serial comms" )
pico_set_program_url( SerialTest1 "https://github.com/igormiktor/CARRTv3" )


pico_enable_stdio_uart( SerialTest1 1 )
pico_enable_stdio_usb( SerialTest1 0 )

# Add the standard library to the build
target_link_libraries( SerialTest1
        pico_multicore
        pico_stdlib 
        )

# Add the standard include files to the build
target_include_directories( SerialTest1 PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../../carrt          
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries( SerialTest1 
        hardware_i2c
        hardware_timer
        hardware_clocks
        )

# create map/bin/hex file etc.
pico_add_extra_outputs( SerialTest1 )
