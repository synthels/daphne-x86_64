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

#include <stdint.h>

#include <lib/hashmap.h>
#include <lib/tree.h>
#include <lib/lock.h>

#include <generic/time/time.h>
#include <generic/forbia/errno.h>

#define VFS_MAX_FILE_NAME 256
#define MAX_REGISTERED_FILESYSTEMS 15

struct fs_node;
struct stat;

typedef int   (*open_t) (struct fs_node *, int); /* node, flags */
typedef int   (*close_t)(struct fs_node *);
typedef void  (*ioctl_t)(int, int);
typedef int   (*write_t)(struct fs_node *, size_t, void *, size_t); /* node, offset, buffer, size */
typedef int   (*read_t) (struct fs_node *, size_t, void *, size_t); /* node, offset, buffer, size */
typedef int   (*mount_t)(const char *, struct fs_node *); /* mount path, node */
typedef int   (*stat_t) (struct fs_node *, struct stat *); /* node, stat buffer */

enum fs_node_type {
    FS_FILE = 0,
    FS_ROOT = 1,
    /* This is not an actual directory -as in a filesystem defined thing-, it is more
       of an in-memory container for other files */
    FS_DIRECTORY = 2
};

struct fs_node {
    char  *name; /* Filename */
    int    type; /* File type (fs_node_type) */
    char  *fs;   /* Filesystem name (if applicable) */

    open_t   open;
    close_t  close;
    ioctl_t  ioctl;
    write_t  write;
    read_t   read;
    stat_t   stat;

    bool ref; /* Set if this node is currently open */
    struct tree_node *vfs_ptr; /* Pointer to vfs entry */

    uint64_t inode;
    void *device;
};

struct fs_descriptor {
    char *name;
    /* Will be called by vfs_mount whenever
       a file from this filesystem is mounted.
       The filesystem can then set its own
       open/close and read/write functions. */
    mount_t mount;
};

struct stat {
    int     st_dev;     /* ID of device containing file */
    int     st_ino;     /* inode number */
    int     st_mode;    /* protection */
    size_t  st_size;    /* total size, in bytes */
    time_t  st_atime;   /* time of last access */
    time_t  st_mtime;   /* time of last modification */
    time_t  st_ctime;   /* time of last status change */
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

/**
 * vfs_register_fs
 *   brief: register filesystem
 */
void vfs_register_fs(struct fs_descriptor *fs);

/**
 * vfs_open
 *   brief: open vfs node
 */
int vfs_open(struct fs_node *node, int flags);

/**
 * vfs_close
 *   brief: close vfs node
 */
int vfs_close(struct fs_node *node);

/**
 * fwrite
 *   brief: write to vfs node
 */
int fwrite(struct fs_node *node, size_t offset, void *buffer, size_t size);

/**
 * fread
 *   brief: read from vfs node
 */
int fread(struct fs_node *node, size_t offset, void *buffer, size_t size);

/**
 * fstat
 *   brief: get file status
 */
int fstat(struct fs_node *node, struct stat *buf);

/**
 * vfs_get_base_name
 *   brief: get the part of path right after the last seperator
 */
char *vfs_get_base_name(char *path);

/**
 * vfs_get_dir_name
 *   brief: get the part of path right before the last seperator
 */
char *vfs_get_dir_name(char *path);

/**
 * vfs_canonicalise
 *   brief: inserts leading and trailing slash to path
 */
char *vfs_canonicalise(char *path);

/**
 * kopen
 *   brief: open file from path
 */
void *kopen(const char *path, int flags);

/**
 * kclose
 *   brief: close fs_node
 */
int kclose(struct fs_node *node);
