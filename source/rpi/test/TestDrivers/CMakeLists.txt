# Basic tests of all the various drivers


add_executable( LcdOff LcdOff.cpp )
target_compile_options( LcdOff PRIVATE -Wall -pthread )
target_compile_definitions( LcdOff PRIVATE 
    BUILDING_FOR_RPI0=$<BOOL:${BUILDING_FOR_RPI0}>        
    USE_CARRTRPI0_STDIO=$<BOOL:${CARRT_RPI0_ENABLE_STDIO_OUTPUT}>
    USE_PIGPIOD=$<BOOL:${CARRT_RPI_USE_PIGPIOD}>
    DEBUGUTILS_ON=$<BOOL:${CARRT_ENABLE_DEBUGUTILS}> 
    DEBUGRPI0=$<CONFIG:DEBUG> 
)
target_link_libraries( LcdOff PRIVATE 
    rpi_driver_library 
    shared_library 
    $<IF:$<BOOL:${CARRT_RPI_USE_PIGPIOD}>,pigpiod_if2,pigpio> 
    pthread 
    rt 
)

    
add_executable( LcdTest LcdTest.cpp  )
target_compile_options( LcdTest PRIVATE -Wall -pthread )
target_compile_definitions( LcdTest PRIVATE 
    BUILDING_FOR_RPI0=$<BOOL:${BUILDING_FOR_RPI0}>        
    USE_CARRTRPI0_STDIO=$<BOOL:${CARRT_RPI0_ENABLE_STDIO_OUTPUT}>
    USE_PIGPIOD=$<BOOL:${CARRT_RPI_USE_PIGPIOD}>
    DEBUGUTILS_ON=$<BOOL:${CARRT_ENABLE_DEBUGUTILS}> 
    DEBUGRPI0=$<CONFIG:DEBUG> 
)
target_link_libraries( LcdTest PRIVATE 
    rpi_driver_library 
    shared_library 
    $<IF:$<BOOL:${CARRT_RPI_USE_PIGPIOD}>,pigpiod_if2,pigpio> 
    pthread 
    rt 
)


add_executable( KeypadTest KeypadTest.cpp )
target_compile_options( KeypadTest PRIVATE -Wall -pthread )
target_compile_definitions( KeypadTest PRIVATE 
    BUILDING_FOR_RPI0=$<BOOL:${BUILDING_FOR_RPI0}>        
    USE_CARRTRPI0_STDIO=$<BOOL:${CARRT_RPI0_ENABLE_STDIO_OUTPUT}>
    USE_PIGPIOD=$<BOOL:${CARRT_RPI_USE_PIGPIOD}>
    DEBUGUTILS_ON=$<BOOL:${CARRT_ENABLE_DEBUGUTILS}> 
    DEBUGRPI0=$<CONFIG:DEBUG> 
)
target_link_libraries( KeypadTest PRIVATE 
    rpi_driver_library 
    shared_library 
    $<IF:$<BOOL:${CARRT_RPI_USE_PIGPIOD}>,pigpiod_if2,pigpio> 
    pthread 
    rt 
)


add_executable( MotorTest MotorTest.cpp )
target_compile_options( MotorTest PRIVATE -Wall -pthread )
target_compile_definitions( MotorTest PRIVATE 
    BUILDING_FOR_RPI0=$<BOOL:${BUILDING_FOR_RPI0}>        
    USE_CARRTRPI0_STDIO=$<BOOL:${CARRT_RPI0_ENABLE_STDIO_OUTPUT}>
    USE_PIGPIOD=$<BOOL:${CARRT_RPI_USE_PIGPIOD}>
    DEBUGUTILS_ON=$<BOOL:${CARRT_ENABLE_DEBUGUTILS}> 
    DEBUGRPI0=$<CONFIG:DEBUG> 
)
target_link_libraries( MotorTest PRIVATE 
    rpi_driver_library 
    shared_library 
    $<IF:$<BOOL:${CARRT_RPI_USE_PIGPIOD}>,pigpiod_if2,pigpio> 
    pthread 
    rt 
)


