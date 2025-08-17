add_executable( SerialTest3
        SerialTest3.cpp
        ../../carrt/EventManager.cpp 
        )
        
pico_set_program_name( SerialTest3 "SerialTest3" )
pico_set_program_version( SerialTest3 "0.1")
pico_set_program_description( SerialTest3 "Basic testing of RPi Zero to Pico serial comms" )
pico_set_program_url( SerialTest3 "https://github.com/igormiktor/CARRTv3" )


pico_enable_stdio_uart( SerialTest3 1 )
pico_enable_stdio_usb( SerialTest3 0 )

# Add the standard include files to the build
target_include_directories( SerialTest3 PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CARRT_PICO_DIRECTORY}
  ${CARRT_PICO_DIRECTORY}/carrt     
#  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries( SerialTest3 
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
pico_add_extra_outputs( SerialTest3 )
