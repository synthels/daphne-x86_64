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
 */

#ifndef FRAME_TREE
#define FRAME_TREE

#include <memory/mm.h>
#include <stdint.h>
#include <kernel.h>

/*
 * A frame tree is a tree that keeps track of whole 512B frames of memory.
 * They operate like simple binary trees and always have
 * a height of 3, tracking whole 512B regions and their
 * 256 hi-lo and 128-lhi-llo-rhi-rlo components
 */
struct frame_tree_node {
	uint32_t *addr;
	uint16_t value;
	uint8_t free;
	struct frame_tree_node *left;
	struct frame_tree_node *right;
};

#define NODES_PER_FRAME 15 /* (2^4)-1 */
#define ALLOC_NODE(node) node = (struct frame_tree_node *) palloc(sizeof(struct frame_tree_node) * NODES_PER_FRAME)

/* Get the frame tree for the memory range (from, from + 512) */
void get_frame_tree(uint32_t from, uint32_t lower, uint32_t upper, struct frame_tree_node *tree);

#endif
