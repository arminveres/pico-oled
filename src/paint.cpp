#include "paint.hpp"

#include <array>
#include <cstdint>
#include <limits>

#include "Debug.hpp"
#include "display.hpp"
#include "fonts.hpp"
#include "paint_enums.hpp"
#include "types.hpp"

using namespace pico_oled::paint;

auto Paint::create_image(u16 Width, u16 Height, eRotation rotation, eImageColors Color) -> void {
    this->m_image_buf = {};

    this->m_width_memory = Width;
    this->m_height_memory = Height;
    this->m_color = Color;
    this->m_scale = eScaling::DOUBLE;

    this->m_width_byte = (Width % 8 == 0) ? (Width / 8) : (Width / 8 + 1);
    this->m_height_byte = Height;

    this->m_rotation = rotation;
    this->m_mirror = eMirrorOrientiation::MIRROR_NONE;

    switch (this->m_rotation) {
        case eRotation::eROTATE_0:
        case eRotation::eROTATE_180: {
            this->m_width = Width;
            this->m_height = Height;
        } break;

        case eRotation::eROTATE_90:
        case eRotation::eROTATE_270: {
            this->m_width = Height;
            this->m_height = Width;
        } break;
    }
}

auto Paint::select_image(pico_oled::ImBuf image) -> void { this->m_image_buf = image; }

auto Paint::get_image() const -> const ImBuf & { return this->m_image_buf; }
auto Paint::set_rotation(const eRotation rotation) -> void { this->m_rotation = rotation; }

auto Paint::set_mirror_orientation(eMirrorOrientiation mirror) -> void { this->m_mirror = mirror; }

auto Paint::set_scale(eScaling scale) -> void {
    this->m_scale = scale;

    switch (scale) {
        case eScaling::DOUBLE: {
            this->m_width_byte = (this->m_width_memory % 8 == 0) ? this->m_width_memory / 8
                                                                 : this->m_width_memory / (9);
        } break;
        case eScaling::QUAD: {
            this->m_width_byte = (this->m_width_memory % 4 == 0) ? this->m_width_memory / 4
                                                                 : this->m_width_memory / (5);
        } break;
        case eScaling::HEXDEC: {
            this->m_width_byte = (this->m_width_memory % 2 == 0) ? this->m_width_memory / 2
                                                                 : this->m_width_memory / (3);
        } break;
        case eScaling::QW: {
            this->m_width_byte = this->m_width_memory * 2;
        } break;
        default:
            Debug("Set Scale Input parameter error\r\n");
            Debug("Scale Only support: 2 4 16 65\r\n");
            break;
    }
}

auto Paint::draw_pixel(u16 Xpoint, u16 Ypoint, eImageColors Color) -> void {
    if (Xpoint > this->m_width || Ypoint > this->m_height) {
        Debug("Exceeding display boundaries\r\n");
        return;
    }
    u16 X, Y;

    switch (this->m_rotation) {
        case eRotation::eROTATE_0: {
            X = Xpoint;
            Y = Ypoint;
        } break;
        case eRotation::eROTATE_90: {
            X = this->m_width_memory - Ypoint - 1;
            Y = Xpoint;
        } break;
        case eRotation::eROTATE_180: {
            X = this->m_width_memory - Xpoint - 1;
            Y = this->m_height_memory - Ypoint - 1;
        } break;
        case eRotation::eROTATE_270: {
            X = Ypoint;
            Y = this->m_height_memory - Xpoint - 1;
        } break;
        default:
            return;
    }

    switch (this->m_mirror) {
        case eMirrorOrientiation::MIRROR_NONE:
            break;
        case eMirrorOrientiation::MIRROR_HORIZONTAL: {
            X = this->m_width_memory - X - 1;
        } break;
        case eMirrorOrientiation::MIRROR_VERTICAL: {
            Y = this->m_height_memory - Y - 1;
        } break;
        case eMirrorOrientiation::MIRROR_ORIGIN: {
            X = this->m_width_memory - X - 1;
            Y = this->m_height_memory - Y - 1;
        } break;
        default:
            return;
    }

    if (X > this->m_width_memory || Y > this->m_height_memory) {
        Debug("Exceeding display boundaries\r\n");
        return;
    }

    switch (this->m_scale) {
        case eScaling::DOUBLE: {
            u32 Addr = X / 8 + Y * this->m_width_byte;
            u8 Rdata = this->m_image_buf[Addr];
            if (Color == eImageColors::BLACK)
                this->m_image_buf[Addr] = Rdata & ~(0x80 >> (X % 8));
            else
                this->m_image_buf[Addr] = Rdata | (0x80 >> (X % 8));

        } break;
        case eScaling::QUAD: {
            u32 Addr = X / 4 + Y * this->m_width_byte;
            auto col = static_cast<u32>(Color) % 4;  // Guaranteed color scale is 4  --- 0~3
            u8 Rdata = this->m_image_buf[Addr];

            Rdata = Rdata & (~(0xC0 >> ((X % 4) * 2)));
            this->m_image_buf[Addr] = Rdata | ((col << 6) >> ((X % 4) * 2));
        } break;
        case eScaling::HEXDEC: {
            u32 Addr = X / 2 + Y * this->m_width_byte;
            u8 Rdata = this->m_image_buf[Addr];
            auto col = static_cast<u32>(Color) % 16;
            Rdata = Rdata & (~(0xf0 >> ((X % 2) * 4)));
            this->m_image_buf[Addr] = Rdata | ((col << 4) >> ((X % 2) * 4));
        } break;
        case eScaling::QW: {
            u32 Addr = X * 2 + Y * this->m_width_byte;
            auto col = static_cast<u32>(Color);
            this->m_image_buf[Addr] = 0xff & (col >> 8);
            this->m_image_buf[Addr + 1] = 0xff & col;
        } break;
    }
}

