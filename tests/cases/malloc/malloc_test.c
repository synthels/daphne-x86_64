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

#include "malloc_test.h"

test_result_t *malloc_test(void)
{
    for (int i = 0; i < MALLOC_TEST_RUNS; i++) {
        int *ptr = kmalloc(sizeof(int) * (i + 1));
        if (!ptr)
            return test_result(TEST_FAILED, "allocated memory is null");
        if (!kfree(ptr))
            return test_result(TEST_SUCCESS, "kfree couldn't free memory");
    }

    return test_result(TEST_SUCCESS, "");
}
