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
 * Task contexts (x86_64)
 */

#include "context.h"

struct context *init_context(void)
{
    struct context *c = kmalloc(sizeof(struct context));
    c->page_table = vmalloc();
    c->regs = kmalloc(sizeof(regs_t));
    c->regs->cs = KERNEL_CS;
    c->regs->ss = KERNEL_CS;
    return c;
}

void destroy_context(struct context *c)
{
    /* TODO: vfree page table */
    kfree(c->regs);
    kfree(c);
}
