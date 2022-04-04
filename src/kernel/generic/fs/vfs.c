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

declare_lock(vfs_lock);

/* Filesystem tree */
struct tree *fs;

/* Filesystem hashmap */
struct hashmap *filesystems;

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
        file->type = FS_DIRECTORY;
        file->vfs_ptr = tree_insert_child(parent, file);
        strncpy(file->name, name, VFS_MAX_FILE_NAME);
        return file->vfs_ptr;
    }
    return node;
}

/**
 * @brief Mount node at some path
 *
 * Mounts some node at a certain virtual path, where
 * it can be accessed through the vfs.
 *
 * NOTE: Most UNIX vfs implementations would usually mount something like this:
 *
 *       node->name = "mynode"
 *       vfs_mount("/path/", node);
 *
 *       And mean that the node can now be accessed at /path/. With this
 *       vfs_mount, this node is actually mounted at /path/mynode, with
 *       /path/ being marked as a directory. If you want to mount node at
 *       /path/ instead, you'd do:
 *
 *       node->name = "path";
 *       vfs_mount("/", node);
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

    /* Maybe consider some multi-processor architecture 
       for the vfs? */
    lock(&vfs_lock);
    /* Caller wants node mouted on root! */
    if (!strncmp(path, "/", 2)) {
        node->vfs_ptr = fs->root;
        tree_insert_child(fs->root, node);
        unlock(&vfs_lock);
        return node;
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
                    unlock(&vfs_lock);
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
        if (node->fs) {
            struct fs_descriptor *filesystem = hashmap_get(filesystems, node->fs);
            if (filesystem) {
                filesystem->mount(path, node);
            }
        }
        node->vfs_ptr = tree_insert_child(level, node);
        node->ref = false;
        unlock(&vfs_lock);
        return node;
    } else {
        unlock(&vfs_lock);
        struct fs_node *ret = (struct fs_node *) level->data;
        return ret;
    }
    unlock(&vfs_lock);
}

/**
 * @brief Unmount node at some path
 *
 * Unmounts some node at a certain virtual path, freeing
 * all memory used by it
 *
 * @param path (absolute) path (starts with '/' and ends with '/')
 */
int vfs_unmount(const char *path)
{
    struct fs_node *node;
    if (!(node = vfs_mount(path, NULL))) {
        return -ENOENT;
    }
    if (node->vfs_ptr) {
        tree_free_node(node->vfs_ptr);
        return ENORM;
    } else {
        pr_err("vfs_unmount: %s not mounted!", path);
        return -ENOENT;
    }
}

/**
 * @brief Register a new filesystem
 *
 * @param fs Filesystem descriptor
 */
void vfs_register_fs(struct fs_descriptor *fs)
{
    if (!hashmap_get(filesystems, fs->name))
        hashmap_insert(filesystems, fs->name, fs);
}

/**
 * @brief Mount vfs root
 */
void vfs_mount_root(void)
{
    fs = tree();
    struct fs_node *root = kmalloc(sizeof(struct fs_node));
    root->name = NULL;
    root->type = FS_ROOT;
    root->fs = NULL;

    root->ioctl = NULL;
    root->open = NULL;
    root->write = NULL;
    root->read = NULL;
    fs->root = tree_node(root);
}

/**
 * @brief Open file
 *
 * Opens a vfs_node, allowing write and read calls
 * to go through
 */
int vfs_open(struct fs_node *node, int flags)
{
    if (node->ref) {
        return -EBUSY;
    } else {
        if (node->type == FS_DIRECTORY) {
            return -EISDIR;
        }
        node->ref = true;
        if (node->open)
            return node->open(node, flags);
        else
            return -EBADFD;
    }
}

/**
 * @brief Close file
 *
 * Closes a vfs_node
 */
int vfs_close(struct fs_node *node)
{
    if (node->ref && node->close) {
        int ret = node->close(node);
        node->ref = false;
        return ret;
    } else {
        return -EBADFD;
    }
}

/**
 * @brief Write to file
 *
 * @param offset Offset to start writing at
 * @param buffer Data to write
 * @param size Buffer size
 */
int fwrite(struct fs_node *node, size_t offset, void *buffer, size_t size)
{
    if (node->ref && node->write) {
        return node->write(node, offset, buffer, size);
    } else {
        return -EBADFD;
    }
}

/**
 * @brief Read from file
 *
 * @param offset Offset to start reading from
 * @param buffer Buffer to write data to
 * @param size Read size
 */
int fread(struct fs_node *node, size_t offset, void *buffer, size_t size)
{
    if (node->ref && node->read) {
        return node->read(node, offset, buffer, size); 
    } else {
        return -EBADFD;
    }
}

/**
 * @brief Get file status
 */
int fstat(struct fs_node *node, struct stat *buf)
{
    if (node->stat)
        return node->stat(node, buf);
    else
        return -EBADFD;
}

/**
 * @brief Open file from path
 *
 * @param path (absolute) path
 * @param flags flags
 */
void *kopen(const char *path, int flags)
{
    struct fs_node *node;
    if (!(node = vfs_mount(path, NULL))) {
        return NULL;
    }
    return (vfs_open(node, flags) != -EBUSY) ? node : NULL;
}

/**
 * @brief Get the part of path right after the last seperator
 */
char *vfs_get_base_name(char *path)
{
    char *p = strdup(path);
    size_t last = strlen(p)-1;
    for (size_t i = last; i > 0; --i) {
        if (path[i] == '/') {
            return &p[i+1];
        }
    }

    return p;
}

/**
 * @brief Get the part of path right before the last seperator
 */
char *vfs_get_dir_name(char *path)
{
    char *p = strdup(path);
    size_t last = strlen(path)-1;
    for (size_t i = last; i > 0; --i) {
        if (p[i] == '/') {
            p[i] = 0;
            break;
        }
    }

    return p;
}

/**
 * @brief Inserts leading and trailing slash to path
 */
char *vfs_canonicalise(char *path)
{
    size_t len = strlen(path);
    char *p = kmalloc(len + 2);
    strcpy(&p[1], path);
    p[0] = '/';
    p[len + 1] = '/';
    return p;
}

/**
 * @brief Initialise vfs
 */
void vfs_init(void)
{
    vfs_mount_root();
    filesystems = hashmap(MAX_REGISTERED_FILESYSTEMS);
    pr_info("vfs: mounted root");
}
