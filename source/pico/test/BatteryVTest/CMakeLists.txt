add_executable( BatteryVTest
BatteryVTest.cpp
        ../../carrt/EventManager.cpp 
        )
        
pico_set_program_name( BatteryVTest "BatteryVTest" )
pico_set_program_version( BatteryVTest "0.1")
pico_set_program_description( BatteryVTest "Basic testing of Pico GPIO interrupts" )
pico_set_program_url( BatteryVTest "https://github.com/igormiktor/CARRTv3" )


pico_enable_stdio_uart( BatteryVTest 1 )
pico_enable_stdio_usb( BatteryVTest 0 )

# Add the standard include files to the build
target_include_directories( BatteryVTest PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CARRT_PICO_DIRECTORY}
  ${CARRT_PICO_DIRECTORY}/carrt     
#  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any libraries
target_link_libraries( BatteryVTest 
        hardware_i2c
        hardware_timer
        hardware_clocks
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
pico_add_extra_outputs( BatteryVTest )
