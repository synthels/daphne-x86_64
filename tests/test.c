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
 *
 * Unit tests
 */

#include "test.h"

/* tests... (can't put these in the header file) */
#include "cases/malloc/malloc_test.h"
/* ... */

void run_unit_tests(void)
{
    unit_test("malloc test", malloc_test);
}

void unit_test(const char *name, test_t test)
{
    test_result_t *res = test();
    if (!res->result) {
        log_test("test \"%s\" failed with error \"%s\"", name, res->err);
        panic("test failed, kernel hung");
    }
    log_test("test \"%s\" passed", name);
}

test_result_t *test_result(bool result, char *err)
{
    test_result_t *res = kmalloc(sizeof(test_result_t));
    res->result = result;
    res->err = err;
    return res;

    /* Since the kernel hangs after the tests complete, we don't need to free the results! */
}
