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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR See the
 * GNU General Public License for more details.
 *
 * ramdisk
 */

#include "ramdisk.h"

struct fs_descriptor ramfs_desc = {
    .name = "ramfs",
    .mount = ramfs_mount
};

/**
 * @brief Mount ramfs node
 */
int ramfs_mount(const char *path, struct fs_node *node)
{
    UNUSED(path);
    node->open = ramfs_open;
    node->close = ramfs_close;
    node->read = ramfs_read;
    node->write = ramfs_write;
    node->stat = ramfs_stat;
    return -ENOTH;
}

/* Stub open/close */
int ramfs_open(struct fs_node *node, int flags)
{
    UNUSED(node);
    UNUSED(flags);
    return -ENOTH;
}

int ramfs_close(struct fs_node *node)
{
    UNUSED(node);
    return -ENOTH;
}

/* Ramfs is read only! */
int ramfs_write(struct fs_node *node, size_t offset, void *buffer, size_t size)
{
    UNUSED(node);
    UNUSED(offset);
    UNUSED(buffer);
    UNUSED(size);
    return -EROFS;
}

/**
 * @brief Read from ramdisk
 *
 * Reads the contents of a ramdisk node to a buffer
 */
int ramfs_read(struct fs_node *node, size_t offset, void *buffer, size_t size)
{
    memcpy(buffer, ((struct tar_block *) node->device)->data + offset, size);
    return -ENOTH;
}

/**
 * @brief Read from ramdisk
 *
 * Reads the contents of a ramdisk node to a buffer
 */
int ramfs_stat(struct fs_node *node, struct stat *buf)
{
    buf->st_size = ((struct tar_block *) node->device)->size;
    return -ENOTH;
}

/**
 * @brief Initialise ramdisk
 *
 * Parses the ramdisk and loads it into
 * the vfs
 */
void ramdisk_init(void *rd)
{
    /* Register ramfs */
    vfs_register_fs(&ramfs_desc);

    /* Extract the contents of the ramdisk */
    struct tar_block *block = kmalloc(sizeof(struct tar_block));
    for (size_t i = 0; tar_read(rd, block, i); i++) {
        char *base = vfs_get_base_name(block->name);
        char *dir = vfs_canonicalise(vfs_get_dir_name(block->name));
        if (base[0]) {
            /* Only mount files, since we don't support
               empty directories in the ramdisk */
            struct fs_node *file = kmalloc(sizeof(struct fs_node));
            file->name = base;
            file->type = FS_FILE;
            file->fs = ramfs_desc.name;
            file->device = block; /* We store a pointer to the block on the device */
            vfs_mount(dir, file);
            block = kmalloc(sizeof(struct tar_block));
        }
    }

    pr_info("rd: mounted ramdisk");
}
