cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(PN532DriverProto C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(test
    test.c
)

pico_add_extra_outputs(PN532DriverProto.c)

target_link_libraries(test pico_stdlib)
