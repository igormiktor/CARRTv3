add_executable( BNORaw
        BNORaw.c
        bno055_support.c
        ../../drivers/BNO055/bno055.c
)
        
pico_set_program_name( BNORaw "BNORaw" )
pico_set_program_version( BNORaw "0.1")
pico_set_program_description( BNORaw "Basic testing of Pico talking to BNO055" )
pico_set_program_url( BNORaw "https://github.com/igormiktor/CARRTv3" )


pico_enable_stdio_uart( BNORaw 1 )
pico_enable_stdio_usb( BNORaw 0 )


# Add the standard include files to the build
target_include_directories( BNORaw PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CARRT_PICO_DIRECTORY}
#  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

target_compile_options( BNORaw PRIVATE -Wall )
target_compile_definitions( BNORaw PRIVATE
        -DDEBUGUTILS_ON=${DEBUGUTILS_VAL} 
        -DUSE_CARRTPICO_STDIO=${USE_CARRTPICO_STDIO_VAL}
        "$<$<CONFIG:DEBUG>:-DDEBUGPICO=1>" 
        -DBNO055_API=1
        -DMY_I2C_PORT=i2c0
)

# Add any user requested libraries
target_link_libraries( BNORaw PRIVATE

        pico_driver_library
        pico_multicore
        pico_stdlib 
        hardware_i2c
        hardware_timer
        hardware_clocks
)

# create map/bin/hex file etc.
pico_add_extra_outputs( BNORaw )