auto Paint::clear_color(eImageColors Color) -> void {
    /// numeric version of enum
    auto num_color = static_cast<u32>(Color);

    switch (this->m_scale) {
        case eScaling::DOUBLE:
        case eScaling::QUAD: {
            for (u16 Y = 0; Y < this->m_height_byte; Y++) {
                for (u16 X = 0; X < this->m_width_byte; X++) {  // 8 pixel =  1 byte
                    u32 Addr = X + Y * this->m_width_byte;
                    this->m_image_buf[Addr] = num_color;
                }
            }
        } break;
        case eScaling::HEXDEC: {
            for (u16 Y = 0; Y < this->m_height_byte; Y++) {
                for (u16 X = 0; X < this->m_width_byte; X++) {  // 8 pixel =  1 byte
                    u32 Addr = X + Y * this->m_width_byte;
                    num_color &= 0x0f;
                    this->m_image_buf[Addr] = (num_color << 4) | num_color;
                }
            }
        } break;
        case eScaling::QW: {
            for (u16 Y = 0; Y < this->m_height_byte; Y++) {
                for (u16 X = 0; X < this->m_width_byte; X++) {  // 8 pixel =  1 byte
                    u32 Addr = X * 2 + Y * this->m_width_byte;
                    this->m_image_buf[Addr] = 0xff & (num_color >> 8);
                    this->m_image_buf[Addr + 1] = 0xff & num_color;
                }
            }
        } break;
    }
}

auto Paint::ClearWindows(u16 Xstart, u16 Ystart, u16 Xend, u16 Yend, eImageColors Color) -> void {
    for (u16 Y = Ystart; Y < Yend; Y++) {
        for (u16 X = Xstart; X < Xend; X++) {  // 8 pixel =  1 byte
            this->draw_pixel(X, Y, Color);
        }
    }
}

auto Paint::draw_point(
    u16 Xpoint, u16 Ypoint, eImageColors color, eDotSize epxsize, eDotStyle dot_style) -> void {
    if (Xpoint > this->m_width || Ypoint > this->m_height) {
        Debug("Paint_DrawPoint Input exceeds the normal display range\r\n");
        printf("Xpoint = %d , this->m_width = %d  \r\n ", Xpoint, this->m_width);
        printf("Ypoint = %d , this->m_height = %d  \r\n ", Ypoint, this->m_height);
        return;
    }

    const auto dotsize = static_cast<int>(epxsize);

    if (dot_style == eDotStyle::DOT_FILL_AROUND) {
        for (int XDir_Num = 0; XDir_Num < 2 * dotsize - 1; XDir_Num++) {
            for (int YDir_Num = 0; YDir_Num < 2 * dotsize - 1; YDir_Num++) {
                if (Xpoint + XDir_Num - dotsize < 0 || Ypoint + YDir_Num - dotsize < 0) break;
                // printf("x = %d, y = %d\r\n", Xpoint + XDir_Num - eDotStyle::value(Dot_style),
                // Ypoint + YDir_Num - eDotStyle::value(Dot_style));
                this->draw_pixel(Xpoint + XDir_Num - dotsize, Ypoint + YDir_Num - dotsize, color);
            }
        }
        return;
    }

    for (int XDir_Num = 0; XDir_Num < dotsize; XDir_Num++) {
        for (int YDir_Num = 0; YDir_Num < dotsize; YDir_Num++) {
            this->draw_pixel(Xpoint + XDir_Num - 1, Ypoint + YDir_Num - 1, color);
        }
    }
}

