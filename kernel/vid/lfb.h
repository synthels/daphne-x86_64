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
 */

#ifndef __LFB
#define __LFB

#include <stdint.h>
#include <malloc/malloc.h>
#include <forbia/kernel.h>

#define MAX_CONTEXTS 50

struct pos {
    uint16_t x;
    uint16_t y;
};

struct color {
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
} __attribute__((packed));

struct gfx_context {
    struct pos pos;
    uint16_t width;
    uint16_t height;
    int handle;
    int z_index; /* TODO :) */
};

struct lfb_info {
    uint16_t screen_width;
    uint16_t screen_height;
    uint16_t screen_pitch;
};

/**
 * lfb_init
 *   brief: init video service
 */
void lfb_init(uint16_t _width, uint16_t _height, uint64_t _framebuffer, uint16_t _pitch);

/**
 * lfb_init
 *   brief: get video info
 */
void lfb_get_info(struct lfb_info *info);

/**
 * lfb_get_ctx_info
 *   brief: get context info
 */
void lfb_get_ctx_info(int handle, struct lfb_info *info);

/**
 * lfb_create_ctx
 *   brief: create context
 */
int lfb_create_ctx(struct gfx_context *ctx, struct pos _pos, uint16_t _width, uint16_t _height);

/**
 * lfb_destroy_ctx
 *   brief: destroy context
 */
int lfb_destroy_ctx(int handle);

/**
 * lfb_set_pixel
 *   brief: set pixel at position (x, y) in context
 *          ctx
 */
int lfb_set_pixel(int handle, uint16_t x, uint16_t y, struct color c);

#endif
