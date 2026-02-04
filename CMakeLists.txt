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
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(MidiKeyboard C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

# Add executable. Default name is the project name, version 0.1

add_executable(MidiKeyboard 
        src/main.cpp
        src/mux.c
        src/adc_scanner.c
        src/signal_processing.c
        src/calibration.c
        src/fsm.c
        src/usb_descriptors.c
        src/midi_handler.c
        )

pico_set_program_name(MidiKeyboard "MidiKeyboard")
pico_set_program_version(MidiKeyboard "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(MidiKeyboard 1)
pico_enable_stdio_usb(MidiKeyboard 0)

# Add the standard library to the build
target_link_libraries(MidiKeyboard
        pico_stdlib
        tinyusb_device
        )

# Add the standard include files to the build
target_include_directories(MidiKeyboard PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(MidiKeyboard 
        hardware_spi
        hardware_i2c
        hardware_pio
        hardware_adc
        )

pico_add_extra_outputs(MidiKeyboard)

