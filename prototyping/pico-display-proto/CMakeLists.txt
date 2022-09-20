cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(pico-display-troyera C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(display-troyera
    display-troyera.c
)
pico_enable_stdio_usb(display-troyera 1)
pico_enable_stdio_uart(display-troyera 0)

pico_add_extra_outputs(display-troyera)

target_link_libraries(display-troyera
    pico_stdlib
    hardware_spi
)
