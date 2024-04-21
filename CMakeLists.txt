# Copyright (c) 2024 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19.0)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os CACHE INTERNAL "")
set(MBED_CONFIG_PATH ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "")
set(APP_TARGET XJEL2645NewVersion)

set(MBED_PRINTF_LIB "std")

add_compile_definitions(ECS_NO_RTTI)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "No optimization, debug info included")
    add_compile_options(-O0 -g)
endif ()

include(${MBED_PATH}/tools/cmake/app.cmake)

project(${APP_TARGET})

add_subdirectory(${MBED_PATH})

include_directories(${gaia_INCLUDE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/CMSIS_Math/Include
        ${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/Joystick
        ${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/MatrixLibrary
        ${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/Joystick
        ${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/N5110
        ${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/pico_headers
        ${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/Logger
        ${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty/ECS
)

file(GLOB_RECURSE My_SOURCES "Core/*.c" "Core/*.cpp")
file(GLOB_RECURSE Thirdparty_SOURCES
        "Thirdparty/CMSIS_Math/*.c"
        "Thirdparty/CMSIS_Math/*.cpp"
        "Thirdparty/Joystick/*.c"
        "Thirdparty/Joystick/*.cpp"
        "Thirdparty/MatrixLibrary/*.c"
        "Thirdparty/MatrixLibrary/*.cpp"
        "Thirdparty/N5110/*.cpp"
        "Thirdparty/N5110/*.c"
        "Thirdparty/Logger/*.c"
        "Thirdparty/Logger/*.cpp"
        "Thirdparty/ECS/*.c"
        "Thirdparty/ECS/*.cpp"
)

foreach(filepath ${My_SOURCES})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND My_SOURCES_re ${filename})
endforeach()

foreach(filepath ${Thirdparty_SOURCES})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND Thirdparty_SOURCES_re ${filename})
endforeach()

add_executable(${APP_TARGET}
    ${My_SOURCES_re}
    ${Thirdparty_SOURCES_re}
)

target_link_libraries(${APP_TARGET}
    mbed-os
)

mbed_set_post_build(${APP_TARGET})

option(VERBOSE_BUILD "Have a verbose build process")
if(VERBOSE_BUILD)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()