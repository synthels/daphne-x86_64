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

#include <stddef.h>
#include <stdint.h>

#include <generic/malloc/malloc.h>
#include <generic/forbia/types.h>

typedef uint8_t byte;

#ifndef NULL
    #define NULL (void *) 0x0
#endif

/**
 * strlen
 *   brief: return number of characters in
 *          the null terminated string str
 *   parameters:
 *     - str: string
 */
size_t strlen(const char *str);

/**
 * memset
 *   brief: copy c to to the first sz characters
 *          pointed to by s
 *   parameters:
 *     - s:  string
 *     - c:  character
 *     - sz: number of bytes
 */
void *memset(void *s, int c, size_t sz);

/**
 * strcpy
 *   brief: copy null terminated string src to
 *          dest
 *   parameters:
 *     - dest: destination string
 *     - src:  source string
 */
void strcpy(char *dest, const char *src);

/**
 * strncpy
 *   brief: copy string src to
 *          dest
 *   parameters:
 *     - dest: destination string
 *     - src:  source string
 *     - n:    number of characters to copy
 */
void strncpy(char *dest, const char *src, size_t n);

/**
 * memcpy
 *   brief: copy data from src to dst
 *   parameters:
 *     - dst:  destination
 *     - src:  source
 *     - len:  length
 */
void *memcpy(void *dst, const void *src, size_t len);

/**
 * memmove
 *   brief: copy data from src to dst, while taking
 *          care of overlaps
 *   parameters:
 *     - dest:  destination
 *     - src:   source
 *     - n:     length
 */
void *memmove(void *dest, const void *src, size_t n);

/**
 * memcmp
 *   brief: compare first n bytes of str1 and str2
 *   parameters:
 *     - str1:  first string
 *     - str2:  second string
 *     - n:     length
 */
int memcmp(const void *str1, const void *str2, size_t n);

/**
 * strncmp
 *   brief: compare first n bytes of s1 and s2
 *   parameters:
 *     - str1:  first string
 *     - str2:  second string
 *     - n:     length
 */
int strncmp(const char *s1, const char *s2, size_t n);

/**
 * strcmp
 *   brief: compare s1 and s2
 *   parameters:
 *     - str1:  first string
 *     - str2:  second string
 *     - n:     length
 */
int strcmp(const char* s1, const char* s2);

/**
 * strcat
 *   brief: append src to dest
 *   parameters:
 *     - src:   source string
 *     - dest:  second string
 */
char *strcat(char *dest, const char *src);

/**
 * strdup
 *   brief: duplicate string
 *   parameters:
 *     - src: string
 */
char *strdup(const char *src);
