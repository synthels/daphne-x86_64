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
	for (int i = 0; i < MALLOC_TESTS; i++) {
		struct test_struct *ptr = kmalloc(sizeof(struct test_struct));
		ptr->a = i;
		ptr->b = i + 1;
		ptr->c = i + 2;

		struct test_struct *ptr1 = kmalloc(sizeof(struct test_struct));
		ptr1->a = i;
		ptr1->b = i + 2;
		ptr1->c = i + 3;

		struct test_struct *ptr2 = kmalloc(sizeof(struct test_struct));
		ptr2->a = i;
		ptr2->b = i + 3;
		ptr2->c = i + 4;

		struct test_struct *ptr3 = kmalloc(sizeof(struct test_struct));
		ptr3->a = i;
		ptr3->b = i + 4;
		ptr3->c = i + 5;

		/* Check if values are correct */
		TEST_ASSERT(ptr->a == i, "kmalloc() test (a1)");
		TEST_ASSERT(ptr->b == i + 1, "kmalloc() test (b1)");
		TEST_ASSERT(ptr->c == i + 2, "kmalloc() test (c1)");

		TEST_ASSERT(ptr1->a == i, "kmalloc() test (a2)");
		TEST_ASSERT(ptr1->b == i + 2, "kmalloc() test (b2)");
		TEST_ASSERT(ptr1->c == i + 3, "kmalloc() test (c2)");

		TEST_ASSERT(ptr2->a == i, "kmalloc() test (a3)");
		TEST_ASSERT(ptr2->b == i + 3, "kmalloc() test (b3)");
		TEST_ASSERT(ptr2->c == i + 4, "kmalloc() test (c3)");

		TEST_ASSERT(ptr3->a == i, "kmalloc() test (a4)");
		TEST_ASSERT(ptr3->b == i + 4, "kmalloc() test (b4)");
		TEST_ASSERT(ptr3->c == i + 5, "kmalloc() test (c4)");

		/* Allocate another arbitrarily sized page as a sanity check */
		kmalloc(sizeof(struct test_struct) * 23);

		kfree(ptr);
		kfree(ptr1);
		kfree(ptr2);
		kfree(ptr3);
	}

	/* String test */
	char *str = kmalloc(11);
	str[0] = 'T';
	str[1] = 'E';
	str[2] = 'S';
	str[3] = 'T';
	str[4] = ' ';
	str[5] = 'P';
	str[6] = 'A';
	str[7] = 'S';
	str[8] = 'S';
	str[9] = 'E';
	str[10] = 'D';

	printk("If the following string: \"%s\" reads \"TEST PASSED\", kmalloc is probably working!", str);
}
