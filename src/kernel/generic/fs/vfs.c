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

/* Filesystem tree */
struct tree *fs;

/**
 * @brief Match tree node child with filename
 *
 * Passed to tree_match_child for file matching
 *
 * @param parent file fs_node structure
 * @param node data file name
 */
bool vfs_match_name(void *file, void *data)
{
    return !strncmp(((struct fs_node *) file)->name, data, VFS_MAX_FILE_NAME);
}

/**
 * @brief Create node or traverse
 *
 * Creates file or traverses it if it already exists
 *
 * @param parent parent node
 * @param node name file name
 */
struct tree_node *create_or_traverse(struct tree_node *parent, char *name, struct fs_node *opt)
{
    struct tree_node *node;
    if (!(node = tree_match_child(parent, name, vfs_match_name))) {
        /* If opt was passed as NULL, it means that vfs_mount doesn't want
           any new files to be created. */
        if (!opt)
            return NULL;

        /* Create node */
        struct fs_node *file = kmalloc(sizeof(struct fs_node));
        file->name = kmalloc(VFS_MAX_FILE_NAME);
        file->type = FIS_DIRECTORY;
        strncpy(file->name, name, VFS_MAX_FILE_NAME);
        return tree_insert_child(parent, file);
    }
    return node;
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
struct fs_node *vfs_mount(const char *path, struct fs_node *node)
{
    /* FIXME: Doesn't check if the path ends with a slash, which
       is required! */
    if (path[0] != '/' || !path) {
        pr_err("vfs: calls to vfs_mount must use absolute paths!");
        return NULL;
    }

    char buf[VFS_MAX_FILE_NAME];
    struct tree_node *level = fs->root;
    for (int i = 0, j = 0; path[i]; i++, j++) {
        char p = path[i];
        if (p == '/') {
            if (i > 0) {
                buf[j] = 0;
                /* Return if create_or_traverse returns NULL! (only when node is NULL) */
                if (!(level = create_or_traverse(level, buf, node))) {
                    pr_err("vfs_mount: %s: no node mounted on this path", path);
                    return NULL;
                }
                memset(buf, 0, VFS_MAX_FILE_NAME);
            }
            /* Reset buf index to -1, so that it gets reset to 0
               in the next iteration! */
            j = -1;
        } else {
            buf[j] = p;
        }
    }

    /* Now we have traversed the tree and we can mount
       our node! */
    if (node) {
        tree_insert_child(level, node);
        return node;
    } else {
        return (struct fs_node *) level->data;
    }
}

struct fs_node *vfs_open(const char *path)
{
    /* Passing NULL to vfs_mount makes it
       return the node mounted at this
       path, without creating any new nodes */
    return vfs_mount(path, NULL);
}

/**
 * @brief Mount vfs root
 */
void vfs_mount_root(void)
{
    fs = tree();
    struct fs_node *root = kmalloc(sizeof(struct fs_node));
    root->name = NULL;
    root->type = FIS_ROOT;

    root->ioctl = NULL;
    root->open = NULL;
    root->write = NULL;
    root->read = NULL;
    fs->root = tree_node(root);
}

void vfs_init(void)
{
    vfs_mount_root();
}
