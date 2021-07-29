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

/**
 * Our ANSI-like escape sequences:
 * format: \xff[XXXXXXX
 *         where every X is any number 
 *         between 1-9.
 *
 * @ Overwriting the current line:
 * \xff[0000000
 * If this sequence is found anywhere in the string, the
 * current line will be overwritten with this string.
 *
 * @ Printing with colors:
 * \xff[1YYYXXX
 * where every Y is a multiplier value between 0-9
 * and every X is a number between 0-9.
 *
 * The final (R,G,B) color is calculated by multiplying
 * the first Y with the first X (R), the 2nd Y with the second X (G)
 * and the third Y with the thrid X (B).
 */

#include "shrimp.h"

/* Context info */
static int handle;
static uint16_t ctx_width, ctx_height;
static uint16_t shrimp_x = 0, shrimp_y = 0;

/* Terminal buffer */
static char **shrimp_buf;
static size_t shrimp_index = 0;
static int impl_newln = 0;

static int strcut(char *str, int begin, int len)
{
    int l = strlen(str);
    if (len < 0) len = l - begin;
    if (begin + len > l) len = l - begin;
    memmove(str + begin, str + begin + len, l - len + 1);
    return len;
}

void shrimp_update(void);

/* Parse "ANSI" sequences */
void ansi_parse(char *str)
{
    bool escape_found = false;
    for (size_t j = 0; j < strlen(str); j++) {
        /* Parse escape sequences */
        char c = str[j];
        if (c == ESCAPE_SEQ) escape_found = true; 
        if (escape_found) {
            ++j; /* skip bracket */
            for (size_t k = j; k < strlen(str); ++k) {
                c = str[k];
                switch (c) {
                    case ESCAPE_OVERWRITE:
                        /* Overwrite current line */
                        if (shrimp_y > 0) {
                            /* Cut the escape sequence */
                            strcut(str, k-2, 9);
                            /* Free overwritten line */
                            kfree(shrimp_buf[--shrimp_index]);
                            shrimp_buf[shrimp_index] = NULL;
                            shrimp_buf[shrimp_index] = str;
                            shrimp_update(); /* Note: no, this doesn't cause infinite recursion */
                        }
                        break;
                    case ESCAPE_COLOR_PRINT:
                        /* Print with color, TODO */
                        break;
                }
            }
        }
    }
}

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
            shrimp_y = ((ctx_height / FONT_HEIGHT) - FBTERM_OFFSET) - 1;
            shrimp_index = shrimp_y;
            /* Free topmost string (fixes the
               memory leaks) */
            kfree(shrimp_buf[0]);
            for (size_t i = 0; i < shrimp_y; i++) {
                shrimp_buf[i] = shrimp_buf[i+1];
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
        /* Parse "ansi" sequences */
        ansi_parse(str);
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
