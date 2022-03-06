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
 */

#pragma once


#include <stddef.h>
#include <stdint.h>

#include <generic/forbia/kernel.h>
#include <lib/panic.h>
#include <lib/lock.h>
#include <generic/memory/mem.h>

#include <lib/string.h>

#include "object.h"
#include "mmu.h"

#define PAGE_SIZE 4096
#define SLAB_PAGES_INITIAL 12
#define SLAB_MAX_SIZE SIZE_MAX

/* Magic number for headers */
#define MALLOC_HEADER_MAGIC 0xcafebabe

struct malloc_page {
    struct malloc_page *next;
    uint64_t *base;
};

struct malloc_slab {
    struct malloc_slab *next;
    struct malloc_page *pages;
    bool realloc;
    size_t length;
    size_t size;
};

struct malloc_header {
    uint64_t magic;
    size_t true_size;
    struct malloc_slab *slab;
    struct malloc_page *page;
};

struct object_pool {
    const char *name;
    struct malloc_slab *slab;
    obj_ctor ctor;
};

/**
 * kmalloc
 *   brief: attempt to allocate a page on the kmalloc_slab of size n
 */
void *kmalloc(size_t n);

/**
 * krealloc
 *   brief: reallocate pointer 
 *          (UNSAFE: does not check if ptr.size - size < 0)
 */
void *krealloc(void *ptr, size_t size);

/**
 * kfree
 *   brief: attempt to free memory
 *          allocated by kmalloc
 */
void *kfree(void *ptr);

/**
 * pool_create
 *   brief: create pool for object
 */
struct object_pool *pool_create(const char *name, obj_ctor ctor, size_t size);

/**
 * pool_alloc
 *   brief: allocate from object pool
 */
void *pool_alloc(struct object_pool *pool);

/**
 * pool_realloc
 *   brief: reallocate object
 */
void *pool_realloc(struct object_pool *pool, void *ptr, size_t size);

/**
 * pool_free
 *   brief: free pool object
 */
void *pool_free(void *object);
