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
  DOT_PIXEL_1X1 = 1, // 1 x 1
  DOT_PIXEL_2X2,     // 2 X 2
  DOT_PIXEL_3X3,     // 3 X 3
  DOT_PIXEL_4X4,     // 4 X 4
  DOT_PIXEL_5X5,     // 5 X 5
  DOT_PIXEL_6X6,     // 6 X 6
  DOT_PIXEL_7X7,     // 7 X 7
  DOT_PIXEL_8X8,     // 8 X 8
};
#define DOT_PIXEL_DFT DOT_PIXEL_1X1 // Default dot pilex

/**
 * Point size fill style
 **/
enum class eDotStyle {
  DOT_FILL_AROUND = 1, // dot pixel 1 x 1
  DOT_FILL_RIGHTUP,    // dot pixel 2 X 2
};
#define DOT_STYLE_DFT DOT_FILL_AROUND // Default dot pilex

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

/**
 * Custom structure of a time attribute
 **/
struct PaintTime {
  UWORD Year;  // 0000
  UBYTE Month; // 1 - 12
  UBYTE Day;   // 1 - 30
  UBYTE Hour;  // 0 - 23
  UBYTE Min;   // 0 - 59
  UBYTE Sec;   // 0 - 59
};

/**
 * Image attributes
 **/
struct Paint {
  UBYTE *Image;
  UWORD Width;
  UWORD Height;
  UWORD WidthMemory;
  UWORD HeightMemory;
  UWORD Color;
  eRotation m_rotation;
  UWORD Mirror;
  UWORD WidthByte;
  UWORD HeightByte;
  UWORD Scale;

  /// Init and create new image
  ///
  /// @param:
  ///     image   :   Pointer to the image cache
  ///     width   :   The width of the picture
  ///     Height  :   The height of the picture
  ///     Color   :   Whether the picture is inverted
  auto create_image(UBYTE *image, UWORD Width, UWORD Height, eRotation rotation,
                    UWORD Color);
  /// Select Image
  auto select_image(UBYTE *image);

  auto set_rotation(const eRotation rotation);

/******************************************************************************
function:	Select Image mirror
parameter:
    mirror   :Not mirror,Horizontal mirror,Vertical mirror,Origin mirror
******************************************************************************/
  auto SetMirroring(UBYTE mirror);
  auto SetPixel(UWORD Xpoint, UWORD Ypoint, UWORD Color);
  auto SetScale(UBYTE scale);

  auto Clear(UWORD Color);
  auto ClearWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend,
                    UWORD Color);

  // Dr
  auto DrawPoint(UWORD Xpoint, UWORD Ypoint, UWORD Color, eDotSize Dot_Pixel,
                 eDotStyle Dot_FillWay);
  auto DrawLine(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color,
                eDotSize Line_width, eLineStyle Line_Style);
  auto DrawRectangle(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend,
                     UWORD Color, eDotSize Line_width, eDrawFilling Draw_Fill);
  auto DrawCircle(UWORD X_Center, UWORD Y_Center, UWORD Radius, UWORD Color,
                  eDotSize Line_width, eDrawFilling Draw_Fill);

  // Distring
  auto DrawChar(UWORD Xstart, UWORD Ystart, const char Acsii_Char, sFONT *Font,
                UWORD Color_Foreground, UWORD Color_Background);
  auto DrawString_EN(UWORD Xstart, UWORD Ystart, const char *pString,
                     sFONT *Font, UWORD Color_Foreground,
                     UWORD Color_Background);
  auto DrawNum(UWORD Xpoint, UWORD Ypoint, double Nummber, sFONT *Font,
               UWORD Digit, UWORD Color_Foreground, UWORD Color_Background);
  auto DrawTime(UWORD Xstart, UWORD Ystart, PaintTime *pTime, sFONT *Font,
                UWORD Color_Foreground, UWORD Color_Background);

  // pi
  auto DrawBitMap(const unsigned char *image_buffer);
  auto DrawBitMap_Block(const unsigned char *image_buffer, UBYTE Region);

  auto DrawImage(const unsigned char *image, UWORD xStart, UWORD yStart,
                 UWORD W_Image, UWORD H_Image);
  auto DrawImage1(const unsigned char *image, UWORD xStart, UWORD yStart,
                  UWORD W_Image, UWORD H_Image);
  auto BmpWindows(unsigned char x, unsigned char y, const unsigned char *pBmp,
                  unsigned char chWidth, unsigned char chHeight);
};

#endif
