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

#include "struct Vectorest.h"

test_result_t *struct Vectorest(void)
{
    struct vector *v = vector();
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    vec_push_ptr(v, &a);
    vec_push_ptr(v, &b);
    vec_push_ptr(v, &c);
    vec_push_ptr(v, &d);

    TEST_ASSERT(vec_get_as(v, 1, int) == 2, "vector item incorrect");

    return test_result(TEST_SUCCESS, "");
}