auto Paint::draw_line(u16 Xstart,
                      u16 Ystart,
                      u16 Xend,
                      u16 Yend,
                      eImageColors Color,
                      eDotSize Line_width,
                      eLineStyle Line_Style) -> void {
    if (Xstart > this->m_width || Ystart > this->m_height || Xend > this->m_width ||
        Yend > this->m_height) {
        Debug("Paint_DrawLine Input exceeds the normal display range\r\n");
        return;
    }

    u16 Xpoint = Xstart;
    u16 Ypoint = Ystart;
    int dx = (int)Xend - (int)Xstart >= 0 ? Xend - Xstart : Xstart - Xend;
    int dy = (int)Yend - (int)Ystart <= 0 ? Yend - Ystart : Ystart - Yend;

    // Increment direction, 1 is positive, -1 is counter;
    int XAddway = Xstart < Xend ? 1 : -1;
    int YAddway = Ystart < Yend ? 1 : -1;

    // Cumulative error
    int Esp = dx + dy;
    char Dotted_Len = 0;

    for (;;) {
        Dotted_Len++;
        // Painted dotted line, 2 point is really virtual
        if (Line_Style == eLineStyle::LINE_STYLE_DOTTED && Dotted_Len % 3 == 0) {
            // Debug("LINE_DOTTED\r\n");
            if (Color == eImageColors::BLACK)
                this->draw_point(
                    Xpoint, Ypoint, eImageColors::BLACK, Line_width, eDotStyle::DOT_FILL_DEFAULT);
            else
                this->draw_point(
                    Xpoint, Ypoint, eImageColors::WHITE, Line_width, eDotStyle::DOT_FILL_DEFAULT);
            Dotted_Len = 0;
        } else {
            this->draw_point(Xpoint, Ypoint, Color, Line_width, eDotStyle::DOT_FILL_DEFAULT);
        }
        if (2 * Esp >= dy) {
            if (Xpoint == Xend) break;
            Esp += dy;
            Xpoint += XAddway;
        }
        if (2 * Esp <= dx) {
            if (Ypoint == Yend) break;
            Esp += dx;
            Ypoint += YAddway;
        }
    }
}

auto Paint::draw_rectangle(u16 Xstart,
                           u16 Ystart,
                           u16 Xend,
                           u16 Yend,
                           eImageColors Color,
                           eDotSize Line_width,
                           eDrawFilling Draw_Fill) -> void {
    if (Xstart > this->m_width || Ystart > this->m_height || Xend > this->m_width ||
        Yend > this->m_height) {
        Debug("Input exceeds the normal display range\r\n");
        return;
    }

    if (Draw_Fill == eDrawFilling::DRAW_FILL_FULL) {
        for (u16 Ypoint = Ystart; Ypoint < Yend; Ypoint++) {
            this->draw_line(
                Xstart, Ypoint, Xend, Ypoint, Color, Line_width, eLineStyle::LINE_STYLE_SOLID);
        }
    } else {
        this->draw_line(
            Xstart, Ystart, Xend, Ystart, Color, Line_width, eLineStyle::LINE_STYLE_SOLID);
        this->draw_line(
            Xstart, Ystart, Xstart, Yend, Color, Line_width, eLineStyle::LINE_STYLE_SOLID);
        this->draw_line(Xend, Yend, Xend, Ystart, Color, Line_width, eLineStyle::LINE_STYLE_SOLID);
        this->draw_line(Xend, Yend, Xstart, Yend, Color, Line_width, eLineStyle::LINE_STYLE_SOLID);
    }
}

