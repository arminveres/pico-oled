/*****************************************************************************
* | File        :   OLED_1in3_c.h
* | Author      :
* | Function    :   1.3inch OLEDDrive function
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2021-03-16
* | Info        :
* -----------------------------------------------------------------------------
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
******************************************************************************/
#ifndef __PICO_OLED_1IN3_HPP
#define __PICO_OLED_1IN3_HPP

#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <hardware/spi.h>
#include <pico/time.h>

#include <array>
#include <cstdio>

#include "types.hpp"

volatile static const auto SPI_PORT = spi1;

static constexpr auto EPD_DC_PIN = 8;
static constexpr auto EPD_CS_PIN = 9;
static constexpr auto EPD_CLK_PIN = 10;
static constexpr auto EPD_MOSI_PIN = 11;
static constexpr auto EPD_RST_PIN = 12;
static constexpr auto EPD_BL_PIN = 13;

static constexpr auto IIC_CMD = 0x00;
static constexpr auto IIC_RAM = 0x40;
static constexpr auto IIC_ADDR = 0x3D;
/// I2C port, use default
volatile static auto I2C_PORT = i2c0;
/// I2C data
static constexpr auto EPD_SDA_PIN = 4;
/// I2C Clock
static constexpr auto EPD_SCL_PIN = 5;

namespace pico_oled {

static constexpr u8 k_width = 128;
static constexpr u8 k_height = 64;
static constexpr u32 k_imsize = k_width * k_height / 8;

using ImBuf = std::array<u8, k_imsize>;

enum class eConType { I2C, SPI };

template <eConType T>
struct Display {
    struct Regs {
        static constexpr auto DISABLE_DISP_ON = 0xA4;
        static constexpr auto DISABLE_DISP_OFF = 0xA5;
        static constexpr auto TURN_DISP_OFF = 0xAE;
        static constexpr auto TURN_DISP_ON = 0xAF;
        static constexpr auto DISP_COL_NORMAL = 0xa6;
        static constexpr auto DISP_COL_REV = 0xa7;
        // 0x00-0x0F
        static constexpr auto SET_LOW_COL_ADR = 0x00;
        // 0x10-0x17
        static constexpr auto SET_HIGH_COL_ADR = 0x10;
        // Memory addressing mode
        static constexpr auto SET_MEM_HORI_ADDRING = 0x20;
        static constexpr auto SET_MEM_VERT_ADDRING = 0x21;
        /// Contrast Control MODE; [0x00, 0xFF]
        static constexpr auto CONTRAST_CTRL = 0x81;
        // Set segment remap
        static constexpr auto SET_SEGMENT_REMAP_NORM = 0xA0;
        static constexpr auto SET_SEGMENT_REMAP_REV = 0xA1;
        /// Set Multiplex Ration; followed by value [0x00, 0x7F]
        static constexpr auto SET_MULTIPLEX_RATIO = 0xA8;
        /// Set Display offset; \[0x00, 0x7F\]
        static constexpr auto SET_DISP_OFFSET = 0xD3;
        // DC-DC voltage Converter
        static constexpr auto SET_DCDC_ON = 0xAD;
        static constexpr auto SET_DCDC_PUMP_ON = 0x8A;
        static constexpr auto SET_DCDC_PUMP_OFF = 0x8B;
        /// Specify page address to load display RAM to page address reg; [0xB0, 0xBF]
        // SET_PAGE_ADR = 0xB0-0xbf;
        // Set Common output scan direction

        // SET_OSC_DIR = 0xC0 - 0xC8
        /// Set Display Clock Divide Ratio/Oscillator Frequency: (Double Bytes
        /// Command) [0x00; 0xFF]
        static constexpr auto SET_OSC_CLOCK_DIV = 0xD5;
        /// Set Dis-charge/Pre-charge Period: (Double Bytes Command) [0x00; 0xFF]
        static constexpr auto SET_PRECHARGE_PER = 0xD9;
        // Set VCOM Deselect Level: (Double Bytes Command)
        static constexpr auto SET_VCOM_DESEL = 0xDB;
        // Set Display Start Line:（Double Bytes Command）
        static constexpr auto SET_START_LINE = 0xDC;
        // Read-Modify-Write
        static constexpr auto SET_READ_MOD_WRITE = 0xE0;
        // Read-Modify-Write
        static constexpr auto END_READ_MOD_WRITE = 0xEE;
        static constexpr auto NOP = 0xE3;
    };

    auto show(const ImBuf &imbuf) const {
        constexpr auto l_width = k_width / 8;
        // write starting row
        write_to_reg(0xB0u);

        for (u8 j = 0; j < k_height; ++j) {
            const u8 column = k_height - 1 - j;
            write_to_reg(Regs::SET_LOW_COL_ADR + (column & 0x0Fu));
            write_to_reg(Regs::SET_HIGH_COL_ADR + (column >> 0x04u));

            for (u8 i = 0; i < l_width; ++i) {
                auto temp = imbuf[i + j * l_width];
                temp = reverse_byte(temp);
                write_data(temp);
            }
        }
    }

    auto clear() const {
        constexpr auto l_width = k_width / 8;
        // write starting row
        write_to_reg(0xB0u);
        for (u8 j = 0; j < k_height; ++j) {
            const u8 column = k_height - 1 - j;
            write_to_reg(Regs::SET_LOW_COL_ADR + (column & 0x0Fu));
            write_to_reg(Regs::SET_HIGH_COL_ADR + (column >> 0x4u));
            for (u16 i = 0; i < l_width; ++i) {
                write_data(0x00u);
            }
        }
    }

