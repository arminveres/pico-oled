#ifndef __PICO_OLED_PAINT_ENUMS_HPP
#define __PICO_OLED_PAINT_ENUMS_HPP

namespace pico_oled::paint {

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
enum class eDotStyle {
    DOT_FILL_AROUND = 1,  // dot pixel 1 x 1
    DOT_FILL_RIGHTUP,     // dot pixel 2 X 2
    DOT_FILL_DEFAULT = DOT_FILL_AROUND
};

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

}  // namespace pico_oled::paint
#endif
