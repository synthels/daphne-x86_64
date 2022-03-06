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

#include "vec.h"

static struct object_pool *vec_pool;

struct vector *vector(void)
{
    static bool _first_vec = true;
    if (_first_vec) {
        vec_pool = pool_create("vectors", NULL, sizeof(void *) * VEC_CAPACITY);
    }
    struct vector *v = kmalloc(sizeof(struct vector));
    v->data = pool_alloc(vec_pool);
    v->_Size = VEC_CAPACITY;
    v->size = 0;
    return v;
}

void vec_resize(struct vector *v)
{
    v->_Size += VEC_CAPACITY;
    v->data = pool_realloc(vec_pool, v->data, sizeof(void *) * v->_Size);
}

void vec_push(struct vector *v, void *e)
{
    if (v->size >= v->_Size) {
        vec_resize(v);
    }
    v->data[v->size++] = e;
}

void *vec_get(struct vector *v, size_t i)
{
    if (i < v->size) {
        return v->data[i];
    } else {
        return NULL;
    }
}

void vec_free(struct vector *v)
{
    pool_free(v->data);
    kfree(v);
}
