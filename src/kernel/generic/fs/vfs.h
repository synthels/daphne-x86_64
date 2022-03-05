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

#define VFS_GRAPH_INITIAL_SIZE 10

#include <stdint.h>

#include <generic/malloc/malloc.h>

enum FS {
    /* TODO */
    EMPTY_ENUM = 0
};

struct fs_info {
    enum FS fs;
    /* Path, length, ... */
};

struct vfs_file {
    int permissions;    /* TODO */
    const char *name;   /* Limited to 255 */
    struct fs_info fs;
};

struct vfs_node {
    int vertex;
    struct vfs_file *file;
    struct vfs_node *next;
};

struct vfs_graph {
    size_t size;
    struct node **adj_lists;
};

/**
 * vfs_init
 *   brief: initialise vfs
 */
void vfs_init(void);