    auto reset() const {
        gpio_put(EPD_RST_PIN, 1);
        sleep_ms(100);
        gpio_put(EPD_RST_PIN, 0);
        sleep_ms(100);
        gpio_put(EPD_RST_PIN, 1);
        sleep_ms(100);
    }

    static auto reverse_byte(u8 byte) {
        byte = static_cast<u8>((byte & 0x55) << 1) | ((byte & 0xaa) >> 1);
        byte = static_cast<u8>((byte & 0x33) << 2) | ((byte & 0xcc) >> 2);
        byte = static_cast<u8>((byte & 0x0f) << 4) | ((byte & 0xf0) >> 4);
        return byte;
    }

    Display() {
        reset();
        init_regs();
        sleep_ms(200);
        write_to_reg(Regs::TURN_DISP_ON);
    };

    ~Display() = default;
    Display(Display &&) = delete;
    Display(const Display &) = delete;
    Display &operator=(Display &&) = delete;
    Display &operator=(const Display &) = delete;

   private:
    /// Wirte to register, can be subseded by a (double) byte command
    auto write_to_reg(const u8 reg) const {
        if constexpr (T == eConType::SPI) {
            gpio_put(EPD_DC_PIN, 0);
            gpio_put(EPD_CS_PIN, 0);
            spi_write_blocking(SPI_PORT, &reg, 1);
            gpio_put(EPD_CS_PIN, 1);

        } else if constexpr (T == eConType::I2C) {
            const u8 data[2] = {IIC_CMD, reg};
            i2c_write_blocking(I2C_PORT, IIC_ADDR, data, 3, false);
        }
    }

    /// Writes data to device, special command for i2c
    auto write_data(const u8 reg) const {
        if constexpr (T == eConType::SPI) {
            gpio_put(EPD_DC_PIN, 1);
            gpio_put(EPD_CS_PIN, 0);
            spi_write_blocking(SPI_PORT, &reg, 1);
            gpio_put(EPD_CS_PIN, 1);

        } else if constexpr (T == eConType::I2C) {
            const u8 data[2] = {IIC_RAM, reg};
            i2c_write_blocking(I2C_PORT, IIC_ADDR, data, 3, false);
        }
    }

    auto init_regs() const {
        write_to_reg(Regs::TURN_DISP_OFF);

        write_to_reg(Regs::SET_LOW_COL_ADR);
        write_to_reg(Regs::SET_HIGH_COL_ADR);

        write_to_reg(0xB0);  // set page address

        write_to_reg(Regs::SET_START_LINE);
        write_to_reg(0x00);  // set to beginning

        write_to_reg(Regs::CONTRAST_CTRL);
        write_to_reg(0x6f);
        write_to_reg(Regs::SET_MEM_VERT_ADDRING);

        write_to_reg(Regs::SET_SEGMENT_REMAP_NORM);
        write_to_reg(0xC0);  // set com scan direction
        write_to_reg(Regs::DISABLE_DISP_ON);

        write_to_reg(Regs::DISP_COL_NORMAL);
        write_to_reg(Regs::SET_MULTIPLEX_RATIO);
        write_to_reg(0x3F);  // 1/64 duty

        write_to_reg(Regs::SET_DISP_OFFSET);
        write_to_reg(0x60);

        write_to_reg(Regs::SET_OSC_CLOCK_DIV);
        write_to_reg(0x41);

        write_to_reg(Regs::SET_PRECHARGE_PER);
        write_to_reg(0x22);

        write_to_reg(Regs::SET_VCOM_DESEL);
        write_to_reg(0x35);

        write_to_reg(Regs::SET_DCDC_ON);
        write_to_reg(Regs::SET_DCDC_PUMP_ON);

#if 0
        write_reg(0xAEu); /*turn off OLED display*/

        write_reg(0x00u); /*set lower column address*/
        write_reg(0x10u); /*set higher column address*/

        write_reg(0xB0u); /*set page address*/

        write_reg(0xDCu); /*set display start line*/
        write_reg(0x00u);

        write_reg(0x81u); /*contract control*/
        write_reg(0x6Fu); /*128*/
        write_reg(0x21u); /* Set Memory addressing mode (0x20/0x21) */

        write_reg(0xA0u); /*set segment remap*/
        write_reg(0xC0u); /*Com scan direction*/
        write_reg(0xA4u); /*Disable Entire Display On (0xA4/0xA5)*/

        write_reg(0xA6u); /*normal / reverse*/
        write_reg(0xA8u); /*multiplex ratio*/
        write_reg(0x3Fu); /*duty = 1/64*/

        write_reg(0xD3u); /*set display offset*/
        write_reg(0x60u);

        write_reg(0xD5u); /*set osc division*/
        write_reg(0x41u);

        write_reg(0xD9u); /*set pre-charge period*/
        write_reg(0x22u);

        write_reg(0xDBu); /*set vcomh*/
        write_reg(0x35u);

        write_reg(0xADu); /*set charge pump enable*/
        write_reg(0x8Au); /*Set DC-DC enable (a=0:disable; a=1:enable) */
#endif
    }
};
}  // namespace pico_oled

#endif
