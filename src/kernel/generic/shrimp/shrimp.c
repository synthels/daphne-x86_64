/*
 * Copyright (C) 2022 synthels <synthels.me@gmail.com>
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
 *
 * Our ANSI-like escape sequences:
 * format: \xff[XXXXXXX
 *         where every X is any number 
 *         between 1-9.
 *
 * Printing with colors:
 * \xff[1XXXXXX
 * Where XXXXXX is the hex code of the color. The special value
 * \xff[1ffffff resets to FG_COLOR (not always white!)
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
static uint64_t lfb_address;

static int impl_newln = 0;

static uint32_t hextol(char *hex)
{
    uint32_t val = 0;
    for (size_t i = 0; i < 6; i++) {
        uint8_t byte = *hex++; 
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;    
        val = (val << 4) | (byte & 0xF);
    }

    return val;
}

void shrimp_update(char *str);

/**
 * @brief parse these "ANSI" escape sequences
 *
 * Called by shrimp_update every time an 
 * escape sequence is found.
 *
 * @param str the string to be parsed
 * @param color if a color sequence is found, the color is put here
 * @param reset set when a new string is ready to be parsed
 */
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
                    case ESCAPE_COLOR_PRINT:
                        if (!memcmp(&str[k], "1ffffff", 7)) {
                            *color = FG_COLOR;
                            return;
                        }
                        /* Calculate color */
                        uint32_t hex = hextol(&str[k+1]);
                        color->r = ((hex >> 16) & 0xFF);
                        color->g = ((hex >> 8) & 0xFF);
                        color->b = ((hex) & 0xFF);
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

/**
 * @brief Clear the framebuffer
 */
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

/**
 * @brief Initialise shrimp
 */
void shrimp_init(int _handle)
{
    handle = _handle;
    struct lfb_info info;
    lfb_get_ctx_info(_handle, &info);
    ctx_width  = info.screen_width;
    ctx_height = info.screen_height;
    lfb_address = lfb_get_addr();
    /* Clear terminal */
    shrimp_clear();
}

/**
 * @brief Free terminal buffer
 */
void shrimp_kill(void)
{
    shrimp_clear();
}

/**
 * @brief Put character at (x, y)
 */
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

/**
 * @brief Put character at (x, y)
 *
 * Renders character-by-character & is
 * wrap aware
 */
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
            memmove((uint64_t *) lfb_address, (uint64_t *) (lfb_address + sizeof(uint32_t) * ctx_width * FONT_HEIGHT), (ctx_height - FONT_HEIGHT) * ctx_width * 4);
            memset((uint64_t *) (lfb_address + sizeof(uint32_t) * (ctx_height - FONT_HEIGHT) * ctx_width), 0x00, FONT_HEIGHT * ctx_width * 4);
        }
        return;
    }

    /* Print character */
    _shrimp_putc(a, shrimp_x++, shrimp_y, c);
}

/**
 * @brief Print new string
 *
 * Draws new string while taking care of escape sequences & scrolling
 */
void shrimp_update(char *str)
{
    /* Flush buffer */
    struct color color = FG_COLOR;
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
        /* FIX: setting this fixes some scrolling issues */
        newline = false;
        impl_newln++;
    }
    ansi_parse(str, &color, true);
}

/**
 * @brief Add string to buffer & update
 */
int shrimp_print(const char *str)
{
    shrimp_update((char *) str);
    return 0;
}
