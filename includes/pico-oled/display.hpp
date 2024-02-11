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
#include <variant>

#include "types.hpp"

static constexpr auto IIC_CMD = 0x00;
static constexpr auto IIC_RAM = 0x40;

static constexpr auto I2C_PORT = i2c1;
static const auto SPI_PORT = spi1;

static constexpr auto EPD_RST_PIN = 12;
static constexpr auto EPD_DC_PIN = 8;
static constexpr auto EPD_CS_PIN = 9;
static constexpr auto EPD_BL_PIN = 13;
static constexpr auto EPD_CLK_PIN = 10;
static constexpr auto EPD_MOSI_PIN = 11;
static constexpr auto EPD_SCL_PIN = 7;
static constexpr auto EPD_SDA_PIN = 6;

namespace pico_oled {

enum class eConType { I2C, SPI };

template <eConType T>
class Display {
   public:
    static constexpr u8 k_width = 128;
    static constexpr u8 k_height = 64;
    static constexpr u32 k_imsize = k_width * k_height / 8;

    enum class eRegisters {
        DISABLE_DISP_ON = 0xA4,
        DISABLE_DISP_OFF = 0xA5,
        TURN_DISP_OFF = 0xAE,
        TURN_DISP_ON = 0xAF,
        DISP_COL_NORMAL = 0xa6,
        DISP_COL_REV = 0xa7,
        /// 0x00-0x0F
        SET_LOW_COL_ADR = 0x00,
        /// 0x10-0x17
        SET_HIGH_COL_ADR = 0x10,
        /// Memory addressing mode
        SET_MEM_HORI_ADDRING = 0x20,
        SET_MEM_VERT_ADDRING = 0x21,
        /// Contrast Control MODE, [0x00, 0xFF]
        CONTRAST_CTRL = 0x81,
        /// Set segment remap
        SET_SEGMENT_REMAP_NORM = 0xA0,
        SET_SEGMENT_REMAP_REV = 0xA1,
        /// Set Multiplex Ration, followed by value [0x00, 0x7F]
        SET_MULTIPLEX_RATIO = 0xA8,
        /// Set Display offset, \[0x00, 0x7F\]
        SET_DISP_OFFSET = 0xD3,
        /// DC-DC voltage Converter
        SET_DCDC_ON = 0xAD,
        SET_DCDC_PUMP_ON = 0x8A,
        SET_DCDC_PUMP_OFF = 0x8B,
        /// Specify page address to load display RAM to page address reg, [0xB0, 0xBF]
        // SET_PAGE_ADR = 0xB0-0xbf,
        /// Set Common output scan direction
        // SET_OSC_DIR = 0xC0 - 0xC8
        /// Set Display Clock Divide Ratio/Oscillator Frequency: (Double Bytes Command) [0x00, 0xFF]
        SET_OSC_CLOCK_DIV = 0xD5,
        /// Set Dis-charge/Pre-charge Period: (Double Bytes Command) [0x00, 0xFF]
        SET_PRECHARDE_PER = 0xD9,
        /// Set VCOM Deselect Level: (Double Bytes Command)
        SET_VCOM_DESEL = 0xDB,
        /// Set Display Start Line:（Double Bytes Command）
        SET_START_LINE = 0xDC,
        /// Read-Modify-Write
        SET_READ_MOD_WRITE = 0xE0,
        /// Read-Modify-Write
        END_READ_MOD_WRITE = 0xEE,
        NOP = 0xE3,
    };
    Display() {
        reset();
        init_regs();
        sleep_ms(200);
        write_reg(eRegisters::TURN_DISP_ON);
    }

    // auto show(const u8 *imbuf) const {
    // template <u32 imsize>
    // auto show(const std::array<u8, imsize> &imbuf) const {
    auto show(const std::array<u8, k_imsize> &imbuf) const {
        constexpr auto l_width = k_width / 8;
        // write starting row
        write_reg(0xB0);
        for (u16 i = 0; i < k_height; ++i) {
            const auto column = k_width - 1 - i;
            write_reg(get_reg_val(eRegisters::SET_LOW_COL_ADR) + (column & 0x0f));
            write_reg(get_reg_val(eRegisters::SET_HIGH_COL_ADR) + (column >> 0x4));
            for (u16 j = 0; j < l_width; ++j) {
                const auto temp = reverse_byte(imbuf[i + j + l_width]);
                write_data(temp);
            }
        }
    }

