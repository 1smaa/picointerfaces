# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C   standards
project(wifitest C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(main 
    main.cpp
)

add_library(${PROJECT_NAME} STATIC
    esp01s.cpp
)

# Create map/bin/hex/uf2 files for the executable
pico_add_extra_outputs(main)

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(main
    pico_stdlib
    ${PROJECT_NAME}
    hardware_uart
)

# Link to pico_stdlib for the library
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_uart
)

# Enable usb output, disable uart output
pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)
