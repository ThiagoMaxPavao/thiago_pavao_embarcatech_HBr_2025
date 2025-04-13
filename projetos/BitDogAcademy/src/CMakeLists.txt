# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(BitDogAcademy C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(BitDogAcademy
        BitDogAcademy.c
        inc/ssd1306.c
        inc/neopixel.c
        inc/buzzer.c
        inc/joystick.c
        inc/buttons.c
        inc/tests.c
        inc/util.c
        activities/math_integer_division.c
        activities/state_machine.c
        fonts/acme_5_outlines_font.c
        fonts/BMSPA_font.c
        fonts/bubblesstandard_font.c
        fonts/crackers_font.c)

pico_set_program_name(BitDogAcademy "BitDogAcademy")
pico_set_program_version(BitDogAcademy "0.1")

# Generate PIO header
pico_generate_pio_header(BitDogAcademy ${CMAKE_CURRENT_LIST_DIR}/ws2818b.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(BitDogAcademy 0)
pico_enable_stdio_usb(BitDogAcademy 1)

# Add the standard library to the build
target_link_libraries(BitDogAcademy
        pico_stdlib
        hardware_i2c
        hardware_pio
        hardware_clocks
        hardware_pwm
        hardware_adc
        )

# Add the standard include files to the build
target_include_directories(BitDogAcademy PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/inc # Include directory
        ${CMAKE_CURRENT_LIST_DIR}/activities # Activities directory
        ${CMAKE_CURRENT_LIST_DIR}/fonts # Fonts directory
)

# Add any user requested libraries
target_link_libraries(BitDogAcademy 
        )

pico_add_extra_outputs(BitDogAcademy)