    auto clear() const {
        constexpr auto l_width = k_width / 8;
        // write starting row
        write_reg(0xB0);
        for (u16 i = 0; i < k_height; ++i) {
            const auto column = k_width - 1 - i;
            write_reg(get_reg_val(eRegisters::SET_LOW_COL_ADR) + (column & 0x0f));
            write_reg(get_reg_val(eRegisters::SET_HIGH_COL_ADR) + (column >> 0x4));
            for (u16 j = 0; j < l_width; ++j) {
                write_data(0x00);
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
        byte = ((byte & 0x55) << 1) | ((byte & 0xaa) >> 1);
        byte = ((byte & 0x33) << 2) | ((byte & 0xcc) >> 2);
        byte = ((byte & 0x0f) << 4) | ((byte & 0xf0) >> 4);
        return byte;
    }
    static auto get_reg_val(const eRegisters reg) { return static_cast<u8>(reg); }

    ~Display() {}

    Display(Display &&) = delete;
    Display(const Display &) = delete;
    Display &operator=(Display &&) = delete;
    Display &operator=(const Display &) = delete;

   private:
    inline auto write_reg(const std::variant<eRegisters, int> var_reg) const {
        u8 reg = 0;
        std::visit(
            [&reg](auto &&arg) {
                using reg_type = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<reg_type, eRegisters>) {
                    // Handle enum case
                    reg = static_cast<u8>(arg);
                } else if constexpr (std::is_same_v<reg_type, int>) {
                    // Handle int case
                    reg = arg;
                }
            },
            var_reg);

        if constexpr (T == eConType::SPI) {
            gpio_put(EPD_DC_PIN, 0);
            gpio_put(EPD_CS_PIN, 0);
            spi_write_blocking(SPI_PORT, &reg, 1);
            gpio_put(EPD_CS_PIN, 1);

        } else if constexpr (T == eConType::I2C) {
            const u8 data[2] = {IIC_CMD, reg};
            i2c_write_blocking(i2c1, 0x3C, data, 2, false);
        }
    }

    inline auto write_data(const u8 reg) const {
        if constexpr (T == eConType::I2C) {
            gpio_put(EPD_DC_PIN, 1);
            gpio_put(EPD_CS_PIN, 0);
            spi_write_blocking(SPI_PORT, &reg, 1);
            gpio_put(EPD_CS_PIN, 1);

        } else if constexpr (T == eConType::SPI) {
            const u8 data[2] = {IIC_RAM, reg};
            i2c_write_blocking(i2c1, 0x3C, data, 2, false);
        }
    }

    auto init_regs() const {
        write_reg(eRegisters::TURN_DISP_OFF);

        write_reg(eRegisters::SET_LOW_COL_ADR);
        write_reg(eRegisters::SET_HIGH_COL_ADR);
        write_reg(0xB0);

        write_reg(eRegisters::SET_START_LINE);
        write_reg(0x00);  // set to beginning

        write_reg(eRegisters::CONTRAST_CTRL);
        write_reg(0x6f);
        write_reg(eRegisters::SET_MEM_VERT_ADDRING);

        write_reg(eRegisters::SET_SEGMENT_REMAP_NORM);
        write_reg(0xC0);  // set com scan direction
        write_reg(eRegisters::DISABLE_DISP_ON);

        write_reg(eRegisters::DISP_COL_NORMAL);
        write_reg(eRegisters::SET_MULTIPLEX_RATIO);
        write_reg(0x3F);  // 1/64 duty

        write_reg(eRegisters::SET_DISP_OFFSET);
        write_reg(0x60);

        write_reg(eRegisters::SET_OSC_CLOCK_DIV);
        write_reg(0x41);

        write_reg(eRegisters::SET_PRECHARDE_PER);
        write_reg(0x22);

        write_reg(eRegisters::SET_VCOM_DESEL);
        write_reg(0x35);

        write_reg(eRegisters::SET_DCDC_ON);
        write_reg(eRegisters::SET_DCDC_PUMP_ON);
    }
};
}  // namespace pico_oled

#endif
