# Create both a library (with most of CARRT Pico so it can be shared with testing code)
# and an executable (which is the actual CARRT Pico executable)


# Set up automated extraction of git HEAD info

add_custom_target( git_head_info
  COMMAND "${CMAKE_COMMAND}"
  "-D" "SRC_DIR=${CMAKE_CURRENT_SOURCE_DIR}"
  "-D" "BIN_DIR=${CMAKE_CURRENT_BINARY_DIR}"
  "-P" "${PROJECT_SOURCE_DIR}/cmake/version.cmake"
  COMMENT "Generating git version file"
)

# The Library

add_library( CarrtPico_library STATIC )


add_dependencies( CarrtPico_library git_head_info )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )


target_sources( CarrtPico_library
    PRIVATE
        BuildInfo.cpp
        CarrtPicoReset.cpp
        Core1.cpp
        EventHandlers.cpp
        EventManager.cpp 
        EventProcessor.cpp
        MainProcess.cpp
        PicoSerialMessages.cpp
        PicoState.cpp
    PUBLIC FILE_SET HEADERS FILES
        BuildInfo.h    
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
# But add these to set up versioning
set( CARRTPICO_VERSION_STR "${CARRTPICO_VERSION_MAJOR}.${CARRTPICO_VERSION_MINOR}.${CARRTPICO_VERSION_REVISION}" )
target_compile_definitions( CarrtPico_library PUBLIC 
    -DCARRTPICO_VERSION_MAJOR=${CARRTPICO_VERSION_MAJOR} 
    -DCARRTPICO_VERSION_MINOR=${CARRTPICO_VERSION_MINOR} 
    -DCARRTPICO_VERSION_REVISION=${CARRTPICO_VERSION_REVISION}
    -DCARRTPICO_VERSION_STR="${CARRTPICO_VERSION_STR}" 
)

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
        CarrtPicoBinaryInfo.cpp 
        CarrtPicoMain.cpp
)
        
pico_set_program_name( CarrtPico "CarrtPico" )
pico_set_program_version( CarrtPico "0.9.0" )       # TODO / FIXME
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
