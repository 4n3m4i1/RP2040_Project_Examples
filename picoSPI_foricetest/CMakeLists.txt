cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(spitester C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

pico_sdk_init()

add_executable(spitester
        main.c
)

target_link_libraries(spitester 
        pico_stdlib
        hardware_spi
)

pico_add_extra_outputs(spitester)

# For USB printf, disable UART
pico_enable_stdio_usb(${PROJECT_NAME} 1)