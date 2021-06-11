#include "bytes.h"

struct bytes {
    void *freelist;
} bytes_cache[PAGE_ORDER];

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

void *byte_alloc(uint32_t nbytes)
{
    void *tmp;
    uint32_t step, num, current;
    int order = 0, i;

    while(nbytes /= 2)
        order++;

    if(order >= PAGE_ORDER) {
        i = nbytes/PAGE_SIZE + nbytes%PAGE_SIZE? 1:0;

        return page_alloc(i);
    }

    if(order < 2)
        order = 2;

    step = pow(2, order);
    num = PAGE_SIZE/step;

    if(!bytes_cache[order].freelist) {
        tmp = page_alloc(1);
        bytes_cache[order].freelist = tmp;

        for (i = 0; i < num; i++) {
            current = (uint32_t)tmp + i * step;
            *(uint32_t *)current = current + step;
        }

        *(uint32_t *)current = (uint32_t)NULL;
    }

    tmp = bytes_cache[order].freelist;
    bytes_cache[order].freelist = (void *)(*(uint32_t *)tmp);

    return tmp;
}

void byte_free(void *p, uint32_t nbytes)
{
    int order = 0;

    while(nbytes /= 2)
        order++;

    if(order >= PAGE_ORDER)
        return page_free(p);

    if(order < 2)
        order = 2;

    *(uint32_t *)p = (uint32_t)bytes_cache[order].freelist;
    bytes_cache[order].freelist = p;
}
