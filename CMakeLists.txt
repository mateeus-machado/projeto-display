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

project(projeto-display C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(projeto-display
        projeto-display.c
        src/matriz_led.c
        src/led_rgb.c
        src/botoes.c
        src/animacao.c
        src/ssd1306.c
)

pico_set_program_name(projeto-display "projeto-display")
pico_set_program_version(projeto-display "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(projeto-display 1)
pico_enable_stdio_usb(projeto-display 1)

pico_generate_pio_header(projeto-display ${CMAKE_CURRENT_LIST_DIR}/projeto_display.pio)

# Add the standard library to the build
target_link_libraries(projeto-display
        pico_stdlib
        hardware_pio
        hardware_adc
        pico_bootrom
        hardware_spi
        hardware_i2c
        )

# Add the standard include files to the build
target_include_directories(projeto-display PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(projeto-display)

