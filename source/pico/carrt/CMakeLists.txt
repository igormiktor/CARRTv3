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

# Set up versioning
set( CARRT3_VERSION_STR "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_REVISION}" )
target_compile_definitions( CarrtPico PUBLIC 
    -DCARRT3_VERSION_MAJOR=${VERSION_MAJOR} 
    -DCARRT3_VERSION_MINOR=${VERSION_MINOR} 
    -DCARRT3_VERSION_REVISION=${VERSION_REVISION}
    -DCARRT3_VERSION_STR="${CARRT_VERSION_STR}" 
)

# Capture the current git head hash
set( Git_Head_Hash_h "${CMAKE_CURRENT_BINARY_DIR}/GitHeadHash.h" )
set( makeGitInfoHeader "${CMAKE_CURRENT_BINARY_DIR}/makeGitInfoHeader" )

add_custom_command(
    OUTPUT 
        ${makeGitInfoHeader}
    COMMAND 
        echo "#!/bin/sh" > ${makeGitInfoHeader}
    COMMAND 
        echo "echo \"#ifndef GitHeadHash_h\" > $1" >> ${makeGitInfoHeader}
    COMMAND 
        echo "echo \"#define GitHeadHash_h\" >> $1" >> ${makeGitInfoHeader}
    COMMAND 
        echo "echo \"// This is the git HEAD sha1 hash\" >> $1" >> ${makeGitInfoHeader}
    COMMAND 
        echo "echo \"#define CARRT_GIT_HEAD_SHA1 `git rev-parse --short HEAD`\" >> $1" >> ${makeGitInfoHeader}
    COMMAND 
        echo "echo \"#endif // GitHeadHash_h\" >> $1" >> ${makeGitInfoHeader}
    COMMAND 
        chmod +x ${makeGitInfoHeader}
    VERBATIM
)

set_source_files_properties(
    ${makeGitInfoHeader}
    PROPERTIES GENERATED TRUE
)

set( Git_Current_Branch "${PROJECT_SOURCE_DIR}/../../.git/HEAD" )
file( 
    GLOB Git_Branch_Heads "${PROJECT_SOURCE_DIR}/../../.git/refs/heads/*"
)

add_custom_command(
    OUTPUT 
        ${Git_Head_Hash_h}
    DEPENDS 
        ${makeGitInfoHeader}
        ${Git_Current_Branch}
        ${Git_Branch_Heads}
    COMMAND 
        ${makeGitInfoHeader} ${Git_Head_Hash_h}
    VERBATIM
)

set_source_files_properties(
    ${GitHead_Hash_h}
    PROPERTIES GENERATED TRUE
    HEADER_FILE_ONLY TRUE
)

add_custom_target(
    BuildHashInfo
    DEPENDS 
        ${Git_Head_Hash_h}
        ${Git_Current_Branch}
        ${Git_Branch_Heads}
)

add_library( GitHeadHashLib INTERFACE )
target_sources( GitHeadHashLib 
    INTERFACE 
        FILE_SET HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        FILES 
            ${Git_Head_Hash_h}
)

add_dependencies( GitHeadHashLib BuildHashInfo )

# Add any user requested libraries
target_link_libraries( CarrtPico PRIVATE
        CarrtPico_library
        GitHeadHashLib
)

# create map/bin/hex file etc.
pico_add_extra_outputs( CarrtPico )