add_executable( DriveTest DriveTest.cpp )
target_compile_options( DriveTest PRIVATE -Wall -pthread )
target_compile_definitions( DriveTest PRIVATE 
    BUILDING_FOR_RPI0=$<BOOL:${BUILDING_FOR_RPI0}>        
    USE_CARRTRPI0_STDIO=$<BOOL:${CARRT_RPI0_ENABLE_STDIO_OUTPUT}>
    USE_PIGPIOD=$<BOOL:${CARRT_RPI_USE_PIGPIOD}>
    DEBUGUTILS_ON=$<BOOL:${CARRT_ENABLE_DEBUGUTILS}> 
    DEBUGRPI0=$<CONFIG:DEBUG> 
)
target_link_libraries( DriveTest PRIVATE  
    rpi_driver_library 
    shared_library 
    $<IF:$<BOOL:${CARRT_RPI_USE_PIGPIOD}>,pigpiod_if2,pigpio> 
    pthread 
    rt 
)


add_executable( BeepTest BeepTest.cpp )
target_compile_options( BeepTest PRIVATE -Wall -pthread )
target_compile_definitions( BeepTest PRIVATE 
    BUILDING_FOR_RPI0=$<BOOL:${BUILDING_FOR_RPI0}>        
    USE_CARRTRPI0_STDIO=$<BOOL:${CARRT_RPI0_ENABLE_STDIO_OUTPUT}>
    USE_PIGPIOD=$<BOOL:${CARRT_RPI_USE_PIGPIOD}>
    DEBUGUTILS_ON=$<BOOL:${CARRT_ENABLE_DEBUGUTILS}> 
    DEBUGRPI0=$<CONFIG:DEBUG> 
)
target_link_libraries( BeepTest PRIVATE  
    rpi_driver_library 
    shared_library 
    $<IF:$<BOOL:${CARRT_RPI_USE_PIGPIOD}>,pigpiod_if2,pigpio> 
    pthread 
    rt 
)


add_executable( ServoTest ServoTest.cpp )
target_compile_options( ServoTest PRIVATE -Wall -pthread )
target_compile_definitions( ServoTest PRIVATE 
    BUILDING_FOR_RPI0=$<BOOL:${BUILDING_FOR_RPI0}>        
    USE_CARRTRPI0_STDIO=$<BOOL:${CARRT_RPI0_ENABLE_STDIO_OUTPUT}>
    USE_PIGPIOD=$<BOOL:${CARRT_RPI_USE_PIGPIOD}>
    DEBUGUTILS_ON=$<BOOL:${CARRT_ENABLE_DEBUGUTILS}> 
    DEBUGRPI0=$<CONFIG:DEBUG> 
)
target_link_libraries( ServoTest PRIVATE 
    rpi_driver_library 
    shared_library 
    $<IF:$<BOOL:${CARRT_RPI_USE_PIGPIOD}>,pigpiod_if2,pigpio> 
    pthread 
    rt 
)


add_executable( ServoTestPulse ServoTestPulse.cpp )
target_compile_options( ServoTestPulse PRIVATE -Wall -pthread )
target_compile_definitions( ServoTestPulse PRIVATE 
    BUILDING_FOR_RPI0=$<BOOL:${BUILDING_FOR_RPI0}>        
    USE_CARRTRPI0_STDIO=$<BOOL:${CARRT_RPI0_ENABLE_STDIO_OUTPUT}>
    USE_PIGPIOD=$<BOOL:${CARRT_RPI_USE_PIGPIOD}>
    DEBUGUTILS_ON=$<BOOL:${CARRT_ENABLE_DEBUGUTILS}> 
    DEBUGRPI0=$<CONFIG:DEBUG> 
)
target_link_libraries( ServoTestPulse PRIVATE 
    rpi_driver_library 
    shared_library 
    $<IF:$<BOOL:${CARRT_RPI_USE_PIGPIOD}>,pigpiod_if2,pigpio> 
    pthread 
    rt 
)


add_executable( LidarTest LidarTest.cpp )
target_compile_options( LidarTest PRIVATE -Wall -pthread )
target_compile_definitions( LidarTest PRIVATE 
    BUILDING_FOR_RPI0=$<BOOL:${BUILDING_FOR_RPI0}>        
    USE_CARRTRPI0_STDIO=$<BOOL:${CARRT_RPI0_ENABLE_STDIO_OUTPUT}>
    USE_PIGPIOD=$<BOOL:${CARRT_RPI_USE_PIGPIOD}>
    DEBUGUTILS_ON=$<BOOL:${CARRT_ENABLE_DEBUGUTILS}> 
    DEBUGRPI0=$<CONFIG:DEBUG> 
)
target_link_libraries( LidarTest PRIVATE  
    rpi_driver_library 
    shared_library 
    $<IF:$<BOOL:${CARRT_RPI_USE_PIGPIOD}>,pigpiod_if2,pigpio> 
    pthread 
    rt 
)
