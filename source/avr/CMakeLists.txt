
cmake_minimum_required( VERSION 3.7 )



set( AVR_UPLOADTOOL avrdude )
set( AVR_PROGRAMMER arduino )
set( AVR_UPLOADTOOL_PORT "/dev/ftdi" )

set( AVR_MCU atmega328p )
set( AVR_MCU_SPEED 12000000 )

#set( CMAKE_TOOLCHAIN_FILE $ENV{HOME}/.cmake/generic-gcc-avr.cmake )
set( CMAKE_TOOLCHAIN_FILE $ENV{HOME}/.cmake/avr-gcc-toolchain.cmake )

project( CompileCheck )

set( CMAKE_BUILD_TYPE Release )

enable_language( ASM )

include_directories( "." )


set( TestSrcs
        SerialResponderMain.cpp
        EventClock.cpp
        EventManager.cpp
    )


set( AvrUtilsSrcs
        AVRTools/Analog2Digital.cpp
        AVRTools/InitSystem.cpp
        AVRTools/MemUtils.cpp
        AVRTools/SystemClock.cpp
        AVRTools/Pwm.cpp
        AVRTools/RingBuffer.cpp
        AVRTools/USART0.cpp
#        AVRTools/USART1.cpp
#        AVRTools/USART2.cpp
#        AVRTools/USART3.cpp
#        AVRTools/USART0Minimal.cpp
#        AVRTools/USART1Minimal.cpp
#        AVRTools/USART2Minimal.cpp
#        AVRTools/USART3Minimal.cpp
        AVRTools/Writer.cpp
        AVRTools/Reader.cpp
        AVRTools/abi.cpp
        AVRTools/new.cpp
        AVRTools/I2cMaster.cpp
#        AVRTools/I2cSlave.cpp
        AVRTools/I2cLcd.cpp
        AVRTools/SimpleDelays.S
        AVRTools/SPI.cpp
    )


add_avr_executable( TrinketTest ${TestSrcs} ${AvrUtilsSrcs} )
