#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <pico/stdio.h>
#include <pico/time.h>

#include <cstdio>

#include "display.hpp"
#include "fonts.hpp"
#include "paint.hpp"
#include "paint_enums.hpp"

/// @brief Clear terminal output
[[gnu::always_inline]]
static inline auto clear_term() {
    std::printf("\x1B[1;1H\x1B[2J");
}

[[noreturn]] auto draw_stuff() {
    clear_term();
    using namespace pico_oled::paint;

    // NOTE(aver): Watch out, the i2c only works if the resistor is resolderet
    // pico_oled::Display<pico_oled::eConType::I2C> display;

    pico_oled::Display<pico_oled::eConType::SPI> display;
    Paint paint;
    sleep_ms(1000);

    display.clear();
    paint.create_image(
        pico_oled::k_width, pico_oled::k_height, eRotation::eROTATE_0, eImageColors::WHITE);

    {
        paint.clear_color(pico_oled::paint::eImageColors::BLACK);
        sleep_ms(1000);
        paint.draw_en_string(10,
                             10,
                             "Hello there",
                             pico_oled::font::Font24,
                             pico_oled::paint::eImageColors::BLACK,
                             pico_oled::paint::eImageColors::WHITE);

        // 3.Show image on page1
        display.show(paint.get_image());

        // display.show(gImage_1inch3_C_1);
    }
    {
        sleep_ms(1000);

        paint.clear_color(pico_oled::paint::eImageColors::BLACK);

        printf("Paint_NewImage\r\n");
        // Paint_NewImage(BlackImage, OLED_1in3_C_WIDTH, OLED_1in3_C_HEIGHT, 0, WHITE);
        paint.create_image(
            pico_oled::k_width, pico_oled::k_height, eRotation::eROTATE_0, eImageColors::WHITE);

        printf("Drawing\r\n");
        // 1.Select Image
        sleep_ms(500);
        paint.clear_color(eImageColors::BLACK);

        // 2.Drawing on the image
        printf("Drawing:page 1\r\n");
        paint.draw_point(20, 10, eImageColors::WHITE, eDotSize::DOT_PIXEL_1X1);
        paint.draw_point(30, 10, eImageColors::WHITE, eDotSize::DOT_PIXEL_2X2);
        paint.draw_point(40, 10, eImageColors::WHITE, eDotSize::DOT_PIXEL_3X3);

        paint.draw_line(10,
                        10,
                        10,
                        20,
                        eImageColors::WHITE,
                        eDotSize ::DOT_PIXEL_1X1,
                        eLineStyle::LINE_STYLE_SOLID);
        paint.draw_line(20,
                        20,
                        20,
                        30,
                        eImageColors::WHITE,
                        eDotSize ::DOT_PIXEL_1X1,
                        eLineStyle::LINE_STYLE_SOLID);
        paint.draw_line(30,
                        30,
                        30,
                        40,
                        eImageColors::WHITE,
                        eDotSize ::DOT_PIXEL_1X1,
                        eLineStyle::LINE_STYLE_SOLID);
        paint.draw_line(40,
                        40,
                        40,
                        50,
                        eImageColors::WHITE,
                        eDotSize ::DOT_PIXEL_1X1,
                        eLineStyle::LINE_STYLE_SOLID);

        paint.draw_circle(60,
                          30,
                          15,
                          eImageColors::WHITE,
                          eDotSize::DOT_PIXEL_1X1,
                          eDrawFilling::DRAW_FILL_EMPTY);
        paint.draw_circle(100,
                          40,
                          20,
                          eImageColors::WHITE,
                          eDotSize::DOT_PIXEL_1X1,
                          eDrawFilling::DRAW_FILL_FULL);

        paint.draw_rectangle(50,
                             30,
                             60,
                             40,
                             eImageColors::WHITE,
                             eDotSize::DOT_PIXEL_1X1,
                             eDrawFilling::DRAW_FILL_EMPTY);
        paint.draw_rectangle(90,
                             30,
                             110,
                             50,
                             eImageColors::BLACK,
                             eDotSize::DOT_PIXEL_1X1,
                             eDrawFilling::DRAW_FILL_FULL);

        // 3.Show image on page1
        display.show(paint.get_image());
    }
    {
        sleep_ms(2000);
        paint.clear_color(eImageColors::BLACK);
        paint.draw_en_string(
            10, 0, "Pico-OLED", pico_oled::font::Font16, eImageColors::WHITE, eImageColors::BLACK);
        paint.draw_en_string(10,
                             17,
                             "Hello World",
                             pico_oled::font::Font8,
                             eImageColors::WHITE,
                             eImageColors::BLACK);
        paint.draw_number(
            10, 30, 123.1, pico_oled::font::Font8, 2, eImageColors::WHITE, eImageColors::WHITE);
        paint.draw_number(
            10, 43, 987654.2, pico_oled::font::Font12, 2, eImageColors::WHITE, eImageColors::WHITE);
        display.show(paint.get_image());
    }

    sleep_ms(2000);
    paint.clear_color(eImageColors::BLACK);

    // display.show(gImage_1inch3_C_1);
    // sleep_ms(5000);

    paint.create_image(
        pico_oled::k_width, pico_oled::k_height, eRotation::eROTATE_180, eImageColors::WHITE);
    paint.clear_color(eImageColors::BLACK);

    constexpr int key0 = 11;
    constexpr int key1 = 12;
    int key = 0;

    gpio_set_dir(key0, GPIO_IN);
    gpio_set_dir(key1, GPIO_IN);

    while (1) {
        if (gpio_get(key1)) {
            paint.draw_rectangle(115,
                                 5,
                                 125,
                                 15,
                                 pico_oled::paint::eImageColors::WHITE,
                                 pico_oled::paint::eDotSize::DOT_PIXEL_2X2,
                                 pico_oled::paint::eDrawFilling::DRAW_FILL_FULL);
            key = 1;
        } else {
            paint.draw_rectangle(115,
                                 5,
                                 125,
                                 15,
                                 pico_oled::paint::eImageColors::BLACK,
                                 pico_oled::paint::eDotSize::DOT_PIXEL_2X2,
                                 pico_oled::paint::eDrawFilling::DRAW_FILL_EMPTY);
        }

        if (gpio_get(key0)) {
            paint.draw_rectangle(115,
                                 50,
                                 125,
                                 60,
                                 pico_oled::paint::eImageColors::WHITE,
                                 pico_oled::paint::eDotSize::DOT_PIXEL_2X2,
                                 pico_oled::paint::eDrawFilling::DRAW_FILL_FULL);
        } else {
            paint.draw_rectangle(115,
                                 50,
                                 125,
                                 60,
                                 pico_oled::paint::eImageColors::BLACK,
                                 pico_oled::paint::eDotSize::DOT_PIXEL_2X2,
                                 pico_oled::paint::eDrawFilling::DRAW_FILL_EMPTY);
            key = 1;
        }

        if (key) {
            display.show(paint.get_image());
            paint.clear_color(pico_oled::paint::eImageColors::BLACK);
        }
    }
}

