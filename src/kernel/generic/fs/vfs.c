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

static struct vfs *fs;

bool vfs_match_name(void *node, void *name)
{
    return !strcmp(((struct fs_node *) node)->file->name, name);
}

/**
 * @brief Mount node at some path
 *
 * Mounts some node at a certain virtual path, where
 * it can be accessed through the vfs.
 *
 * @param path (absolute) path (starts with '/' and ends with '/')
 * @param node node to be mounted
 */
void vfs_mount(const char *path, struct fs_node *node)
{
    /* Check if path is absolute */
    UNUSED(node);
    if (path[0] != '/' || !path) {
        pr_err("vfs: calls to vfs_mount must use absolute paths!");
        return;
    }
}

/**
 * @brief Mount root
 */
void vfs_mount_root(void)
{
    fs = kmalloc(sizeof(struct vfs));
    fs->tree = tree();
    fs->tree->root->data = kmalloc(sizeof(struct fs_node));
    struct fs_node *root = fs->tree->root->data;
    root->file = kmalloc(sizeof(struct vfs_file));
    root->file->name = "root";
    root->ioctl = NULL;
    root->write = NULL;
    root->read = NULL;
}

/**
 * @brief Initialise vfs
 */
void vfs_init(void)
{
    vfs_mount_root();
}
