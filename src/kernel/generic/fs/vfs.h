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

#include <lib/tree.h>
#include <generic/forbia/errno.h>

#define VFS_MAX_FILE_NAME 256

typedef int   (*open_t) (const char *, int);
typedef void  (*close_t)(int);
typedef void  (*ioctl_t)(int, int);
typedef void  (*write_t)(const void *, size_t);
typedef void *(*read_t) (int, size_t);

enum fs_node_type {
    FIS_FILE = 0,
    FIS_ROOT = 1,
    FIS_DIRECTORY = 2,
    FIS_SOCKET = 3
};

struct fs_node {
    char  *name; /* Filename */
    int    type; /* File type */

    ioctl_t  ioctl;
    open_t   open;
    write_t  write;
    read_t   read;

    struct tree_node *vfs_ptr;
};

/**
 * vfs_init
 *   brief: init vfs
 */
void vfs_init(void);

/**
 * vfs_mount
 *   brief: mount vfs node
 */
struct fs_node *vfs_mount(const char *path, struct fs_node *node);

/**
 * vfs_unmount
 *   brief: unmount vfs node
 */
int vfs_unmount(const char *path);