auto Paint::draw_circle(u16 X_Center,
                        u16 Y_Center,
                        u16 Radius,
                        eImageColors Color,
                        eDotSize Line_width,
                        eDrawFilling Draw_Fill) -> void {
    if (X_Center > this->m_width || Y_Center >= this->m_height) {
        Debug("Paint_DrawCircle Input exceeds the normal display range\r\n");
        return;
    }

    // Draw a circle from(0, R) as a starting point
    int16_t XCurrent, YCurrent;
    XCurrent = 0;
    YCurrent = Radius;

    // Cumulative error,judge the next point of the logo
    int16_t Esp = 3 - (Radius << 1);

    if (Draw_Fill == eDrawFilling::DRAW_FILL_FULL) {
        while (XCurrent <= YCurrent) {  // Realistic circles
            for (i16 sCountY = XCurrent; sCountY <= YCurrent; sCountY++) {
                this->draw_point(X_Center + XCurrent,
                                 Y_Center + sCountY,
                                 Color,
                                 eDotSize::DOT_PIXEL_DFT,
                                 eDotStyle::DOT_FILL_DEFAULT);  // 1
                this->draw_point(X_Center - XCurrent,
                                 Y_Center + sCountY,
                                 Color,
                                 eDotSize::DOT_PIXEL_DFT,
                                 eDotStyle::DOT_FILL_DEFAULT);  // 2
                this->draw_point(X_Center - sCountY,
                                 Y_Center + XCurrent,
                                 Color,
                                 eDotSize::DOT_PIXEL_DFT,
                                 eDotStyle::DOT_FILL_DEFAULT);  // 3
                this->draw_point(X_Center - sCountY,
                                 Y_Center - XCurrent,
                                 Color,
                                 eDotSize::DOT_PIXEL_DFT,
                                 eDotStyle::DOT_FILL_DEFAULT);  // 4
                this->draw_point(X_Center - XCurrent,
                                 Y_Center - sCountY,
                                 Color,
                                 eDotSize::DOT_PIXEL_DFT,
                                 eDotStyle::DOT_FILL_DEFAULT);  // 5
                this->draw_point(X_Center + XCurrent,
                                 Y_Center - sCountY,
                                 Color,
                                 eDotSize::DOT_PIXEL_DFT,
                                 eDotStyle::DOT_FILL_DEFAULT);  // 6
                this->draw_point(X_Center + sCountY,
                                 Y_Center - XCurrent,
                                 Color,
                                 eDotSize::DOT_PIXEL_DFT,
                                 eDotStyle::DOT_FILL_DEFAULT);  // 7
                this->draw_point(X_Center + sCountY,
                                 Y_Center + XCurrent,
                                 Color,
                                 eDotSize::DOT_PIXEL_DFT,
                                 eDotStyle::DOT_FILL_DEFAULT);
            }
            if (Esp < 0)
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * (XCurrent - YCurrent);
                YCurrent--;
            }
            XCurrent++;
        }
    } else {  // Draw a hollow circle
        while (XCurrent <= YCurrent) {
            this->draw_point(X_Center + XCurrent,
                             Y_Center + YCurrent,
                             Color,
                             Line_width,
                             eDotStyle::DOT_FILL_DEFAULT);  // 1
            this->draw_point(X_Center - XCurrent,
                             Y_Center + YCurrent,
                             Color,
                             Line_width,
                             eDotStyle::DOT_FILL_DEFAULT);  // 2
            this->draw_point(X_Center - YCurrent,
                             Y_Center + XCurrent,
                             Color,
                             Line_width,
                             eDotStyle::DOT_FILL_DEFAULT);  // 3
            this->draw_point(X_Center - YCurrent,
                             Y_Center - XCurrent,
                             Color,
                             Line_width,
                             eDotStyle::DOT_FILL_DEFAULT);  // 4
            this->draw_point(X_Center - XCurrent,
                             Y_Center - YCurrent,
                             Color,
                             Line_width,
                             eDotStyle::DOT_FILL_DEFAULT);  // 5
            this->draw_point(X_Center + XCurrent,
                             Y_Center - YCurrent,
                             Color,
                             Line_width,
                             eDotStyle::DOT_FILL_DEFAULT);  // 6
            this->draw_point(X_Center + YCurrent,
                             Y_Center - XCurrent,
                             Color,
                             Line_width,
                             eDotStyle::DOT_FILL_DEFAULT);  // 7
            this->draw_point(X_Center + YCurrent,
                             Y_Center + XCurrent,
                             Color,
                             Line_width,
                             eDotStyle::DOT_FILL_DEFAULT);  // 0

            if (Esp < 0)
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * (XCurrent - YCurrent);
                YCurrent--;
            }
            XCurrent++;
        }
    }
}