auto init_hw() {
    sleep_ms(1000);
    stdio_init_all();

    // SPI Config
    spi_init(SPI_PORT, 1000 * 1000);
    gpio_set_function(EPD_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(EPD_MOSI_PIN, GPIO_FUNC_SPI);

    /// General GPIO config
    gpio_init(EPD_RST_PIN);
    gpio_set_dir(EPD_RST_PIN, GPIO_OUT);
    gpio_init(EPD_DC_PIN);
    gpio_set_dir(EPD_DC_PIN, GPIO_OUT);
    gpio_init(EPD_CS_PIN);
    gpio_set_dir(EPD_CS_PIN, GPIO_OUT);
    // gpio_init(EPD_BL_PIN);
    // gpio_set_dir(EPD_BL_PIN, GPIO_OUT);

    gpio_set_dir(EPD_CS_PIN, GPIO_OUT);
    // gpio_set_dir(EPD_BL_PIN, GPIO_OUT);

    gpio_put(EPD_CS_PIN, 1);
    gpio_put(EPD_DC_PIN, 0);
    // gpio_put(EPD_BL_PIN, 1);

#ifdef I2C_SOLDERED
    // I2C Config
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(EPD_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(EPD_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(EPD_SDA_PIN);
    gpio_pull_up(EPD_SCL_PIN);
#endif

    printf("Successfull init\n");
    return 0;
}

auto scanI2C() {
    sleep_ms(250);
    auto reserved_addr = [](u8 addr) { return (addr & 0x78) == 0 || (addr & 0x78) == 0x78; };
    printf("\nI2C Bus Scan\n");
    printf(" 0 1 2 3 4 5 6 7 8 9 A B C D E F\n");

    for (u8 addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }

        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.

        // Skip over any reserved addresses.
        int ret;
        uint8_t rxdata;
        if (reserved_addr(addr)) {
            ret = PICO_ERROR_GENERIC;
        } else {
            ret = i2c_read_blocking(I2C_PORT, addr, &rxdata, 1, false);
        }
        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : " ");
    }
    printf("Done.\n");
    return 0;
}

[[noreturn]] auto main() -> int {
    init_hw();
    // scanI2C();
    draw_stuff();
}
