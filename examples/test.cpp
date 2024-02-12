#include <hardware/gpio.h>
#include <pico/stdio.h>
#include <pico/time.h>

#include <array>
#include <cstdio>
#include <memory>

#include "ImageData.hpp"
#include "display.hpp"
#include "fonts.hpp"
#include "paint.hpp"
#include "paint_enums.hpp"

[[noreturn]] auto draw_stuff() {
    using namespace pico_oled::paint;

    pico_oled::Display<pico_oled::eConType::SPI> display;
    Paint paint;

    u8 imgbuf[pico_oled::k_imsize];
    // u8 *imgbuf = new u8[pico_oled::k_imsize];
    // std::array<u8, display.k_imsize> img;
    // auto img = std::make_shared<std::array<u8, pico_oled::k_imsize>>();

    // imgbuf[0] = 0xf0u;

    display.clear();

    paint.create_image(
        imgbuf, pico_oled::k_width, pico_oled::k_height, eRotation::eROTATE_0, eImageColors::WHITE);
    paint.select_image(imgbuf);

    sleep_ms(500);
    paint.clear_color(pico_oled::paint::eImageColors::BLACK);
    paint.draw_en_string(10,
                         0,
                         "Hello there",
                         pico_oled::font::Font24,
                         pico_oled::paint::eImageColors::WHITE,
                         pico_oled::paint::eImageColors::BLACK);

    // 3.Show image on page1
    display.show(imgbuf);
    // display.show(gImage_1inch3_C_1.data());

    sleep_ms(1000);

    paint.clear_color(pico_oled::paint::eImageColors::BLACK);

    printf("Paint_NewImage\r\n");
    // Paint_NewImage(BlackImage, OLED_1in3_C_WIDTH, OLED_1in3_C_HEIGHT, 0, WHITE);
    paint.create_image(
        imgbuf, pico_oled::k_width, pico_oled::k_height, eRotation::eROTATE_0, eImageColors::WHITE);

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

    paint.draw_circle(
        60, 30, 15, eImageColors::WHITE, eDotSize::DOT_PIXEL_1X1, eDrawFilling::DRAW_FILL_EMPTY);
    paint.draw_circle(
        100, 40, 20, eImageColors::WHITE, eDotSize::DOT_PIXEL_1X1, eDrawFilling::DRAW_FILL_FULL);

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

    imgbuf[0] = 0xf0;

    // 3.Show image on page1
    display.show(imgbuf);

    sleep_ms(2000);
    paint.clear_color(eImageColors::BLACK);
    paint.draw_en_string(
        10, 0, "Pico-OLED", pico_oled::font::Font16, eImageColors::WHITE, eImageColors::BLACK);
    paint.draw_en_string(
        10, 17, "Hello World", pico_oled::font::Font8, eImageColors::WHITE, eImageColors::BLACK);
    paint.draw_number(
        10, 30, 123.1, pico_oled::font::Font8, 2, eImageColors::WHITE, eImageColors::WHITE);
    paint.draw_number(
        10, 43, 987654.2, pico_oled::font::Font12, 2, eImageColors::WHITE, eImageColors::WHITE);
    display.show(imgbuf);
    sleep_ms(2000);
    paint.clear_color(eImageColors::BLACK);

    // display.show(gImage_1inch3_C_1);
    // sleep_ms(5000);

    paint.create_image(imgbuf,
                       pico_oled::k_width,
                       pico_oled::k_height,
                       eRotation::eROTATE_180,
                       eImageColors::WHITE);
    paint.clear_color(eImageColors::BLACK);

    constexpr int key0 = 15;
    constexpr int key1 = 17;
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
            display.show(imgbuf);
            paint.clear_color(pico_oled::paint::eImageColors::BLACK);
        }
    }
}

auto init_hw() {
    sleep_ms(1000);
    stdio_init_all();

    // SPI Config
    spi_init(SPI_PORT, 10000 * 1000);
    gpio_set_function(EPD_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(EPD_MOSI_PIN, GPIO_FUNC_SPI);

    /// General GPIO config
    gpio_init(EPD_RST_PIN);
    gpio_set_dir(EPD_RST_PIN, GPIO_OUT);
    gpio_init(EPD_DC_PIN);
    gpio_set_dir(EPD_DC_PIN, GPIO_OUT);
    gpio_init(EPD_CS_PIN);
    gpio_set_dir(EPD_CS_PIN, GPIO_OUT);
    gpio_init(EPD_BL_PIN);
    gpio_set_dir(EPD_BL_PIN, GPIO_OUT);

    gpio_set_dir(EPD_CS_PIN, GPIO_OUT);
    gpio_set_dir(EPD_BL_PIN, GPIO_OUT);

    gpio_put(EPD_CS_PIN, 1);
    gpio_put(EPD_DC_PIN, 0);
    gpio_put(EPD_BL_PIN, 1);

#if 1
    // I2C Config
    i2c_init(i2c1, 300 * 1000);
    gpio_set_function(EPD_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(EPD_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(EPD_SDA_PIN);
    gpio_pull_up(EPD_SCL_PIN);
#endif  // 0

    printf("Successfull init\n");
}
[[noreturn]] auto main() -> int {
    init_hw();
    draw_stuff();
}