auto Paint::draw_char(u16 Xpoint,
                      u16 Ypoint,
                      const char Acsii_Char,
                      const font::Font &Font,
                      eImageColors Color_Foreground,
                      eImageColors Color_Background) -> void {
    if (Xpoint > this->m_width || Ypoint > this->m_height) {
        Debug("Paint_DrawChar Input exceeds the normal display range\r\n");
        return;
    }

    const u32 Char_Offset =
        (Acsii_Char - ' ') * Font.Height * (Font.Width / 8 + (Font.Width % 8 ? 1 : 0));
    const auto *ptr = &Font.table[Char_Offset];

    for (u16 Page = 0; Page < Font.Height; Page++) {
        for (u16 Column = 0; Column < Font.Width; Column++) {
            // To determine whether the font background color and screen background
            // color is consistent
            // this process is to speed up the scan
            if (eImageColors::FONT_BACKGROUND == Color_Background) {
                if (*ptr & (0x80 >> (Column % 8)))
                    this->draw_pixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
                // Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Foreground,
                // DOT_PIXEL_DFT, DOT_STYLE_DFT);
            } else {
                if (*ptr & (0x80 >> (Column % 8))) {
                    this->draw_pixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
                    // Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Foreground,
                    // DOT_PIXEL_DFT, DOT_STYLE_DFT);
                } else {
                    this->draw_pixel(Xpoint + Column, Ypoint + Page, Color_Background);
                    // Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Background,
                    // DOT_PIXEL_DFT, DOT_STYLE_DFT);
                }
            }
            // One pixel is 8 bits
            if (Column % 8 == 7) ptr++;
        }  // Write a line
        if (Font.Width % 8 != 0) ptr++;
    }  // Write all
}

auto Paint::draw_en_string(u16 Xstart,
                           u16 Ystart,
                           const char *pString,
                           const font::Font &Font,
                           eImageColors Color_Foreground,
                           eImageColors Color_Background) -> void {
    u16 Xpoint = Xstart;
    u16 Ypoint = Ystart;

    if (Xstart > this->m_width || Ystart > this->m_height) {
        Debug("Paint_DrawString_EN Input exceeds the normal display range\r\n");
        return;
    }

    while (*pString != '\0') {
        // if X direction filled , reposition to(Xstart,Ypoint),Ypoint is Y
        // direction plus the Height of the character
        if ((Xpoint + Font.Width) > this->m_width) {
            Xpoint = Xstart;
            Ypoint += Font.Height;
        }

        // If the Y direction is full, reposition to(Xstart, Ystart)
        if ((Ypoint + Font.Height) > this->m_height) {
            Xpoint = Xstart;
            Ypoint = Ystart;
        }

        this->draw_char(Xpoint, Ypoint, *pString, Font, Color_Background, Color_Foreground);

        // The next character of the address
        pString++;

        // The next word of the abscissa increases the font of the broadband
        Xpoint += Font.Width;
    }
}

auto Paint::draw_number(u16 Xpoint, u16 Ypoint, float Number, const font::Font &Font, u16 precision,
                        eImageColors Color_Foreground, eImageColors Color_Background) -> void {
    u16 Num_Bit = 0;
    u16 Str_Bit = 0;
    i32 int_part = static_cast<i32>(Number);                  // Integer part of the number
    float frac_part = Number - static_cast<float>(int_part);  // Fractional part of the number

    // Hold the final string to be shown on display.
    std::array<char, std::numeric_limits<char>::max()> string_arr = {0};
    std::array<u8, std::numeric_limits<u8>::max()> number_arr = {0};

    if (Xpoint > this->m_width || Ypoint > this->m_height) {
        Debug("Paint_DisNum Input exceeds the normal display range\r\n");
        return;
    }

    if (Number < 0) {  // Handle negative numbers
        string_arr[Str_Bit++] = '-';
        int_part = -int_part;
        frac_part = -frac_part;
    } else if (Number == 0) {  // Zeroes are not handled below, so add it manually
        string_arr[0] = '0';
    }

    // Handle digits after the decimal point
    if (precision > 0) {
        // shift the fractional parts as many times as necessary
        for (u8 i = 0; i < precision; i++) {
            frac_part *= 10;
        }
        auto frac_int = static_cast<u32>(frac_part);

        // Converts the fractional part to a string
        for (u8 i = 0; i < precision; i++) {
            number_arr[Num_Bit++] = frac_int % 10 + '0';
            frac_int /= 10;
        }

        number_arr[Num_Bit++] = '.';
    }

    // Converts the integer part to a string
    while (int_part) {
        number_arr[Num_Bit++] = static_cast<u32>(int_part) % 10 + '0';
        int_part /= 10;
    }

    // Invert the string (both integer and fractional parts)
    while (Num_Bit > 0) {
        string_arr[Str_Bit++] = number_arr[--Num_Bit];
    }

    // Display the string
    this->draw_en_string(Xpoint, Ypoint, string_arr.data(), Font, Color_Background,
                         Color_Foreground);
}

