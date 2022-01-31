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
 * string.h functions
 */

#include "string.h"

size_t strlen(const char *str)
{
    const char *s;
    for (s = str; *s; ++s) { }
    return(s - str);
}

void *memset(void *s, int c, size_t sz)
{
    uint32_t* p;
    uint32_t x = c & 0xff;
    byte xx = c & 0xff;
    byte *pp = (byte *) s;
    size_t tail;

    while (((byte) (uintptr_t) pp & 3) && sz--)
        *pp++ = xx;
    p = (uint32_t *) pp;

    tail = sz & 3;

    x |= x << 8;
    x |= x << 16;

    sz >>= 2;

    while (sz--)
        *p++ = x;

    pp = (byte *) p;
    while (tail--)
        *pp++ = xx;

    return s;
}

void strcpy(char *dest, const char *src) 
{
    size_t i = 0;
    while ((dest[i] = src[i]) != '\0') {
        i++;
    } 
}

void *memcpy(void *dst, const void *src, size_t len)
{
    size_t i;
    /*
     * Do long-by-long copy if memory is properly
     * aligned, do byte-by-byte copy otherwise
     */
    if ((uintptr_t) dst % sizeof(long) == 0 && (uintptr_t) src % sizeof(long) == 0 && len % sizeof(long) == 0) {
        long *d = dst;
        const long *s = src;
        for (i = 0; i < len / sizeof(long); i++) d[i] = s[i];
    } else {
        char *d = dst;
        const char *s = src;
        for (i = 0; i < len; i++) d[i] = s[i];
    }
    return dst;
}

void *memmove(void *dest, const void *src, size_t n)
{
    signed char operation;
    size_t end;
    size_t current;
    if(dest != src) {
        if(dest < src) {
            operation = 1;
            current = 0;
            end = n;
        } else {
            operation = -1;
            current = n - 1;
            end = -1;
        }
        for(; current != end; current += operation) *(((unsigned char *) dest) + current) = *(((unsigned char *) src) + current);
    }

    return dest;
}

int memcmp(const void *str1, const void *str2, size_t n)
{
    const unsigned char *s1 = (const unsigned char *) str1;
    const unsigned char *s2 = (const unsigned char *) str2;
    while (n --> 0) {
        if (*s1++ != *s2++) return s1[-1] < s2[-1] ? -1 : 1;
    }
    return 0;
}
