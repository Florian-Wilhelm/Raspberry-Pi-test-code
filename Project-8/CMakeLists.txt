cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD pico_w)
include(pico_sdk_import.cmake)
project(TSL25911-ambient-light-measurement C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(TSL25911-ambient-light-measurement
    TSL25911-ambient-light-measurement.c
    )
    
target_include_directories(TSL25911-ambient-light-measurement PRIVATE
${CMAKE_CURRENT_LIST_DIR}
)

# pull in common dependencies
target_link_libraries(TSL25911-ambient-light-measurement pico_stdlib hardware_i2c)

# create map/bin/hex file etc.
pico_add_extra_outputs(TSL25911-ambient-light-measurement)
