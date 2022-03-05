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
 * Low level context manager
 */

/* TODO: back buffer */

#include "lfb.h"

static uint64_t framebuffer;
static uint16_t width, height, pitch;

/* Graphics contexts */
static struct gfx_context **contexts;
/* Last context handle */
static int last_handle = 0;

void lfb_init(uint16_t _width, uint16_t _height, uint64_t _framebuffer, uint16_t _pitch)
{
    framebuffer = _framebuffer;
    width       = _width;
    height      = _height;
    pitch       = _pitch;

    /* Allocate contexts */
    contexts = kmalloc(MAX_CONTEXTS * sizeof(struct gfx_context *));
}

void lfb_get_info(struct lfb_info *info)
{
    info->screen_width  = width;
    info->screen_height = height;
    info->screen_pitch  = pitch;
}

void lfb_get_ctx_info(int handle, struct lfb_info *info)
{
    struct gfx_context *ctx = contexts[handle];
    info->screen_width  = ctx->width;
    info->screen_height = ctx->height;
}

int lfb_create_ctx(struct gfx_context *ctx, struct pos _pos, uint16_t _width, uint16_t _height)
{
    if (ctx->handle >= MAX_CONTEXTS || _pos.x > width || _pos.y > height) return -1;
    ctx->pos    = _pos;
    ctx->width  = _width;
    ctx->height = _height;
    ctx->handle = last_handle;
    contexts[last_handle++] = ctx;
    return 0;
}

uint64_t lfb_get_addr(void)
{
    return framebuffer;
}

int lfb_destroy_ctx(int handle)
{
    if (handle >= MAX_CONTEXTS) return -1;
    contexts[handle] = NULL;
    return 0;
}

int lfb_set_pixel(int handle, uint16_t x, uint16_t y, struct color c)
{
    if (handle > MAX_CONTEXTS) return -1;
    struct gfx_context *ctx = contexts[handle];
    
    /* Destroyed context */
    if (ctx == NULL) return -1;
    if (x <= ctx->width && y <= ctx->height) {
        const uint64_t realpos = (ctx->pos.x + x) + (width * (y + ctx->pos.y));
        struct color *frb = (struct color *) framebuffer;
        frb[realpos] = c;
        return 0;
    } else {
        /* invalid position */
        return -1;
    }
}
