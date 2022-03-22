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
 * N-ary tree
 */

#include "tree.h"

struct tree *tree(void)
{
    struct tree *t = kmalloc(sizeof(struct tree));
    t->root = kmalloc(sizeof(struct tree_node));
    t->root->children = list();
    return t;
}

struct tree_node *tree_match_child(struct tree_node *parent, void *data, tree_matching_func match)
{
    tree_children_foreach(parent, child) {
        if (match(tree_child_get(child), data)) {
            return child->data;
        }
    }
    return NULL;
}

struct tree_node *tree_insert_child(struct tree_node *parent, void *data)
{
    struct tree_node *node = tree_node(data);
    node->parent = parent;
    node->node = list_insert(parent->children, node);
    return node;
}

void tree_free_node(struct tree_node *node)
{
    static bool first = true;
    /* Free node and all its children */
    if (!node) return;
    /* Remove node from parent's children,
       making it inaccessible */
    if (first) {
        list_remove(node->parent->children, node->node);
        first = false;
    }
    /* Free node */
    tree_children_foreach(node, child) {
        tree_free_node(child->data);
    }
    kfree(node);
}

struct tree_node *tree_node(void *data)
{
    struct tree_node *node = kmalloc(sizeof(struct tree_node));
    node->data = data;
    node->node = NULL;
    node->children = list();
    return node;
}
