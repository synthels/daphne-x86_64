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

bool vfs_match_name(struct tree_node *node, void *name)
{
    pr_warn("matching %s against %s", ((struct fs_node *) node)->file->name, name); // this is cucked??
    return 0;//!strncmp(((struct fs_node *) node)->file->name, name, VFS_MAX_FILE_NAME);
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
    if (path[0] != '/' || !path) {
        pr_err("vfs: calls to vfs_mount must use absolute paths!");
        return;
    }

    /* Parse path */
    struct tree_node *vfs_level = fs->tree->root;
    char *buf = kmalloc(VFS_MAX_FILE_NAME);
    for (int i = 0, j = 0; path[i]; ++i, ++j) {
        char p = path[i];
        if (p == '/') {
            j = -1;
            if (i > 0) {
                buf[i] = 0;
                pr_err("%s", buf);
                struct tree_node *child;
                if (!(child = tree_match_child(vfs_level, buf, vfs_match_name))) {
                    struct fs_node *n = kmalloc(sizeof(struct fs_node));
                    n->file = kmalloc(sizeof(struct vfs_file));
                    n->file->name = kmalloc(VFS_MAX_FILE_NAME);
                    strcpy(n->file->name, buf);
                    child = tree_node(n);
                    tree_insert_child(vfs_level, child);
                }
                memset(buf, 0, VFS_MAX_FILE_NAME);
                vfs_level = child;
            }
        } else {
            buf[j] = p;
        }
    }
    kfree(buf);
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
    root->flags = VFS_IS_PSEUDOFILE;
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
