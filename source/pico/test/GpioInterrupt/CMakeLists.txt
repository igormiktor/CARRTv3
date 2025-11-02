add_executable( GpioInterruptTest
GpioInterruptTest.cpp
        ../../carrt/EventManager.cpp 
        )
        
pico_set_program_name( GpioInterruptTest "GpioInterruptTest" )
pico_set_program_version( GpioInterruptTest "0.1")
pico_set_program_description( GpioInterruptTest "Basic testing of Pico GPIO interrupts" )
pico_set_program_url( GpioInterruptTest "https://github.com/igormiktor/CARRTv3" )


pico_enable_stdio_uart( GpioInterruptTest 1 )
pico_enable_stdio_usb( GpioInterruptTest 0 )

# Add the standard include files to the build
target_include_directories( GpioInterruptTest PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CARRT_PICO_DIRECTORY}
  ${CARRT_PICO_DIRECTORY}/carrt     
#  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any libraries
target_link_libraries( GpioInterruptTest 
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
pico_add_extra_outputs( GpioInterruptTest )
