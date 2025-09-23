add_executable(i2c_bus_scan
        i2cScan.c
        )

# pull in common dependencies and additional i2c hardware support
target_link_libraries(i2c_bus_scan pico_stdlib hardware_i2c hardware_clocks )

# create map/bin/hex file etc.
pico_add_extra_outputs(i2c_bus_scan)

