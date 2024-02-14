#ifndef __PICO_OLED_PAINT_HPP
#define __PICO_OLED_PAINT_HPP

#include "display.hpp"
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

/// Image attributes
///
/// Holds an image that is drawn upon
struct Paint {
   private:
    ImBuf m_image_buf;
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

   public:
    /// Init and create new image
    ///
    /// @param:
    ///     image   :   Pointer to the image cache
    ///     width   :   The width of the picture
    ///     Height  :   The height of the picture
    ///     Color   :   Whether the picture is inverted
    auto create_image(u16 Width, u16 Height, eRotation rotation, eImageColors Color) -> void;

    /// Select Image
    auto select_image(ImBuf image) -> void;

    auto get_image() const -> const ImBuf &;

    auto set_rotation(const eRotation rotation) -> void;

    auto set_mirror_orientation(eMirrorOrientiation mirror) -> void;

    auto draw_pixel(u16 Xpoint, u16 Ypoint, eImageColors Color) -> void;

    /// Sets scaling and updates `m_width_byte`
    auto set_scale(eScaling scale) -> void;

    auto clear_color(eImageColors Color) -> void;

    /// Clear color of window
    auto ClearWindows(u16 Xstart, u16 Ystart, u16 Xend, u16 Yend, eImageColors Color) -> void;

    // Draw point
    auto draw_point(u16 Xpoint,
                    u16 Ypoint,
                    eImageColors Color,
                    eDotSize Dot_Pixel,
                    eDotStyle Dot_FillWay = eDotStyle::DOT_FILL_DEFAULT) -> void;

    /// Draw line with arbitraty slope
    auto draw_line(u16 Xstart,
                   u16 Ystart,
                   u16 Xend,
                   u16 Yend,
                   eImageColors Color,
                   eDotSize Line_width,
                   eLineStyle Line_Style) -> void;

    /// Draw a rectangle
    /// @params:
    ///     Xstart ：Rectangular  Starting Xpoint point coordinates,
    ///     Ystart ：Rectangular  Starting Xpoint point coordinates,
    ///     Xend   ：Rectangular  End point Xpoint coordinate,
    ///     Yend   ：Rectangular  End point Ypoint coordinate,
    ///     Color  ：The color of the Rectangular segment,
    ///     Line_width: Line width,
    ///     Draw_Fill : Whether to fill the inside of the rectangle,
    auto draw_rectangle(u16 Xstart,
                        u16 Ystart,
                        u16 Xend,
                        u16 Yend,
                        eImageColors Color,
                        eDotSize Line_width,
                        eDrawFilling Draw_Fill) -> void;

    /// Use the 8-point method to draw a circle of the specified size at the specified position->
    /// @params:
    ///    X_Center  ：Center X coordinate,
    ///    Y_Center  ：Center Y coordinate,
    ///    Radius    ：circle Radius,
    ///    Color     ：The color of the ：circle segment,
    ///    Line_width: Line width,
    ///    Draw_Fill : Whether to fill the inside of the Circle,
    auto draw_circle(u16 X_Center,
                     u16 Y_Center,
                     u16 Radius,
                     eImageColors Color,
                     eDotSize Line_width,
                     eDrawFilling Draw_Fill) -> void;

    /// Show English characters
    ///
    /// @params:
    ///    Xpoint           ：X coordinate,
    ///    Ypoint           ：Y coordinate,
    ///    Acsii_Char       ：To display the English characters,
    ///    Font             ：A structure pointer that displays a character size,
    ///    Color_Foreground : Select the foreground color,
    ///    Color_Background : Select the background color,
    auto draw_char(u16 Xstart,
                   u16 Ystart,
                   const char Acsii_Char,
                   const font::Font &Font,
                   eImageColors Color_Foreground,
                   eImageColors Color_Background) -> void;

    /// Display the string
    ///
    /// @params:
    ///     Xstart           ：X coordinate,
    ///     Ystart           ：Y coordinate,
    ///     pString          ：The first address of the English string to be displayed,
    ///     Font             ：A structure pointer that displays a character size,
    ///     Color_Foreground : Select the foreground color,
    ///     Color_Background : Select the background color,
    auto draw_en_string(u16 Xstart,
                        u16 Ystart,
                        const char *pString,
                        const font::Font &Font,
                        eImageColors Color_Foreground,
                        eImageColors Color_Background) -> void;

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
    auto draw_number(u16 Xpoint,
                     u16 Ypoint,
                     double Nummber,
                     const font::Font &Font,
                     u16 Digit,
                     eImageColors Color_Foreground,
                     eImageColors Color_Background) -> void;

    /// Display time
    ///
    /// parameter:
    ///     Xstart           ：X coordinate,
    ///     Ystart           : Y coordinate,
    ///     pTime            : Time-related structures,
    ///     Font             ：A structure pointer that displays a character size,
    ///     Color_Foreground : Select the foreground color,
    ///     Color_Background : Select the background color,
    auto draw_time(u16 Xstart,
                   u16 Ystart,
                   const PaintTime &pTime,
                   const font::Font &Font,
                   eImageColors Color_Foreground,
                   eImageColors Color_Background) -> void;

    /// Display monochrome bitmap
    ///
    /// @params:
    ///    image_buffer ：A picture data converted to a bitmap
    /// @note:
    ///    Use a computer to convert the image into a corresponding array,
    ///    and then embed the array directly into Imagedata.cpp as a .c file.
    auto draw_bitmap(const unsigned char *image_buffer) -> void;

    auto draw_bitmap_block(const unsigned char *image_buffer, u8 Region) -> void;

    auto draw_image(const unsigned char *image, u16 xStart, u16 yStart, u16 W_Image, u16 H_Image)
        -> void;

    auto bmp_windows(const u8 x, const u8 y, const u8 *pBmp, const u8 chWidth, const u8 chHeight)
        -> void;
};
}  // namespace pico_oled::paint
#endif
