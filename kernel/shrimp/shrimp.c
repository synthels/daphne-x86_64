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
 * Shrimp - early kernel terminal
 */

/* TODO: ANSI (like) Escape sequences */

#include "shrimp.h"

/* Context info */
static int handle;
static uint16_t ctx_width, ctx_height;
static uint16_t shrimp_x = 0, shrimp_y = 0;

/* Terminal buffer */
static char **shrimp_buf;
static size_t shrimp_index = 0;
static int impl_newln = 0;

/* Clear terminal */
void shrimp_clear(void)
{
    shrimp_x = 0;
    shrimp_y = 0;
    const struct color c = BG_COLOR;
    for (uint16_t y = 0; y < ctx_height; y++) {
        for (uint16_t x = 0; x < ctx_width; x++) {
            lfb_set_pixel(handle, x, y, c);
        }
    }
}

/* Init fbterm */
void shrimp_init(int _handle)
{
    handle = _handle;
    struct lfb_info info;
    lfb_get_ctx_info(_handle, &info);
    ctx_width  = info.screen_width;
    ctx_height = info.screen_height;
    /* Allocate terminal buffer */
    shrimp_buf = kmalloc((((ctx_height / FONT_HEIGHT) - FBTERM_OFFSET) * sizeof(char *)));

    /* Clear terminal */
    shrimp_clear();
}

/* Free terminal buffer */
void shrimp_kill(void)
{
    kfree(shrimp_buf);
    shrimp_clear();
}

/* Put character at (x,y) */
void _shrimp_putc(char a, uint16_t _x, uint16_t _y, struct color c)
{
    const uint16_t *bmp = shrimp_font[(int) a];
    for (uint8_t y = 0; y < FONT_HEIGHT; y++) {
        for (uint8_t x = 0; x < FONT_WIDTH; x++) {
            if (bmp[y] & (1 << (15-x)))
                lfb_set_pixel(handle, FBTERM_OFFSET + x + (_x * FONT_WIDTH), FBTERM_OFFSET + y + (_y * FONT_HEIGHT), c);
        }
    }
}

void shrimp_putc(char a)
{
    /* We don't really need tabs */
    if (a == '\t') return;
    /* Change row for newlines */
    if (a == '\n') {
        impl_newln = 0;
        shrimp_x   = 0;
        shrimp_y++;
        /* Scroll terminal if it fills up */
        if (shrimp_y >= ((ctx_height / FONT_HEIGHT) - FBTERM_OFFSET)) {
            shrimp_y = ((ctx_height / FONT_HEIGHT) - FBTERM_OFFSET) - 2;
            shrimp_index = shrimp_y;
            /* Move all lines one line up */
            for (int i = shrimp_y; i > 0; i--) {
                shrimp_buf[i-1] = shrimp_buf[i];
            }
        }
        return;
    }

    /* Print character */
    struct color c = FG_COLOR;
    _shrimp_putc(a, shrimp_x++, shrimp_y, c);
}

/* Update terminal */
void shrimp_update(void)
{
    /* Clear terminal */
    shrimp_clear();
    /* Flush buffer */
    for (size_t i = 0; i < shrimp_index; i++) {
        char *str = shrimp_buf[i];
        /* Set on implicit newline */
        bool newline = false;
        for (size_t j = 0; j < strlen(str); j++) {
            /* Go to next line when this line is filled up */
            if ((impl_newln > ((ctx_width / FONT_WIDTH) - FBTERM_OFFSET)) && str[j] != '\n') {
                shrimp_putc('\n');
                newline = true;
                impl_newln = 0;
            }
            /* Skip space after newline */
            if (j > 0) {
                if (((str[j-1] == '\n') || newline) && str[j] == ' ') {
                    newline = false;
                    continue;
                }
            }
            /* Draw character */
            shrimp_putc(str[j]);
            impl_newln++;
        }
    }
}

void shrimp_print(char *str)
{
    /* Add string to buffer */
    shrimp_buf[shrimp_index++] = str;
    /* Print string! */
    shrimp_update();
}
