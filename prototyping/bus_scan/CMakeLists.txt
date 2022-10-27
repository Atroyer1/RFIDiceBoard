cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(i2c_bus_scan C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(i2c_bus_scan
        bus_scan.c
        )

pico_add_extra_outputs(i2c_bus_scan)

# pull in common dependencies and additional i2c hardware support
target_link_libraries(i2c_bus_scan pico_stdlib hardware_i2c)
