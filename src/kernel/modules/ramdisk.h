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
 */

#pragma once

#include <lib/panic.h>
#include <lib/tar.h>

#include <generic/fs/vfs.h>

#include <stdint.h>
#include <stddef.h>

/**
 * ramdisk_init
 *   brief: init ramdisk
 */
void ramdisk_init(void *rd);

/**
 * ramfs_mount
 *   brief: mount ramfs node
 */
int ramfs_mount(const char *path, struct fs_node *node);

/**
 * IO
 *   brief: all IO operations needed for ramfs
 */
int ramfs_open(struct fs_node *node, int flags);
int ramfs_close(struct fs_node *node);
int ramfs_write(struct fs_node *node, size_t offset, void *buffer, size_t size);
int ramfs_read(struct fs_node *node, size_t offset, void *buffer, size_t size);
