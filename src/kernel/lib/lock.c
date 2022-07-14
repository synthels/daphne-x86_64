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
 * Synchronization primitives
 */

#include "lock.h"

void lock(mutex_t *mutex)
{
    asm volatile("cli");

    while(!__sync_bool_compare_and_swap(mutex, 0, 1)) {
        asm("pause");
    }
    __sync_synchronize();
}

void unlock(mutex_t *mutex)
{
    asm volatile("sti");

    *mutex = 0;
}
