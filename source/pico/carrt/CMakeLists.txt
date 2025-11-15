# Create both a library (with most of CARRT Pico so it can be shared with testing code)
# and an executable (which is the actual CARRT Pico executable)

# The Library

add_library( CarrtPico_library STATIC )

target_sources( CarrtPico_library
    PRIVATE
        CarrtPicoBinaryInfo.cpp 
        CarrtPicoReset.cpp
        Core1.cpp
        EventHandlers.cpp
        EventManager.cpp 
        EventProcessor.cpp
        MainProcess.cpp
        PicoSerialMessages.cpp
        PicoState.cpp
    PUBLIC FILE_SET HEADERS FILES
        CarrtPicoDefines.h
        CarrtPicoReset.h
        Core1.h
        DebugMacros.h
        Event.h
        EventHandler.h
        EventHandlers.h
        EventManager.h
        EventProcessor.h
        MainProcess.h
        PicoState.h
)

# Compile definitions and options inhereted from link libs (shared_library is the "root")

target_link_libraries( CarrtPico_library PUBLIC
        pico_driver_library
        shared_library
        utils_library
        pico_multicore
        pico_stdlib
        hardware_i2c
        hardware_timer
        hardware_clocks
)


# The executable

add_executable( CarrtPico )

target_sources( CarrtPico 
    PRIVATE 
        CarrtPicoMain.cpp
)
        
pico_set_program_name( CarrtPico "CarrtPico" )
pico_set_program_version( CarrtPico "0.1" )
pico_set_program_description( CarrtPico "Main program for Pico as part of CARRTv3" )
pico_set_program_url( CarrtPico "https://github.com/igormiktor/CARRTv3" )


pico_enable_stdio_uart( CarrtPico ${USE_CARRTPICO_STDIO_VAL} )
pico_enable_stdio_usb( CarrtPico 0 )

target_compile_options( CarrtPico PRIVATE -Wall )

# Add any user requested libraries
target_link_libraries( CarrtPico PRIVATE
        CarrtPico_library
)

# create map/bin/hex file etc.
pico_add_extra_outputs( CarrtPico )
