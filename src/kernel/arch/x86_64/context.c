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
 * Task contexts (x64)
 */

#include "context.h"

struct _Context *init_context(size_t heap, uint64_t stack)
{
    struct _Context *c = kmalloc(sizeof(struct _Context));
    c->page_table = vmalloc(heap);
    c->regs = kmalloc(sizeof(regs_t));
    c->regs->rsp = stack;
    return c;
}
