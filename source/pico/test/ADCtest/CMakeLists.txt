add_executable( ADCtest 
        adctest.cpp 
        )
        
pico_set_program_name( ADCtest "ADC Test" )
pico_set_program_version( ADCtest "0.1" )
pico_set_program_description( ADCtest "ADC testing & demo program" )
pico_set_program_url( ADCtest "https://github.com/igormiktor/CARRTv3" )

pico_enable_stdio_uart(ADCtest 1)
pico_enable_stdio_usb(ADCtest 0)

# Add the standard library to the build
target_link_libraries(ADCtest
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(ADCtest PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CARRT_PICO_DIRECTORY}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(ADCtest 
#        hardware_spi
        hardware_adc
        hardware_i2c
        hardware_timer
        hardware_clocks
        )

# create map/bin/hex file etc.
pico_add_extra_outputs(ADCtest)

