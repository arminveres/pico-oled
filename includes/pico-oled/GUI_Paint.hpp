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
  u16 Year;  // 0000
  u8 Month; // 1 - 12
  u8 Day;   // 1 - 30
  u8 Hour;  // 0 - 23
  u8 Min;   // 0 - 59
  u8 Sec;   // 0 - 59
};

/**
 * Image attributes
 **/
struct Paint {
  u8 *Image;
  u16 Width;
  u16 Height;
  u16 WidthMemory;
  u16 HeightMemory;
  u16 Color;
  eRotation m_rotation;
  u16 Mirror;
  u16 WidthByte;
  u16 HeightByte;
  u16 Scale;

  /// Init and create new image
  ///
  /// @param:
  ///     image   :   Pointer to the image cache
  ///     width   :   The width of the picture
  ///     Height  :   The height of the picture
  ///     Color   :   Whether the picture is inverted
  auto create_image(u8 *image, u16 Width, u16 Height, eRotation rotation,
                    u16 Color);
  /// Select Image
  auto select_image(u8 *image);

  auto set_rotation(const eRotation rotation);

/******************************************************************************
function:	Select Image mirror
parameter:
    mirror   :Not mirror,Horizontal mirror,Vertical mirror,Origin mirror
******************************************************************************/
  auto SetMirroring(u8 mirror);
  auto SetPixel(u16 Xpoint, u16 Ypoint, u16 Color);
  auto SetScale(u8 scale);

  auto Clear(u16 Color);
  auto ClearWindows(u16 Xstart, u16 Ystart, u16 Xend, u16 Yend,
                    u16 Color);

  // Dr
  auto DrawPoint(u16 Xpoint, u16 Ypoint, u16 Color, eDotSize Dot_Pixel,
                 eDotStyle Dot_FillWay);
  auto DrawLine(u16 Xstart, u16 Ystart, u16 Xend, u16 Yend, u16 Color,
                eDotSize Line_width, eLineStyle Line_Style);
  auto DrawRectangle(u16 Xstart, u16 Ystart, u16 Xend, u16 Yend,
                     u16 Color, eDotSize Line_width, eDrawFilling Draw_Fill);
  auto DrawCircle(u16 X_Center, u16 Y_Center, u16 Radius, u16 Color,
                  eDotSize Line_width, eDrawFilling Draw_Fill);

  // Distring
  auto DrawChar(u16 Xstart, u16 Ystart, const char Acsii_Char, sFONT *Font,
                u16 Color_Foreground, u16 Color_Background);
  auto DrawString_EN(u16 Xstart, u16 Ystart, const char *pString,
                     sFONT *Font, u16 Color_Foreground,
                     u16 Color_Background);
  auto DrawNum(u16 Xpoint, u16 Ypoint, double Nummber, sFONT *Font,
               u16 Digit, u16 Color_Foreground, u16 Color_Background);
  auto DrawTime(u16 Xstart, u16 Ystart, PaintTime *pTime, sFONT *Font,
                u16 Color_Foreground, u16 Color_Background);

  // pi
  auto DrawBitMap(const unsigned char *image_buffer);
  auto DrawBitMap_Block(const unsigned char *image_buffer, u8 Region);

  auto DrawImage(const unsigned char *image, u16 xStart, u16 yStart,
                 u16 W_Image, u16 H_Image);
  auto DrawImage1(const unsigned char *image, u16 xStart, u16 yStart,
                  u16 W_Image, u16 H_Image);
  auto BmpWindows(unsigned char x, unsigned char y, const unsigned char *pBmp,
                  unsigned char chWidth, unsigned char chHeight);
};

#endif
