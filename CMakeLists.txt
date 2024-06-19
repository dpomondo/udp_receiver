# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/dpoyesac/Projects/pico/pico-sdk")

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(udp_receiver C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(udp_receiver "")
add_subdirectory(lcd1602)
add_subdirectory(src)

pico_set_program_name(udp_receiver "udp_receiver")
pico_set_program_version(udp_receiver "0.1")

pico_enable_stdio_uart(udp_receiver 1)
pico_enable_stdio_usb(udp_receiver 0)

# Add the standard library to the build
target_link_libraries(udp_receiver
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(udp_receiver PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(udp_receiver 
        hardware_i2c
        hardware_pio
        hardware_watchdog
        hardware_clocks
        lcd1602
        pico_cyw43_arch_lwip_threadsafe_background
        )

pico_add_extra_outputs(udp_receiver)

