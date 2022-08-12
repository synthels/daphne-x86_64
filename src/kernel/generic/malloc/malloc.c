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
 * synthels' slab allocator
 *
 * This is currently a very barebones object allocator, working
 * with slabs, without any caches (although we should implement them at some point).
 * For a quick gist, kmalloc allocates memory on small slabs with sizes ranging from 64 to
 * any arbitrary size. For bigger allocations, we expose routines that access special "object pools" of
 * arbitrary sizes.
 */

#include "malloc.h"

declare_lock(malloc_lock);
declare_lock(realloc_lock);
declare_lock(free_lock);
declare_lock(pool_create_lock);
declare_lock(pool_alloc_lock);
declare_lock(pool_realloc_lock);
declare_lock(pool_free_lock);

static struct malloc_slab root = {NULL, NULL, false, 0, SLAB_MAX_SIZE};

/* Exponential function (should not be here) */
static unsigned int exp(unsigned int n)
{
    return (1 << n);
}

/* Default object constructor */
static void null_ctor(void *obj)
{
    UNUSED(obj);
}

/**
 * @brief Append any number of pages to a page
 *
 * @param slab the slab in question :^)
 * @param page the page that new pages will be appended to
 * @param n Number of pages to append
 */
void slab_append_pages_to_page(struct malloc_slab *slab, struct malloc_page *page, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        struct malloc_page *p = mmu_alloc(sizeof(struct malloc_page));
        p->base = mmu_alloc(slab->size + sizeof(struct malloc_header));

        page->next = p;
        p->next = NULL;
        page = page->next;
    }
    slab->length += n;
}

/**
 * @brief Create an initial list of pages for the slab
 *
 * @param slab the slab in question :^)
 */
void pages_create_initial_list(struct malloc_slab *slab)
{
    slab_append_pages_to_page(slab, slab->pages, SLAB_PAGES_INITIAL);
}

/**
 * @brief Append any number of pages to a slab
 *
 * @param slab the slab in question :^)
 * @param n Number of pages to append
 */
void slab_append_pages(struct malloc_slab *slab, size_t n)
{
    struct malloc_page *end = slab->pages;
    while (end->next) {
        /* Skip to last page */
        end = end->next;
    }
    slab_append_pages_to_page(slab, end, n);
}

/**
 * @brief Create a slab and append it to root
 *
 * @param size Slab size
 */
struct malloc_slab *slab_create(size_t size)
{
    static struct malloc_slab *prev = &root;
    struct malloc_slab *slab = mmu_alloc(sizeof(struct malloc_slab));
    slab->size = size;
    slab->length = 1;
    slab->realloc = false;
    slab->pages = mmu_alloc(sizeof(struct malloc_page));
    slab->pages->base = mmu_alloc(slab->size + sizeof(struct malloc_header));
    pages_create_initial_list(slab);

    prev->next = slab;
    slab->next = NULL;
    prev = prev->next;
    return slab;
}

/**
 * @brief Remove page from slab
 *
 * @param slab the slab in question :^)
 * @param page Page which will be removed
 */
void slab_remove_page(struct malloc_slab *slab, struct malloc_page *page)
{
    if (page->next && page) {
        struct malloc_page **iter = &(slab->pages);
        while ((*iter) != page)
            iter = &(*iter)->next;
        *iter = page->next;
    }
}

/**
 * @brief Push page to slab
 *
 * @param slab The slab in question :^)
 * @param page The page to be pushed
 */
void slab_push_page(struct malloc_slab *slab, struct malloc_page *page)
{
    struct malloc_page *iter = slab->pages;
    while (iter->next)
        iter = iter->next;
    iter->next = page;
    page->next = NULL;
}

/**
 * @brief Pops the top page from a slab
 *
 * @param slab Well, you get it by now
 * @todo  This fails for some large allocations
 */
struct malloc_page *slab_pop_page(struct malloc_slab *slab)
{
    struct malloc_page *page = slab->pages->next;

    if (slab->realloc) {
        /* Remove & free all pages */
        while (page) {
            slab_remove_page(slab, page);
            page = page->next;
        }
        slab->realloc = false;
        slab->length = 0;
    }

    /* Skip to adding new pages if there are none */
    if (slab->length > 0) {
        /* Set malloc header */
        struct malloc_header *header = (struct malloc_header *) (page->base);
        header->slab = slab;
        header->page = page;
        header->magic = MALLOC_HEADER_MAGIC;
        header->true_size = slab->size + sizeof(struct malloc_header);
        /* Remove this page from the list */
        slab_remove_page(slab, page);
        slab->length--;
        return page;
    }

    /* No free page, push new pages! */
    slab_append_pages_to_page(slab, slab->pages, SLAB_PAGES_INITIAL);
    /* We can now allocate from the new pages */
    return slab_pop_page(slab);
}

/**
 * @brief Pick the best-fit slab from a list of slabs for this allocation
 *
 * @param n Slab list size
 * @param alloaction_size Allocation size
 */
struct malloc_page *slab_pick_and_allocate(struct malloc_slab *slabs, size_t n, size_t allocation_size)
{
    /* We initially point best_fit to root, since root has the maximum allowed
       size for any slab */
    struct malloc_slab *best_fit = &root;

    struct malloc_slab *s = slabs;
    for (size_t i = 0; i < n; i++) {
        /* Find best-fit slab */
        if ((s->size >= allocation_size) && (s->size < best_fit->size)) {
            best_fit = s;
        }
        s = s->next;
    }

