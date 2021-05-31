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
 * Malloc test
 */

#include "malloc_test.h"

void do_malloc_test(void)
{
	for (int i = 0; i < 1; i++) {
		struct test_struct *ptr = kmalloc(sizeof(struct test_struct));
		ptr->a = i;
		ptr->b = i + 1;
		ptr->c = i + 2;

		struct test_struct *ptr1 = kmalloc(sizeof(struct test_struct));
		ptr1->a = i;
		ptr1->b = i + 3;
		ptr1->c = i + 45;

		/* Check if values are correct */
		TEST_ASSERT(ptr->a == i, "Malloc test (a)");
		TEST_ASSERT(ptr->b == i + 1, "Malloc test (b)");
		TEST_ASSERT(ptr->c == i + 2, "Malloc test (c)");

		TEST_ASSERT(ptr1->a == i, "Malloc test (a)");
		TEST_ASSERT(ptr1->b == i + 3, "Malloc test (b)");
		TEST_ASSERT(ptr1->c == i + 45, "Malloc test (c)");
	}
}
