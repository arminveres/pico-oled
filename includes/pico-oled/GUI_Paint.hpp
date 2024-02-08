#ifndef __GUI_PAINT_H
#define __GUI_PAINT_H

#include "DEV_Config.hpp"
#include "fonts.hpp"

/**
 * Display rotate
 **/
enum class eRotation {
    eROTATE_0 = 0,
    eROTATE_90 = 90,
    eROTATE_180 = 180,
    eROTATE_270 = 270,
};

/**
 * Display Flip
 **/
enum class eMirrorOrientiation {
    MIRROR_NONE = 0x00,
    MIRROR_HORIZONTAL = 0x01,
    MIRROR_VERTICAL = 0x02,
    MIRROR_ORIGIN = 0x03,
};

/**
 * image color
 **/
enum class eImageColors {
    WHITE = 0xFFFF,
    BLACK = 0x0000,
    BLUE = 0x001F,
    BRED = 0XF81F,
    GRED = 0XFFE0,
    GBLUE = 0X07FF,
    RED = 0xF800,
    MAGENTA = 0xF81F,
    GREEN = 0x07E0,
    CYAN = 0x7FFF,
    YELLOW = 0xFFE0,
    BROWN = 0XBC40,
    BRRED = 0XFC07,
    GRAY = 0X8430,

    IMAGE_BACKGROUND = WHITE,
    FONT_FOREGROUND = BLACK,
    FONT_BACKGROUND = WHITE,
};

/**
 * The size of the point
 **/
enum class eDotSize {
    DOT_PIXEL_1X1 = 1,  // 1 x 1
    DOT_PIXEL_2X2,      // 2 X 2
    DOT_PIXEL_3X3,      // 3 X 3
    DOT_PIXEL_4X4,      // 4 X 4
    DOT_PIXEL_5X5,      // 5 X 5
    DOT_PIXEL_6X6,      // 6 X 6
    DOT_PIXEL_7X7,      // 7 X 7
    DOT_PIXEL_8X8,      // 8 X 8
    DOT_PIXEL_DFT = DOT_PIXEL_1X1
};

/// Point size fill style
///
/// TODO(aver): Really don't know why the original guy used this for calculations ... refactor...
struct eDotStyle {
    enum {
        DOT_FILL_AROUND = 1,  // dot pixel 1 x 1
        DOT_FILL_RIGHTUP,     // dot pixel 2 X 2
        DOT_FILL_DEFAULT = DOT_FILL_AROUND
    };
    static inline auto value(const decltype(eDotStyle::DOT_FILL_AROUND) &eval) {
        return static_cast<int>(eval);
    }
};
using eStyle = decltype(eDotStyle::DOT_FILL_AROUND);

/**
 * Line style, solid or dashed
 **/
enum class eLineStyle {
    LINE_STYLE_SOLID = 0,
    LINE_STYLE_DOTTED,
};

/**
 * Whether the graphic is filled
 **/
enum class eDrawFilling {
    DRAW_FILL_EMPTY = 0,
    DRAW_FILL_FULL,
};

enum class eScaling { DOUBLE = 2, QUAD = 4, HEXDEC = 16, QW = 65 };

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
    u16 m_color;
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
    auto create_image(u8 *image, u16 Width, u16 Height, eRotation rotation, u16 Color);
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
    auto draw_char(u16 Xstart, u16 Ystart, const char Acsii_Char, sFONT *Font,
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
    auto draw_en_string(u16 Xstart, u16 Ystart, const char *pString, sFONT *Font,
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
    auto draw_number(u16 Xpoint, u16 Ypoint, double Nummber, sFONT *Font, u16 Digit,
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
    auto draw_time(u16 Xstart, u16 Ystart, PaintTime *pTime, sFONT *Font,
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

#endif
