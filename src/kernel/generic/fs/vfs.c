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
 * Virtual filesystem
 */

#include "vfs.h"

static struct object_pool *file_pool;
static struct vfs_graph *graph;

/**
 * @brief Create initial vfs graph
 */
void vfs_create_graph(void)
{
    graph = kmalloc(sizeof(struct vfs_graph));
    graph->size = VFS_GRAPH_INITIAL_SIZE;
    graph->adj_lists = kmalloc(VFS_GRAPH_INITIAL_SIZE * sizeof(struct node *));

    for (int i = 0; i < VFS_GRAPH_INITIAL_SIZE; i++) {
        graph->adj_lists[i] = NULL;
    }
}

/**
 * @brief Initialise vfs
 */
void vfs_init(void)
{
    file_pool = pool_create("files", NULL, sizeof(struct vfs_file));
}
