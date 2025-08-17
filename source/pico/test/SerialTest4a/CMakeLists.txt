add_executable( SerialTest4a
        SerialTest4a.cpp
        ../../carrt/EventManager.cpp 
        )
        
pico_set_program_name( SerialTest4a "SerialTest4a" )
pico_set_program_version( SerialTest4a "0.1")
pico_set_program_description( SerialTest4a "Basic testing of RPi Zero to Pico serial comms" )
pico_set_program_url( SerialTest4a "https://github.com/igormiktor/CARRTv3" )


pico_enable_stdio_uart( SerialTest4a 1 )
pico_enable_stdio_usb( SerialTest4a 0 )


# Add the standard include files to the build
target_include_directories( SerialTest4a PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CARRT_PICO_DIRECTORY}
  ${CARRT_PICO_DIRECTORY}/carrt  
#  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries( SerialTest4a PRIVATE
        pico_driver_library
        shared_library
        utils_library
        pico_multicore
        pico_stdlib 
        hardware_i2c
        hardware_timer
        hardware_clocks
        )

# create map/bin/hex file etc.
pico_add_extra_outputs( SerialTest4a )
