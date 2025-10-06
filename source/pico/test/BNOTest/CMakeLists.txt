add_executable( BNOTest
        BNOTest.cpp
        ../../carrt/CarrtPicoReset.cpp
        ../../carrt/Core1.cpp
        ../../carrt/EventManager.cpp 
        ../../carrt/PicoSerialCommands.cpp
        ../../carrt/PicoState.cpp
)
        
pico_set_program_name( BNOTest "BNOTest" )
pico_set_program_version( BNOTest "0.1")
pico_set_program_description( BNOTest "Basic testing of Pico talking to BNO055" )
pico_set_program_url( BNOTest "https://github.com/igormiktor/CARRTv3" )


pico_enable_stdio_uart( BNOTest 1 )
pico_enable_stdio_usb( BNOTest 0 )


# Add the standard include files to the build
target_include_directories( BNOTest PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CARRT_PICO_DIRECTORY}
  ${CARRT_PICO_DIRECTORY}/carrt  
#  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

target_compile_options( CarrtPico PRIVATE -Wall )
target_compile_definitions( BNOTest PRIVATE
        -DDEBUGUTILS_ON=${DEBUGUTILS_VAL} 
        -DUSE_CARRTPICO_STDIO=${USE_CARRTPICO_STDIO_VAL}
        "$<$<CONFIG:DEBUG>:-DDEBUGPICO=1>" 
)

# Add any user requested libraries
target_link_libraries( BNOTest PRIVATE
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
pico_add_extra_outputs( BNOTest )

