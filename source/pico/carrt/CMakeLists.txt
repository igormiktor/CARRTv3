add_executable( CarrtPico 
        CarrtPicoBinaryInfo.cpp 
        CarrtPicoMain.cpp
        CarrtPicoReset.cpp
        Core1.cpp
        EventManager.cpp 
        MainProcess.cpp
)

        
pico_set_program_name( CarrtPico "CarrtPico" )
pico_set_program_version( CarrtPico "0.1" )
pico_set_program_description( CarrtPico "Main program for Pico as part of CARRTv3" )
pico_set_program_url( CarrtPico "https://github.com/igormiktor/CARRTv3" )


pico_enable_stdio_uart( CarrtPico 1 )
pico_enable_stdio_usb( CarrtPico 0 )

# Add the standard library to the build
#target_link_libraries( CarrtPico
#        pico_multicore
#        pico_stdlib )

# Add the standard include files to the build
#target_include_directories( CarrtPico PRIVATE
#  ${CMAKE_CURRENT_LIST_DIR}
#  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
#)

target_compile_options( CarrtPico PRIVATE -Wall )
target_compile_definitions( CarrtPico PRIVATE 
        -DDEBUGUTILS_ON=${DEBUGUTILS_VAL} 
)


# Add any user requested libraries
target_link_libraries( CarrtPico PRIVATE
        pico_driver_library
        shared_library
        pico_multicore
        pico_stdlib
        hardware_i2c
        hardware_timer
        hardware_clocks
)

# create map/bin/hex file etc.
pico_add_extra_outputs( CarrtPico )