    if (!best_fit->pages) {
        return NULL;
    }

    return slab_pop_page(best_fit);
}

/**
 * @brief Free slab page
 *
 * @param ptr ptr to page
 */
void *slab_free(void *ptr)
{
    if (!ptr)
        return NULL;

    /* Arithmetic on void pointers is illegal! */
    struct malloc_header *header = (struct malloc_header *) ((uint8_t *) ptr - sizeof(struct malloc_header));

    /* Invalid pointer! */
    if (header->magic != MALLOC_HEADER_MAGIC)
        return NULL;

    /* Just put the page back! */
    slab_push_page(header->slab, header->page);
    return ptr;
}

/**
 * @brief Create object pool
 *
 * @param name pool name
 * @param ctor object constructor
 * @param size slab size
 */
struct object_pool *pool_create(const char *name, obj_ctor ctor, size_t size)
{
    lock(&pool_create_lock);
    struct object_pool *pool = mmu_alloc(sizeof(struct object_pool));
    pool->name = name;
    pool->slab = slab_create(size);
    pool->ctor = ctor ? ctor : null_ctor;
    unlock(&pool_create_lock);
    return pool;
}

/**
 * @brief Allocate object from pool
 *
 * @param pool object pool
 */
void *pool_alloc(struct object_pool *pool)
{
    lock(&pool_alloc_lock);
    /* TODO: cache constructed objects */
    struct malloc_page *page = slab_pop_page(pool->slab);
    pool->ctor(page);
    unlock(&pool_alloc_lock);
    return (page->base + sizeof(struct malloc_header) / 8);
}

void *pool_realloc(struct object_pool *pool, void *ptr, size_t size)
{
    lock(&pool_realloc_lock);
    struct malloc_header *header = (struct malloc_header *) ((uint8_t *) ptr - sizeof(struct malloc_header));
    size_t ptr_size = header->true_size;

    if (!size) {
        unlock(&pool_realloc_lock);
        return NULL;
    } else if (!ptr) {
        unlock(&pool_realloc_lock);
        return pool_alloc(pool);
    }
    else if (size <= ptr_size) {
        unlock(&pool_realloc_lock);
        return ptr;
    } else {
        /* Increase pool size */
        pool->slab->size = size;
        pool->slab->realloc = true;
        void *new = pool_alloc(pool);
        if (new) {
            memcpy(new, ptr, ptr_size);
            pool_free(ptr);
        }
        unlock(&pool_realloc_lock);
        return new;
    }

    unlock(&pool_realloc_lock);
    return NULL;
}

/**
 * @brief Free object from pool
 *
 * @param object object returned by pool_alloc
 * @param pool object pool
 */
void *pool_free(void *object)
{
    lock(&pool_free_lock);
    /* TODO: cache constructed objects */
    void *ret = slab_free(object);
    unlock(&pool_free_lock);
    return ret;
}

/**
 * @brief Allocate a page on one of the "small" kmalloc slabs
 *
 * @param n Allocation size
 */
void *kmalloc(size_t n)
{
    lock(&malloc_lock);
    static struct malloc_slab *kmalloc_slabs = NULL;
    static size_t slabs_top = 0;
    /* First call, create some initial slabs */
    if (!kmalloc_slabs) {
        /* Create 4 slabs of sizes 64, 128, 256, 512 and 1024 */
        kmalloc_slabs = slab_create(64 * exp(0));
        slabs_top = 64 * exp(KMALLOC_SLABS - 1);
        struct malloc_slab *s = kmalloc_slabs;
        for (int i = 1; i < KMALLOC_SLABS; i++) {
            s->next = slab_create(64 * exp(i));
            s = s->next;
        }
    }

    /* We don't have big enough slabs! */
    if (n > slabs_top) {
        struct malloc_slab **s = &kmalloc_slabs;
        while ((*s)->next) {
            (*s) = (*s)->next;
        }
        (*s) = slab_create(n);
    }

    /* Allocate on fitting slab */
    struct malloc_page *page = slab_pick_and_allocate(kmalloc_slabs, KMALLOC_SLABS - 1, n);

    /* Return address right next to header */
    unlock(&malloc_lock);
    return (page->base + sizeof(struct malloc_header) / 8);
}

/**
 * @brief Reallocates a given area of memory previously allocated by kmalloc
 *
 * @param ptr pointer to memory
 * @param size new size
 */
void *krealloc(void *ptr, size_t size)
{
    lock(&realloc_lock);
    struct malloc_header *header = (struct malloc_header *) ((uint8_t *) ptr - sizeof(struct malloc_header));
    size_t ptr_size = header->true_size;

    if (!size) {
        kfree(ptr);
        unlock(&realloc_lock);
        return NULL;
    } else if (!ptr) {
        unlock(&realloc_lock);
        return kmalloc(size);
    }
    else if (size <= ptr_size) {
        unlock(&realloc_lock);
        return ptr;
    } else {
        void *new = kmalloc(size);
        if (new) {
            memcpy(new, ptr, ptr_size);
            kfree(ptr);
        }
        unlock(&realloc_lock);
        return new;
    }

    unlock(&realloc_lock);
    return NULL;
}

/**
 * @brief Frees a given area of memory previously allocated by kmalloc
 *
 * @param ptr pointer to memory
 */
void kfree(void *ptr)
{
    lock(&free_lock);
    slab_free(ptr);
    unlock(&free_lock);
}
