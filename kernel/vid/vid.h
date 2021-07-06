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

#ifndef VID_
#define VID_

#include <stdint.h>
#include <mem/malloc.h>
#include <kernel.h>

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

struct vid_info {
	uint16_t screen_width;
	uint16_t screen_height;
	uint16_t screen_pitch;
};

/**
 * vid_init
 *   brief: init video service
 */
void vid_init(uint16_t _width, uint16_t _height, uint64_t _framebuffer, uint16_t _pitch);

/**
 * vid_init
 *   brief: get video info
 */
void vid_get_info(struct vid_info *info);

/**
 * vid_create_ctx
 *   brief: create context
 */
errcode_t vid_create_ctx(struct gfx_context *ctx, struct pos _pos, uint16_t width, uint16_t height);

/**
 * vid_destroy_ctx
 *   brief: destroy context
 */
errcode_t vid_destroy_ctx(int handle);

/**
 * vid_set_pixel
 *   brief: set pixel at position (x, y) in context
 *          ctx
 */
errcode_t vid_set_pixel(int handle, uint16_t x, uint16_t y, struct color c);

#endif
