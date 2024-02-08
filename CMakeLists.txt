cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

include(cmake/pico_sdk_import.cmake)
project(pico-oled
    C
    CXX
    ASM
)

set(PICO_TOOLCHAIN_PATH /opt/gcc-arm-none-eabi/bin)
set(PICO_BOARD pico_w)
# set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

pico_sdk_init()

include_directories(includes/pico-oled)

add_library(pico-oled-main
    src/OLED_1in3_c.cpp
)

target_link_libraries(pico-oled-main PUBLIC
    pico_stdlib
    hardware_spi
    hardware_i2c
    hardware_pwm
    hardware_adc
)

add_library(pico-oled-gui
    src/GUI_Paint.cpp
)

target_link_libraries(pico-oled-gui PUBLIC
    pico_stdlib
    hardware_spi
    hardware_i2c
    hardware_pwm
    hardware_adc
)

add_library(pico-oled-fonts
    src/font12.cpp
    src/font16.cpp
    src/font20.cpp
    src/font24.cpp
    src/font8.cpp
)

add_library(pico-oled-config
    src/DEV_Config.cpp
)

target_link_libraries(pico-oled-config PUBLIC
    pico_stdlib
    hardware_spi
    hardware_i2c
    hardware_pwm
    hardware_adc
)
