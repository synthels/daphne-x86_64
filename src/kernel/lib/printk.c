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
 * printk function
 */

#include "printk.h"

/* Don't let anyone see this... */
static char *printf_buf;
static int log_level = NORMAL;

declare_lock(printf_lock);

/* Not too bad, right? */
int vsprintf(char *buf, const char *fmt, va_list args)
{
    char c;
    for (int i = 0; (c = *fmt++);) {
        buf[i++] = c;
        if (c == '%') {
            /* Allocate memory for the va_arg strings */
            char *str = kmalloc(sizeof(char) * VSPRINTF_BUFFER_SIZE);
            c = *fmt++;
            switch (c) {
                /* Strings */
                case 's':
                    /* Free string here again, since the other free
                       will not work, as we are essentially losing
                       the pointer here */
                    kfree(str);
                    str = va_arg(args, char *);
                    break;
                /* Unsigned (no prettier way to do this) */
                case 'u':
                    switch (*fmt++) {
                        case 'i':
                            uitoa(va_arg(args, uint64_t), str);
                            break;
                    }
                    break;
                case 'i':
                    itoa(va_arg(args, int64_t), str);
                    break;
                /* Hex */
                case 'x':
                    uitoh(va_arg(args, uint64_t), str);
                    break;
                /* Binary */
                case 'b':
                    /* TODO */
                    break;
                /* Just print a '%' */
                case '%':
                    /* Same as above... */
                    kfree(str);
                    str = "%";
                    break;
                /* Unknown type */
                default:
                    return EINVAL;
            }
            /* Skip '%' sign */
            i--;
            /* Copy string to buffer */
            for (; (c = *str++); i++) {
                buf[i] = c;
            }

            /* We don't need str anymore */
            kfree(str);
        }
    }

    return NOERR;
}

size_t vsprintf_length(const char *fmt, va_list args)
{
    char c;
    size_t length = 0;
    char *str = kmalloc(VSPRINTF_BUFFER_SIZE * sizeof(char));
    for (int i = 0; (c = *fmt++);) {
        if (c == '%') {
            c = *fmt++;
            switch (c) {
                /* Strings */
                case 's':
                    length += strlen(va_arg(args, char *));
                    break;
                /* Unsigned (no prettier way to do this) */
                case 'u':
                    switch (*fmt++) {
                        case 'i':
                            uitoa(va_arg(args, uint64_t), str);
                            break;
                    }
                    break;
                case 'i':
                    itoa(va_arg(args, int64_t), str);
                    break;
                /* Hex */
                case 'x':
                    uitoh(va_arg(args, uint64_t), str);
                    break;
                /* Binary */
                case 'b':
                    /* TODO */
                    break;
                /* Just print a '%' */
                case '%':
                    length++;
                    break;
                /* Unknown type */
                default:
                    break;
            }
            /* Skip '%' sign */
            i--;
            /* Copy string to buffer */
            for (; (c = *str++); i++) {
                length++;
            }
            length += strlen(str);
        }
    }

    kfree(str);
    return length;
}

int printf(printk_out_func out, const char *fmt, ...)
{    
    lock(&printf_lock);

    va_list ap;
    va_list cp;
    va_start(ap, fmt);

    /* Make a copy of the list
       since va_arg pops elements off of it! */
    va_copy(cp, ap);
    va_start(cp, fmt);
    size_t len = vsprintf_length(fmt, cp);

    /* This will be later freed by shrimp, so it's fine */
    printf_buf = kmalloc(sizeof(char) * len);

    for (size_t i = 0; i < len; i++) {
        printf_buf[i] = '\0';
    }

    int err = vsprintf(printf_buf, fmt, ap);
    va_end(ap);
    out(printf_buf);

    unlock(&printf_lock);
    return err;
}

int printk(int level, const char *fmt, ...)
{
    int err = NOERR;
    if (level >= log_level) {
        err = printf(shrimp_print, fmt);
    }
    return err;
}

int get_log_level(void)
{
    return log_level;
}

void set_log_level(int level)
{
    log_level = level;
}
