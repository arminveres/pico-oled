/**
 ******************************************************************************
 * @file    fonts.h
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    18-February-2014
 * @brief   Header for fonts.c file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without
 *modification, are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *notice, this list of conditions and the following disclaimer in the
 *documentation and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 * @author      Armin Veres (@arminveres)
 * @date        11-February-2024
 * @attention   Updated to cpp
 */

#ifndef __PICO_OLED_FONTS_HPP
#define __PICO_OLED_FONTS_HPP

#include "types.hpp"

namespace pico_oled::font {

#define OFFSET_BITMAP
constexpr auto MAX_HEIGHT_FONT = 41;
constexpr auto MAX_WIDTH_FONT = 32;

// ASCII
struct Font {
    const u8 *table;
    u16 Width;
    u16 Height;
};

extern Font Font24;
extern Font Font20;
extern Font Font16;
extern Font Font12;
extern Font Font8;

}  // namespace pico_oled::font

#endif /* __FONTS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
