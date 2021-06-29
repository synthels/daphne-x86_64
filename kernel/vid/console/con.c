/*
 * Copyright (C) 2020 synthels <synthels.me@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * video interface
 */

#include "con.h"

void con_plot_px(int x, int y, uint32_t pixel, uint64_t fb_base, uint64_t pps)
{
	*((uint32_t *) (fb_base + 4 * pps * y + 4 * x)) = pixel;
}

// void con_plot_char(uint32_t *buffer, uint32_t x, uint32_t y, uint32_t color, char charcode)
// {
//     char p = (charcode * 128) - 128; // Size of a font's character
//     for (int l = 0; l < 16; l++) {
//         for (int c = 0; c < 8; c++) {
//             if (font_system_8x16[p] == 1) {
//                 con_plot_px(x + c, y + l, color, buffer, 0);
//             }
//             p++;
//         }
//     }
// }
