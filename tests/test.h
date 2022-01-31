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

#include <lib/printk.h>
#include <generic/forbia/types.h>
#include <generic/malloc/malloc.h>

typedef struct {
    bool result;
    char *err;
} test_result_t;

typedef test_result_t *(*test_t)(void);

#define TEST_SUCCESS true
#define TEST_FAILED  false

#define TEST_ASSERT(P, msg) if (!(P)) return test_result(TEST_FAILED, msg)

/**
 * run_unit_tests
 *   brief: run all tests
 */
void run_unit_tests(void);

/**
 * unit_test
 *   brief: run unit test
 *   parameters:
 *     - name: test name
 *     - test: testcase
 */
void unit_test(const char *name, test_t test);

/**
 * test_result
 *   brief: return test result
 */
test_result_t *test_result(bool result, char *err);
