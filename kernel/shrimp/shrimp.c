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
 * and every X is a number between 0-9. (can also be any ASCII character,
 * in that case it will be interpreted as c - 48). The special value
 * \xff[1ffffff] resets to FG_COLOR
 *
 * The final (R,G,B) color is calculated by multiplying
 * the first Y with the first X (R), the 2nd Y with the second X (G)
 * and the third Y with the thrid X (B).
 *
 * WARNING: be very careful with escape sequences. After seeing
 *          \xff in a string, the terminal expects exactly 8
 *          characters right after. If it does not get them,
 *          very bad things could happen and heap overflows
 *          could occur
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
void ansi_parse(char *str, struct color *color, bool reset)
{
    static size_t saved_index = 0;
    size_t _strlen = strlen(str);
    /* Start parsing new string */
    if (reset) { 
        saved_index = 0;
        return;
    }
    /* No more string left to parse */
    if (saved_index >= _strlen) return;
    for (size_t j = saved_index; j < _strlen; j++) {
        /* Parse escape sequences */
        char c = str[j];
        if (c == ESCAPE_SEQ) {
            ++j; /* skip bracket */
            for (size_t k = j; k < _strlen; k++) {
                c = str[k];
                switch (c) {
                    case ESCAPE_OVERWRITE:
                        /* Overwrite current line */
                        if (shrimp_y > 0) {
                            /* Cut the escape sequence */
                            strcut(str, k-2, 9);
                            /* Free overwritten line */
                            kfree(shrimp_buf[--shrimp_index]);
                            shrimp_buf[shrimp_index] = str;
                            shrimp_update(); /* Note: no, this doesn't cause infinite recursion */
                        }
                        break;
                    case ESCAPE_COLOR_PRINT:
                        if (!memcmp(&str[k], "1ffffff", 7)) {
                            *color = FG_COLOR;
                            return;
                        }
                        /* Calculate color */
                        color->r = ((int) str[k+1] - 48) * ((int) str[k+4] - 48);
                        color->g = ((int) str[k+2] - 48) * ((int) str[k+5] - 48);
                        color->b = ((int) str[k+3] - 48) * ((int) str[k+6] - 48);
                        color->a = 255;
                        /* Save the index right after the escape in the string, so that
                           we can continue where we left off last time */
                        saved_index = k + 5;
                        return;
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

void shrimp_putc(char a, struct color c)
{
    /* We don't really need tabs */
    if (a == '\t') return;
    /* Change row for newlines */
    if (a == '\n') {
        impl_newln = 0;
        shrimp_x   = 0;
        shrimp_y++;
        /* Scroll terminal if it fills up */
        if (shrimp_y >= ((ctx_height / FONT_HEIGHT))) {
            shrimp_y = ((ctx_height / FONT_HEIGHT)) - 1;
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
    _shrimp_putc(a, shrimp_x++, shrimp_y, c);
}

/* Update terminal */
void shrimp_update(void)
{
    /* Clear terminal */
    shrimp_clear();
    /* Flush buffer */
    for (size_t i = 0; i < shrimp_index; i++) {
        struct color color = FG_COLOR;
        char *str = shrimp_buf[i];
        /* Set on implicit newline */
        bool newline = false;
        for (size_t j = 0; j < strlen(str); j++) {
            /* Parse "ansi" sequences */
            /* A bit hacky to make colors work correctly, oh well! */
            if (str[j] == ESCAPE_SEQ) {
                ansi_parse(str, &color, false);
                if (str[j + 2] == ESCAPE_COLOR_PRINT) {
                    j += 9;
                }
            }
            /* Go to next line when this line is filled up */
            if ((impl_newln > ((ctx_width / FONT_WIDTH) - FBTERM_OFFSET)) && str[j] != '\n') {
                shrimp_putc('\n', FG_COLOR);
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
            shrimp_putc(str[j], color);
            impl_newln++;
        }
        ansi_parse(str, &color, true);
    }
}

void shrimp_print(char *str)
{
    /* Add string to buffer */
    shrimp_buf[shrimp_index++] = str;
    /* Print string! */
    shrimp_update();
}
