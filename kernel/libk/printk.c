/*
 * Copyright (C) 2020 synthels <synthels.me@gmail.com>
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
static char *printk_buf;
static int log_level = NORMAL;

/* Not too bad, right? */
int vsprintf(const char *fmt, va_list args)
{
    char c;
    for (int i = 0; (c = *fmt++);) {
        printk_buf[i++] = c;
        if (c == '%') {
            c = *fmt++;
            /* Allocate memory for the va_arg strings */
            char *str = kmalloc(sizeof(char) * __PRINTK_BUFFER_SIZE);
            switch (c) {
                /* Strings */
                case 's':
                    str = va_arg(args, char *);
                    break;
                /* Unsigned (no prettier way to do this) */
                case 'u':
                    switch (*fmt++) {
                        case 'i':
                            uitoa(va_arg(args, uint64_t), str);
                            break;
                        /* Hex */
                        case 'x':
                            /* TODO */
                            break;
                        /* Binary */
                        case 'b':
                            /* TODO */
                            break;
                    }
                    break;
                case 'i':
                    itoa(va_arg(args, int64_t), str);
                    break;
                /* Hex */
                case 'x':
                    /* TODO */
                    break;
                /* Binary */
                case 'b':
                    /* TODO */
                    break;
                /* Just print a '%' */
                case '%':
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
                printk_buf[i] = c;
            }

            /* We don't need str anymore */
            kfree(str);
        }
    }

    return NOERR;
}

int printk(int level, const char *fmt, ...)
{
    if (level >= log_level) {
        /* This will be later freed by shrimp, so it's fine */
        printk_buf = kmalloc(sizeof(char) * __PRINTK_BUFFER_SIZE);

        for (size_t i = 0; i < __PRINTK_BUFFER_SIZE; i++) {
            printk_buf[i] = '\0';
        }

        va_list ap;
        va_start(ap, fmt);
        int err = vsprintf(fmt, ap);
        va_end(ap);
        shrimp_print(printk_buf);

        return err;
    }

    return NOERR;
}

int get_log_level(void)
{
    return log_level;
}
