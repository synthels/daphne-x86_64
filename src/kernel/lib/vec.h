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

#include <generic/malloc/malloc.h>

#define VEC_CAPACITY 6

struct Vector {
    void **data;
    size_t size;
    size_t _Size; /* Actual internal size */
};

/**
 * vec_get_as
 *   brief: get element i as type T from v
 */
#define vec_get_as(v, i, T) *((T *) vec_get(v, i))

/**
 * vec_get_as_ptr
 *   brief: get element i as pointer of T from v
 */
#define vec_get_as_ptr(v, i, T) (T) vec_get(v, i)

/**
 * vec_foreach
 *   brief: loop over vector
 */
#define vec_foreach(v, it) for (size_t it = 0; it < v->size; ++it)

/**
 * vec_push_ptr
 *   brief: push element
 */
#define vec_push_ptr(v, e) vec_push(v, (void *) e);

/**
 * vector
 *   brief: allocate new vector
 */
struct Vector *vector(void);

/**
 * vec_push
 *   brief: push element to vector
 */
void vec_push(struct Vector *v, void *e);

/**
 * vec_get
 *   brief: get element from vector
 */
void *vec_get(struct Vector *v, size_t i);

/**
 * vec_free
 *   brief: free vector
 */
void vec_free(struct Vector *v);
