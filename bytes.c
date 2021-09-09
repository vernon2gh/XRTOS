#include "bytes.h"

struct bytes {
    void *freelist;
} bytes_cache[PAGE_ORDER];

/*
 * Initialize the freelist variable of different 2^order list
 */
void byte_init(void)
{
    int order;

    for(order = 0; order < PAGE_ORDER; order++)
        bytes_cache[order].freelist = NULL;
}

static inline uint32_t pow(uint32_t base, uint32_t order)
{
    uint32_t tmp = 1;

    while(order) {
        tmp *= base;
        order--;
    }

    return tmp;
}

static inline uint32_t get_order(uint32_t nbytes, uint32_t base)
{
    uint32_t order = 0;

    while(nbytes /= base) {
        order++;
    }

    return order;
}

/*
 * Allocate bytes from different 2^order list
 *
 * nbytes: the number of byte for want to allocate
 */
void *byte_alloc(uint32_t nbytes)
{
    void *tmp;
    uint32_t step, num;
    pointer_t current;
    int mini_order, order = 0, i;

    while(nbytes / pow(2, order))
        order++;

    if(order >= PAGE_ORDER) {
        i = (nbytes/PAGE_SIZE) + (nbytes%PAGE_SIZE? 1:0);

        return page_alloc(i);
    }

    mini_order = get_order(sizeof(pointer_t), 2);
    if(order < mini_order)
        order = mini_order;

    step = pow(2, order);
    num = PAGE_SIZE/step;

    if(!bytes_cache[order].freelist) {
        tmp = page_alloc(1);
        bytes_cache[order].freelist = tmp;

        for (i = 0; i < num; i++) {
            current = (pointer_t)tmp + i * step;
            *(pointer_t *)current = current + step;
        }

        *(pointer_t *)current = (pointer_t)NULL;
    }

    tmp = bytes_cache[order].freelist;
    bytes_cache[order].freelist = (void *)(*(pointer_t *)tmp);

    return tmp;
}

/*
 * Free bytes from different 2^order list
 *
 * p     : at-first address of byte for want to free
 * nbytes: the number of byte for want to free
 */
void byte_free(void *p, uint32_t nbytes)
{
    int mini_order, order = 0;

    while(nbytes / pow(2, order))
        order++;

    if(order >= PAGE_ORDER)
        return page_free(p);

    mini_order = get_order(sizeof(pointer_t), 2);
    if(order < mini_order)
        order = mini_order;

    *(pointer_t *)p = (pointer_t)bytes_cache[order].freelist;
    bytes_cache[order].freelist = p;
}
