#ifndef __PICO_OLED_PAINT_HPP
#define __PICO_OLED_PAINT_HPP

#include "fonts.hpp"
#include "paint_enums.hpp"
#include "types.hpp"

namespace pico_oled::paint {

/**
 * Custom structure of a time attribute
 **/
struct PaintTime {
    u16 Year;  // 0000
    u8 Month;  // 1 - 12
    u8 Day;    // 1 - 30
    u8 Hour;   // 0 - 23
    u8 Min;    // 0 - 59
    u8 Sec;    // 0 - 59
};

/**
 * Image attributes
 **/
struct Paint {
    u8 *m_image_buf;
    u16 m_width;
    u16 m_height;
    u16 m_width_memory;
    u16 m_height_memory;
    eImageColors m_color;
    eRotation m_rotation;
    eMirrorOrientiation m_mirror;
    u16 m_width_byte;
    u16 m_height_byte;
    eScaling m_scale;

    /// Init and create new image
    ///
    /// @param:
    ///     image   :   Pointer to the image cache
    ///     width   :   The width of the picture
    ///     Height  :   The height of the picture
    ///     Color   :   Whether the picture is inverted
    auto create_image(u8 *image, u16 Width, u16 Height, eRotation rotation, eImageColors Color);
    /// Select Image
    auto select_image(u8 *image);

    auto set_rotation(const eRotation rotation);

    auto set_mirror_orientation(eMirrorOrientiation mirror);

    auto draw_pixel(u16 Xpoint, u16 Ypoint, eImageColors Color);

    /// Sets scaling and updates `m_width_byte`
    auto set_scale(eScaling scale);

    auto clear_color(eImageColors Color);

    /// Clear color of window
    auto ClearWindows(u16 Xstart, u16 Ystart, u16 Xend, u16 Yend, eImageColors Color);

    // Draw point
    auto draw_point(u16 Xpoint, u16 Ypoint, eImageColors Color, eDotSize Dot_Pixel,
                    eStyle Dot_FillWay);

    /// Draw line with arbitraty slope
    auto draw_line(u16 Xstart, u16 Ystart, u16 Xend, u16 Yend, eImageColors Color,
                   eDotSize Line_width, eLineStyle Line_Style);

    /// Draw a rectangle
    /// @params:
    ///     Xstart ：Rectangular  Starting Xpoint point coordinates,
    ///     Ystart ：Rectangular  Starting Xpoint point coordinates,
    ///     Xend   ：Rectangular  End point Xpoint coordinate,
    ///     Yend   ：Rectangular  End point Ypoint coordinate,
    ///     Color  ：The color of the Rectangular segment,
    ///     Line_width: Line width,
    ///     Draw_Fill : Whether to fill the inside of the rectangle,
    auto draw_rectangle(u16 Xstart, u16 Ystart, u16 Xend, u16 Yend, eImageColors Color,
                        eDotSize Line_width, eDrawFilling Draw_Fill);

    /// Use the 8-point method to draw a circle of the specified size at the specified position->
    /// @params:
    ///    X_Center  ：Center X coordinate,
    ///    Y_Center  ：Center Y coordinate,
    ///    Radius    ：circle Radius,
    ///    Color     ：The color of the ：circle segment,
    ///    Line_width: Line width,
    ///    Draw_Fill : Whether to fill the inside of the Circle,
    auto draw_circle(u16 X_Center, u16 Y_Center, u16 Radius, eImageColors Color,
                     eDotSize Line_width, eDrawFilling Draw_Fill);

    /// Show English characters
    ///
    /// @params:
    ///    Xpoint           ：X coordinate,
    ///    Ypoint           ：Y coordinate,
    ///    Acsii_Char       ：To display the English characters,
    ///    Font             ：A structure pointer that displays a character size,
    ///    Color_Foreground : Select the foreground color,
    ///    Color_Background : Select the background color,
    auto draw_char(u16 Xstart, u16 Ystart, const char Acsii_Char, const font::Font &Font,
                   eImageColors Color_Foreground, eImageColors Color_Background);

    /// Display the string
    ///
    /// @params:
    ///     Xstart           ：X coordinate,
    ///     Ystart           ：Y coordinate,
    ///     pString          ：The first address of the English string to be displayed,
    ///     Font             ：A structure pointer that displays a character size,
    ///     Color_Foreground : Select the foreground color,
    ///     Color_Background : Select the background color,
    auto draw_en_string(u16 Xstart, u16 Ystart, const char *pString, const font::Font &Font,
                        eImageColors Color_Foreground, eImageColors Color_Background);

    /// Display nummber
    ///
    /// @param:
    ///     Xstart           ：X coordinate,
    ///     Ystart           : Y coordinate,
    ///     Nummber          : The number displayed,
    ///     Font             ：A structure pointer that displays a character size,
    ///     Digit            : Fractional width,
    ///     Color_Foreground : Select the foreground color,
    ///     Color_Background : Select the background color,
    auto draw_number(u16 Xpoint, u16 Ypoint, double Nummber, const font::Font &Font, u16 Digit,
                     eImageColors Color_Foreground, eImageColors Color_Background);

    /// Display time
    ///
    /// parameter:
    ///     Xstart           ：X coordinate,
    ///     Ystart           : Y coordinate,
    ///     pTime            : Time-related structures,
    ///     Font             ：A structure pointer that displays a character size,
    ///     Color_Foreground : Select the foreground color,
    ///     Color_Background : Select the background color,
    auto draw_time(u16 Xstart, u16 Ystart, const PaintTime &pTime, const font::Font &Font,
                   eImageColors Color_Foreground, eImageColors Color_Background);

    /// Display monochrome bitmap
    ///
    /// @params:
    ///    image_buffer ：A picture data converted to a bitmap
    /// @note:
    ///    Use a computer to convert the image into a corresponding array,
    ///    and then embed the array directly into Imagedata.cpp as a .c file.
    auto draw_bitmap(const unsigned char *image_buffer);

    auto draw_bitmap_block(const unsigned char *image_buffer, u8 Region);

    auto draw_image(const unsigned char *image, u16 xStart, u16 yStart, u16 W_Image, u16 H_Image);

    auto bmp_windows(unsigned char x, unsigned char y, const unsigned char *pBmp,
                     unsigned char chWidth, unsigned char chHeight);
};
}  // namespace pico_oled::paint
#endif
