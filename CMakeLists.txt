cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

# include(cmake/pico_sdk_import.cmake)
include(../ael-cpp/external/pico-sdk/pico_sdk_init.cmake)
project(pico-oled C CXX ASM)

set(PICO_TOOLCHAIN_PATH /opt/gcc-arm-none-eabi/bin)
set(PICO_BOARD pico_w)
# set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

pico_sdk_init()

include_directories(includes/pico-oled)

add_compile_options(
    # -Werror
    -Wall
    -Wextra
    -Wconversion
    -Wpedantic
    # note covered by above
    -Wshadow
    -Wundef
    -Wdouble-promotion
    -Os
    -mtune=cortex-m0plus
    -fno-common
    -fstack-usage
    -ffunction-sections
    -fdata-sections
)

add_link_options(
    -Wl,-gc-sections
    -Wl,-print-memory-usage
)


add_library(pico-oled-fonts STATIC
    src/font12.cpp
    src/font16.cpp
    src/font20.cpp
    src/font24.cpp
    src/font8.cpp
)

target_include_directories(pico-oled-fonts PUBLIC
    includes
)

add_library(pico-oled-paint STATIC
    src/paint.cpp
)

target_link_libraries(pico-oled-paint PUBLIC
    pico-oled-fonts

    pico_stdlib
    hardware_spi
    hardware_i2c
    # hardware_adc
)

target_include_directories(pico-oled-paint PUBLIC
    includes
)

add_executable(test
    examples/test.cpp
)

target_link_libraries(test PUBLIC
    pico-oled-fonts
    pico-oled-paint

    pico_stdlib
    hardware_spi
    hardware_i2c
    # hardware_adc
)


# enable usb output, disable uart output
pico_enable_stdio_usb(test 1)
pico_enable_stdio_uart(test 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(test)
