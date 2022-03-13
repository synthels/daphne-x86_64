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
#include <lib/list.h>

#include <generic/malloc/malloc.h>

struct tree_node {
    void *data;
    struct list *children;
};

struct tree {
    struct tree_node *root;
};

typedef bool (*tree_matching_func)(struct tree_node *, void *);

#define tree_children_foreach(node, it) list_foreach(node->children, it)
#define tree_child_get(child) ((struct tree_node *) child->data)->data

/**
 * tree
 *   brief: allocate new tree
 */
struct tree *tree(void);

/**
 * tree_match_child
 *   brief: get child for which match(p, d) rings true
 */
void *tree_match_child(struct tree_node *parent, void *data, tree_matching_func match);

/**
 * tree_insert_child
 *   brief: insert child to parent
 */
void tree_insert_child(struct tree_node *parent, void *data);

/**
 * tree_node
 *   brief: tree node constructor
 */
struct tree_node *tree_node(void *data);
