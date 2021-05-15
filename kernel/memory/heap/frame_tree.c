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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Frame tree
 */

#include "frame_tree.h"

static size_t trees_allocated = 0;

/* Configures a single tree node */
static void fnode(uint32_t from, uint32_t size, uint32_t i, struct frame_tree_node *node)
{
	node->value = size;
	node->addr = (uint32_t *) (from + i * size);
	node->free = 1;
}

/* Allocate all nodes of a tree */
static void anode(struct frame_tree_node *tree)
{
	if (trees_allocated > MAX_TREES) {
		logk("memory/heap: couldn't allocate tree!");
		return;
	}

	ALLOC_NODE(tree->left);
	ALLOC_NODE(tree->right);
	ALLOC_NODE(tree->left->left);
	ALLOC_NODE(tree->left->right);
	ALLOC_NODE(tree->right->left);
	ALLOC_NODE(tree->right->right);
	trees_allocated++;
}

/* Get the frame tree for memory frame (from, from + 512) */
void tmap(uint32_t from, uint32_t lower, uint32_t upper, struct frame_tree_node *tree)
{
	/* Range out of bounds */
	if (from < lower || (from + 512 > upper)) {
		logk("memory/heap: frame is out of bounds!");
		return;
	}

	/*
	 * This is a very weird way of writing this, but
	 * to be fair it's very easy to read & understand
	 * and way faster than a for loop
	 */
	anode(tree);
	/* Track all 512 bytes */
	fnode(from, 512, 1, tree);
	/* Track low 256 & high 256 bytes */
	fnode(from, 256, 1, tree->left);
	fnode(from, 256, 2, tree->right);
	/* Track low left & right 128 bytes */
	fnode(from, 128, 1, tree->left->left);
	fnode(from, 128, 2, tree->left->right);
	/* Track high left & right 128 bytes */
	fnode(from, 128, 1, tree->right->left);
	fnode(from, 128, 2, tree->right->right);
}
