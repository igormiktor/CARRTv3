add_executable( SerialTest4
        SerialTest4.cpp
        ../../carrt/EventManager.cpp 
        )
        
pico_set_program_name( SerialTest4 "SerialTest4" )
pico_set_program_version( SerialTest4 "0.1")
pico_set_program_description( SerialTest4 "Basic testing of RPi Zero to Pico serial comms" )
pico_set_program_url( SerialTest4 "https://github.com/igormiktor/CARRTv3" )


pico_enable_stdio_uart( SerialTest4 1 )
pico_enable_stdio_usb( SerialTest4 0 )

# Add the standard library to the build
target_link_libraries( SerialTest4
        pico_multicore
        pico_stdlib 
        )

# Add the standard include files to the build
target_include_directories( SerialTest4 PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CARRT_PICO_DIRECTORY}
  ${CARRT_PICO_DIRECTORY}/carrt     
  ${CARRT_PICO_DIRECTORY}/shared     
#  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries( SerialTest4 
        hardware_i2c
        hardware_timer
        hardware_clocks
        )

# create map/bin/hex file etc.
pico_add_extra_outputs( SerialTest4 )