auto Paint::draw_time(u16 Xstart,
                      u16 Ystart,
                      const PaintTime &pTime,
                      const font::Font &Font,
                      eImageColors Color_Foreground,
                      eImageColors Color_Background) -> void {
    constexpr char value[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    const auto Dx = Font.Width;

    // Write data into the cache
    this->draw_char(
        Xstart, Ystart, value[pTime.Hour / 10], Font, Color_Background, Color_Foreground);
    this->draw_char(
        Xstart + Dx, Ystart, value[pTime.Hour % 10], Font, Color_Background, Color_Foreground);
    this->draw_char(
        Xstart + Dx + Dx / 4 + Dx / 2, Ystart, ':', Font, Color_Background, Color_Foreground);
    this->draw_char(Xstart + Dx * 2 + Dx / 2,
                    Ystart,
                    value[pTime.Min / 10],
                    Font,
                    Color_Background,
                    Color_Foreground);
    this->draw_char(Xstart + Dx * 3 + Dx / 2,
                    Ystart,
                    value[pTime.Min % 10],
                    Font,
                    Color_Background,
                    Color_Foreground);
    this->draw_char(
        Xstart + Dx * 4 + Dx / 2 - Dx / 4, Ystart, ':', Font, Color_Background, Color_Foreground);
    this->draw_char(
        Xstart + Dx * 5, Ystart, value[pTime.Sec / 10], Font, Color_Background, Color_Foreground);
    this->draw_char(
        Xstart + Dx * 6, Ystart, value[pTime.Sec % 10], Font, Color_Background, Color_Foreground);
}

auto Paint::draw_image(const u8 *image, u16 xStart, u16 yStart, u16 W_Image, u16 H_Image) -> void {
    for (i32 j = 0; j < H_Image; j++) {
        for (i32 i = 0; i < W_Image; i++) {
            if (xStart + i < this->m_width_memory &&
                yStart + j < this->m_height_memory) {  // Exceeded part does not display
                auto col = static_cast<eImageColors>((*(image + j * W_Image * 2 + i * 2 + 1)) << 8 |
                                                     (*(image + j * W_Image * 2 + i * 2)));
                this->draw_pixel(xStart + i, yStart + j, col);
                // Using arrays is a property of sequential storage, accessing the
                // original array by
                // algorithm j*W_Image*2 			   Y offset i*2 X offset
            }
        }
    }
}

auto Paint::draw_bitmap(const u8 *image_buffer) -> void {
    for (u16 y = 0; y < this->m_height_byte; y++) {
        for (u16 x = 0; x < this->m_width_byte; x++) {  // 8 pixel =  1 byte
            const u32 Addr = x + y * this->m_width_byte;
            this->m_image_buf[Addr] = static_cast<u8>(image_buffer[Addr]);
        }
    }
}

auto Paint::draw_bitmap_block(const u8 *image_buffer, u8 Region) -> void {
    for (u16 y = 0; y < this->m_height_byte; y++) {
        for (u16 x = 0; x < this->m_width_byte; x++) {  // 8 pixel =  1 byte
            const u32 Addr = x + y * this->m_width_byte;
            this->m_image_buf[Addr] = static_cast<u8>(
                image_buffer[Addr + (this->m_height_byte) * this->m_width_byte * (Region - 1)]);
        }
    }
}

auto Paint::bmp_windows(const u8 x, const u8 y, const u8 *pBmp, const u8 chWidth, const u8 chHeight)
    -> void {
    const u16 byteWidth = (chWidth + 7) / 8;

    for (u16 j = 0; j < chHeight; j++) {
        for (u16 i = 0; i < chWidth; i++) {
            if (*(pBmp + j * byteWidth + i / 8) & (128 >> (i & 7))) {
                this->draw_pixel(x + i, y + j, eImageColors::WHITE);
            }
        }
    }
}
